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
interp_nam     = "interp"
except_nam     = "noexcepthandler"

# values in XML configuration file giving specific module parameters (<module_name> section)
# which are stored in opts with key <module_name>_<parameter> (eg SMESH_plugins)
plugins_nam    = "plugins"

# values passed as arguments, NOT read from XML config file, but set from within this script
appname_nam    = "appname"
port_nam       = "port"
appname        = "SalomeApp"

# values of boolean type (must be '0' or '1').
# xml_parser.boolValue() is used for correct setting
boolKeys = ( gui_nam, splash_nam, logger_nam, file_nam, xterm_nam, portkill_nam, killall_nam, interp_nam, except_nam )

# values of list type
listKeys = ( containers_nam, embedded_nam, key_nam, modules_nam, standalone_nam, plugins_nam )

# return application version (uses GUI_ROOT_DIR (or KERNEL_ROOT_DIR in batch mode) +/bin/salome/VERSION)
def version():
    root_dir = os.environ.get( 'KERNEL_ROOT_DIR', '' )     # KERNEL_ROOT_DIR or "" if not found
    root_dir = os.environ.get( 'GUI_ROOT_DIR', root_dir )  # GUI_ROOT_DIR or KERNEL_ROOT_DIR or "" if both not found
    filename = root_dir+'/bin/salome/VERSION'
    str = open( filename, "r" ).readline() # str = "THIS IS SALOME - SALOMEGUI VERSION: 3.0.0"
    match = re.search( r':\s+([a-zA-Z0-9.]+)\s*$', str )
    if match :
        return match.group( 1 )
    return ''

# calculate and return configuration file id in order to unically identify it
# for example: for 3.1.0a1 the id is 301000101
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
    ver = ver * 10000 + dev
    return ver

# get user configuration file name
def userFile():
    v = version()
    if not v:
        return ""        # not unknown version
    filename = "%s/.%src.%s" % (os.environ['HOME'], appname, v)
    if os.path.exists(filename):
        return filename  # user preferences file for the current version exists
    # initial id
    id0 = version_id( v )
    # get all existing user preferences files
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

### xml reader for launch configuration file usage

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

### searching for launch configuration files
# the rule:
# - environment variable {'appname'+'Config'} (SalomeAppConfig) contains list of directories (';' as devider)
# - these directories contain 'appname'+'.xml' (SalomeApp.xml) configuration files
# - these files are analyzed beginning with the last one (last directory in the list)
# - if a key is found in next analyzed cofiguration file - it will be replaced
# - the last configuration file to be analyzed - ~/.'appname'+'rc' (~/SalomeApprc) (if it exists)
# - but anyway, if user specifies a certain option in a command line - it will replace the values
# - specified in configuration file(s)
# - once again the order of settings (next setting replaces the previous ones):
# -     SalomeApp.xml files in directories specified by SalomeAppConfig env variable
# -     .SalomeApprc file in user's catalogue
# -     command line

config_var = appname+'Config'
# set resources variables if not yet set
if os.getenv("GUI_ROOT_DIR"):
    if not os.getenv("SUITRoot"):
        os.environ["SUITRoot"] = os.getenv("GUI_ROOT_DIR") + "/share/salome"
    if not os.getenv(config_var):
        os.environ[config_var] = os.getenv("GUI_ROOT_DIR") + "/share/salome/resources"
    pass
else :
    if not os.getenv("SUITRoot"):
        os.environ["SUITRoot"] = ""
    if not os.getenv(config_var):
        os.environ[config_var] = ""

dirs = os.environ[config_var]
dirs = re.split('[;|:]', dirs )
dirs.reverse() # reverse order, like in "path" variable - FILO-style processing

_opts = {} # assiciative array of options to be filled

# SalomeApp.xml files in directories specified by SalomeAppConfig env variable
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

# SalomeApprc file in user's catalogue
filename = userFile()
if filename and not os.path.exists(filename):
    print "Configure parser: Warning : could not find user configuration file"
else:
    try:
        p = xml_parser(filename, _opts)
        _opts = p.opts
    except:
        print 'Configure parser: Error : can not read user configuration file'

args = _opts

# --- setting default values of keys if they were NOT set in config files ---
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

### searching for my port

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

# -----------------------------------------------------------------------------

