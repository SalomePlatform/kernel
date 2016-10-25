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

import os, glob, string, sys, re
import xml.sax
import optparse
import types

from salome_utils import verbose, getPortNumber, getHomeDir

# names of tags in XML configuration file
doc_tag = "document"
sec_tag = "section"
par_tag = "parameter"
import_tag = "import"

# names of attributes in XML configuration file
nam_att = "name"
val_att = "value"

# certain values in XML configuration file ("launch" section)
lanch_nam      = "launch"
help_nam       = "help"
gui_nam        = "gui"
splash_nam     = "splash"
logger_nam     = "logger"
xterm_nam      = "xterm"
file_nam       = "file"
portkill_nam   = "portkill"
killall_nam    = "killall"
modules_nam    = "modules"
embedded_nam   = "embedded"
standalone_nam = "standalone"
key_nam        = "key"
terminal_nam   = "terminal"
interp_nam     = "interp"
except_nam     = "noexcepthandler"
terminal_nam   = "terminal"
pinter_nam     = "pinter"
batch_nam      = "batch"
test_nam       = "test"
play_nam       = "play"
lang_nam       = "language"
gdb_session_nam = "gdb_session"
ddd_session_nam = "ddd_session"
valgrind_session_nam = "valgrind_session"
shutdown_servers_nam = "shutdown_servers"
foreground_nam = "foreground"
wake_up_session_nam = "wake_up_session"

# values in XML configuration file giving specific module parameters (<module_name> section)
# which are stored in opts with key <module_name>_<parameter> (eg SMESH_plugins)
plugins_nam    = "plugins"

# values passed as arguments, NOT read from XML config file, but set from within this script
appname_nam    = "appname"
port_nam       = "port"
useport_nam    = "useport"
salomecfgname  = "salome"
salomeappname  = "SalomeApp"
script_nam     = "pyscript"

# possible choices for the "embedded" and "standalone" parameters
embedded_choices   = [ "registry", "study", "moduleCatalog", "cppContainer", "SalomeAppEngine" ]
standalone_choices = [ "registry", "study", "moduleCatalog", "cppContainer"]

# values of boolean type (must be '0' or '1').
# xml_parser.boolValue() is used for correct setting
boolKeys = ( gui_nam, splash_nam, logger_nam, file_nam, xterm_nam, portkill_nam, killall_nam, except_nam, pinter_nam, shutdown_servers_nam )
intKeys = ( interp_nam, )

# values of list type
listKeys = ( embedded_nam, key_nam, modules_nam, standalone_nam, plugins_nam )

###
# Get the application version
# Uses GUI_ROOT_DIR (or KERNEL_ROOT_DIR in batch mode) +/bin/salome/VERSION file
###
def version():
    try:
        filename = None
        root_dir = os.environ.get( 'KERNEL_ROOT_DIR', '' ) # KERNEL_ROOT_DIR or "" if not found
        version_file = os.path.join(root_dir, 'bin', 'salome', 'VERSION')
        if root_dir and os.path.exists( version_file ):
            filename = version_file
        root_dir = os.environ.get( 'GUI_ROOT_DIR', '' )    # GUI_ROOT_DIR "" if not found
        version_file = os.path.join(root_dir, 'bin', 'salome', 'VERSION')
        if root_dir and os.path.exists( version_file ):
            filename = version_file
        if filename:
            str = open( filename, "r" ).readline() # str = "THIS IS SALOME - SALOMEGUI VERSION: 3.0.0"
            match = re.search( r':\s+([a-zA-Z0-9.]+)\s*$', str )
            if match :
                return match.group( 1 )
    except:
        pass
    return ''

###
# Calculate and return configuration file unique ID
# For example: for SALOME version 3.1.0a1 the id is 300999701
###
def version_id(fname):
    major = minor = release = dev1 = dev2 = 0
    vers = fname.split(".")
    if len(vers) > 0:
      try:
        major = int(vers[0])
      except ValueError:
        # If salome version given is DEV, the call to int('DEV') will fail with
        # a ValueError exception
        pass
    try:
      if len(vers) > 1: minor = int(vers[1])
    except ValueError:
      # If salome version given is 7.DEV, the call to int('DEV') will fail with
      # a ValueError exception
      pass
    if len(vers) > 2:
        mr = re.search(r'^([0-9]+)([A-Z]|RC)?([0-9]*)',vers[2], re.I)
        if mr:
            release = int(mr.group(1))
            if mr.group(2):
                tag = mr.group(2).strip().lower()
                if tag == "rc":
                    dev1 = 49 # release candidate
                elif tag:
                    dev1 = ord(tag)-ord('a')+1
                pass
            if mr.group(3):
                dev2 = int(mr.group(3).strip())
            pass
        pass
    dev = dev1 * 100 + dev2
    ver = major
    ver = ver * 100 + minor
    ver = ver * 100 + release
    ver = ver * 10000
    if dev > 0: ver = ver - dev
    return ver

