#  Copyright (C) 2011  CEA/DEN, EDF R&D, OPEN CASCADE
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
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

from studyedit import getStudyEditor

DEFAULT_NAME = "Variables"

def createVarListObj(fatherSobj, inputVarList = [], outputVarList = [],
                     name = DEFAULT_NAME, icon = None, typeId = None):
    studyId = fatherSobj.GetStudy()._get_StudyId()
    editor = getStudyEditor(studyId)
    sobj = editor.createItem(fatherSobj,
                             name = name,
                             icon = icon,
                             typeId = typeId)
    attr = editor.builder.FindOrCreateAttribute(sobj, "AttributeParameter")
    attr.SetStrArray("inputVarList", inputVarList)
    attr.SetStrArray("outputVarList", outputVarList)

def getVarList(sobj):
    studyId = sobj.GetStudy()._get_StudyId()
    editor = getStudyEditor(studyId)
    (found, attr) = editor.builder.FindAttribute(sobj, "AttributeParameter")
    if not found:
        return (None, None)
    inputVarList = attr.GetStrArray("inputVarList")
    outputVarList = attr.GetStrArray("outputVarList")
    return (inputVarList, outputVarList)
