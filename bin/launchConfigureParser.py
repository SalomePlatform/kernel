import os, glob, string, sys
import xml.sax

# -----------------------------------------------------------------------------

### xml reader for launch configuration file usage

class xml_parser:
    def __init__(self, fileName):
        self.space = []
        self.opts = {}
        parser = xml.sax.make_parser()
        parser.setContentHandler(self)
        parser.parse(fileName)
        pass

    def CorrectBoolean(self, str):
        if str in ("yes", "y", "1"):
            return 1
        elif str in ("no", "n", "0"):
            return 0
        else:
            return str
        pass

    def startElement(self, name, attrs):
        #print "startElement name=",name
        #print "startElement attrs=",attrs.getNames()
        self.space.append(name)
        self.current = None

        if self.space[:2] == ["Configuration-list","launchoptions"] and len(self.space) == 3:
            self.current = name
        elif self.space == ["Configuration-list","modules-list"]:
            self.opts["modules"] = []
        elif self.space == ["Configuration-list","modules-list","module"] and "name" in attrs.getNames():
            for field in attrs.getNames():
                if field == "name":
                    self.currentModuleName = str(attrs.getValue("name"))
                    self.opts["modules"].append(self.currentModuleName)
                else:
                    self.opts[str(attrs.getValue("name"))+"_"+str(field)] = self.CorrectBoolean(attrs.getValue(field))
                    pass
                pass
        elif self.space == ["Configuration-list","modules-list","module","plugin"] and "name" in attrs.getNames():
            key = str(self.currentModuleName)+"_plugins"
            if not self.opts.has_key(key):
                self.opts[key]=[]
                pass
            self.opts[key].append(attrs.getValue("name"))
        elif self.space == ["Configuration-list","embedded-list"]:
            self.opts["embedded"] = []
            pass
        elif self.space == ["Configuration-list","standalone-list"]:
            self.opts["standalone"] = []
            pass
        elif self.space == ["Configuration-list","containers-list"]:
            self.opts["containers"] = []
            pass
        pass

    def endElement(self, name):
        p = self.space.pop()
        self.current = None
        pass

    def characters(self, content):
        #print "Characters content:",content
        if self.current:
            self.opts[self.current] = self.CorrectBoolean(content)
        elif self.space == ["Configuration-list","embedded-list", "embeddedserver"]:
            self.opts["embedded"].append(content)
        elif self.space == ["Configuration-list","standalone-list", "standaloneserver"]:
            self.opts["standalone"].append(content)
        elif self.space == ["Configuration-list","containers-list", "containertype"]:
            self.opts["containers"].append(content)
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

### searching for launch configuration file : $HOME/.$(application_name)/$(application_name).launch

appname = None
dirname = None
filename = None
for bindir in glob.glob(os.environ["KERNEL_ROOT_DIR"]+"/bin/*"):
    appname = string.split(bindir, "/").pop()
    print 'Application name: "'+appname+'"'
    # find version number
    versnb = ""
    try:
      file = open(os.environ["KERNEL_ROOT_DIR"]+"/bin/"+appname+"/VERSION", "r")
      s = file.read()
      l = string.split(s, ":")
      vl = string.split(l[1], " ")
      i = 0
      while len(versnb) == 0:
        versnb = vl[i]
        i += 1
        pass
      versnb = string.split(versnb, "\n")[0]
      print "Version ",versnb
    except:
      pass
    # end find version number
    dirname = os.environ["HOME"]+"/."+appname+"_"+versnb
    filename = dirname+"/"+appname+".launch"
    if not os.path.exists(filename) and \
       not os.path.exists(os.environ["KERNEL_ROOT_DIR"]+"/bin/"+appname+"/"+appname+".launch"):
        filename = None
    else:
        break
    pass
if not appname:
    print "Can not find application name"
    if not os.have_key("KERNEL_ROOT_DIR"):
        print "KERNEL_ROOT_DIR environment variable must be set"
        pass
    sys.exit(1);