###
# Get default user configuration file name
# For SALOME, it is:
# - on Linux:   ~/.config/salome/SalomeApprc[.<version>]
# - on Windows: ~/SalomeApp.xml[.<version>]
# where <version> is an optional version number
###
def defaultUserFile(appname=salomeappname, cfgname=salomecfgname):
    v = version()
    filetmpl = sys.platform == "win32" and "{0}.xml.{1}" or "{0}rc.{1}"
    paths = []
    paths.append(getHomeDir())
    paths.append(".config")
    if cfgname: paths.append(cfgname)
    paths.append(filetmpl.format(appname, v))
    return os.path.join(*paths)

###
# Get user configuration file name
###
def userFile(appname, cfgname):
    # get app version
    v = version()
    if not v: return None                        # unknown version

    # get default user file name
    filename = defaultUserFile(appname, cfgname)
    if not filename: return None                 # default user file name is bad

    # check that default user file exists
    if os.path.exists(filename): return filename # user file is found

    # otherwise try to detect any appropriate user file

    # ... calculate default version id
    id0 = version_id(v)
    if not id0: return None                      # bad version id -> can't detect appropriate file

    # ... get all existing user preferences files
    filetmpl1 = sys.platform == "win32" and "{0}.xml.*" or "{0}rc.*"
    filetmpl2 = sys.platform == "win32" and filetmpl1 or "." + filetmpl1
    files = []
    if cfgname:
        # Since v6.6.0 - in ~/.config/salome directory, without dot prefix
        files += glob.glob(os.path.join(getHomeDir(), ".config", cfgname, filetmpl1.format(appname)))
        # Since v6.5.0 - in ~/.config/salome directory, dot-prefixed (backward compatibility)
        if filetmpl2 and filetmpl2 != filetmpl1:
            files += glob.glob(os.path.join(getHomeDir(), ".config", cfgname, filetmpl2.format(appname)))
        pass
    # old style (before v6.5.0) - in ~ directory, dot-prefixed
    if filetmpl2 and filetmpl2 != filetmpl1:
        files += glob.glob(os.path.join(getHomeDir(), filetmpl2.format(appname)))
    pass

    # ... loop through all files and find most appopriate file (with closest id)
    appr_id   = -1
    appr_file = ""
    for f in files:
        ff = os.path.basename( f )
        if sys.platform == "win32":
            match = re.search( r'^{0}\.xml\.([a-zA-Z0-9.]+)$'.format(appname), ff )
        else:
            match = re.search( r'^\.?{0}rc\.([a-zA-Z0-9.]+)$'.format(appname), ff )
        if match:
            ver = version_id(match.group(1))
            if not ver: continue                 # bad version id -> skip file
            if appr_id < 0 or abs(appr_id-id0) > abs(ver-id0):
                appr_id   = ver
                appr_file = f
                pass
            elif abs(appr_id-id0) == abs(ver-id0):
                if not os.path.basename(f).startswith("."): appr_file = f
                pass
            pass
        pass
    return appr_file

# --

def process_containers_params( standalone, embedded ):
    # 1. filter inappropriate containers names
    if standalone is not None:
        standalone = filter( lambda x: x in standalone_choices, standalone )
    if embedded is not None:
        embedded   = filter( lambda x: x in embedded_choices,   embedded )

    # 2. remove containers appearing in 'standalone' parameter from the 'embedded'
    # parameter --> i.e. 'standalone' parameter has higher priority
    if standalone is not None and embedded is not None:
        embedded = filter( lambda x: x not in standalone, embedded )

    # 3. return corrected parameters values
    return standalone, embedded

# -----------------------------------------------------------------------------

###
# XML reader for launch configuration file usage
###

section_to_skip = ""

