import sys,os,time
import string
from omniORB import CORBA

# Import the stubs for the Naming service
import CosNaming

# -----------------------------------------------------------------------------

class Server:
   XTERM="/usr/bin/X11/xterm -iconic -e "
   CMD=""

   def run(self):
       commande=self.XTERM+self.CMD
       print commande
       ier=os.system(commande)
       if ier:print "Commande failed"

# -----------------------------------------------------------------------------

class NamingServer(Server):
   XTERM=""
   USER=os.getenv('USER')
   if USER is None:
      USER='anonymous'
   os.system("mkdir -m 777 -p /tmp/logs")
   LOGDIR="/tmp/logs/" + USER
   os.system("mkdir -m 777 -p " + LOGDIR)
   CMD="runNS.sh > " + LOGDIR + "/salomeNS.log 2>&1"

# -----------------------------------------------------------------------------

class client:

   def __init__(self):
      # Initialise the ORB
      self.orb=CORBA.ORB_init(sys.argv, CORBA.ORB_ID)
      # Initialise the Naming Service
      self.initNS()

   # --------------------------------------------------------------------------

   def initNS(self):
      # Obtain a reference to the root naming context
      obj         = self.orb.resolve_initial_references("NameService")
      try:
          self.rootContext = obj._narrow(CosNaming.NamingContext)
          return
      except CORBA.COMM_FAILURE:
          self.rootContext = None
          print "Lancement du Naming Service",
          
      # On lance le Naming Server (doit etre dans le PATH)
      NamingServer().run()
      print "Searching Naming Service ",
      ncount=0
      delta=0.1
      while(ncount < 10):
          ncount += 1
          try:
              obj = self.orb.resolve_initial_references("NameService")
              self.rootContext = obj._narrow(CosNaming.NamingContext)
              break
          except (CORBA.COMM_FAILURE,CORBA.OBJECT_NOT_EXIST):
              self.rootContext = None
              sys.stdout.write('+')
              sys.stdout.flush()
              time.sleep(delta)

      if self.rootContext is None:
          print "Failed to narrow the root naming context"
          sys.exit(1)
      print " found in %s seconds " % ((ncount-1)*delta)

   # --------------------------------------------------------------------------

   def showNScontext(self,context,dec=''):
      bl,bi=context.list(0)
      ok,b=bi.next_one()
      while(ok):
         for s in b.binding_name :
            print "%s%s.%s" %(dec,s.id,s.kind)
            if s.kind == "dir":
               obj=context.resolve([s])
               scontext = obj._narrow(CosNaming.NamingContext)
               self.showNScontext(scontext,dec=dec+'  ')
         ok,b=bi.next_one()

   # --------------------------------------------------------------------------

   def showNS(self):
      """ Show the content of NS"""
      self.showNScontext(self.rootContext)

   # --------------------------------------------------------------------------

   def Resolve(self, Path):
      resolve_path=string.split(Path,'/')
      if resolve_path[0] == '': del resolve_path[0]
      dir_path=resolve_path[:-1]
      context_name=[]
      for e in dir_path:
         context_name.append(CosNaming.NameComponent(e,"dir"))
      context_name.append(CosNaming.NameComponent(resolve_path[-1],"object"))

      try:
          obj = self.rootContext.resolve(context_name)
      except CosNaming.NamingContext.NotFound, ex:
          obj = None
      except CosNaming.NamingContext.InvalidName, ex:
          obj = None
      except CosNaming.NamingContext.CannotProceed, ex:
          obj = None
      except CORBA.COMM_FAILURE, ex:
          obj = None
      return obj

   # --------------------------------------------------------------------------

   def waitNS(self,name,typobj=None,maxcount=40):
      count=0
      delta=0.5
      print "Searching %s in Naming Service " % name,
      while(1):
          count += 1
          if count > maxcount : raise "Impossible de trouver %s" % name
          obj=self.Resolve(name)
          if obj : 
              print " found in %s seconds " % ((count-1)*delta)
              break
          else:
              sys.stdout.write('+')
              sys.stdout.flush()
              time.sleep(delta)
 
      if typobj is None:return obj

      nobj = obj._narrow(typobj)
      if nobj is None:
            print "%s exists but is not a %s" % (name,typobj)
      return nobj

   # --------------------------------------------------------------------------

   def ResolveLogger(self, name):
      context_name=[]
      context_name.append(CosNaming.NameComponent(name,""))

      try:
          obj = self.rootContext.resolve(context_name)
      except CosNaming.NamingContext.NotFound, ex:
          obj = None
      except CosNaming.NamingContext.InvalidName, ex:
          obj = None
      except CosNaming.NamingContext.CannotProceed, ex:
          obj = None
      except CORBA.COMM_FAILURE, ex:
          obj = None
      return obj
   
   # --------------------------------------------------------------------------

   def waitLogger(self,name,typobj=None,maxcount=40):
      count=0
      delta=0.5
      print "Searching %s in Naming Service " % name,
      while(1):
          count += 1
          if count > maxcount : raise "Impossible de trouver %s" % name
          obj=self.ResolveLogger(name)
          if obj : 
              print " found in %s seconds " % ((count-1)*delta)
              break
          else:
              sys.stdout.write('+')
              sys.stdout.flush()
              time.sleep(delta)
 
      if typobj is None:return obj

      nobj = obj._narrow(typobj)
      if nobj is None:
            print "%s exists but is not a %s" % (name,typobj)
      return nobj

