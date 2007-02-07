#!/usr/bin/env python

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
   def initNS(self,args):
      # Obtain a reference to the root naming context
      obj         = self.orb.resolve_initial_references("NameService")
      try:
          self.rootContext = obj._narrow(CosNaming.NamingContext)
          return
      except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
          print "It's not a valid naming service"
          self.rootContext = None
          raise

clt=client()
print "Naming Service address: ",clt.orb.object_to_string(clt.rootContext)

clt.showNS()

session=clt.waitNS("/Kernel/Session")
catalog=clt.waitNS("/Kernel/ModulCatalog")
studyMgr=clt.waitNS("/myStudyManager")
import salome
salome.salome_init()
from salome import lcc
