# Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
#           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful
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

# names of tags in XML configuration file
doc_tag = "document"
sec_tag = "section"
par_tag = "parameter"

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
containers_nam = "containers"
key_nam        = "key"
terminal_nam   = "terminal"
interp_nam     = "interp"
except_nam     = "noexcepthandler"
terminal_nam   = "terminal"
case_nam       = "test"

# values in XML configuration file giving specific module parameters (<module_name> section)
# which are stored in opts with key <module_name>_<parameter> (eg SMESH_plugins)
plugins_nam    = "plugins"

# values passed as arguments, NOT read from XML config file, but set from within this script
appname_nam    = "appname"
port_nam       = "port"
appname        = "SalomeApp"
script_nam     = "pyscript"

# values of boolean type (must be '0' or '1').
# xml_parser.boolValue() is used for correct setting
boolKeys = ( gui_nam, splash_nam, logger_nam, file_nam, xterm_nam, portkill_nam, killall_nam, interp_nam, except_nam )

# values of list type
listKeys = ( containers_nam, embedded_nam, key_nam, modules_nam, standalone_nam, plugins_nam )

###
# Get the application version
# Uses GUI_ROOT_DIR (or KERNEL_ROOT_DIR in batch mode) +/bin/salome/VERSION file
###
def version():
    try:
        filename = None
        root_dir = os.environ.get( 'KERNEL_ROOT_DIR', '' ) # KERNEL_ROOT_DIR or "" if not found
        if root_dir and os.path.exists( root_dir + "/bin/salome/VERSION" ):
            filename = root_dir + "/bin/salome/VERSION"
        root_dir = os.environ.get( 'GUI_ROOT_DIR', '' )    # GUI_ROOT_DIR "" if not found
        if root_dir and os.path.exists( root_dir + "/bin/salome/VERSION" ):
            filename = root_dir + "/bin/salome/VERSION"
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
def version_id( fname ):
    vers = fname.split(".")
    major   = int(vers[0])
    minor   = int(vers[1])
    mr = re.search(r'^([0-9]+)([A-Za-z]?)([0-9]*)',vers[2])
    release = dev = 0
    if mr:
        release = int(mr.group(1))
        dev1 = dev2 = 0
        if len(mr.group(2)): dev1 = ord(mr.group(2))
        if len(mr.group(3)): dev2 = int(mr.group(3))
        dev = dev1 * 100 + dev2
    else:
        return None
    ver = major
    ver = ver * 100 + minor
    ver = ver * 100 + release
    ver = ver * 10000
    if dev > 0: ver = ver - 10000 + dev
    return ver

###
# Get user configuration file name
###
def userFile():
    v = version()
    if not v:
        return ""        # not unknown version
    if sys.platform == "win32":
      filename = "%s\%s.xml.%s" % (os.environ['HOME'], appname, v)
    else:
      filename = "%s/.%src.%s" % (os.environ['HOME'], appname, v)
    if os.path.exists(filename):
        return filename  # user preferences file for the current version exists
    # initial id
    id0 = version_id( v )
    # get all existing user preferences files
    if sys.platform == "win32":
      files = glob.glob( os.environ['HOME'] + "\." + appname + ".xml.*" )
    else:
      files = glob.glob( os.environ['HOME'] + "/." + appname + "rc.*" )
    f2v = {}
    for file in files:
        match = re.search( r'\.%src\.([a-zA-Z0-9.]+)$'%appname, file )
        if match: f2v[file] = match.group(1)
    last_file = ""
    last_version = 0
    for file in f2v:
        ver = version_id( f2v[file] )
        if ver and abs(last_version-id0) > abs(ver-id0):
            last_version = ver
            last_file = file
    return last_file

# -----------------------------------------------------------------------------

###
# XML reader for launch configuration file usage
###

section_to_skip = ""

