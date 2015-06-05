#!/usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2015  CEA/DEN, EDF R&D, OPEN CASCADE
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

###############################################
############### IMPORTANT NOTE ################
###############################################
# The salomeConsole.py script is obsolete.    #
# Please consider the new salome launcher.    #
###############################################


import os
import sys
import glob

#-------------------------------
# Python completion and others if you want
# You should have set PYTHONSTARTUP env variable
# or import user should try to import $HOME/.pythonrc.py
#-------------------------------
import user

#-------------------------------
# Get major CORBA objects
#-------------------------------
from omniORB import CORBA
import CosNaming
import salome_utils

import orbmodule

def getRunningSession():
  omniorbUserPath = os.getenv("OMNIORB_USER_PATH")
  files = glob.glob(os.path.join(omniorbUserPath,".omniORB_"+salome_utils.getUserName()+"_*[!last].cfg"))

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
#

class client(orbmodule.client):
  def initNS(self,args):
    # Obtain a reference to the root naming context
    obj = self.orb.resolve_initial_references("NameService")
    try:
      self.rootContext = obj._narrow(CosNaming.NamingContext)
      return
    except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
      print "It's not a valid naming service"
      self.rootContext = None
      raise
#

def startClient():
  try:
    clt=client()
  except Exception:
    sys.exit(1)
  #
  print "Naming Service address: ",clt.orb.object_to_string(clt.rootContext)

  clt.showNS()

  session_server = clt.Resolve('/Kernel/Session')
  if session_server:
    session=clt.waitNS("/Kernel/Session")
  catalog=clt.waitNS("/Kernel/ModulCatalog")
  studyMgr=clt.waitNS("/myStudyManager")
  import salome
  salome.salome_init()
  from salome import lcc
#

getRunningSession()
startClient()