### command line options reader

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
      key = source[i][2]
    else:
      key = source[i][1]
      pass
    
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

### read command-line options : each arg given in command line supersedes arg from xml config file
cmd_opts = {}
try:
    cmd_opts = options_parser(sys.argv[1:])
    kernel_root_dir=os.environ["KERNEL_ROOT_DIR"]
except:
    cmd_opts["h"] = 1
    pass

### check all options are right

opterror=0
for opt in cmd_opts:
    if not opt in ("h","g","l","f","x","m","e","s","c","p","k","t","i","r"):
        print "Configure parser: Error : command line error : -%s" % opt
        opterror=1

if opterror == 1:
    cmd_opts["h"] = 1

if cmd_opts.has_key("h"):
    print """USAGE: runSalome.py [options]
    [command line options] :
    --help or -h                  : print this help
    --gui or -g                   : launching with GUI
    --terminal -t                 : launching without gui (to deny --gui)
    --logger or -l                : redirect messages in a CORBA collector
    --file=filename or -f=filename: redirect messages in a log file  
    --xterm or -x                 : execute servers in xterm console (messages appear in xterm windows)
    --modules=module1,module2,... : salome module list (modulen is the name of Salome module to load)
    or -m=module1,module2,...
    --embedded=registry,study,moduleCatalog,cppContainer
    or -e=registry,study,moduleCatalog,cppContainer
                                  : embedded CORBA servers (default: registry,study,moduleCatalog,cppContainer)
                                  : (logger,pyContainer,supervContainer can't be embedded
    --standalone=registry,study,moduleCatalog,cppContainer,pyContainer,supervContainer
    or -s=registry,study,moduleCatalog,cppContainer,pyContainer,supervContainer
                                  : standalone CORBA servers (default: pyContainer,supervContainer)
    --containers=cpp,python,superv: (obsolete) launching of containers cpp, python and supervision
    or -c=cpp,python,superv       : = get default from -e and -s
    --portkill or -p              : kill the salome with current port
    --killall or -k               : kill all salome sessions
    --interp=n or -i=n            : number of additional xterm to open, with session environment
    -z                            : display splash screen
    -r                            : disable centralized exception handling mechanism
    
    For each Salome module, the environment variable <modulen>_ROOT_DIR must be set.
    The module name (<modulen>) must be uppercase.
    KERNEL_ROOT_DIR is mandatory.
    """
    sys.exit(1)
    pass

### apply command-line options to the arguments
for opt in cmd_opts:
    if opt == 'g':
        args[gui_nam] = 1
    elif opt == 'z':
	args[splash_nam] = 1
    elif opt == 'r':
	args[except_nam] = 1
    elif opt == 'l':
        args[logger_nam] = 1
    elif opt == 'f':
        args[file_nam] = cmd_opts['f']
    elif opt == 'x':
        args[xterm_nam] = 1
    elif opt == 'i':
        args[interp_nam] = cmd_opts['i']
    elif opt == 'm':
        args[modules_nam] = cmd_opts['m']
    elif opt == 'e':
        args[embedded_nam] = cmd_opts['e']
    elif opt == 's':
        args[standalone_nam] = cmd_opts['s']
    elif opt == 'c':
        args[containers_nam] = cmd_opts['c']
    elif opt == 'p':
        args[portkill_nam] = 1
    elif opt == 'k':
        args[killall_nam] = 1
        pass
    pass

# if --modules (-m) command line option is not given
# try SALOME_MODULES environment variable
if not cmd_opts.has_key( "m" ) and os.getenv( "SALOME_MODULES" ):
    args[modules_nam] = re.split( "[:;,]", os.getenv( "SALOME_MODULES" ) )
    pass

# 'terminal' must be processed in the end: to deny any 'gui' options
if 't' in cmd_opts:
    args[gui_nam] = 0
    pass

# now modify SalomeAppConfig environment variable
dirs = re.split('[;|:]', os.environ[config_var] )
for m in args[modules_nam]:
    if m not in ["KERNEL", "GUI", ""] and os.getenv("%s_ROOT_DIR"%m):
        dirs.append( os.getenv("%s_ROOT_DIR"%m) +  "/share/salome/resources" )
os.environ[config_var] = ":".join(dirs)
