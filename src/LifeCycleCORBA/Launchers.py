import os
import Utils_Identity
import ConfigParser

class Config:
   defaults={'user':'','opt':''}
   def __init__(self):
       self.config=ConfigParser.ConfigParser(self.defaults)
       self.config.read(os.path.join(os.environ['HOME'],Utils_Identity.getapplipath(),'launchers.cfg'))
   def computers(self):
       return self.config.sections()
   def options(self,computer):
       if not self.config.has_section(computer):
          return self.defaults
       else:
          d={}
          for opt in self.config.options(computer):
             d[opt]=self.config.get(computer,opt)
          return d

config=None
def setConfig(conf):
    global config
    config=conf

setConfig(Config())
       
class Launcher:
   def Slaunch(self,computer,CMD):
       self.launch(computer,CMD.split())

class SSH(Launcher):
   def launch(self,computer,CMD):
       print computer,CMD
       conf=config.options(computer)
       print conf
       command=["ssh",computer]
       if conf["user"]:
          command=command+['-l',conf["user"]]
       if conf["opt"]:
          command=command+string.split(conf["opt"])
       command=command+[os.path.join(Utils_Identity.getapplipath(),'runSession')]+CMD
       command=command+['>', "/tmp/" + CMD[1] + "_" + computer + ".log 2>&1"]

       print command
       pid = os.spawnvp(os.P_NOWAIT, command[0], command)

launcher=None
_register={}

def register(name,klass):
    _register[name]=klass

register("ssh",SSH())

def setLauncher(name):
    global launcher
    launcher=_register[name]

def getLauncher(computer):
    """
      Return a launcher for the computer
      If can be specified in config
      If not return the default one : launcher
    """
    conf=config.options(computer)
    name=conf.get("launcher","")
    return _register.get(name,launcher)

setLauncher("ssh")

try:
   # Try to import a plugin module (user defined) that
   # can define other launchers and register them
   # 1- define a class
   # 2- register it with : Launchers.register(name,klass)
   # 3- eventually set it as default one : Launchers.setLauncher(name)
   import pluginLauncher
except:
   import traceback
   traceback.print_exc()
   pass
