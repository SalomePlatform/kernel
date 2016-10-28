#! /usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

# --- names of tags in XML configuration file
appli_tag   = "application"
context_tag = "context"
sha1_collect_tag = "sha1_collections"
modules_tag = "modules"
module_tag  = "module"
samples_tag = "samples"
extra_tests_tag = "extra_tests"
extra_test_tag = "extra_test"
resources_tag = "resources"

# --- names of attributes in XML configuration file
nam_att  = "name"
path_att = "path"
gui_att  = "gui"

# -----------------------------------------------------------------------------

# --- xml reader for SALOME application configuration file

class xml_parser:
    def __init__(self, fileName ):
        print "Configure parser: processing %s ..." % fileName
        self.space = []
        self.config = {}
        self.config["modules"] = []
        self.config["guimodules"] = []
        self.config["extra_tests"] = []
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
        # --- if we are analyzing "context" element then store its "path" attribute
        if self.space == [appli_tag, context_tag] and path_att in attrs.getNames():
            self.config["context_path"] = attrs.getValue( path_att )
            pass
        # --- if we are analyzing "sha1_collection" element then store its "path" attribute
        if self.space == [appli_tag, sha1_collect_tag] and path_att in attrs.getNames():
            self.config["sha1_collect_path"] = attrs.getValue( path_att )
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
    except xml.sax.SAXParseException, inst:
        print inst.getMessage()
        print "Configure parser: parse error in configuration file %s" % filename
        pass
    except xml.sax.SAXException, inst:
        print inst.args
        print "Configure parser: error in configuration file %s" % filename
        pass
    except:
        print "Configure parser: Error : can not read configuration file %s, check existence and rights" % filename
        pass

    if verbose:
        for cle,val in _config.items():
            print cle, val
            pass

    # Remove CTestTestfile.cmake; this file will be filled by successive calls to link_module and link_extra_test
    try:
      ctest_file = os.path.join(home_dir, 'bin', 'salome', 'test', "CTestTestfile.cmake")
      os.remove(ctest_file)
    except:
      pass

    for module in _config.get("modules", []):
        if _config.has_key(module):
            print "--- add module ", module, _config[module]
            options = params()
            options.verbose = verbose
            options.clear = 0
            options.prefix = home_dir
            options.module_name = module
            options.module_path = _config[module]
            virtual_salome.link_module(options)
            pass
        pass

    for extra_test in _config.get("extra_tests", []):
        if _config.has_key(extra_test):
            print "--- add extra test ", extra_test, _config[extra_test]
            options = params()
            options.verbose = verbose
            options.clear = 0
            options.prefix = home_dir
            options.extra_test_name = extra_test
            options.extra_test_path = _config[extra_test]
            virtual_salome.link_extra_test(options)
            pass
        pass

    # Generate CTestCustom.cmake to handle long output
    ctest_custom = os.path.join(home_dir, 'bin', 'salome', 'test', "CTestCustom.cmake")
    with open(ctest_custom, 'w') as f:
      f.write("SET(CTEST_CUSTOM_MAXIMUM_PASSED_TEST_OUTPUT_SIZE 1048576) # 1MB\n")
      f.write("SET(CTEST_CUSTOM_MAXIMUM_FAILED_TEST_OUTPUT_SIZE 1048576) # 1MB\n")

    appliskel_dir = os.path.join(home_dir, 'bin', 'salome', 'appliskel')

    for fn in ('envd',
               'getAppliPath.py',
               'kill_remote_containers.py',
               'runRemote.sh',
               'salome',
               'update_catalogs.py',
               '.bashrc',
               ):
        virtual_salome.symlink( os.path.join( appliskel_dir, fn ), os.path.join( home_dir, fn) )
        pass

    if filename != os.path.join(home_dir,"config_appli.xml"):
        shutil.copyfile(filename, os.path.join(home_dir,"config_appli.xml"))
        pass


    # Add .salome-completion.sh file
    shutil.copyfile(os.path.join(appliskel_dir, ".salome-completion.sh"),
                    os.path.join(home_dir, ".salome-completion.sh"))


    # Creation of env.d directory
    virtual_salome.mkdir(os.path.join(home_dir,'env.d'))

    if _config.has_key("context_path") and os.path.isfile(_config["context_path"]):
        shutil.copyfile(_config["context_path"],
                        os.path.join(home_dir, 'env.d', 'envProducts.cfg'))
        pass
    else:
        print "WARNING: context file does not exist"
        pass

    if _config.has_key("sha1_collect_path") and os.path.isfile(_config["sha1_collect_path"]):
        shutil.copyfile(_config["sha1_collect_path"],
                        os.path.join(home_dir, 'sha1_collections.txt'))
        pass
    else:
        print "WARNING: context file does not exist"
        pass

    # Create configuration file: configSalome.cfg
    with open(os.path.join(home_dir, 'env.d', 'configSalome.cfg'),'w') as f:
        command = "[SALOME ROOT_DIR (modules) Configuration]\n"
        f.write(command)
        for module in _config.get("modules", []):
            command = module + '_ROOT_DIR=${HOME}/${APPLI}\n'
            f.write(command)
            pass
        if _config.has_key("samples_path"):
            command = 'DATA_DIR=' + _config["samples_path"] +'\n'
            f.write(command)
            pass
        if _config.has_key("resources_path") and os.path.isfile(_config["resources_path"]):
            command = 'USER_CATALOG_RESOURCES_FILE=' + os.path.abspath(_config["resources_path"]) +'\n'
            f.write(command)

    # Create configuration file: configGUI.cfg
    with open(os.path.join(home_dir, 'env.d', 'configGUI.cfg'),'w') as f:
        command = """[SALOME GUI Configuration]
SalomeAppConfig=${HOME}/${APPLI}
SUITRoot=${HOME}/${APPLI}/share/salome
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
    os.chmod(users_dir, 0777)

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
        print "ERROR: config file %s does not exist. It is mandatory." % options.config
        sys.exit(1)

    install(prefix=options.prefix, config_file=options.config, verbose=options.verbose)
    pass

# -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()
    pass
