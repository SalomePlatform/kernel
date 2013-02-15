#! /usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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
usage="""usage: %prog [options]
Typical use is:
  python appli_gen.py
Typical use with options is:
  python appli_gen.py --verbose --prefix=<install directory> --config=<configuration file>
"""

import os, glob, string, sys, re
import shutil
import xml.sax
import optparse
import virtual_salome

# --- names of tags in XML configuration file
appli_tag   = "application"
prereq_tag  = "prerequisites"
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

    def boolValue( self, str ):
        if str in ("yes", "y", "1"):
            return 1
        elif str in ("no", "n", "0"):
            return 0
        else:
            return str
        pass

    def startElement(self, name, attrs):
        self.space.append(name)
        self.current = None
        # --- if we are analyzing "prerequisites" element then store its "path" attribute
        if self.space == [appli_tag, prereq_tag] and path_att in attrs.getNames():
            self.config["prereq_path"] = attrs.getValue( path_att )
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
        p = self.space.pop()
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
    dirbak=namedir+".bak"
    if os.path.exists(dirbak):
      shutil.rmtree(dirbak)
    os.rename(namedir,dirbak)
    os.listdir(dirbak) #sert seulement a mettre a jour le systeme de fichier sur certaines machines
  os.makedirs(namedir)

def install(prefix,config_file,verbose=0):
    home_dir=os.path.abspath(os.path.expanduser(prefix))
    filename=os.path.abspath(os.path.expanduser(config_file))
    _config={}
    try:
        p = xml_parser(filename)
        _config = p.config
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
        for cle in _config.keys():
            print cle, _config[cle]
            pass

    for module in _config["modules"]:
        print "--- add module ", module, _config[module]
        options = params()
        options.verbose=verbose
        options.clear=0
        options.prefix=home_dir
        options.module=_config[module]
        virtual_salome.link_module(options)
        pass

    appliskel_dir=os.path.join(home_dir,'bin','salome','appliskel')

    for fn in ('envd',
               'getAppliPath.py',
               'searchFreePort.sh',
               'runRemote.sh',
               'runAppli',
               'runConsole',
               'runSession',
               'runSalomeScript',
               'runTests',
               'update_catalogs.py',
               'kill_remote_containers.py',
               '.bashrc',
               ):
        virtual_salome.symlink("./bin/salome/appliskel/"+fn,os.path.join(home_dir, fn))
        pass

    if filename != os.path.join(home_dir,"config_appli.xml"):
        command = "cp -p " + filename + ' ' + os.path.join(home_dir,"config_appli.xml")
        os.system(command)
        pass

    virtual_salome.mkdir(os.path.join(home_dir,'env.d'))
    if os.path.isfile(_config["prereq_path"]):
        command='cp -p ' + _config["prereq_path"] + ' ' + os.path.join(home_dir,'env.d','envProducts.sh')
        os.system(command)
        pass
    else:
        print "WARNING: prerequisite file does not exist"
        pass

    #environment file: configSalome.sh
    f =open(os.path.join(home_dir,'env.d','configSalome.sh'),'w')
    for module in _config["modules"]:
        command='export '+ module + '_ROOT_DIR=${HOME}/${APPLI}\n'
        f.write(command)
        pass
    if _config.has_key("samples_path"):
        command='export DATA_DIR=' + _config["samples_path"] +'\n'
        f.write(command)
        pass
    if _config.has_key("resources_path") and os.path.isfile(_config["resources_path"]):
        command='export USER_CATALOG_RESOURCES_FILE=' + os.path.abspath(_config["resources_path"]) +'\n'
        f.write(command)

    f.close()


    #environment file: configGUI.sh
    f =open(os.path.join(home_dir,'env.d','configGUI.sh'),'w')
    command = """export SalomeAppConfig=${HOME}/${APPLI}
export SUITRoot=${HOME}/${APPLI}/share/salome
export DISABLE_FPE=1
export MMGT_REENTRANT=1
"""
    f.write(command)
    f.close()

    #SalomeApp.xml file
    f =open(os.path.join(home_dir,'SalomeApp.xml'),'w')
    command="""<document>
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
    <parameter name="standalone" value="pyContainer"/>
  </section>
</document>
"""
    mods=[]
    #Keep all modules except KERNEL and GUI
    for m in _config["modules"]:
      if m in ("KERNEL","GUI"):continue
      mods.append(m)
    f.write(command % ",".join(mods))
    f.close()

    #Add USERS directory with 777 permission to store users configuration files
    users_dir=os.path.join(home_dir,'USERS')
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

    install(prefix=options.prefix,config_file=options.config,verbose=options.verbose)
    pass

# -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()
    pass
