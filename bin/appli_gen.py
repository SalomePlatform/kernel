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
from pathlib import Path
import sys
import shutil
import virtual_salome
import xml.sax
import subprocess
import venv

usage = """%(prog)s [options]
Typical use is:
  %(prog)s
Typical use with options is:
  %(prog)s --verbose --prefix=<install directory> --config=<configuration file>
"""

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


def path_with_destdir(path_str: str | Path) -> Path:
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


class params:
    pass



def update_shebang(source_path, target_path, home_dir):
    # Chemin vers l'interpréteur Python de l'application
    python_executable = Path(home_dir, "bin") / Path(sys.executable).name

    # Lire le contenu du fichier source
    source_file = Path(source_path)
    lines = source_file.read_text().splitlines()

    # Vérifier et mettre à jour le shebang
    if lines[0].startswith('#!'):
        lines[0] = f'#!{python_executable}'
    else:
        lines.insert(0, f'#!{python_executable}')

    # Écrire le contenu mis à jour dans le fichier cible
    target_file = Path(target_path)
    target_file.write_text('\n'.join(lines) + '\n')


def makedirs(namedir):
    if os.path.exists(namedir):
        dirbak = namedir + ".bak"
        if os.path.exists(dirbak):
            shutil.rmtree(dirbak)
        os.rename(namedir, dirbak)
        os.listdir(
            dirbak
        )  # sert seulement a mettre a jour le systeme de fichier sur certaines machines
    os.makedirs(namedir)


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