class xml_parser:
    def __init__(self, fileName, _opts, _importHistory):
        #warning _importHistory=[] is NOT good: is NOT empty,reinitialized after first call
        if verbose(): print "Configure parser: processing %s ..." % fileName
        self.fileName = os.path.abspath(fileName)
        self.importHistory = _importHistory
        self.importHistory.append(self.fileName)
        self.space = []
        self.opts = _opts
        self.section = section_to_skip
        parser = xml.sax.make_parser()
        parser.setContentHandler(self)
        parser.parse(fileName)
        standalone, embedded = process_containers_params( self.opts.get( standalone_nam ),
                                                          self.opts.get( embedded_nam ) )
        if standalone is not None:
            self.opts[ standalone_nam ] = standalone
        if embedded is not None:
            self.opts[ embedded_nam ] = embedded
        pass

    def boolValue( self, str ):
        strloc = str
        if isinstance(strloc, types.UnicodeType):
            strloc = strloc.encode().strip()
        if isinstance(strloc, types.StringType):
            strlow = strloc.lower()
            if strlow in   ("1", "yes", "y", "on", "true", "ok"):
                return True
            elif strlow in ("0", "no", "n", "off", "false", "cancel"):
                return False
        return strloc
        pass

    def intValue( self, str ):
        strloc = str
        if isinstance(strloc, types.UnicodeType):
            strloc = strloc.encode().strip()
        if isinstance(strloc, types.StringType):
            strlow = strloc.lower()
            if strlow in   ("1", "yes", "y", "on", "true", "ok"):
                return 1
            elif strlow in ("0", "no", "n", "off", "false", "cancel"):
                return 0
            else:
                return string.atoi(strloc)
        return strloc
        pass

    def strValue( self, str ):
        strloc = str
        try:
            if isinstance(strloc, types.UnicodeType): strloc = strloc.encode().strip()
            else: strloc = strloc.strip()
        except:
            pass
        return strloc

    def startElement(self, name, attrs):
        self.space.append(name)
        self.current = None

        # if we are importing file
        if self.space == [doc_tag, import_tag] and nam_att in attrs.getNames():
            self.importFile( attrs.getValue(nam_att) )

        # if we are analyzing "section" element and its "name" attribute is
        # either "launch" or module name -- set section_name
        if self.space == [doc_tag, sec_tag] and nam_att in attrs.getNames():
            section_name = attrs.getValue( nam_att )
            if section_name in [lanch_nam, lang_nam]:
                self.section = section_name # launch section
            elif self.opts.has_key( modules_nam ) and \
                 section_name in self.opts[ modules_nam ]:
                self.section = section_name # <module> section
            else:
                self.section = section_to_skip # any other section
            pass

        # if we are analyzing "parameter" elements - children of either
        # "section launch" or "section "<module>"" element, then store them
        # in self.opts assiciative array (key = [<module>_ + ] value of "name" attribute)
        elif self.section != section_to_skip           and \
             self.space == [doc_tag, sec_tag, par_tag] and \
             nam_att in attrs.getNames()               and \
             val_att in attrs.getNames():
            nam = attrs.getValue( nam_att )
            val = attrs.getValue( val_att )
            if self.section == lanch_nam: # key for launch section
                key = nam
            else:                         # key for <module> section
                key = self.section + "_" + nam
            key = self.strValue( key )
            if nam in boolKeys:
                self.opts[key] = self.boolValue( val )  # assign boolean value: 0 or 1
            elif nam in intKeys:
                self.opts[key] = self.intValue( val )   # assign integer value
            elif nam in listKeys:
                self.opts[key] = [ self.strValue( a ) for a in re.split( "[:;,]", val ) ] # assign list value: []
            else:
                self.opts[key] = self.strValue( val ) # string value
            pass
        pass

    def endElement(self, name):
        self.space.pop()
        self.current = None
        if self.section != section_to_skip and name == sec_tag:
            self.section = section_to_skip
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

    def importFile(self, fname):
        # get absolute name
        if os.path.isabs (fname) :
            absfname = fname
        else:
            absfname = os.path.join(os.path.dirname(self.fileName), fname)

        # check existing and registry file
        for ext in ["", ".xml", ".XML"] :
            if os.path.exists(absfname + ext) :
                absfname += ext
                if absfname in self.importHistory :
                    if verbose(): print "Configure parser: Warning : file %s is already imported" % absfname
                    return # already imported
                break
            pass
        else:
            if verbose(): print "Configure parser: Error : file %s does not exist" % absfname
            return

        # importing file
        try:
            # copy current options
            import copy
            opts = copy.deepcopy(self.opts)
            # import file
            imp = xml_parser(absfname, opts, self.importHistory)
            # merge results
            for key in imp.opts.keys():
                if not self.opts.has_key(key):
                    self.opts[key] = imp.opts[key]
                    pass
                pass
            pass
        except:
            if verbose(): print "Configure parser: Error : can not read configuration file %s" % absfname
        pass


# -----------------------------------------------------------------------------

booleans = { '1': True , 'yes': True , 'y': True , 'on' : True , 'true' : True , 'ok'     : True,
             '0': False, 'no' : False, 'n': False, 'off': False, 'false': False, 'cancel' : False }

boolean_choices = booleans.keys()

def check_embedded(option, opt, value, parser):
    from optparse import OptionValueError
    assert value is not None
    if parser.values.embedded:
        embedded = filter( lambda a: a.strip(), re.split( "[:;,]", parser.values.embedded ) )
    else:
        embedded = []
    if parser.values.standalone:
        standalone = filter( lambda a: a.strip(), re.split( "[:;,]", parser.values.standalone ) )
    else:
        standalone = []
    vals = filter( lambda a: a.strip(), re.split( "[:;,]", value ) )
    for v in vals:
        if v not in embedded_choices:
            raise OptionValueError( "option %s: invalid choice: %r (choose from %s)" % ( opt, v, ", ".join( map( repr, embedded_choices ) ) ) )
        if v not in embedded:
            embedded.append( v )
            if v in standalone:
                del standalone[ standalone.index( v ) ]
                pass
    parser.values.embedded = ",".join( embedded )
    parser.values.standalone = ",".join( standalone )
    pass

def check_standalone(option, opt, value, parser):
    from optparse import OptionValueError
    assert value is not None
    if parser.values.embedded:
        embedded = filter( lambda a: a.strip(), re.split( "[:;,]", parser.values.embedded ) )
    else:
        embedded = []
    if parser.values.standalone:
        standalone = filter( lambda a: a.strip(), re.split( "[:;,]", parser.values.standalone ) )
    else:
        standalone = []
    vals = filter( lambda a: a.strip(), re.split( "[:;,]", value ) )
    for v in vals:
        if v not in standalone_choices:
            raise OptionValueError( "option %s: invalid choice: %r (choose from %s)" % ( opt, v, ", ".join( map( repr, standalone_choices ) ) ) )
        if v not in standalone:
            standalone.append( v )
            if v in embedded:
                del embedded[ embedded.index( v ) ]
                pass
    parser.values.embedded = ",".join( embedded )
    parser.values.standalone = ",".join( standalone )
    pass

