#!/usr/bin/env python

import os
import sys
#-------------------------------
# Python completion and others if you want
# You should have set PYTHONSTARTUP env variable
# or import user should try to import $HOME/.pythonrc.py
#-------------------------------
import user

#-------------------------------
# Get major CORBA objects
#-------------------------------
import CORBA
import CosNaming
# There are cyclic dependencies between Engines, SALOME and SALOMEDS.
# import first Engines, then SALOME and then SALOMEDS
# Or use reload(Engines) to be safe.
import Engines
import SALOME
import SALOMEDS
import SALOME_ModuleCatalog
reload(Engines)
reload(SALOME)
reload(SALOMEDS)

import LifeCycleCORBA
import orbmodule
from runSalome import *

import Utils_Identity
files = glob.glob(os.path.join(os.environ["HOME"],Utils_Identity.getapplipath(),".omni*.cfg"))

filename=""
if len(files)==1:
  filename=files[0]
else:
  print "You have %d sessions running" % len(files)
  for f in files:
     print "Session:",f
     rep= raw_input("Do you want to connect to this session [y|n]")
     if rep == "y":
        filename=f
        break
     
if filename != "":
  os.environ['OMNIORB_CONFIG']=filename
else:
  rep= raw_input("Do you want to try a local session on port 2810 ? [y|n]")
  if rep == "y":
     # Try a local session running on port 2810
     sys.argv=sys.argv+['-ORBInitRef','NameService=corbaname::localhost:2810']
  else:
     sys.exit(1)

print sys.argv

#direct adress from clt.orb.object_to_string(clt.rootContext)
#sys.argv=sys.argv+['-ORBInitRef','NameService=IOR:010000000100000000000000010000000000000023000000010100000a0000006c6f63616c686f737400fa0a0b0000004e616d6553657276696365']

class client(orbmodule.client):
   def initNS(self):
      # Obtain a reference to the root naming context
      obj         = self.orb.resolve_initial_references("NameService")
      try:
          self.rootContext = obj._narrow(CosNaming.NamingContext)
          return
      except CORBA.COMM_FAILURE:
          print "It's not a valid naming service"
          self.rootContext = None
          raise

clt=client()
print "Naming Service address: ",clt.orb.object_to_string(clt.rootContext)

clt.showNS()

session=clt.waitNS("/Kernel/Session")
catalog=clt.waitNS("/Kernel/ModulCatalog")
studyMgr=clt.waitNS("/myStudyManager")
lcc = LifeCycleCORBA.LifeCycleCORBA(clt.orb)

#c=lcc.FindOrStartContainer("","totoPy")
#print c
