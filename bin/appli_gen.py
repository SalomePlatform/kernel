#! /usr/bin/env python3
# Copyright (C) 2007-2026  CEA, EDF, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

# \file appli_gen.py
#  Create a %SALOME application (virtual Salome installation)
#
from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
import xml.sax
from pathlib import Path

# --- names of tags in XML configuration file
appli_tag = "application"
parent_appli_tag = "parent_application"
prereq_tag = "prerequisites"
context_tag = "context"
venv_directory_tag = "venv_directory"
sha1_collect_tag = "sha1_collections"
modules_tag = "modules"
module_tag = "module"
samples_tag = "samples"
extra_tests_tag = "extra_tests"
extra_test_tag = "extra_test"
resources_tag = "resources"
env_modules_tag = "env_modules"
env_module_tag = "env_module"
python_tag = "python"
absolute_appli_dir_tag = "absolute_appli_dir"
runner_tag = "runner"

# --- names of attributes in XML configuration file
name_att = "name"
path_att = "path"
gui_att = "gui"
version_att = "version"

CURDIR = Path(__file__).parent
APPLISKEL_DIR = CURDIR / "appliskel"
DEFAULT_LAUNCHER_NAME = "salome"
DEFAULT_ABSOLUTE_APPLI_DIR = "SALOME"
DESTDIR = os.getenv("DESTDIR", "")


def undestdir(path: str) -> str:
    if DESTDIR and path.startswith(DESTDIR):
        return path[len(DESTDIR):]
    return path


def path_with_destdir(path_str: str| Path) -> Path:
    path = Path(path_str)
    if DESTDIR and Path(DESTDIR) not in path.parents:
        path = Path(DESTDIR) / path.relative_to(path.root)
    return path


class xml_parser:
    """XML reader for SALOME application configuration file"""

    def __init__(self, fileName):
        print("Configure parser: processing %s ..." % fileName)
        self.space = []
        self.config = {}
        self.config[modules_tag] = {}
        self.config["guimodules"] = []
        self.config[extra_tests_tag] = {}
        self.config[env_modules_tag] = []
        parser = xml.sax.make_parser()
        parser.setContentHandler(self)
        parser.parse(fileName)

    def boolValue(self, text):
        if text in ("yes", "y", "1"):
            return 1
        elif text in ("no", "n", "0"):
            return 0
        else:
            return text

    def startElement(self, name, attrs):
        self.space.append(name)
        self.current = None
        # --- if we are analyzing "prerequisites" element then store its "path" attribute
        if self.space == [appli_tag, prereq_tag] and path_att in attrs.getNames():
            self.config[prereq_tag] = undestdir(attrs.getValue(path_att))

        # --- if we are analyzing "context" element then store its "path" attribute
        if self.space == [appli_tag, context_tag] and path_att in attrs.getNames():
            self.config[context_tag] = undestdir(attrs.getValue(path_att))

        # --- if we are analyzing "venv_directory" element then store its "path" attribute
        if (
            self.space == [appli_tag, venv_directory_tag]
            and path_att in attrs.getNames()
        ):
            self.config[venv_directory_tag] = undestdir(attrs.getValue(path_att))

        # --- if we are analyzing "absolute_appli_dir" element then store its "name" attribute
        if (
            self.space == [appli_tag, absolute_appli_dir_tag]
            and name_att in attrs.getNames()
        ):
            self.config[absolute_appli_dir_tag] = attrs.getValue(name_att)

        # --- if we are analyzing "runner" element then store its "name" attribute
        if self.space == [appli_tag, runner_tag] and name_att in attrs.getNames():
            self.config[runner_tag] = attrs.getValue(name_att)

        # --- if we are analyzing "parent_application" element then store its "name" attribute
        if self.space == [appli_tag, parent_appli_tag] and name_att in attrs.getNames():
            self.config[parent_appli_tag] = attrs.getValue(name_att)

        # --- if we are analyzing "sha1_collection" element then store its "path" attribute
        if self.space == [appli_tag, sha1_collect_tag] and path_att in attrs.getNames():
            self.config[sha1_collect_tag] = undestdir(attrs.getValue(path_att))

        # --- if we are analyzing "python" element then store its "version" attribute
        if self.space == [appli_tag, python_tag] and version_att in attrs.getNames():
            self.config[python_tag] = attrs.getValue(version_att)

        # --- if we are analyzing "resources" element then store its "path" attribute
        if self.space == [appli_tag, resources_tag] and path_att in attrs.getNames():
            self.config[resources_tag] = undestdir(attrs.getValue(path_att))

        # --- if we are analyzing "samples" element then store its "path" attribute
        if self.space == [appli_tag, samples_tag] and path_att in attrs.getNames():
            self.config[samples_tag] = undestdir(attrs.getValue(path_att))

        # --- if we are analyzing "module" element then store its "name" and "path" attributes
        elif (
            self.space == [appli_tag, modules_tag, module_tag]
            and name_att in attrs.getNames()
            and path_att in attrs.getNames()
        ):
            name = attrs.getValue(name_att)
            path = undestdir(attrs.getValue(path_att))
            gui = 1
            if gui_att in attrs.getNames():
                gui = self.boolValue(attrs.getValue(gui_att))

            self.config[modules_tag][name] = path
            if gui:
                self.config["guimodules"].append(name)

        # --- if we are analyzing "env_module" element then store its "name" attribute
        elif (
            self.space == [appli_tag, env_modules_tag, env_module_tag]
            and name_att in attrs.getNames()
        ):
            name = attrs.getValue(name_att)
            self.config[env_modules_tag].append(name)
        # --- if we are analyzing "extra_test" element then store its "name" and "path" attributes
        elif (
            self.space == [appli_tag, extra_tests_tag, extra_test_tag]
            and name_att in attrs.getNames()
            and path_att in attrs.getNames()
        ):
            name = attrs.getValue(name_att)
            path = undestdir(attrs.getValue(path_att))
            self.config[extra_tests_tag][name] = path

    def endElement(self, name):
        self.space.pop()
        self.current = None

    def characters(self, content):
        pass

    def processingInstruction(self, target, data):
        pass

    def setDocumentLocator(self, locator):
        pass

    def startDocument(self):
        self.read = None

    def endDocument(self):
        self.read = None