def install(prefix, config_file, verbose=0):
    home_dir = os.path.abspath(os.path.expanduser(prefix))
    filename = os.path.abspath(os.path.expanduser(config_file))

    venv.create(
        env_dir=home_dir,
        with_pip=True,
    )

    _config = {}
    from_json = False
    if filename.suffix == ".json":
        try:
            _config = json.loads(filename.read_text()).get(appli_tag)
            from_json = True
        except json.JSONDecodeError as e:
            print(e.msg)
            print(f"Configure parser: parse error in configuration file {filename}")
            return 1
    else:
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

    json_config = {
        appli_tag: {
            modules_tag: {},
            extra_tests_tag: [],
        }
    }

    # Add KERNEL if not in config
    # We use KERNEL that provides appli_gen script
    modules = _config.get("modules", [])
    if "KERNEL" not in modules:
        # We suppose that appli_gen is installed in KERNEL_ROOT_DIR/bin/salome
        kernel_path = Path(__file__).resolve().parents[2].as_posix()
        _config["KERNEL"] = kernel_path
        modules.insert(0, "KERNEL")
    kernel_module_path = None

    for module in modules:
        if module in _config:
            module_path = _config[module]
            print("--- add module ", module, module_path)
            options = params()
            options.verbose = verbose
            options.clear = 0
            options.prefix = home_dir
            options.module_name = module
            options.module_path = module_path
            virtual_salome.link_module(options)
            # To fix GEOM_TestXAO issue https://codev-tuleap.cea.fr/plugins/tracker/?aid=16599
            if module == "GEOM":
                # link <appli_path>/bin/salome/test/<module> to <module_path>/bin/salome/test
                test_dir=os.path.join(home_dir,'bin','salome', 'test')
                module_dir=os.path.abspath(options.module_path)
                xao_link=os.path.join(module_dir,'bin','salome', 'test', "xao")
                print("link %s --> %s"%(os.path.join(test_dir, "xao"), xao_link))
                virtual_salome.symlink(xao_link, os.path.join(test_dir, "xao"))
            if module == "KERNEL":
                kernel_module_path = module_path
            pass
        pass

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

    for fn in ('envd',
               'runRemote.sh',
               'runRemoteSSL.sh',
               '.bashrc',
               ):
        virtual_salome.symlink( os.path.join( appliskel_dir, fn ), os.path.join( home_dir, fn) )
        pass

    # Copy launcher and make it executable
    name = _config.get(runner_tag)
    if not name:
        name = DEFAULT_LAUNCHER_NAME
    launcher = appli_dir / name
    shutil.copyfile(APPLISKEL_DIR / "appli_launcher", launcher)
    mode = os.stat(launcher).st_mode
    mode |= (mode & 0o444) >> 2  # copy R bits to X
    os.chmod(launcher, mode)

    # Copy utilitaries needed by the launcher
    for script in "parseConfigFile.py", "salomeContext.py", "salomeContextUtils.py":
        shutil.copyfile(CURDIR / script, bin_salome_d / script)

    for module, module_path in _config.get(modules_tag, {}).items():
        module_path = undestdir(module_path)
        print("--- add module", module, module_path)
        json_config[appli_tag][modules_tag][module] = module_path

    for extra_test, extra_test_path in _config.get(extra_tests_tag, {}).items():
        extra_test_path = undestdir(extra_test_path)
        print("--- add extra test", extra_test, extra_test_path)
        json_config[appli_tag][extra_tests_tag].append(extra_test_path)

    config_file = absolute_appli_path / "config_appli.xml"
    if from_json:
        config_file = absolute_appli_path / "config_appli.json"
    if filename != config_file:
        shutil.copyfile(filename, config_file)

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
        with open(os.path.join(home_dir, 'env_modules.json'), 'w') as fd:
            json.dump({"env_modules": env_modules}, fd)
        with open(os.path.join(home_dir, 'env.d', 'envModules.sh'), 'w') as fd:
            fd.write('#!/bin/bash\n')
            fd.write('module load %s\n' % (' '.join(env_modules)))

    # Copy salome / salome_mesa scripts:
    for scripts in ('getAppliPath.py',
                    'kill_remote_containers.py',
                    '.salome_run',
                    'update_catalogs.py',
                    'salome',
                    'salome_mesa',
                    ):
        salome_script = os.path.join(appliskel_dir, scripts)
        salome_file = os.path.join(home_dir, scripts)
        update_shebang(salome_script, salome_file, home_dir)
        os.chmod(salome_file, 0o755)

    # Add .salome-completion.sh file
    #  shutil.copyfile(APPLISKEL_DIR / ".salome-completion.sh", appli_dir / ".salome-completion.sh")

    if prereq_tag:
        prereq_tag_path = path_with_destdir(_config[prereq_tag])
        if prereq_tag_path.is_file():
            shutil.copyfile(prereq_tag_path, env_d / "envProducts.sh")
        else:
            print(f"WARNING: prerequisite file {prereq_tag_path} does not exist")

    with open(os.path.join(home_dir, 'env.d', 'configSalome.sh'),'w') as f:
        for module in _config.get("modules", []):
            command = 'export '+ module + '_ROOT_DIR=${HOME}/${APPLI}\n'
            f.write(command)
            pass
        if "samples_path" in _config:
            command = 'export DATA_DIR=' + _config["samples_path"] +'\n'
            f.write(command)
            pass
        if "resources_path" in _config and os.path.isfile(_config["resources_path"]):
            command = 'export USER_CATALOG_RESOURCES_FILE=' + os.path.abspath(_config["resources_path"]) +'\n'
            f.write(command)
        command ="""\
export PATH=${HOME}/${APPLI}/bin/salome:$PATH
export LD_LIBRARY_PATH=${HOME}/${APPLI}/lib/salome:$LD_LIBRARY_PATH
"""
        f.write(command)
        # Create environment variable for the salome test
        for module in _config.get("modules", []):
            command = "export LD_LIBRARY_PATH=${HOME}/${APPLI}/bin/salome/test/" + module + "/lib:$LD_LIBRARY_PATH\n"
            f.write(command)
            pass
        # Create environment for plugins GEOM
        command = "export GEOM_PluginsList=BREPPlugin:STEPPlugin:IGESPlugin:STLPlugin:XAOPlugin:VTKPlugin:AdvancedGEOM\n"
        f.write(command)
        # Create environment for Healing
        command = "export CSF_ShHealingDefaults=${HOME}/${APPLI}/share/salome/resources/geom\n"
        f.write(command)
        # Create environment for Meshers
        command = "export SMESH_MeshersList=StdMeshers:HYBRIDPlugin:HexoticPLUGIN:GMSHPlugin:GHS3DPlugin:NETGENPlugin:HEXABLOCKPlugin:BLSURFPlugin:GHS3DPRLPlugin\nexport SALOME_StdMeshersResources=${HOME}/${APPLI}/share/salome/resources/smesh\n"
        f.write(command)
        # Create environment for virtual env
        if venv_directory_path:
            command = """# SALOME venv Configuration
export SALOME_VENV_DIRECTORY=${HOME}/${APPLI}/venv
export PATH=${HOME}/${APPLI}/venv/bin:$PATH
export LD_LIBRARY_PATH=${HOME}/${APPLI}/venv/lib:$LD_LIBRARY_PATH
export PYTHONPATH=${HOME}/${APPLI}/venv/lib/python%s/site-packages
""" % (versionPython)
            f.write(command)
            pass

    if sha1_collect_tag in _config:
        sha1_collect_tag_path = path_with_destdir(_config[sha1_collect_tag])
        if sha1_collect_tag_path.is_file():
            shutil.copyfile(sha1_collect_tag_path, env_d / "sha1_collections.txt")
        else:
            print(f"WARNING: sha1 collections file {sha1_collect_tag_path} does not exist")

    version_python = f"{sys.version_info.major}.{sys.version_info.minor}"
    if python_tag in _config:
        version_python_split = _config[python_tag].split(".")
        version_python = version_python_split[0] + "." + version_python_split[1]
    elif _config.get(prereq_tag):
        cmd = ""
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
        config_salome_lines.append(f"DATA_DIR={undestdir(_config[samples_tag])}\n")
    if resources_tag in _config and Path(_config[resources_tag]).is_file():
        config_salome_lines.append(
            f"USER_CATALOG_RESOURCES_FILE={undestdir(Path(_config[resources_tag]).absolute().as_posix())}\n"
        )

    if config_salome_lines:
        config_salome_lines.insert(0, "[SALOME ROOT_DIR (modules) Configuration]")
        config_salome = Path(env_d / "configSalome.cfg")
        with config_salome.open("w") as f:
            f.write("\n".join(config_salome_lines))

    with open(appli_dir / "config_appli.json", "w") as json_config_file:
        json.dump(json_config, json_config_file, indent=4)

def main():
    parser = argparse.ArgumentParser(usage=usage)

    parser.add_argument('--prefix', default='.', metavar="<install directory>",
                      help="Installation directory (default %(default)s)")

    parser.add_argument('--config', default='config_appli.xml',
                      metavar="<configuration file>",
                      help="XML configuration file (default %(default)s)")

    parser.add_argument('-v', '--verbose', action='count',
                      default=0, help="Increase verbosity")

    options = parser.parse_args()
    if not os.path.exists(options.config):
        print("ERROR: config file %s does not exist. It is mandatory." % options.config)
        sys.exit(1)

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