def store_boolean (option, opt, value, parser, *args):
    if isinstance(value, types.StringType):
        try:
            value_conv = booleans[value.strip().lower()]
            for attribute in args:
                setattr(parser.values, attribute, value_conv)
        except KeyError:
            raise optparse.OptionValueError(
                "option %s: invalid boolean value: %s (choose from %s)"
                % (opt, value, boolean_choices))
    else:
        for attribute in args:
            setattr(parser.values, attribute, value)

def CreateOptionParser (theAdditionalOptions=None, exeName=None):
    if theAdditionalOptions is None:
        theAdditionalOptions = []
    # GUI/Terminal. Default: GUI
    help_str = "Launch without GUI (in the terminal mode)."
    o_t = optparse.Option("-t",
                          "--terminal",
                          action="store_false",
                          dest="gui",
                          help=help_str)

    help_str = "Launch in Batch Mode. (Without GUI on batch machine)"
    o_b = optparse.Option("-b",
                          "--batch",
                          action="store_true",
                          dest="batch",
                          help=help_str)

    help_str = "Launch in GUI mode [default]."
    o_g = optparse.Option("-g",
                          "--gui",
                          action="store_true",
                          dest="gui",
                          help=help_str)

    # Show Desktop (inly in GUI mode). Default: True
    help_str  = "1 to activate GUI desktop [default], "
    help_str += "0 to not activate GUI desktop (Session_Server starts, but GUI is not shown). "
    help_str += "Ignored in the terminal mode."
    o_d = optparse.Option("-d",
                          "--show-desktop",
                          metavar="<1/0>",
                          #type="choice", choices=boolean_choices,
                          type="string",
                          action="callback", callback=store_boolean, callback_args=('desktop',),
                          dest="desktop",
                          help=help_str)
    help_str  = "Do not activate GUI desktop (Session_Server starts, but GUI is not shown). "
    help_str += "The same as --show-desktop=0."
    o_o = optparse.Option("-o",
                          "--hide-desktop",
                          action="store_false",
                          dest="desktop",
                          help=help_str)

    # Use logger or log-file. Default: nothing.
    help_str = "Redirect messages to the CORBA collector."
    #o4 = optparse.Option("-l", "--logger", action="store_true", dest="logger", help=help_str)
    o_l = optparse.Option("-l",
                          "--logger",
                          action="store_const", const="CORBA",
                          dest="log_file",
                          help=help_str)
    help_str = "Redirect messages to the <log-file>"
    o_f = optparse.Option("-f",
                          "--log-file",
                          metavar="<log-file>",
                          type="string",
                          action="store",
                          dest="log_file",
                          help=help_str)

    # Configuration XML file. Default: see defaultUserFile() function
    help_str  = "Parse application settings from the <file> "
    help_str += "instead of default %s" % defaultUserFile()
    o_r = optparse.Option("-r",
                          "--resources",
                          metavar="<file>",
                          type="string",
                          action="store",
                          dest="resources",
                          help=help_str)

    # Use own xterm for each server. Default: False.
    help_str = "Launch each SALOME server in own xterm console"
    o_x = optparse.Option("-x",
                          "--xterm",
                          action="store_true",
                          dest="xterm",
                          help=help_str)

    # Modules. Default: Like in configuration files.
    help_str  = "SALOME modules list (where <module1>, <module2> are the names "
    help_str += "of SALOME modules which should be available in the SALOME session)"
    o_m = optparse.Option("-m",
                          "--modules",
                          metavar="<module1,module2,...>",
                          type="string",
                          action="append",
                          dest="modules",
                          help=help_str)

    # Embedded servers. Default: Like in configuration files.
    help_str  = "CORBA servers to be launched in the Session embedded mode. "
    help_str += "Valid values for <serverN>: %s " % ", ".join( embedded_choices )
    help_str += "[by default the value from the configuration files is used]"
    o_e = optparse.Option("-e",
                          "--embedded",
                          metavar="<server1,server2,...>",
                          type="string",
                          action="callback",
                          dest="embedded",
                          callback=check_embedded,
                          help=help_str)

    # Standalone servers. Default: Like in configuration files.
    help_str  = "CORBA servers to be launched in the standalone mode (as separate processes). "
    help_str += "Valid values for <serverN>: %s " % ", ".join( standalone_choices )
    help_str += "[by default the value from the configuration files is used]"
    o_s = optparse.Option("-s",
                          "--standalone",
                          metavar="<server1,server2,...>",
                          type="string",
                          action="callback",
                          dest="standalone",
                          callback=check_standalone,
                          help=help_str)

    # Kill with port. Default: False.
    help_str = "Kill SALOME with the current port"
    o_p = optparse.Option("-p",
                          "--portkill",
                          action="store_true",
                          dest="portkill",
                          help=help_str)

    # Kill all. Default: False.
    help_str = "Kill all running SALOME sessions"
    o_k = optparse.Option("-k",
                          "--killall",
                          action="store_true",
                          dest="killall",
                          help=help_str)

    # Additional python interpreters. Default: 0.
    help_str  = "The number of additional external python interpreters to run. "
    help_str += "Each additional python interpreter is run in separate "
    help_str += "xterm session with properly set SALOME environment"
    o_i = optparse.Option("-i",
                          "--interp",
                          metavar="<N>",
                          type="int",
                          action="store",
                          dest="interp",
                          help=help_str)

    # Splash. Default: True.
    help_str  = "1 to display splash screen [default], "
    help_str += "0 to disable splash screen. "
    help_str += "This option is ignored in the terminal mode. "
    help_str += "It is also ignored if --show-desktop=0 option is used."
    o_z = optparse.Option("-z",
                          "--splash",
                          metavar="<1/0>",
                          #type="choice", choices=boolean_choices,
                          type="string",
                          action="callback", callback=store_boolean, callback_args=('splash',),
                          dest="splash",
                          help=help_str)

    # Catch exceptions. Default: True.
    help_str  = "1 (yes,true,on,ok) to enable centralized exception handling [default], "
    help_str += "0 (no,false,off,cancel) to disable centralized exception handling."
    o_c = optparse.Option("-c",
                          "--catch-exceptions",
                          metavar="<1/0>",
                          #type="choice", choices=boolean_choices,
                          type="string",
                          action="callback", callback=store_boolean, callback_args=('catch_exceptions',),
                          dest="catch_exceptions",
                          help=help_str)

    # Print free port and exit
    help_str = "Print free port and exit"
    o_a = optparse.Option("--print-port",
                          action="store_true",
                          dest="print_port", default=False,
                          help=help_str)

    # Do not relink ${HOME}/.omniORB_last.cfg
    help_str = "Do not save current configuration ${HOME}/.omniORB_last.cfg"
    o_n = optparse.Option("--nosave-config",
                          action="store_false",
                          dest="save_config", default=True,
                          help=help_str)

    # Launch with interactive python console. Default: False.
    help_str = "Launch with interactive python console."
    o_pi = optparse.Option("--pinter",
                          action="store_true",
                          dest="pinter",
                          help=help_str)

    # Print Naming service port into a user file. Default: False.
    help_str = "Print Naming Service Port into a user file."
    o_nspl = optparse.Option("--ns-port-log",
                             metavar="<ns_port_log_file>",
                             type="string",
                             action="store",
                             dest="ns_port_log_file",
                             help=help_str)

    # Write/read test script file with help of TestRecorder. Default: False.
    help_str = "Write/read test script file with help of TestRecorder."
    o_test = optparse.Option("--test",
                             metavar="<test_script_file>",
                             type="string",
                             action="store",
                             dest="test_script_file",
                             help=help_str)

    # Reproducing test script with help of TestRecorder. Default: False.
    help_str = "Reproducing test script with help of TestRecorder."
    o_play = optparse.Option("--play",
                             metavar="<play_script_file>",
                             type="string",
                             action="store",
                             dest="play_script_file",
                             help=help_str)

    # gdb session
    help_str = "Launch session with gdb"
    o_gdb = optparse.Option("--gdb-session",
                            action="store_true",
                            dest="gdb_session", default=False,
                            help=help_str)

    # ddd session
    help_str = "Launch session with ddd"
    o_ddd = optparse.Option("--ddd-session",
                            action="store_true",
                            dest="ddd_session", default=False,
                            help=help_str)


    # valgrind session
    help_str = "Launch session with valgrind $VALGRIND_OPTIONS"
    o_valgrind = optparse.Option("--valgrind-session",
                                 action="store_true",
                                 dest="valgrind_session", default=False,
                                 help=help_str)

    # shutdown-servers. Default: False.
    help_str  = "1 to shutdown standalone servers when leaving python interpreter, "
    help_str += "0 to keep the standalone servers as daemon [default]. "
    help_str += "This option is only useful in batchmode "
    help_str += "(terminal mode or without showing desktop)."
    o_shutdown = optparse.Option("-w",
                                 "--shutdown-servers",
                                 metavar="<1/0>",
                                 #type="choice", choices=boolean_choices,
                                 type="string",
                                 action="callback", callback=store_boolean, callback_args=('shutdown_servers',),
                                 dest="shutdown_servers",
                                 help=help_str)

    # foreground. Default: True.
    help_str  = "0 and runSalome exits after have launched the gui, "
    help_str += "1 to launch runSalome in foreground mode [default]."
    o_foreground = optparse.Option("--foreground",
                                   metavar="<1/0>",
                                   #type="choice", choices=boolean_choices,
                                   type="string",
                                   action="callback", callback=store_boolean, callback_args=('foreground',),
                                   dest="foreground",
                                   help=help_str)

    # wake up session
    help_str  = "Wake up a previously closed session. "
    help_str += "The session object is found in the naming service pointed by the variable OMNIORB_CONFIG. "
    help_str += "If this variable is not setted, the last configuration is taken. "
    o_wake_up = optparse.Option("--wake-up-session",
                                action="store_true",
                                dest="wake_up_session", default=False,
                                help=help_str)

    # server launch mode
    help_str = "Mode used to launch server processes (daemon or fork)."
    o_slm = optparse.Option("--server-launch-mode",
                            metavar="<server_launch_mode>",
                            type="choice",
                            choices=["daemon","fork"],
                            action="store",
                            dest="server_launch_mode",
                            help=help_str)

    # use port
    help_str  = "Preferable port SALOME to be started on. "
    help_str += "If specified port is not busy, SALOME session will start on it; "
    help_str += "otherwise, any available port will be searched and used."
    o_port = optparse.Option("--port",
                             metavar="<port>",
                             type="int",
                                   action="store",
                             dest="use_port",
                                   help=help_str)

    help_str  = "Force application language. By default, a language specified in "
    help_str += "the user's preferences is used."
    o_lang = optparse.Option("-a",
                             "--language",
                             action="store",
                             dest="language",
                             help=help_str)

    # All options
    opt_list = [o_t,o_g, # GUI/Terminal
                o_d,o_o, # Desktop
                o_b,     # Batch
                o_l,o_f, # Use logger or log-file
                o_r,     # Configuration XML file
                o_x,     # xterm
                o_m,     # Modules
                o_e,     # Embedded servers
                o_s,     # Standalone servers
                o_p,     # Kill with port
                o_k,     # Kill all
                o_i,     # Additional python interpreters
                o_z,     # Splash
                o_c,     # Catch exceptions
                o_a,     # Print free port and exit
                o_n,     # --nosave-config
                o_pi,    # Interactive python console
                o_nspl,
                o_test,  # Write/read test script file with help of TestRecorder
                o_play,  # Reproducing test script with help of TestRecorder
                o_gdb,
                o_ddd,
                o_valgrind,
                o_shutdown,
                o_foreground,
                o_wake_up,
                o_slm,   # Server launch mode
                o_port,  # Use port
                o_lang,  # Language
                ]

    #std_options = ["gui", "desktop", "log_file", "resources",
    #               "xterm", "modules", "embedded", "standalone",
    #               "portkill", "killall", "interp", "splash",
    #               "catch_exceptions", "print_port", "save_config", "ns_port_log_file"]

    opt_list += theAdditionalOptions

    if not exeName:
      exeName = "%prog"

    a_usage = """%s [options] [STUDY_FILE] [PYTHON_FILE [args] [PYTHON_FILE [args]...]]
Python file arguments, if any, must be comma-separated (without blank characters) and prefixed by "args:" (without quotes), e.g. myscript.py args:arg1,arg2=val,...
"""%exeName
    version_str = "Salome %s" % version()
    pars = optparse.OptionParser(usage=a_usage, version=version_str, option_list=opt_list)

    return pars