class xml_parser:
    def __init__(self, fileName, _opts ):
        print "Configure parser: processing %s ..." % fileName
        self.space = []
        self.opts = _opts
        self.section = section_to_skip
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

        # if we are analyzing "section" element and its "name" attribute is
        # either "launch" or module name -- set section_name
        if self.space == [doc_tag, sec_tag] and nam_att in attrs.getNames():
            section_name = attrs.getValue( nam_att )
            if section_name == lanch_nam:
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
            if nam in boolKeys:
                self.opts[key] = self.boolValue( val )  # assign boolean value: 0 or 1
            elif nam in listKeys:
                self.opts[key] = val.split( ',' )       # assign list value: []
            else:
                self.opts[key] = val;
            pass
        pass

    def endElement(self, name):
        p = self.space.pop()
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

# -----------------------------------------------------------------------------

###
# Command line options parser
###
def options_parser(line):
  source = line
  list = []
  for delimiter in [" ", ",", "="]:
    for o in source:
      list += string.split(o, delimiter)
      pass
    source = list
    list = []
    pass

  result = {}
  i = 0
  while i < len(source):
    if source[i][0] != '-':
      key = None
    elif source[i][1] == '-':
      key = source[i][2:]
    else:
      key = source[i][1:]
      pass

    if key is None and not result:
        raise Exception()
    result[key] = []
    if key:
      i += 1
      pass
    while i < len(source) and source[i][0] != '-':
      result[key].append(source[i])
      i += 1
      pass
    pass
  return result

# -----------------------------------------------------------------------------

###
# Get the environment
###