elif not filename or not os.path.exists(filename):
    filename = dirname+"/"+appname+".launch"
    #filename = os.environ["HOME"]+"/."+appname+"/"+appname+".launch"
    print "Launch configuration file does not exist. Create default:",filename
    os.system("mkdir -p "+dirname)
    #os.system("mkdir -p "+os.environ["HOME"]+"/."+appname)
    os.system("cp -f "+os.environ["KERNEL_ROOT_DIR"]+"/bin/"+appname+"/"+appname+".launch "+filename)
    pass

### get options from launch configuration file

try:
    p = xml_parser(filename)
except:
    print 'Can not read launch configuration file ', filename
    filename = None
    pass

if filename:
    args = p.opts
else:
    args = {}
    pass

# --- args completion
for aKey in ("containers","embedded","key","modules","standalone"):
    if not args.has_key(aKey):
        args[aKey]=[]
for aKey in ("gui","logger","xterm","portkill","killall"):
    if not args.has_key(aKey):
        args[aKey]=0
args["appname"] = appname

### searching for my port

my_port = 2809
try:
  file = open(os.environ["OMNIORB_CONFIG"], "r")
  s = file.read()
  while len(s):
    l = string.split(s, ":")
    if string.split(l[0], " ")[0] == "ORBInitRef":
      my_port = int(l[len(l)-1])
      pass
    s = file.read()
    pass
except:
  pass

args["port"] = my_port

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

  print "source=",source
  
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

try:
    opts = options_parser(sys.argv[1:])
    print "opts=",opts
    kernel_root_dir=os.environ["KERNEL_ROOT_DIR"]
except:
    opts["h"] = 1
    pass

### check all options are right

opterror=0
for opt in opts:
    if not opt in ("h","g","l","x","m","e","s","c","p","k","t"):
        print "command line error: -", opt
        opterror=1

if opterror == 1:
    opts["h"] = 1

if opts.has_key("h"):
    print """USAGE: runSalome.py [options]
    [command line options] :
    --help or -h                  : print this help
    --gui or -g                   : lancement du GUI
    --terminal -t                 : launching without gui (to deny --gui)
    --logger or -l                : redirection des messages dans un fichier
    --xterm or -x                 : les serveurs ouvrent une fenêtre xterm et les messages sont affichés dans cette fenêtre
    --modules=module1,module2,... : où modulen est le nom d'un module Salome à charger dans le catalogue
    or -m=module1,module2,...
    --embedded=registry,study,moduleCatalog,cppContainer
    or -e=registry,study,moduleCatalog,cppContainer
                                  : serveurs CORBA embarqués (par defaut: registry,study,moduleCatalog,cppContainer)
                                  : (logger,pyContainer,supervContainer ne peuvent pas être embarqués
    --standalone=registry,study,moduleCatalog,cppContainer,pyContainer,supervContainer
    or -s=registry,study,moduleCatalog,cppContainer,pyContainer,supervContainer
                                  : executables serveurs CORBA indépendants (par défaut: pyContainer,supervContainer)
    --containers=cpp,python,superv: (obsolete) lancement des containers cpp, python et de supervision
    or -c=cpp,python,superv       : = on prend les defauts de -e et -s
    --portkill or -p              : kill the salome with current port
    --killall or -k               : kill salome
    
    La variable d'environnement <modulen>_ROOT_DIR doit etre préalablement
    positionnée (modulen doit etre en majuscule).
    KERNEL_ROOT_DIR est obligatoire.
    """
    sys.exit(1)
    pass

### apply command-line options to the arguments
for opt in opts:
    if opt == 'g':
        args['gui'] = 1
    elif opt == 'l':
        args['logger'] = 1
    elif opt == 'x':
        args['xterm'] = 1
    elif opt == 'm':
        args['modules'] = opts['m']
    elif opt == 'e':
        args['embedded'] = opts['e']
    elif opt == 's':
        args['standalone'] = opts['s']
    elif opt == 'c':
        args['containers'] = opts['c']
    elif opt == 'p':
        args['portkill'] = 1
    elif opt == 'k':
        args['killall'] = 1
        pass
    pass

# 'terminal' must be processed in the end: to deny any 'gui' options
if 't' in opts:
    args['gui'] = 0
    pass

print "args=",args
