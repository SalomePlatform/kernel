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

## @package orbmodule
# \brief Module that provides a client for %SALOME
#

import sys,os,time
import string
from nameserver import NamingServer
from omniORB import CORBA
from launchConfigureParser import verbose

# Import the stubs for the Naming service
import CosNaming

# -----------------------------------------------------------------------------

class client:
    """Client for SALOME"""

    def __init__(self,args=None):
      # Initialise the ORB
      self.orb=CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

      # Initialise the Naming Service
      self.initNS(args or {})

    # --------------------------------------------------------------------------

    def initNS(self,args):
      # Obtain a reference to the root naming context
      obj = self.orb.resolve_initial_references("NameService")
      try:
          self.rootContext = obj._narrow(CosNaming.NamingContext)
          return
      except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
          self.rootContext = None
          if verbose(): print "Launch Naming Service++",

      # On lance le Naming Server (doit etre dans le PATH)
      test = True
      if args['wake_up_session']:
        test = False
        pass
      if test:
        NamingServer(args).run()
        pass
      print "Searching Naming Service ",
      ncount=0
      delta=0.1
      while(ncount < 100):
          ncount += 1
          try:
              obj = self.orb.resolve_initial_references("NameService")
              self.rootContext = obj._narrow(CosNaming.NamingContext)
              break
          except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
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
      if not context:
        print "[NS] No context"
        return
      else:
        print context

      _,bi = context.list(0)
      if bi is not None:
        ok,b = bi.next_one()
        while(ok):
            for s in b.binding_name :
              print "%s%s.%s" %(dec,s.id,s.kind)
              if s.kind == "dir":
                  obj = context.resolve([s])
                  scontext = obj._narrow(CosNaming.NamingContext)
                  self.showNScontext(scontext,dec=dec+'  ')
            ok,b = bi.next_one()

    # --------------------------------------------------------------------------

    def showNS(self):
      """ Show the content of SALOME naming service """
      self.showNScontext(self.rootContext)

    # --------------------------------------------------------------------------

    def Resolve(self, Path):
      resolve_path = string.split(Path,'/')
      if resolve_path[0] == '': del resolve_path[0]
      dir_path = resolve_path[:-1]
      context_name = []
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
      except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
          obj = None
      return obj

    # --------------------------------------------------------------------------

    def waitNS(self,name,typobj=None,maxcount=240):
      count = 0
      delta = 0.5
      print "Searching %s in Naming Service " % name,
      while(1):
          count += 1
          if count > maxcount : raise RuntimeError, "Impossible de trouver %s" % name
          obj = self.Resolve(name)
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

    if sys.platform != "win32":
      def waitNSPID(self, theName, thePID, theTypObj = None):
        aCount = 0
        aDelta = 0.5
        anObj = None
        print "Searching %s in Naming Service " % theName,
        while(1):
          try:
            os.kill(thePID,0)
          except:
            raise RuntimeError, "Process %d for %s not found" % (thePID,theName)
          aCount += 1
          anObj = self.Resolve(theName)
          if anObj:
            print " found in %s seconds " % ((aCount-1)*aDelta)
            break
          else:
            sys.stdout.write('+')
            sys.stdout.flush()
            time.sleep(aDelta)
            pass
          pass
  
        if theTypObj is None:
          return anObj
  
        anObject = anObj._narrow(theTypObj)
        if anObject is None:
          print "%s exists but is not a %s" % (theName,theTypObj)
        return anObject


    # --------------------------------------------------------------------------

    def ResolveLogger(self, name):
      context_name = []
      context_name.append(CosNaming.NameComponent(name,""))

      try:
          obj = self.rootContext.resolve(context_name)
      except CosNaming.NamingContext.NotFound, ex:
          obj = None
      except CosNaming.NamingContext.InvalidName, ex:
          obj = None
      except CosNaming.NamingContext.CannotProceed, ex:
          obj = None
      except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
          obj = None
      return obj

    # --------------------------------------------------------------------------

    def waitLogger(self,name,typobj=None,maxcount=40):
      count = 0
      delta = 0.5
      print "Searching %s in Naming Service " % name,
      while(1):
          count += 1
          if count > maxcount : raise RuntimeError, "Impossible de trouver %s" % name
          obj = self.ResolveLogger(name)
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