# this attribute is obsolete
args = {}
def get_env():
    ###
    # Collect launch configuration files:
    # - The environment variable "<appname>Config" (SalomeAppConfig) which can
    #   define a list of directories (separated by ':' or ';' symbol) is checked
    # - If the environment variable "<appname>Config" is not set, only
    #   ${GUI_ROOT_DIR}/share/salome/resources/gui is inspected
    # - ${GUI_ROOT_DIR}/share/salome/resources/gui directory is always inspected
    #   so it is not necessary to put it in the "<appname>Config" variable
    # - The directories which are inspected are checked for files "<appname>.xml"
    #  (SalomeApp.xml) which define SALOME configuration
    # - These directories are analyzed beginning from the last one in the list,
    #   so the first directory listed in "<appname>Config" environment variable 
    #   has higher priority: it means that if some configuration options
    #   is found in the next analyzed cofiguration file - it will be replaced
    # - The last configuration file which is parsed is user configuration file
    #   situated in the home directory: "~/.<appname>rc[.<version>]" (~/SalomeApprc.3.2.0)
    #   (if it exists)
    # - Command line options have the highest priority and replace options
    #   specified in configuration file(s)
    ###

    global args
    config_var = appname+'Config'

    separator = ":"
    if os.sys.platform == 'win32':
        separator = ";"


    # set resources variable SaloemAppConfig if it is not set yet 
    dirs = []
    if os.getenv(config_var):
        if sys.platform == 'win32':
            dirs += re.split(';', os.getenv(config_var))
        else:
            dirs += re.split('[;|:]', os.getenv(config_var))
            
    if os.getenv("GUI_ROOT_DIR") and os.path.isdir( os.getenv("GUI_ROOT_DIR") + "/share/salome/resources/gui" ):
        dirs += [os.getenv("GUI_ROOT_DIR") + "/share/salome/resources/gui"]
    os.environ[config_var] = separator.join(dirs)

    dirs.reverse() # reverse order, like in "path" variable - FILO-style processing

    _opts = {} # associative array of options to be filled

    # parse SalomeApp.xml files in directories specified by SalomeAppConfig env variable
    for dir in dirs:
        filename = dir+'/'+appname+'.xml'
        if not os.path.exists(filename):
            print "Configure parser: Warning : could not find configuration file %s" % filename
        else:
            try:
                p = xml_parser(filename, _opts)
                _opts = p.opts
            except:
                print "Configure parser: Error : can not read configuration file %s" % filename
            pass

    # parse .SalomeApprc.<version> file in user's home directory if it exists
    # if user file for the current version is not found the nearest to it is used
    filename = userFile()
    if not filename or not os.path.exists(filename):
        print "Configure parser: Warning : could not find user configuration file"
    else:
        try:
            p = xml_parser(filename, _opts)
            _opts = p.opts
        except:
            print 'Configure parser: Error : can not read user configuration file'

    args = _opts

    # set default values for options which are NOT set in config files
    for aKey in listKeys:
        if not args.has_key( aKey ):
            args[aKey]=[]

    for aKey in boolKeys:
        if not args.has_key( aKey ):
            args[aKey]=0

    if args[file_nam]:
        afile=args[file_nam]
        args[file_nam]=[afile]

    args[appname_nam] = appname

    # get the port number
    my_port = 2809
    try:
      file = open(os.environ["OMNIORB_CONFIG"], "r")
      s = file.read()
      while len(s):
        l = string.split(s, ":")
        if string.split(l[0], " ")[0] == "ORBInitRef" or string.split(l[0], " ")[0] == "InitRef" :
          my_port = int(l[len(l)-1])
          pass
        s = file.read()
        pass
    except:
      pass

    args[port_nam] = my_port

    # read command-line options
    # each option given in command line overrides the option from xml config file
    cmd_opts = {}
    try:
        cmd_opts = options_parser(sys.argv[1:])
        kernel_root_dir=os.environ["KERNEL_ROOT_DIR"]
    except:
        cmd_opts["h"] = 1
        pass

    # check if all command line options are correct
    short_opts = ("h","g","l","f","x","m","e","s","c","p","k","t","i","r","z")
    long_opts = (help_nam,gui_nam,logger_nam,file_nam,xterm_nam,modules_nam,
                 embedded_nam,standalone_nam,containers_nam,portkill_nam,
                 killall_nam,terminal_nam,interp_nam,except_nam,splash_nam,
                 case_nam)
    opterror=0
    for opt in cmd_opts:
        if opt not in short_opts and opt not in long_opts:
            print "Configure parser: Error : command line error : -%s" % opt
            opterror=1

    if opterror == 1:
        cmd_opts["h"] = 1

    if cmd_opts.has_key("h") or cmd_opts.has_key(help_nam):
        print """
        USAGE: runSalome.py [options]

        Command line options:

        --gui             (-g)                 Launch in GUI mode [default].
        --terminal        (-t)                 Launching without GUI (in the terminal mode).
        --terminal=<python_script>[,...]       Launching without GUI (in the terminal mode) and 
               (-t=<python_script>[,...])      additionally import python script(s).
        --logger          (-l)                 Redirect messages to the CORBA collector.
        --file=<file>     (-f=<file>)          Redirect messages to the log file.
        --xterm           (-x)                 Launch each SALOME server in own xterm console.
        --modules=<module1>,<module2>,...      SALOME module list (where <module1>, <module2> 
              (-m=<module1>,<module2>,...)     are the names of SALOME modules which should be
                                               available in the SALOME session).
        --embedded=<server1>,<server2>,...     CORBA servers to be launched in the Session
               (-e=<server1>,<server2>,...)    embedded mode.
                                               Valid values for <serverN>: registry, study,
                                               moduleCatalog, cppContainer
                                               [default: all mentioned].
        --standalone=<server1>,<server2>,...   CORBA servers to be launched in the standalone
                 (-s=<server1>,<server2>,...)  mode (as separate processes).
                                               Valid values for <serverN>: registry, study,
                                               moduleCatalog, cppContainer, pyContainer,
                                               supervContainer
                                               [default: pyContainer,supervContainer].
        --containers=<container1>,...          [obsolete] SALOME containers to be launched.
                 (-c=<container1>,...)         Valid values: cpp, python, superv
                                               [default: use --embedded and --standalone
                                               parameters].
        --portkill        (-p)                 Kill SALOME with the current port.
        --killall         (-k)                 Kill all running SALOME sessions.
        --interp=<N>      (-i=<N>)             The number of additional xterm sessions to open.
                                               In each xterm session SALOME environment is set
                                               properly.
        --splash          (-z)                 Display splash screen.
        --noexcepthandler (-r)                 Disable centralized exception handling
                                               mechanism.
        --test=<hdf_file_andor_python_scripts> HDF file to be opened on GUI starting and/or
                                               Python script(s) to be imported in the GUI
                                               study. The files can appear in arbitrary order.
                                               If the HDF file is given it is opened, otherwise
                                               the new empty study is created.
                                               Python scripts are imported in the order of
                                               their appearance.
                                               This option is avaiable only in GUI mode,
                                               for batch mode use --terminal(-t) option.
        --help            (-h)                 Print this help info

        For each SALOME module, the environment variable <moduleN>_ROOT_DIR must be set.
        KERNEL_ROOT_DIR is mandatory.
        """
        sys.exit(1)
        pass

    # apply command-line options to the arguments
    BATCHMODE_FORCED = False
    NO_SPLASH_FORCED = False
    args[script_nam] = []
    for opt in cmd_opts:
        if   opt in [ 'g', gui_nam ] :
            if not BATCHMODE_FORCED: args[gui_nam] = 1
            if cmd_opts[opt] == ['0']:
                args["session_gui"] = 0
                NO_SPLASH_FORCED = True
                pass
        elif opt in [ 't', terminal_nam ] :
            args[gui_nam] = 0
            args[script_nam] = cmd_opts[opt]
            BATCHMODE_FORCED = True
        elif opt in [ 'z', splash_nam ] :
            if not NO_SPLASH_FORCED:
                args[splash_nam] = 1
                if cmd_opts[opt] == ['0']:
                    args[splash_nam] = 0
                    pass
                pass
            pass
        elif opt in [ 'r', except_nam ] :
            args[except_nam] = 1
        elif opt in [ 'l', logger_nam ] :
            args[logger_nam] = 1
        elif opt in [ 'f', file_nam ] :
            args[file_nam] = cmd_opts[opt]
        elif opt in [ 'x', xterm_nam ] :
            args[xterm_nam] = 1
        elif opt in [ 'i', interp_nam ] :
            args[interp_nam] = cmd_opts[opt]
        elif opt in [ 'm', modules_nam ] :
            args[modules_nam] = cmd_opts[opt]
        elif opt in [ 'e', embedded_nam ] :
            args[embedded_nam] = cmd_opts[opt]
        elif opt in [ 's', standalone_nam ] :
            args[standalone_nam] = cmd_opts[opt]
        elif opt in [ 'c', containers_nam ] :
            args[containers_nam] = cmd_opts[opt]
        elif opt in [ 'p', portkill_nam ] :
            args[portkill_nam] = 1
        elif opt in [ 'k', killall_nam ] :
            args[killall_nam] = 1
        elif opt in [ case_nam ] :
           args[case_nam] = cmd_opts[opt]
        pass

    # if --modules (-m) command line option is not given
    # try SALOME_MODULES environment variable
    if not cmd_opts.has_key( "m" ) and \
       not cmd_opts.has_key( modules_nam ) and \
           os.getenv( "SALOME_MODULES" ):
        args[modules_nam] = re.split( "[:;,]", os.getenv( "SALOME_MODULES" ) )
        pass

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
    for m in args[modules_nam]:
        if m not in ["KERNEL", "GUI", ""] and os.getenv("%s_ROOT_DIR"%m):
            d1 = os.getenv("%s_ROOT_DIR"%m) + "/share/salome/resources/" + m.lower()
            d2 = os.getenv("%s_ROOT_DIR"%m) + "/share/salome/resources"
            if os.path.exists( "%s/%s.xml"%(d1, appname) ):
                dirs.append( d1 )
            elif os.path.exists( "%s/%s.xml"%(d2, appname) ):
                dirs.append( d2 )

    # return arguments
    os.environ[config_var] = separator.join(dirs)
    return args
