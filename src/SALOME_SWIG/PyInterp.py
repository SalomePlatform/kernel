#==============================================================================
#  File      : PyInterp.py
#  Created   : mer oct 17 08:42:01 CEST 2001
#  Author    : Paul RASCLE, EDF
#  Project   : SALOME
#  Copyright : EDF 2001
#  $Header$
#==============================================================================

import sys
from omniORB import CORBA
from LifeCycleCORBA import *
from libSALOME_Swig import *
import SALOMEDS
from SALOME_NamingServicePy import *

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
            print a + ">" + CSO.GetID() + " " + t_name[1]
        t_RefSO = CSO.ReferencedObject()
        if t_RefSO[0] == 1:
            RefSO = t_RefSO[1]
            ofs = 1
            a = ""
            while ofs <= offset:
                a = a + "  "
                ofs = ofs +1
            print a + ">" + RefSO.GetID()
        DumpComponent(Study, CSO, offset+2)

    #--------------------------------------------------------------------------

def DumpStudy(Study):
    itcomp = Study.NewComponentIterator()
    while itcomp.More():
        SC = itcomp.Value()
        itcomp.Next()
        name = SC.ComponentDataType()
        print "-> ComponentDataType is " + name
        DumpComponent(Study, SC, 1)
        

    #--------------------------------------------------------------------------

# initialise the ORB
orb = CORBA.ORB_init([''], CORBA.ORB_ID)

# create an LifeCycleCORBA instance
lcc = LifeCycleCORBA(orb)

# create an SALOMEGUI_Swig instance
sg = SALOMEGUI_Swig()

#create an naming service instance
naming_service = SALOME_NamingServicePy_i(orb)

# get active study name and id
myStudyName = sg.getActiveStudyName()
print myStudyName

myStudyId = sg.getActiveStudyId()
print myStudyId

# get Study Manager reference
obj = naming_service.Resolve('myStudyManager')
myStudyManager = obj._narrow(SALOMEDS.StudyManager)

# get active study
myStudy = myStudyManager.GetStudyByName(myStudyName)

