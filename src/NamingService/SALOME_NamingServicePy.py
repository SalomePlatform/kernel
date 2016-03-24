#! /usr/bin/env python
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

#  SALOME NamingService : wrapping NamingService services
#  File   : SALOME_NamingServicePy.py
#  Author : Estelle Deville, CEA
#  Module : SALOME
#  $Header$
## @package SALOME_NamingServicePy
# \brief Module to manage SALOME naming service from python
#
import sys
import time
from omniORB import CORBA
import CosNaming
import string
from string import *

from SALOME_utilities import *
#=============================================================================

class SALOME_NamingServicePy_i(object):
    """
      A class to manage SALOME naming service from python code
    """
    _orb = None
    _root_context=None
    _current_context=None
    _obj=None
    
    #-------------------------------------------------------------------------

    def __init__(self, orb=None):
        """
        Standard Constructor, with ORB reference.
 
        Initializes the naming service root context
        """
        #MESSAGE ( "SALOME_NamingServicePy_i::__init__" )
        if orb is None:
          orb=CORBA.ORB_init([''], CORBA.ORB_ID)
        self._orb = orb
        # initialize root context and current context
        ok = 0
        steps = 240
        while steps > 0 and ok == 0:
          try:
            obj =self._orb.resolve_initial_references("NameService")
            self._root_context =obj._narrow(CosNaming.NamingContext)
            self._current_context = self._root_context

        
            if self._root_context is None :
              #MESSAGE ( "Name Service Reference is invalid" )
              #sys.exit(1)
              MESSAGE(" Name service not found")
            else:
              ok = 1
          except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
            MESSAGE(" Name service not found")
          time.sleep(0.25)
          steps = steps - 1
        if steps == 0 and self._root_context is None: 
          MESSAGE ( "Name Service Reference is invalid" )
          sys.exit(1)

    #-------------------------------------------------------------------------

    def Register(self,ObjRef, Path):
        """ ns.Register(object,pathname )  
        
        register a CORBA object under a pathname
        """

        MESSAGE ( "SALOME_NamingServicePy_i::Register" )
        _not_exist = 0
        path_list = list(Path)
        if path_list[0]=='/':
            self._current_context = self._root_context
            #delete first '/' before split
            Path=Path[1:]

        result_resolve_path = string.split(Path,'/')
        if len(result_resolve_path)>1:
            # A directory is treated (not only an object name)
            # We had to test if the directory where ObjRef should be recorded 
            # is already done
            # If not, the new context has to be created
            _context_name = []
            for i in range(len(result_resolve_path)-1):
                _context_name.append(CosNaming.NameComponent(result_resolve_path[i],"dir"))
            
            try:
                obj = self._current_context.resolve(_context_name)
                self._current_context = obj._narrow(CosNaming.NamingContext)
            except CosNaming.NamingContext.NotFound, ex:
                _not_exist = 1
            except CosNaming.NamingContext.InvalidName, ex:
                MESSAGE ( "Register : CosNaming.NamingContext.InvalidName" )
            except CosNaming.NamingContext.CannotProceed, ex:
                MESSAGE ( "Register : CosNaming.NamingContext.CannotProceed" )
            except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
                MESSAGE ( "Register : CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE" )

            if _not_exist:
                # at least one context of the complete path is not created, we had
                # to create it or them
                _context_name = []
                for i in range(len(result_resolve_path)-1):
                    _context_name = [CosNaming.NameComponent(result_resolve_path[i],"dir")]

                    try:
                        obj = self._current_context.resolve(_context_name)
                        self._current_context = obj._narrow(CosNaming.NamingContext)
                    except CosNaming.NamingContext.NotFound, ex:
                        #This context is not created. It will be done
                        self._current_context = self._current_context.bind_new_context(_context_name)

        #The current directory is now the directory where the object should 
        #be recorded
         
        _context_name = [CosNaming.NameComponent(result_resolve_path[len(result_resolve_path)-1],"object")]
        try:
            self._current_context.bind(_context_name,ObjRef)
        except CosNaming.NamingContext.NotFound, ex:
            MESSAGE ( "Register : CosNaming.NamingContext.NotFound" )
        except CosNaming.NamingContext.InvalidName, ex:
            MESSAGE ( "Register : CosNaming.NamingContext.InvalidName" )
        except CosNaming.NamingContext.CannotProceed, ex:
            MESSAGE ( "Register : CosNaming.NamingContext.CannotProceed" )
        except CosNaming.NamingContext.AlreadyBound, ex:
            MESSAGE ( "Register : CosNaming.NamingContext.AlreadyBound, object will be rebind" )
            self._current_context.rebind(_context_name,ObjRef)
        except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
            MESSAGE ( "Register : CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE" )

    #-------------------------------------------------------------------------

    def Resolve(self, Path):
        """ ns.Resolve(pathname) -> object

        find a CORBA object (ior) by its pathname
        """
        #MESSAGE ( "SALOME_NamingServicePy_i::Resolve" )
        path_list = list(Path)
        if path_list[0]=='/':
            self._current_context = self._root_context
            #delete first '/' before split
            Path=Path[1:]

        result_resolve_path = string.split(Path,'/')
        _context_name=[]
        for i in range(len(result_resolve_path)-1):
            _context_name.append(CosNaming.NameComponent(result_resolve_path[i],"dir"))
        _context_name.append(CosNaming.NameComponent(result_resolve_path[len(result_resolve_path)-1],"object"))
        try:
            self._obj = self._current_context.resolve(_context_name)
        except CosNaming.NamingContext.NotFound, ex:
            MESSAGE ( "Resolve : CosNaming.NamingContext.NotFound" )
            self._obj = None
        except CosNaming.NamingContext.InvalidName, ex:
            MESSAGE ( "Resolve : CosNaming.NamingContext.InvalidName" )
            self._obj = None
        except CosNaming.NamingContext.CannotProceed, ex:
            MESSAGE ( "Resolve : CosNaming.NamingContext.CannotProceed" )
            self._obj = None
        except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
            MESSAGE ( "Resolve : CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE" )
            self._obj = None
        return self._obj


    #-------------------------------------------------------------------------

    def Create_Directory(self,ObjRef, Path):
        """ ns.Create_Directory(ObjRef, Path) 

        create a sub directory 
        """
        MESSAGE ( "SALOME_NamingServicePy_i::Create_Directory" )
        _not_exist = 0
        path_list = list(Path)
        if path_list[0]=='/':
            self._current_context = self._root_context
            #delete first '/' before split
            Path=Path[1:]

        result_resolve_path = string.split(Path,'/')
        _context_name = []
        for i in range(len(result_resolve_path)):
            _context_name[CosNaming.NameComponent(result_resolve_path[i],"dir")]            
            try:
                obj = self._current_context.resolve(_context_name)
                self._current_context = obj._narrow(CosNaming.NamingContext)
            except CosNaming.NamingContext.NotFound, ex:
                self._current_context = self._current_context.bind_new_context(_context_name)
            except CosNaming.NamingContext.InvalidName, ex:
                MESSAGE ( "Create_Directory : CosNaming.NamingContext.InvalidName" )
            except CosNaming.NamingContext.CannotProceed, ex:
                MESSAGE ( "Create_Directory : CosNaming.NamingContext.CannotProceed" )
            except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
                MESSAGE ( "Create_Directory : CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE" )
 
    def Destroy_Name(self,Path):
      """ ns.Destroy_Name(Path) 

        remove a name in naming service
      """
      resolve_path=string.split(Path,'/')
      if resolve_path[0] == '': del resolve_path[0]
      dir_path=resolve_path[:-1]
      context_name=[]
      for e in dir_path:
         context_name.append(CosNaming.NameComponent(e,"dir"))
      context_name.append(CosNaming.NameComponent(resolve_path[-1],"object"))
      
      try:
        self._root_context.unbind(context_name)
      except CosNaming.NamingContext.NotFound, ex:
        return
      except CORBA.Exception,ex:
        return

    def Destroy_FullDirectory(self,Path):
      """ ns.Destroy_FullDirectory(Path)

        remove recursively a directory
      """
      context_name=[]
      for e in string.split(Path,'/'):
        if e == '':continue
        context_name.append(CosNaming.NameComponent(e,"dir"))

      try:
        context=self._root_context.resolve(context_name)
      except CosNaming.NamingContext.NotFound, ex:
        return
      except CORBA.Exception,ex:
        return

      bl,bi=context.list(0)
      if bi is not None:
         ok,b=bi.next_one()
         while(ok):
            for s in b.binding_name :
               if s.kind == "object":
                  context.unbind([s])
               elif s.kind == "dir":
                  context.unbind([s])
            ok,b=bi.next_one()

      context.destroy()
      self._root_context.unbind(context_name)
