#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
# 
#  This library is free software; you can redistribute it and/or 
#  modify it under the terms of the GNU Lesser General Public 
#  License as published by the Free Software Foundation; either 
#  version 2.1 of the License. 
# 
#  This library is distributed in the hope that it will be useful, 
#  but WITHOUT ANY WARRANTY; without even the implied warranty of 
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
#  Lesser General Public License for more details. 
# 
#  You should have received a copy of the GNU Lesser General Public 
#  License along with this library; if not, write to the Free Software 
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
# 
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
#
#
#
#  File   : salome.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$

from omniORB import CORBA
from LifeCycleCORBA import *
from libSALOME_Swig import *
import SALOMEDS
from SALOME_NamingServicePy import *

from SALOME_utilities import *

#--------------------------------------------------------------------------

def DumpComponent(Study, SO, offset):
    it = Study.NewChildIterator(SO)
    Builder = Study.NewBuilder()
    while it.More():
        CSO = it.Value()
        it.Next()
        anAttr = Builder.FindOrCreateAttribute(CSO, "AttributeName")
        AtName = anAttr._narrow(SALOMEDS.AttributeName)
        t_name = AtName.Value()
        if t_name[0] == 1:
            ofs = 1
            a = ""
            while ofs <= offset:
                a = a + "--"
                ofs = ofs +1
            MESSAGE( a + ">" + str(CSO.GetID()) + " " + str(t_name[1]) )
        t_RefSO = CSO.ReferencedObject()
        if t_RefSO[0] == 1:
            RefSO = t_RefSO[1]
            ofs = 1
            a = ""
            while ofs <= offset:
                a = a + "  "
                ofs = ofs +1
            MESSAGE( a + ">" + str(RefSO.GetID()) )
        DumpComponent(Study, CSO, offset+2)

    #--------------------------------------------------------------------------

def DumpStudy(Study):
    itcomp = Study.NewComponentIterator()
    while itcomp.More():
        SC = itcomp.Value()
        itcomp.Next()
        name = SC.ComponentDataType()
        MESSAGE( "-> ComponentDataType is " + name )
        DumpComponent(Study, SC, 1)
        

    #--------------------------------------------------------------------------

def ImportComponentGUI(ComponentName):
    libName = "lib" + ComponentName + "_Swig"
    command = "from " + libName + " import *"
    exec ( command )
    constructor = ComponentName + "_Swig()"
    command = "gui = " + constructor
    exec ( command )
    return gui

    #--------------------------------------------------------------------------

def SalomeGUIgetAllSelected(self):
    selNumber = self.SelectedCount()
    listSelected = []
    for i in range(selNumber):
        listSelected.append(self.getSelected(i))
    return listSelected

class SalomeGUI(SALOMEGUI_Swig):
    getAllSelected = SalomeGUIgetAllSelected
    
    #--------------------------------------------------------------------------

def IDToObject(id):
    myObj = None
    mySO = myStudy.FindObjectID(id);
    if mySO is not None:
        ok, anAttr = mySO.FindAttribute("AttributeIOR")
        if ok:
            AtIOR = anAttr._narrow(SALOMEDS.AttributeIOR)
            if AtIOR.Value() != "":
                myObj = orb.string_to_object(AtIOR.Value())
    return myObj

def ObjectToSObject(obj):
    mySO = None
    if obj is not None:
        ior =  orb.object_to_string(obj)
        if ior != "":
            mySO = myStudy.FindObjectIOR(ior)
    return mySO

def ObjectToID(obj):
    mySO = ObjectToSObject(obj)
    if mySO:
        return mySO.GetID()
    return ""

def IDToSObject(id):
    mySO = myStudy.FindObjectID(id);
    return mySO

    #--------------------------------------------------------------------------

# initialise the ORB
orb = CORBA.ORB_init([''], CORBA.ORB_ID)

# create an LifeCycleCORBA instance
lcc = LifeCycleCORBA(orb)

# create an SALOMEGUI_Swig instance
sg = SalomeGUI()

#create an naming service instance
naming_service = SALOME_NamingServicePy_i(orb)

# get active study name and id
myStudyName = sg.getActiveStudyName()
MESSAGE( myStudyName )

myStudyId = sg.getActiveStudyId()
MESSAGE( str(myStudyId) )

# get Study Manager reference
obj = naming_service.Resolve('myStudyManager')
myStudyManager = obj._narrow(SALOMEDS.StudyManager)

# get active study
myStudy = myStudyManager.GetStudyByName(myStudyName)

