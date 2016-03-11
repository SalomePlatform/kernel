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

#  File   : PyInterp.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
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
