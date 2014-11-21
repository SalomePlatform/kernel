#! /usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
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
# See http://www.salome-platform.org or email : webmaster.salome@opencascade.com
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

import argparse
import os
import sys
import shutil
import virtual_salome
import xml.sax

# --- names of tags in XML configuration file
appli_tag   = "application"
prereq_tag  = "prerequisites"
context_tag = "context"
system_conf_tag  = "system_conf"
modules_tag = "modules"
module_tag  = "module"
samples_tag = "samples"
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

    for module in _config.get("modules", []):
        if _config.has_key(module):
            print "--- add module ", module, _config[module]
            options = params()
            options.verbose = verbose
            options.clear = 0
            options.prefix = home_dir
            options.module = _config[module]
            virtual_salome.link_module(options)
            pass
        pass

    appliskel_dir = os.path.join(home_dir, 'bin', 'salome', 'appliskel')

    for fn in ('envd',
               'getAppliPath.py',
               'kill_remote_containers.py',
               'runAppli',           # OBSOLETE (replaced by salome)
               'runConsole',         # OBSOLETE (replaced by salome)
               'runRemote.sh',
               'runSalomeScript',
               'runSession',         # OBSOLETE (replaced by salome)
               'salome',
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

    if _config.has_key("prereq_path") and os.path.isfile(_config["prereq_path"]):
        shutil.copyfile(_config["prereq_path"], 
                        os.path.join(home_dir, 'env.d', 'envProducts.sh'))
        pass
    else:
        print "WARNING: prerequisite file does not exist"
        pass

    if _config.has_key("context_path") and os.path.isfile(_config["context_path"]):
        shutil.copyfile(_config["context_path"],
                        os.path.join(home_dir, 'env.d', 'envProducts.cfg'))
        pass
    else:
        print "WARNING: context file does not exist"
        pass

    if _config.has_key("system_conf_path") and os.path.isfile(_config["system_conf_path"]):
        shutil.copyfile(_config["system_conf_path"], 
                        os.path.join(home_dir, 'env.d', 'envConfSystem.sh'))
        pass


    # Create environment file: configSalome.sh
    with open(os.path.join(home_dir, 'env.d', 'configSalome.sh'),'w') as f:
        for module in _config.get("modules", []):
            command = 'export '+ module + '_ROOT_DIR=${HOME}/${APPLI}\n'
            f.write(command)
            pass
        if _config.has_key("samples_path"):
            command = 'export DATA_DIR=' + _config["samples_path"] +'\n'
            f.write(command)
            pass
        if _config.has_key("resources_path") and os.path.isfile(_config["resources_path"]):
            command = 'export USER_CATALOG_RESOURCES_FILE=' + os.path.abspath(_config["resources_path"]) +'\n'
            f.write(command)

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


    # Create environment file: configGUI.sh
    with open(os.path.join(home_dir, 'env.d', 'configGUI.sh'),'w') as f:
        command = """export SalomeAppConfig=${HOME}/${APPLI}
export SUITRoot=${HOME}/${APPLI}/share/salome
export DISABLE_FPE=1
export MMGT_REENTRANT=1
"""
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
        print "ERROR: config file %s does not exist. It is mandatory." % options.config
        sys.exit(1)

    install(prefix=options.prefix, config_file=options.config, verbose=options.verbose)
    pass

# -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()
    pass
