#!/usr/bin/env python
"""Create a virtual Salome installation

"""
usage="""usage: %prog [options]
Typical use is:
  python appli_gen.py 
Use with options:
  python appli_gen.py --prefix=<install directory> --config=<configuration file>
"""

import os, glob, string, sys, re
import xml.sax
import optparse
import virtual_salome

# --- names of tags in XML configuration file
appli_tag   = "application"
prereq_tag  = "prerequisites"
modules_tag = "modules"
module_tag  = "module"
samples_tag = "samples"

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

def install(prefix,config_file):
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

    for cle in _config.keys():
        print cle, _config[cle]
        pass

    for module in _config["modules"]:
        print "--- add module ", module, _config[module]
        options = params()
        options.verbose=0
        options.clear=0
        options.prefix=home_dir
        options.module=_config[module]
        virtual_salome.link_module(options)
        pass

    appliskel_dir=os.path.join(home_dir,'bin','salome','appliskel')

    for fn in ('envd',
               'setAppliPath.sh',
               'searchFreePort.sh',
               'runRemote.sh',
               'runAppli',
               'runConsole',
               'runSession',
               'runTests',
               '.bashrc',
               ):
        virtual_salome.symlink(os.path.join(appliskel_dir, fn),os.path.join(home_dir, fn))
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


    f =open(os.path.join(home_dir,'env.d','configSalome.sh'),'w')
    for module in _config["modules"]:
        command='export '+ module + '_ROOT_DIR=' + home_dir +'\n'
        f.write(command)
        pass
    if _config.has_key("samples_path"):
        command='export DATA_DIR=' + _config["samples_path"] +'\n'
        f.write(command)
        pass
    f.close()


    f =open(os.path.join(home_dir,'env.d','configGUI.sh'),'w')
    command = 'export SalomeAppConfig=' + home_dir +'\n'
    f.write(command)
    command = 'export SUITRoot=' + os.path.join(home_dir,'share','salome') +'\n'
    f.write(command)
    f.write('export DISABLE_FPE=1\n')
    f.write('export MMGT_REENTRANT=1\n')
    f.close()


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
    <parameter name="modules"    value="""
    f.write(command)    
    f.write('"')
    for module in _config["guimodules"][:-1]:
        f.write(module)
        f.write(',')
        pass
    if len(_config["guimodules"]) > 0:
      f.write(_config["guimodules"][-1])
    f.write('"/>')
    command="""
    <parameter name="pyModules"  value=""/>
    <parameter name="embedded"   value="SalomeAppEngine,study,cppContainer,registry,moduleCatalog"/>
    <parameter name="standalone" value="pyContainer,supervContainer"/>
  </section>
</document>
"""
    f.write(command)    
    f.close()

def main():
    parser = optparse.OptionParser(usage=usage)

    parser.add_option('--prefix', dest="prefix", default='.',
                      help="Installation directory (default .)")

    parser.add_option('--config', dest="config", default='config_appli.xml',
                      help="XML configuration file (default config_appli.xml)")

    options, args = parser.parse_args()
    install(prefix=options.prefix,config_file=options.config)
    pass

# -----------------------------------------------------------------------------

if __name__ == '__main__':
    main()
    pass