# -----------------------------------------------------------------------------

###
# Get the environment
###

# this attribute is obsolete
args = {}
#def get_env():
#args = []
def get_env(theAdditionalOptions=None, appname=salomeappname, cfgname=salomecfgname, exeName=None):
    ###
    # Collect launch configuration files:
    # - The environment variable "<appname>Config" (SalomeAppConfig) which can
    #   define a list of directories (separated by ':' or ';' symbol) is checked
    # - If the environment variable "<appname>Config" is not set, only
    #   ${GUI_ROOT_DIR}/share/salome/resources/gui is inspected
    # - ${GUI_ROOT_DIR}/share/salome/resources/gui directory is always inspected
    #   so it is not necessary to put it in the "<appname>Config" variable
    # - The directories which are inspected are checked for files "<appname?salomeappname>.xml"
    #  (SalomeApp.xml) which define SALOME configuration
    # - These directories are analyzed beginning from the last one in the list,
    #   so the first directory listed in "<appname>Config" environment variable
    #   has higher priority: it means that if some configuration options
    #   is found in the next analyzed cofiguration file - it will be replaced
    # - The last configuration file which is parsed is user configuration file
    #   situated in the home directory (if it exists):
    #   * ~/.config/salome/.<appname>rc[.<version>]" for Linux (e.g. ~/.config/salome/.SalomeApprc.6.4.0)
    #   * ~/<appname>.xml[.<version>] for Windows (e.g. ~/SalomeApp.xml.6.4.0)
    # - Command line options have the highest priority and replace options
    #   specified in configuration file(s)
    ###

    if theAdditionalOptions is None:
        theAdditionalOptions = []

    global args
    config_var = appname+'Config'

    # check KERNEL_ROOT_DIR
    kernel_root_dir = os.environ.get("KERNEL_ROOT_DIR", None)
    if kernel_root_dir is None:
        print """
        For each SALOME module, the environment variable <moduleN>_ROOT_DIR must be set.
        KERNEL_ROOT_DIR is mandatory.
        """
        sys.exit(1)

    ############################
    # parse command line options
    pars = CreateOptionParser(theAdditionalOptions, exeName=exeName)
    (cmd_opts, cmd_args) = pars.parse_args(sys.argv[1:])
    ############################

    # Process --print-port option
    if cmd_opts.print_port:
        from searchFreePort import searchFreePort
        searchFreePort({})
        print "port:%s"%(os.environ['NSPORT'])

        try:
            import PortManager
            PortManager.releasePort(os.environ['NSPORT'])
        except ImportError:
            pass

        sys.exit(0)
        pass

    # set resources variable SalomeAppConfig if it is not set yet
    dirs = []
    if os.getenv(config_var):
        if sys.platform == 'win32':
            dirs += re.split(os.pathsep, os.getenv(config_var))
        else:
            dirs += re.split('[;|:]', os.getenv(config_var))

    gui_available = False
    if os.getenv("GUI_ROOT_DIR"):
        gui_resources_dir = os.path.join(os.getenv("GUI_ROOT_DIR"),'share','salome','resources','gui')
        if os.path.isdir( gui_resources_dir ):
            gui_available = True
            dirs.append(gui_resources_dir)
        pass
    if not gui_available:
        kernel_resources_dir = os.path.join(os.getenv("KERNEL_ROOT_DIR"),'bin','salome','appliskel')
        if os.getenv("KERNEL_ROOT_DIR") and os.path.isdir( kernel_resources_dir ):
          dirs.append(kernel_resources_dir)
        pass
    os.environ[config_var] = os.pathsep.join(dirs)

    dirs.reverse() # reverse order, like in "path" variable - FILO-style processing

    try:
        dirs.remove('') # to remove empty dirs if the variable terminate by ":" or if there are "::" inside
    except:
        pass

    _opts = {} # associative array of options to be filled

    # parse SalomeApp.xml files in directories specified by SalomeAppConfig env variable
    for dir in dirs:
        filename = os.path.join(dir, appname+'.xml')
        if not os.path.exists(filename):
            if verbose(): print "Configure parser: Warning : can not find configuration file %s" % filename
        else:
            try:
                p = xml_parser(filename, _opts, [])
                _opts = p.opts
            except:
                if verbose(): print "Configure parser: Error : can not read configuration file %s" % filename
            pass

    # parse user configuration file
    # It can be set via --resources=<file> command line option
    # or is given from default location (see defaultUserFile() function)
    # If user file for the current version is not found the nearest to it is used
    user_config = cmd_opts.resources
    if not user_config:
        user_config = userFile(appname, cfgname)
        if verbose(): print "Configure parser: user configuration file is", user_config
    if not user_config or not os.path.exists(user_config):
        if verbose(): print "Configure parser: Warning : can not find user configuration file"
    else:
        try:
            p = xml_parser(user_config, _opts, [])
            _opts = p.opts
        except:
            if verbose(): print 'Configure parser: Error : can not read user configuration file'
            user_config = ""

    args = _opts

    args['user_config'] = user_config
    #print "User Configuration file: ", args['user_config']

    # set default values for options which are NOT set in config files
    for aKey in listKeys:
        if not args.has_key( aKey ):
            args[aKey] = []

    for aKey in boolKeys:
        if not args.has_key( aKey ):
            args[aKey] = 0

    if args[file_nam]:
        afile=args[file_nam]
        args[file_nam] = [afile]

    args[appname_nam] = appname

    # get the port number
    my_port = getPortNumber()

    args[port_nam] = my_port

    ####################################################
    # apply command-line options to the arguments
    # each option given in command line overrides the option from xml config file
    #
    # Options: gui, desktop, log_file, resources,
    #          xterm, modules, embedded, standalone,
    #          portkill, killall, interp, splash,
    #          catch_exceptions, pinter

    # GUI/Terminal, Desktop, Splash, STUDY_HDF
    args["session_gui"] = False
    args[batch_nam] = False
    args["study_hdf"] = None
    if cmd_opts.gui is not None:
        args[gui_nam] = cmd_opts.gui
    if cmd_opts.batch is not None:
        args[batch_nam] = True

    if not gui_available:
        args[gui_nam] = False

    if args[gui_nam]:
        args["session_gui"] = True
        if cmd_opts.desktop is not None:
            args["session_gui"] = cmd_opts.desktop
            args[splash_nam]    = cmd_opts.desktop
        if args["session_gui"]:
            if cmd_opts.splash is not None:
                args[splash_nam] = cmd_opts.splash
    else:
        args["session_gui"] = False
        args[splash_nam] = False

    # Logger/Log file
    if cmd_opts.log_file is not None:
        if cmd_opts.log_file == 'CORBA':
            args[logger_nam] = True
        else:
            args[file_nam] = [cmd_opts.log_file]

    # Naming Service port log file
    if cmd_opts.ns_port_log_file is not None:
      args["ns_port_log_file"] = cmd_opts.ns_port_log_file

    # Study files
    for arg in cmd_args:
        if arg[-4:] == ".hdf" and not args["study_hdf"]:
            args["study_hdf"] = arg

    # Python scripts
    from salomeContextUtils import getScriptsAndArgs, ScriptAndArgs
    args[script_nam] = getScriptsAndArgs(cmd_args)
    if args[gui_nam] and args["session_gui"]:
        new_args = []
        for sa_obj in args[script_nam]: # args[script_nam] is a list of ScriptAndArgs objects
            script = re.sub(r'^python.*\s+', r'', sa_obj.script)
            new_args.append(ScriptAndArgs(script=script, args=sa_obj.args, out=sa_obj.out))
        #
        args[script_nam] = new_args

    # xterm
    if cmd_opts.xterm is not None: args[xterm_nam] = cmd_opts.xterm

    # Modules
    if cmd_opts.modules is not None:
        args[modules_nam] = []
        listlist = cmd_opts.modules
        for listi in listlist:
            args[modules_nam] += re.split( "[:;,]", listi)
    else:
        # if --modules (-m) command line option is not given
        # try SALOME_MODULES environment variable
        if os.getenv( "SALOME_MODULES" ):
            args[modules_nam] = re.split( "[:;,]", os.getenv( "SALOME_MODULES" ) )
            pass

    # Embedded
    if cmd_opts.embedded is not None:
        args[embedded_nam] = filter( lambda a: a.strip(), re.split( "[:;,]", cmd_opts.embedded ) )

    # Standalone
    if cmd_opts.standalone is not None:
        args[standalone_nam] = filter( lambda a: a.strip(), re.split( "[:;,]", cmd_opts.standalone ) )

    # Normalize the '--standalone' and '--embedded' parameters
    standalone, embedded = process_containers_params( args.get( standalone_nam ),
                                                      args.get( embedded_nam ) )
    if standalone is not None:
        args[ standalone_nam ] = standalone
    if embedded is not None:
        args[ embedded_nam ] = embedded

    # Kill
    if cmd_opts.portkill is not None: args[portkill_nam] = cmd_opts.portkill
    if cmd_opts.killall  is not None: args[killall_nam]  = cmd_opts.killall

    # Interpreter
    if cmd_opts.interp is not None:
        args[interp_nam] = cmd_opts.interp

    # Exceptions
    if cmd_opts.catch_exceptions is not None:
        args[except_nam] = not cmd_opts.catch_exceptions

    # Relink config file
    if cmd_opts.save_config is not None:
        args['save_config'] = cmd_opts.save_config

    # Interactive python console
    if cmd_opts.pinter is not None:
        args[pinter_nam] = cmd_opts.pinter

    # Gdb session in xterm
    if cmd_opts.gdb_session is not None:
        args[gdb_session_nam] = cmd_opts.gdb_session

    # Ddd session in xterm
    if cmd_opts.ddd_session is not None:
        args[ddd_session_nam] = cmd_opts.ddd_session

    # valgrind session
    if cmd_opts.valgrind_session is not None:
        args[valgrind_session_nam] = cmd_opts.valgrind_session

    # Shutdown servers
    if cmd_opts.shutdown_servers is None:
        args[shutdown_servers_nam] = 0
    else:
        args[shutdown_servers_nam] = cmd_opts.shutdown_servers
        pass

    # Foreground
    if cmd_opts.foreground is None:
        args[foreground_nam] = 1
    else:
        args[foreground_nam] = cmd_opts.foreground
        pass

    # wake up session
    if cmd_opts.wake_up_session is not None:
        args[wake_up_session_nam] = cmd_opts.wake_up_session

    ####################################################
    # Add <theAdditionalOptions> values to args
    for add_opt in theAdditionalOptions:
        cmd = "args[\"{0}\"] = cmd_opts.{0}".format(add_opt.dest)
        exec(cmd)
    ####################################################

    # disable signals handling
    if args[except_nam] == 1:
        os.environ["NOT_INTERCEPT_SIGNALS"] = "1"
        pass

    # now modify SalomeAppConfig environment variable
    # to take into account the SALOME modules
    if os.sys.platform == 'win32':
        dirs = re.split('[;]', os.environ[config_var] )
    else:
        dirs = re.split('[;|:]', os.environ[config_var] )
    for module in args[modules_nam]:
        if module not in ["KERNEL", "GUI", ""] and os.getenv("{0}_ROOT_DIR".format(module)):
            d1 = os.path.join(os.getenv("{0}_ROOT_DIR".format(module)),"share","salome","resources",module.lower())
            d2 = os.path.join(os.getenv("{0}_ROOT_DIR".format(module)),"share","salome","resources")
            #if os.path.exists( "%s/%s.xml"%(d1, appname) ):
            if os.path.exists( os.path.join(d1,"{0}.xml".format(salomeappname)) ):
                dirs.append( d1 )
            #elif os.path.exists( "%s/%s.xml"%(d2, appname) ):
            elif os.path.exists( os.path.join(d2,"{0}.xml".format(salomeappname)) ):
                dirs.append( d2 )
        else:
            #print "* '"+m+"' should be deleted from ",args[modules_nam]
            pass

    # Test
    if cmd_opts.test_script_file is not None:
        args[test_nam] = []
        filename = cmd_opts.test_script_file
        args[test_nam] += re.split( "[:;,]", filename )

    # Play
    if cmd_opts.play_script_file is not None:
        args[play_nam] = []
        filename = cmd_opts.play_script_file
        args[play_nam] += re.split( "[:;,]", filename )

    # Server launch command
    if cmd_opts.server_launch_mode is not None:
        args["server_launch_mode"] = cmd_opts.server_launch_mode

    # Server launch command
    if cmd_opts.use_port is not None:
        min_port = 2810
        max_port = min_port + 100
        if cmd_opts.use_port not in xrange(min_port, max_port+1):
            print "Error: port number should be in range [%d, %d])" % (min_port, max_port)
            sys.exit(1)
        args[useport_nam] = cmd_opts.use_port

    if cmd_opts.language is not None:
        langs = args["language_languages"] if "language_languages" in args else []
        if cmd_opts.language not in langs:
            print "Error: unsupported language: %s" % cmd_opts.language
            sys.exit(1)
        args[lang_nam] = cmd_opts.language

    # return arguments
    os.environ[config_var] = os.pathsep.join(dirs)
    #print "Args: ", args
    return args
