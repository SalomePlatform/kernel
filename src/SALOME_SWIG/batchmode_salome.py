#  Copyright (C) 2003  CEA/DEN, EDF R&D
#
#
#
#  File   : batchmode_salome.py
#  Module : SALOME

import salome_shared_modules

from omniORB import CORBA
from LifeCycleCORBA import *
import SALOMEDS
from SALOME_NamingServicePy import *

#--------------------------------------------------------------------------

def ImportComponentGUI(ComponentName):
    libName = "lib" + ComponentName + "_Swig"
    command = "from " + libName + " import *"
    exec ( command )
    constructor = ComponentName + "GUI_Swig()"
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
    #--------------------------------------------------------------------------

def generateName(prefix = None):
    import whrandom;
    int = whrandom.randint(1,1000);
    if prefix is None:
        return "Study" + str(int)
    else :
        return prefix + str(int)

from libSALOME_Swig import *
###from SalomePyQt import *
class SalomeGUI(SALOMEGUI_Swig):
    getAllSelected = SalomeGUIgetAllSelected
   
    def getDesktop(self) :
#	return SalomePyQt.getDesktop()
	return None

    def getSelection(self) :
#	return SalomePyQt.getSelection()
	return None

    #--------------------------------------------------------------------------

def IDToObject(id):
    mySO = myStudy.FindObjectID(id);
    print id
    Builder = myStudy.NewBuilder()
    anAttr = Builder.FindOrCreateAttribute(mySO, "AttributeIOR")
    AtIOR = anAttr._narrow(SALOMEDS.AttributeIOR)
    myObj = orb.string_to_object(AtIOR.Value())
    return myObj

def IDToSObject(id):
    mySO = myStudy.FindObjectID(id);
    return mySO
    #--------------------------------------------------------------------------

#--------------------------------------------------------------------------
# initialise the ORB
orb = CORBA.ORB_init([''], CORBA.ORB_ID)

# create an LifeCycleCORBA instance
lcc = LifeCycleCORBA(orb)

#create a naming service instance
naming_service = SALOME_NamingServicePy_i(orb)

# get Study Manager reference
obj = naming_service.Resolve('myStudyManager')
myStudyManager = obj._narrow(SALOMEDS.StudyManager)

# create new study
myStudy = myStudyManager.NewStudy("Study1")

myStudyName = myStudy._get_Name()

myStudyId = myStudy._get_StudyId()
print myStudyId

