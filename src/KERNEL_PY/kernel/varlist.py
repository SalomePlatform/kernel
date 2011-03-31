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
#  See http://www.salome-platform.org/ or
#  email : webmaster.salome@opencascade.com
#

"""
This module provides classes and functions to handle "Exchange Variables",
i.e. description of variables to be exchanged between a supervision code and a
computation code. These Exchange Variables can be stored in a SObject in
Salome study.
"""

from studyedit import getStudyEditor

DEFAULT_NAME = "Variables"
INPUT_VAR_NAMES = "ExchangeVariables.InputVarNames"
OUTPUT_VAR_NAMES = "ExchangeVariables.OutputVarNames"
REF_ENTRY = "ExchangeVariables.RefEntry"

class Variable:
    """
    This class describes a single variable. For now, it is only described by
    its name. Other attributes are reserved for future use.
    """
    
    def __init__(self, name, dimension = [], minValue = None, maxValue = None,
                 initialValue = None):
        self.name = name
        
        # Reserved for future use
        self.dimension = dimension
        self.minValue = minValue
        self.maxValue = maxValue
        self.initialValue = initialValue


class ExchangeVariables:
    """
    This class describes "Exchange Variables", i.e. a structure containing all
    the necessary information to exchange variables between a supervision code
    and a computation code.

    .. attribute:: inputVarList
    
       This instance attribute is a list of :class:`Variable` objects,
       describing the input variables for the computation code.

    .. attribute:: outputVarList
    
       This instance attribute is a list of :class:`Variable` objects,
       describing the output variables for the computation code.

    .. attribute:: refEntry

       This instance attribute is optional and can be used to store a
       reference to a Salome entry containing an "Exchange Variable" SObject
       that was used to build this one (when the current object has been built
       by selecting variables of interest in a list of potential variables).

    """
    
    def __init__(self, inputVarList = [], outputVarList = [],
                 refEntry = None):
        self.inputVarList = inputVarList
        self.outputVarList = outputVarList
        self.refEntry = refEntry


def createSObjectForExchangeVariables(fatherSobj, exchangeVariables,
                                      name = DEFAULT_NAME,
                                      icon = None, typeId = None):
    """
    Create a SObject to store an :class:`ExchangeVariables` instance.

    :type  fatherSobj: SObject
    :param fatherSobj: parent of the SObject to create.

    :type  exchangeVariables: :class:`ExchangeVariables`
    :param exchangeVariables: :class:`ExchangeVariables` instance to store in
                              Salome study.

    :type  name: string
    :param name: name of the SObject to create.

    :type  icon: string
    :param icon: icon of the SObject to create.

    :type  typeId: integer
    :param typeId: type of the SObject to create.

    :return: the newly created SObject.

    """
    studyId = fatherSobj.GetStudy()._get_StudyId()
    editor = getStudyEditor(studyId)
    sobj = editor.createItem(fatherSobj,
                             name = name,
                             icon = icon,
                             typeId = typeId)
    _setSObjectForExchangeVariables(editor, sobj, exchangeVariables)

def updateSObjectForExchangeVariables(sobj, exchangeVariables,
                                      name = DEFAULT_NAME,
                                      icon = None, typeId = None):
    """
    Update an existing SObject storing an :class:`ExchangeVariables` instance.

    :type  sobj: SObject
    :param sobj: the SObject to update.

    See :func:`createSObjectForExchangeVariables` for the description of the
    other parameters.

    """
    studyId = sobj.GetStudy()._get_StudyId()
    editor = getStudyEditor(studyId)
    editor.setItem(sobj, name = name, icon = icon, typeId = typeId)
    editor.builder.RemoveAttribute(sobj, "AttributeParameter")
    _setSObjectForExchangeVariables(editor, sobj, exchangeVariables)

def _setSObjectForExchangeVariables(editor, sobj, exchangeVariables):
    attr = editor.builder.FindOrCreateAttribute(sobj, "AttributeParameter")
    attr.SetStrArray(INPUT_VAR_NAMES,
                     [x.name for x in exchangeVariables.inputVarList])
    attr.SetStrArray(OUTPUT_VAR_NAMES,
                     [x.name for x in exchangeVariables.outputVarList])
    if exchangeVariables.refEntry is not None:
        attr.SetString(REF_ENTRY, exchangeVariables.refEntry)

def getExchangeVariablesFromSObject(sobj):
    """
    Get an :class:`ExchangeVariables` instance from a SObject that stores it.

    :type  sobj: SObject
    :param sobj: the SObject from which to read the :class:`ExchangeVariables`
                 instance.

    :return: the newly created :class:`ExchangeVariables` instance.

    """
    (found, attr) = sobj.FindAttribute("AttributeParameter")
    if not found:
        return None
    refEntry = None
    if attr.IsSet(REF_ENTRY, 3):
        refEntry = attr.GetString(REF_ENTRY)
    return ExchangeVariables(
            [Variable(name) for name in attr.GetStrArray(INPUT_VAR_NAMES)],
            [Variable(name) for name in attr.GetStrArray(OUTPUT_VAR_NAMES)],
            refEntry)
