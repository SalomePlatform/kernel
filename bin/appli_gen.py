#! /usr/bin/env python3
# Copyright (C) 2007-2023  CEA, EDF, OPEN CASCADE
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

## \file appli_gen.py
#  Create a %SALOME application (virtual Salome installation)
#
usage = """%(prog)s [options]
Typical use is:
  python %(prog)s
Typical use with options is:
  python %(prog)s --verbose --prefix=<install directory> --config=<configuration file>
"""

import os
import sys
import shutil
import virtual_salome
import xml.sax
import optparse
import subprocess

# --- names of tags in XML configuration file
appli_tag   = "application"
prereq_tag  = "prerequisites"
context_tag = "context"
venv_directory_tag = "venv_directory"
sha1_collect_tag = "sha1_collections"
system_conf_tag  = "system_conf"
modules_tag = "modules"
module_tag  = "module"
samples_tag = "samples"
extra_tests_tag = "extra_tests"
extra_test_tag = "extra_test"
resources_tag = "resources"
env_modules_tag = "env_modules"
env_module_tag = "env_module"
python_tag = "python"

# --- names of attributes in XML configuration file
nam_att  = "name"
path_att = "path"
gui_att  = "gui"
version_att = "version"

# -----------------------------------------------------------------------------

# --- xml reader for SALOME application configuration file

class xml_parser:
    def __init__(self, fileName ):
        print("Configure parser: processing %s ..." % fileName)
        self.space = []
        self.config = {}
        self.config["modules"] = []
        self.config["guimodules"] = []
        self.config["extra_tests"] = []
        self.config["env_modules"] = []
        parser = xml.sax.make_parser()
        parser.setContentHandler(self)
        parser.parse(fileName)
        pass

    def boolValue( self, text):
        if text in ("yes", "y", "1"):
            return 1
        elif text in ("no", "n", "0"):
            return 0
        else:
            return text
        pass

    def startElement(self, name, attrs):
        self.space.append(name)
        self.current = None
        # --- if we are analyzing "prerequisites" element then store its "path" attribute
        if self.space == [appli_tag, prereq_tag] and path_att in attrs.getNames():
            self.config["prereq_path"] = attrs.getValue( path_att )
            pass
        # --- if we are analyzing "context" element then store its "path" attribute
        if self.space == [appli_tag, context_tag] and path_att in attrs.getNames():
            self.config["context_path"] = attrs.getValue( path_att )
            pass
        # --- if we are analyzing "venv_directory" element then store its "path" attribute
        if self.space == [appli_tag, venv_directory_tag] and path_att in attrs.getNames():
            self.config["venv_directory_path"] = attrs.getValue( path_att )
            pass
        # --- if we are analyzing "sha1_collection" element then store its "path" attribute
        if self.space == [appli_tag, sha1_collect_tag] and path_att in attrs.getNames():
            self.config["sha1_collect_path"] = attrs.getValue( path_att )
            pass
        # --- if we are analyzing "python" element then store its "version" attribute
        if self.space == [appli_tag, python_tag] and version_att in attrs.getNames():
            self.config["python_version"] = attrs.getValue( version_att )
            pass
        # --- if we are analyzing "system_conf" element then store its "path" attribute
        if self.space == [appli_tag, system_conf_tag] and path_att in attrs.getNames():
            self.config["system_conf_path"] = attrs.getValue( path_att )
            pass
        # --- if we are analyzing "resources" element then store its "path" attribute
        if self.space == [appli_tag, resources_tag] and path_att in attrs.getNames():
            self.config["resources_path"] = attrs.getValue( path_att )
            pass
        # --- if we are analyzing "samples" element then store its "path" attribute
        if self.space == [appli_tag, samples_tag] and path_att in attrs.getNames():
            self.config["samples_path"] = attrs.getValue( path_att )
            pass
        # --- if we are analyzing "module" element then store its "name" and "path" attributes
        elif self.space == [appli_tag,modules_tag,module_tag] and \
            nam_att in attrs.getNames() and \
            path_att in attrs.getNames():
            nam = attrs.getValue( nam_att )
            path = attrs.getValue( path_att )
            gui = 1
            if gui_att in attrs.getNames():
                gui = self.boolValue(attrs.getValue( gui_att ))
                pass
            self.config["modules"].append(nam)
            self.config[nam]=path
            if gui:
                self.config["guimodules"].append(nam)
                pass
            pass
        # --- if we are analyzing "env_module" element then store its "name" attribute
        elif self.space == [appli_tag, env_modules_tag, env_module_tag] and \
                nam_att in attrs.getNames():
            nam = attrs.getValue( nam_att )
            self.config["env_modules"].append(nam)
            pass
        # --- if we are analyzing "extra_test" element then store its "name" and "path" attributes
        elif self.space == [appli_tag,extra_tests_tag,extra_test_tag] and \
            nam_att in attrs.getNames() and \
            path_att in attrs.getNames():
            nam = attrs.getValue( nam_att )
            path = attrs.getValue( path_att )
            self.config["extra_tests"].append(nam)
            self.config[nam]=path
            pass
        pass

    def endElement(self, name):
        self.space.pop()
        self.current = None
        pass

    def characters(self, content):
        pass

    def processingInstruction(self, target, data):
        pass

    def setDocumentLocator(self, locator):
        pass

    def startDocument(self):
        self.read = None
        pass

    def endDocument(self):
        self.read = None
        pass

