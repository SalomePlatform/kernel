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
INPUT_VAR_NAMES = "ExchangeVariables.InputVarNames"
OUTPUT_VAR_NAMES = "ExchangeVariables.OutputVarNames"
REF_ENTRY = "ExchangeVariables.RefEntry"

class Variable:
    
    def __init__(self, name, dimension = [], minValue = None, maxValue = None, initialValue = None):
        self.name = name
        
        # Reserved for future use
        self.dimension = dimension
        self.minValue = minValue
        self.maxValue = maxValue
        self.initialValue = initialValue


class ExchangeVariables:
    
    def __init__(self, inputVarList = [], outputVarList = [], refEntry = None):
        self.inputVarList = inputVarList
        self.outputVarList = outputVarList
        self.refEntry = refEntry


def createSObjectForExchangeVariables(fatherSobj, exchangeVariables,
                                      name = DEFAULT_NAME, icon = None, typeId = None):
    studyId = fatherSobj.GetStudy()._get_StudyId()
    editor = getStudyEditor(studyId)
    sobj = editor.createItem(fatherSobj,
                             name = name,
                             icon = icon,
                             typeId = typeId)
    _setSObjectForExchangeVariables(editor, sobj, exchangeVariables)

def updateSObjectForExchangeVariables(sobj, exchangeVariables,
                                      name = DEFAULT_NAME, icon = None, typeId = None):
    studyId = sobj.GetStudy()._get_StudyId()
    editor = getStudyEditor(studyId)
    editor.setItem(sobj, name = name, icon = icon, typeId = typeId)
    editor.builder.RemoveAttribute(sobj, "AttributeParameter")
    _setSObjectForExchangeVariables(editor, sobj, exchangeVariables)

def _setSObjectForExchangeVariables(editor, sobj, exchangeVariables):
    attr = editor.builder.FindOrCreateAttribute(sobj, "AttributeParameter")
    attr.SetStrArray(INPUT_VAR_NAMES, [x.name for x in exchangeVariables.inputVarList])
    attr.SetStrArray(OUTPUT_VAR_NAMES, [x.name for x in exchangeVariables.outputVarList])
    if exchangeVariables.refEntry is not None:
        attr.SetString(REF_ENTRY, exchangeVariables.refEntry)

def getExchangeVariablesFromSObject(sobj):
    (found, attr) = sobj.FindAttribute("AttributeParameter")
    if not found:
        return None
    refEntry = None
    if attr.IsSet(REF_ENTRY, 3):
        refEntry = attr.GetString(REF_ENTRY)
    return ExchangeVariables([Variable(name) for name in attr.GetStrArray(INPUT_VAR_NAMES)],
                             [Variable(name) for name in attr.GetStrArray(OUTPUT_VAR_NAMES)],
                             refEntry)