def install(
    prefix: Path, config_file: Path, force: bool = False, verbose: int = 0
) -> int:
    appli_dir = path_with_destdir(prefix.expanduser().absolute())
    filename = config_file.expanduser().absolute()

    if not filename.exists():
        print(f"ERROR: config file {filename} does not exist. It is mandatory.")
        return 1

    # Create directories
    if appli_dir.exists():
        if not force:
            print(
                f"Target directory {appli_dir} already exists and force option was not set."
            )
            return 1
    appli_dir.mkdir(parents=True, exist_ok=force)

    _config = {}
    try:
        # We try to load config as JSON format
        _config = json.loads(filename.read_text()).get(appli_tag)
    except json.JSONDecodeError:
        try:
            parser = xml_parser(filename)
            _config = parser.config
        except xml.sax.SAXParseException as inst:
            print(inst.getMessage())
            print(f"Configure parser: parse error in configuration file {filename}")
            return 1
        except xml.sax.SAXException as inst:
            print(inst.args)
            print(f"Configure parser: error in configuration file {filename}")
            return 1
        except Exception:
            print(
                f"Configure parser: Error : can not read configuration file {filename}, check existence and rights"
            )
            return 1

    if verbose:
        for cle, val in _config.items():
            print(cle, val)

    json_config = {appli_tag: {}}

    absolute_appli_dir: str = (
        _config.get(absolute_appli_dir_tag) or DEFAULT_ABSOLUTE_APPLI_DIR
    )
    json_config[appli_tag][absolute_appli_dir_tag] = absolute_appli_dir

    absolute_appli_path = appli_dir / absolute_appli_dir
    if absolute_appli_path.is_dir():
        shutil.rmtree(absolute_appli_path)
    absolute_appli_path.mkdir()

    extra_env_d = absolute_appli_path / "extra.env.d"
    extra_env_d.mkdir()

    bin_salome_d = absolute_appli_path / "bin" / "salome"
    bin_salome_d.mkdir(parents=True)

    env_d = absolute_appli_path / "env.d"
    env_d.mkdir(parents=True)

    # Copy launcher and make it executable
    name = _config.get(runner_tag, DEFAULT_LAUNCHER_NAME)
    json_config[appli_tag][runner_tag] = name
    launcher = appli_dir / name
    shutil.copyfile(APPLISKEL_DIR / "appli_launcher", launcher)
    mode = os.stat(launcher).st_mode
    mode |= (mode & 0o444) >> 2  # copy R bits to X
    os.chmod(launcher, mode)

    # Copy utilitaries needed by the launcher
    for script in "parseConfigFile.py", "salomeContext.py", "salomeContextUtils.py":
        shutil.copyfile(CURDIR / script, bin_salome_d / script)

    modules: dict[str, str] = _config.get(modules_tag, {})
    if modules:
        json_config[appli_tag][modules_tag] = {}
        for module, module_path in modules.items():
            module_path = undestdir(module_path)
            print("--- add module", module, module_path)
            json_config[appli_tag][modules_tag][module] = module_path

    extra_tests: dict[str, str] = _config.get(extra_tests_tag, {})
    if extra_tests:
        json_config[appli_tag][extra_tests_tag] = []
        for extra_test, extra_test_path in extra_tests.items():
            extra_test_path = undestdir(extra_test_path)
            print("--- add extra test", extra_test, extra_test_path)
            json_config[appli_tag][extra_tests_tag].append(extra_test_path)

    venv_directory = _config.get(venv_directory_tag)
    if venv_directory:
        venv_directory_path = undestdir(venv_directory)
        print("--- add venv directory", venv_directory_path)
        json_config[appli_tag][venv_directory_tag] = venv_directory_path

    parent_application = _config.get(parent_appli_tag)
    if parent_application:
        parent_application_path = undestdir(parent_application)
        print("--- add parent application", parent_application_path)
        json_config[appli_tag][parent_appli_tag] = parent_application_path

    # Get the env modules which will be loaded
    # In the same way as: module load [MODULE_LIST]
    env_modules = _config.get(env_modules_tag, [])
    if env_modules:
        json_config[appli_tag][env_modules_tag] = env_modules

    # Add .salome-completion.sh file
    #  shutil.copyfile(APPLISKEL_DIR / ".salome-completion.sh", appli_dir / ".salome-completion.sh")

    if prereq_tag in _config:
        prereq_tag_path = path_with_destdir(_config[prereq_tag])
        if prereq_tag_path.is_file():
            target = env_d / "envProducts.sh"
            shutil.copyfile(prereq_tag_path, target)
            json_config[appli_tag][prereq_tag] = f"{target}"
        else:
            print(f"WARNING: prerequisite file {prereq_tag_path} does not exist")

    if context_tag in _config:
        context_tag_path = path_with_destdir(_config[context_tag])
        if context_tag_path.is_file():
            target = env_d / "envProducts.cfg"
            shutil.copyfile(context_tag_path, target)
            json_config[appli_tag][context_tag] = f"{target}"
        else:
            print(f"WARNING: context file {context_tag_path} does not exist")

    if sha1_collect_tag in _config:
        sha1_collect_tag_path = path_with_destdir(_config[sha1_collect_tag])
        if sha1_collect_tag_path.is_file():
            target = env_d / "sha1_collections.txt"
            shutil.copyfile(sha1_collect_tag_path, target)
            json_config[appli_tag][sha1_collect_tag] = f"{target}"
        else:
            print(f"WARNING: sha1 collections file {sha1_collect_tag_path} does not exist")

    if not parent_application:
        version_python = f"{sys.version_info.major}.{sys.version_info.minor}"
        if python_tag in _config:
            version_python_split = _config[python_tag].split(".")
            version_python = version_python_split[0] + "." + version_python_split[1]
        elif _config.get(prereq_tag):
            cmd = ""
            if prereq_tag in _config:
                prereq_tag_path = path_with_destdir(_config[prereq_tag])
                if prereq_tag_path.exists():
                    cmd += f"source {DESTDIR}{_config[prereq_tag]} && "
            cmd += 'python3 -c "import sys ; sys.stdout.write(f\\"{sys.version_info.major}.{sys.version_info.minor}\\")"'
            version_python = subprocess.check_output(["/bin/bash", "-l", "-c", cmd]).decode(
                "utf-8"
            )
        else:
            print("ERROR: impossible to retrieve python version")
            return 1
        json_config[appli_tag][python_tag] = version_python

    config_salome_lines = []
    if samples_tag in _config:
        config_salome_lines.append(f"DATA_DIR={_config[samples_tag]}\n")
        json_config[appli_tag][samples_tag] = _config[samples_tag]
    if resources_tag in _config and Path(_config[resources_tag]).is_file():
        config_salome_lines.append(
            f"USER_CATALOG_RESOURCES_FILE={Path(_config[resources_tag]).absolute().as_posix()}\n"
        )
        json_config[appli_tag][resources_tag] = _config[resources_tag]

    if config_salome_lines:
        config_salome_lines.insert(0, "[SALOME ROOT_DIR (modules) Configuration]")
        config_salome = Path(env_d / "configSalome.cfg")
        with config_salome.open("w") as f:
            f.write("\n".join(config_salome_lines))

    json_config_file = appli_dir / "config_appli.json"
    json_config_file.write_text(json.dumps(json_config, indent=4))


def main() -> int:
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--prefix",
        type=Path,
        default=Path.cwd(),
        metavar="<install directory>",
        help="Installation directory (default %(default)s)",
    )

    parser.add_argument(
        "--config",
        type=Path,
        default=Path("config_appli.xml"),
        metavar="<configuration file>",
        help="XML or JSON configuration file (default %(default)s)",
    )

    parser.add_argument(
        "-v",
        "--verbose",
        action="count",
        default=0,
        help="Increase verbosity",
    )

    parser.add_argument(
        "-f",
        "--force",
        action="store_true",
        help="Overwrite existing directory",
    )

    args = parser.parse_args()
    return install(
        prefix=args.prefix,
        config_file=args.config,
        force=args.force,
        verbose=args.verbose,
    )


if __name__ == "__main__":
    sys.exit(main())