# -----------------------------------------------------------------------------

class params:
    pass

# -----------------------------------------------------------------------------

def makedirs(namedir):
  if os.path.exists(namedir):
    dirbak = namedir+".bak"
    if os.path.exists(dirbak):
      shutil.rmtree(dirbak)
    os.rename(namedir, dirbak)
    os.listdir(dirbak) #sert seulement a mettre a jour le systeme de fichier sur certaines machines
  os.makedirs(namedir)

def install(prefix, config_file, verbose=0):
    home_dir = os.path.abspath(os.path.expanduser(prefix))
    filename = os.path.abspath(os.path.expanduser(config_file))
    _config = {}
    try:
        parser = xml_parser(filename)
        _config = parser.config
    except xml.sax.SAXParseException as inst:
        print(inst.getMessage())
        print("Configure parser: parse error in configuration file %s" % filename)
        pass
    except xml.sax.SAXException as inst:
        print(inst.args)
        print("Configure parser: error in configuration file %s" % filename)
        pass
    except Exception:
        print("Configure parser: Error : can not read configuration file %s, check existence and rights" % filename)
        pass

    if verbose:
        for cle,val in _config.items():
            print(cle, val)
            pass

    # Remove CTestTestfile.cmake; this file will be filled by successive calls to link_module and link_extra_test
    try:
      ctest_file = os.path.join(home_dir, 'bin', 'salome', 'test', "CTestTestfile.cmake")
      os.remove(ctest_file)
    except Exception:
      pass

    for module in _config.get("modules", []):
        if module in _config:
            print("--- add module ", module, _config[module])
            options = params()
            options.verbose = verbose
            options.clear = 0
            options.prefix = home_dir
            options.module_name = module
            options.module_path = _config[module]
            virtual_salome.link_module(options)
            # To fix GEOM_TestXAO issue https://codev-tuleap.cea.fr/plugins/tracker/?aid=16599
            if module == "GEOM":
                # link <appli_path>/bin/salome/test/<module> to <module_path>/bin/salome/test
                test_dir=os.path.join(home_dir,'bin','salome', 'test')
                module_dir=os.path.abspath(options.module_path)
                xao_link=os.path.join(module_dir,'bin','salome', 'test', "xao")
                print("link %s --> %s"%(os.path.join(test_dir, "xao"), xao_link))
                virtual_salome.symlink(xao_link, os.path.join(test_dir, "xao"))
            pass
        pass

    for extra_test in _config.get("extra_tests", []):
        if extra_test in _config:
            print("--- add extra test ", extra_test, _config[extra_test])
            options = params()
            options.verbose = verbose
            options.clear = 0
            options.prefix = home_dir
            options.extra_test_name = extra_test
            options.extra_test_path = _config[extra_test]
            virtual_salome.link_extra_test(options)
            pass
        pass

    # Sort test labels by name in generated CTestTestfile.cmake
    with open(ctest_file) as f:
        lines = f.readlines()
    lines.sort()
    with open(ctest_file, "w") as f:
        f.write("".join(lines))

    # Generate CTestCustom.cmake to handle long output
    ctest_custom = os.path.join(home_dir, 'bin', 'salome', 'test', "CTestCustom.cmake")
    with open(ctest_custom, 'w') as f:
      f.write("SET(CTEST_CUSTOM_MAXIMUM_PASSED_TEST_OUTPUT_SIZE 1048576) # 1MB\n")
      f.write("SET(CTEST_CUSTOM_MAXIMUM_FAILED_TEST_OUTPUT_SIZE 1048576) # 1MB\n")

    appliskel_dir = os.path.join(prefix, 'bin', 'salome', 'appliskel')

    for fn in ('envd',
               'getAppliPath.py',
               'kill_remote_containers.py',
               'runRemote.sh',
               'runRemoteSSL.sh',
               '.salome_run',
               'update_catalogs.py',
               '.bashrc',
               ):
        virtual_salome.symlink( os.path.join( appliskel_dir, fn ), os.path.join( home_dir, fn) )
        pass

    if filename != os.path.join(home_dir,"config_appli.xml"):
        shutil.copyfile(filename, os.path.join(home_dir,"config_appli.xml"))
        pass

    # Creation of env.d directory
    virtual_salome.mkdir(os.path.join(home_dir,'env.d'))

    venv_directory_path = _config.get('venv_directory_path')
    if venv_directory_path and os.path.isdir(venv_directory_path):
        virtual_salome.symlink(venv_directory_path, os.path.join(home_dir, "venv"))

    # Get the env modules which will be loaded
    # In the same way as: module load [MODULE_LIST]
    env_modules = _config.get('env_modules', [])
    if env_modules:
        with open(os.path.join(home_dir, 'env.d', 'envModules.sh'), 'w') as fd:
            fd.write('#!/bin/bash\n')
            fd.write('module load %s\n' % (' '.join(env_modules)))

    # Copy salome / salome_mesa scripts:

    for scripts in ('salome', 'salome_mesa', 'salome_common.py'):
        salome_script = open(os.path.join(appliskel_dir, scripts)).read()
        salome_file = os.path.join(home_dir, scripts)
        try:
            os.remove(salome_file)
        except Exception:
            pass
        with open(salome_file, 'w') as fd:
            fd.write(salome_script.replace('MODULES = []', 'MODULES = {}'.format(env_modules)))
            os.chmod(salome_file, 0o755)

    # Add .salome-completion.sh file
    shutil.copyfile(os.path.join(appliskel_dir, ".salome-completion.sh"),
                    os.path.join(home_dir, ".salome-completion.sh"))

    if "prereq_path" in _config and os.path.isfile(_config["prereq_path"]):
        shutil.copyfile(_config["prereq_path"],
                        os.path.join(home_dir, 'env.d', 'envProducts.sh'))
        pass
    else:
        print("WARNING: prerequisite file does not exist")
        pass

    if "context_path" in _config and os.path.isfile(_config["context_path"]):
        shutil.copyfile(_config["context_path"],
                        os.path.join(home_dir, 'env.d', 'envProducts.cfg'))
        pass
    else:
        print("WARNING: context file does not exist")
        pass

    if "sha1_collect_path" in _config and os.path.isfile(_config["sha1_collect_path"]):
        shutil.copyfile(_config["sha1_collect_path"],
                        os.path.join(home_dir, 'sha1_collections.txt'))
        pass
    else:
        print("WARNING: sha1 collections file does not exist")
        pass

    if "system_conf_path" in _config and os.path.isfile(_config["system_conf_path"]):
        shutil.copyfile(_config["system_conf_path"],
                        os.path.join(home_dir, 'env.d', 'envConfSystem.sh'))
        pass

    # Create environment file: configSalome.sh

    if "python_version" in _config:
       versionPython_split = _config["python_version"].split('.')
       versionPython = versionPython_split[0] + "." + versionPython_split[1]
    else:
       cmd='source %s && python3 -c "import sys ; sys.stdout.write(\\"{}.{}\\".format(sys.version_info.major,sys.version_info.minor))"' %(_config["prereq_path"])
       versionPython=subprocess.check_output(['/bin/bash', '-l' ,'-c',cmd]).decode("utf-8")

    venv_directory_path = None
    if "venv_directory_path" in _config:
        venv_directory_path = _config["venv_directory_path"]
        venv_bin_directory_path = os.path.join(venv_directory_path, 'bin')
        venv_pip_executable = os.path.join(venv_bin_directory_path, 'pip')
        venv_python_executable = os.path.join(venv_bin_directory_path, 'python')
        if os.path.isdir(venv_directory_path) and os.path.isfile(venv_pip_executable):
            requirement_file = os.path.join(home_dir, 'requirements.txt')
            with open(requirement_file, 'w') as fd:
                subprocess.call([venv_python_executable, '-m', 'pip', 'freeze'], stdout=fd)
        else:
            venv_directory_path = None

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
        # Note: below, PYTHONPATH should not be extended to bin/salome! Python modules must be installed in lib/pythonX.Y, to be fixed (e.g. Kernel SALOME_Container.py)
        command ="""export PATH=${HOME}/${APPLI}/bin/salome:$PATH
export PYTHONPATH=${HOME}/${APPLI}/lib/python%s/site-packages/salome:$PYTHONPATH
export PYTHONPATH=${HOME}/${APPLI}/lib/salome:$PYTHONPATH
export PYTHONPATH=${HOME}/${APPLI}/bin/salome:$PYTHONPATH
export LD_LIBRARY_PATH=${HOME}/${APPLI}/lib/salome:$LD_LIBRARY_PATH
""" %versionPython
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

    # Create configuration file: configSalome.cfg
    with open(os.path.join(home_dir, 'env.d', 'configSalome.cfg'),'w') as f:
        command = "[SALOME ROOT_DIR (modules) Configuration]\n"
        f.write(command)
        for module in _config.get("modules", []):
            command = module + '_ROOT_DIR=${HOME}/${APPLI}\n'
            f.write(command)
            pass
        if "samples_path" in _config:
            command = 'DATA_DIR=' + _config["samples_path"] +'\n'
            f.write(command)
            pass
        if "resources_path" in _config and os.path.isfile(_config["resources_path"]):
            command = 'USER_CATALOG_RESOURCES_FILE=' + os.path.abspath(_config["resources_path"]) +'\n'
            f.write(command)
        command ="""ADD_TO_PATH: ${HOME}/${APPLI}/bin/salome
ADD_TO_PYTHONPATH: ${HOME}/${APPLI}/lib/python%s/site-packages/salome
ADD_TO_PYTHONPATH: ${HOME}/${APPLI}/lib/salome
ADD_TO_LD_LIBRARY_PATH: ${HOME}/${APPLI}/lib/salome
"""%versionPython
        f.write(command)
        for module in _config.get("modules", []):
            command = "ADD_TO_LD_LIBRARY_PATH: ${HOME}/${APPLI}/bin/salome/test/" + module + "/lib\n"
            f.write(command)
            pass
        # Create environment for plugins GEOM
        command = "GEOM_PluginsList=BREPPlugin:STEPPlugin:IGESPlugin:STLPlugin:XAOPlugin:VTKPlugin:AdvancedGEOM\n"
        f.write(command)
        # Create environment for Healing
        command = "CSF_ShHealingDefaults=${HOME}/${APPLI}/share/salome/resources/geom\n"
        f.write(command)
        # Create environment for Meshers
        command = "SMESH_MeshersList=StdMeshers:HYBRIDPlugin:HexoticPLUGIN:GMSHPlugin:GHS3DPlugin:NETGENPlugin:HEXABLOCKPlugin:BLSURFPlugin:GHS3DPRLPlugin\nSALOME_StdMeshersResources=${HOME}/${APPLI}/share/salome/resources/smesh\n"
        f.write(command)
        # Create environment for virtual env
        if venv_directory_path:
            command = """[SALOME venv Configuration]
SALOME_VENV_DIRECTORY: ${HOME}/${APPLI}/venv
ADD_TO_PATH: ${HOME}/${APPLI}/venv/bin
ADD_TO_LD_LIBRARY_PATH: ${HOME}/${APPLI}/venv/lib
ADD_TO_PYTHONPATH: ${HOME}/${APPLI}/venv/lib/python%s/site-packages
""" % (versionPython)
            f.write(command)
            pass

    # Create environment file: configGUI.sh
    dirs_ress_icon = []
    salomeappname  = "SalomeApp"
    with open(os.path.join(home_dir, 'env.d', 'configGUI.sh'),'w') as f:
        for module in _config.get("modules", []):
            if module not in ["KERNEL", "GUI", ""]:
                d = os.path.join(_config[module],"share","salome","resources",module.lower())
                d_appli = os.path.join("${HOME}","${APPLI}","share","salome","resources",module.lower())
                if os.path.exists( os.path.join(d,"{0}.xml".format(salomeappname)) ):
                   dirs_ress_icon.append( d_appli )
        AppConfig="export SalomeAppConfig=${HOME}/${APPLI}:${HOME}/${APPLI}/share/salome/resources/gui/"
        for dir_module in dirs_ress_icon:
             AppConfig=AppConfig+":"+dir_module
        f.write(AppConfig+"\n")
        command = """export SUITRoot=${HOME}/${APPLI}/share/salome
export DISABLE_FPE=1
export MMGT_REENTRANT=1
"""
        f.write(command)

    # Create configuration file: configGUI.cfg
    dirs_ress_icon = []
    with open(os.path.join(home_dir, 'env.d', 'configGUI.cfg'),'w') as f:
        command = """[SALOME GUI Configuration]\n"""
        f.write(command)
        for module in _config.get("modules", []):
            if module not in ["KERNEL", "GUI", ""]:
                d = os.path.join(_config[module],"share","salome","resources",module.lower())
                d_appli = os.path.join("${HOME}","${APPLI}","share","salome","resources",module.lower())
                if os.path.exists( os.path.join(d,"{0}.xml".format(salomeappname)) ):
                   dirs_ress_icon.append( d_appli )
        AppConfig="SalomeAppConfig=${HOME}/${APPLI}:${HOME}/${APPLI}/share/salome/resources/gui/"
        for dir_module in dirs_ress_icon:
             AppConfig=AppConfig+":"+dir_module
        f.write(AppConfig+"\n")
        command = """SUITRoot=${HOME}/${APPLI}/share/salome
DISABLE_FPE=1
MMGT_REENTRANT=1
"""
        f.write(command)

    #SalomeApp.xml file
    with open(os.path.join(home_dir,'SalomeApp.xml'),'w') as f:
        command = """<document>
  <section name="launch">
    <!-- SALOME launching parameters -->
    <parameter name="gui"        value="yes"/>
    <parameter name="splash"     value="yes"/>
    <parameter name="file"       value="no"/>
    <parameter name="key"        value="no"/>
    <parameter name="interp"     value="no"/>
    <parameter name="logger"     value="no"/>
    <parameter name="xterm"      value="no"/>
    <parameter name="portkill"   value="no"/>
    <parameter name="killall"    value="no"/>
    <parameter name="noexcepthandler"  value="no"/>
    <parameter name="modules"    value="%s"/>
    <parameter name="pyModules"  value=""/>
    <parameter name="embedded"   value="SalomeAppEngine,study,cppContainer,registry,moduleCatalog"/>
    <parameter name="standalone" value=""/>
  </section>
</document>
"""
        mods = []
        #Keep all modules except KERNEL and GUI
        for module in _config.get("modules", []):
            if module in ("KERNEL","GUI"):
                continue
            mods.append(module)
        f.write(command % ",".join(mods))

    #Add USERS directory with 777 permission to store users configuration files
    users_dir = os.path.join(home_dir,'USERS')
    makedirs(users_dir)
    os.chmod(users_dir, 0o777)

def main():
    parser = optparse.OptionParser(usage=usage)

    parser.add_option('--prefix', dest="prefix", default='.',
                      help="Installation directory (default .)")

    parser.add_option('--config', dest="config", default='config_appli.xml',
                      help="XML configuration file (default config_appli.xml)")

    parser.add_option('-v', '--verbose', action='count', dest='verbose',
                      default=0, help="Increase verbosity")

    options, args = parser.parse_args()
    if not os.path.exists(options.config):
        print("ERROR: config file %s does not exist. It is mandatory." % options.config)
        sys.exit(1)

    install(prefix=options.prefix, config_file=options.config, verbose=options.verbose)
    pass

# -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()
    pass
