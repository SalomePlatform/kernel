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

#  File   : Help.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
class SalomeDoc:
    def __init__(self, aDoc):
        self.doc = aDoc
    def __repr__(self):
        print self.doc
        return "---"
    def salome(self):
        doc_salome = '''
MODULE : salome
---------------
module salome gives access to Salome ressources:
variables:

  salome.orb             : CORBA
  salome.naming_service  : instance of naming Service class
      methods:
          Resolve(name)  : find a CORBA object (ior) by its pathname
          Register(name) : register a CORBA object under a pathname
  salome.lcc             : instance of lifeCycleCORBA class
      methods:
          FindOrLoadComponent(server,name) :
                           obtain an Engine (CORBA object)
                           or launch the Engine if not found,
                           with a Server name and an Engine name
  salome.sg
      methods:
         updateObjBrowser(bool):
         getActiveStudyId():
         getActiveStudyName():
         
         SelectedCount():      returns number of selected objects
         getSelected(i):       returns entry of selected object number i
         getAllSelected():     returns list of entry of selected objects
         AddIObject(Entry):    select an existing Interactive object
         RemoveIObject(Entry): remove object from selection
         ClearIObjects():      clear selection
         
         Display(*Entry):
         DisplayOnly(Entry):
         Erase(Entry):
         DisplayAll():
         EraseAll():

         IDToObject(Entry):    returns CORBA reference from entry

  salome.myStudyName     : active Study Name
  salome.myStudyId       : active Study Id
  salome.myStudy         : the active Study itself (CORBA ior)
                           methods : defined in SALOMEDS.idl
                                                         
methods:
  salome.DumpStudy(study) : Dump a study, given the ior
---
'''
        print doc_salome
        
    def geompy(self):
        doc_geompy = '''
MODULE : geompy
---------------
module geompy provides an encapsulation of GEOM Engine methods
variables:
  geompy.geom               : a Geometry Engine, found or loaded
                              at first import of module geompy.
                              methods : defined in GEOM_Gen.idl
  geompy.myBuilder          : a study builder
  geompy.father             : GEOM root in current study (salome.myStudy)

methods:
  addToStudy(aShape, aName) : add the shape into the current study
  --- all methods of GEOM_Gen.idl that returns a shape are encapsulated,
      with the same interface : shapes are named with their ior
'''
        print doc_geompy
        
    def supervision(self):
        doc_supervision = '''
MODULES : SALOME_SuperVisionEditor and SALOME_SuperVisionExecutor
-----------------------------------------------------------------
this modules provide access to Editor and Executor Engine methods

See SUPERV.idl

In order to run the example (supervisionexample.py)

    Type : from supervisionexample import *
           supervisionexample.py contains comments

A new python example avoids references to LifeCycleCORBA
                     avoids references to NamingService
                     avoids references to ModuleCatalog
                     avoids SuperVisionComponent creation
                     allows G.Input(...) instead of AddInput(G,...)
                     replaces Editor/Executor with Graph
                     allows Nodes, Ports and Links CORBA objects
                     shortens methods names
                     ...

    See /SuperVisionTest/resources/GraphExample.py
                                   and GraphExample.xml
---
'''
        print doc_supervision
        
    

Help = SalomeDoc('''
Availables modules:
  salome      : gives access to Salome ressources
  geompy      : encapsulation of GEOM Engine methods
  supervision : gives access to SuperVision Engine
To obtain specific help on a module "truc", type: Help.truc()
To run an example, type: import example3
''')
  
