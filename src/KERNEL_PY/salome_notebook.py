#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

#  File   : salome_notebook.py
#  Author : Roman NIKOLAEV, Open CASCADE S.A.S.
#  Module : SALOME
#  $Header:
#
"""
Module salome_notebook gives access to Salome Notebook.
"""

import salome

class PseudoStudyForNoteBook(object):
    
    def __init__(self, **kwargs):
        self.kwargs = kwargs
        pass
    
    def GetVariableNames(self):
        return self.kwargs.keys()
    
    def IsVariable(self, variableName):
        return variableName in self.kwargs
    
    def IsReal(self, variableName):
        val = self.kwargs[variableName]
        try:
            float(val)
            return True
        except:
            pass
        return False
    
    IsInteger = IsReal
    IsBoolean = IsReal
    
    def IsString(self, variableName):
        return not self.IsReal(variableName)
    
    def GetString(self, variableName):
        return self.kwargs[variableName]
    
    def GetReal(self, variableName):
        return float(self.kwargs[variableName])
    
    GetInteger = GetReal
    GetBoolean = GetReal
    
    pass

class NoteBook:
    
    def __init__(self, Study):
        self.myStudy = Study
    
    def set(self, variableName, variable):
        """
        Create (or modify) variable with name "variableName" 
        and value equal "theValue".
        """
        if type(variable) == float:
            self.myStudy.SetReal(variableName, variable)
            
        elif type(variable) == int:
            self.myStudy.SetInteger(variableName, variable)
            
        elif type(variable) == bool:
            self.myStudy.SetBoolean(variableName, variable)
            
        elif type(variable) == str:
            self.myStudy.SetString(variableName, variable)
            
    def get(self, variableName):
        """
        Return value of the variable with name "variableName".
        """
        aResult = None
        if self.myStudy.IsVariable(variableName):
            
            if self.myStudy.IsReal(variableName):
                aResult = self.myStudy.GetReal(variableName)

            elif self.myStudy.IsInteger(variableName):
                aResult = self.myStudy.GetInteger(variableName)

            elif self.myStudy.IsBoolean(variableName):
                aResult = self.myStudy.GetBoolean(variableName)

            elif self.myStudy.IsString(variableName):
                aResult = self.myStudy.GetString(variableName)
                aResult_orig = aResult
                l = self.myStudy.GetVariableNames()
                l.remove(variableName)
                # --
                # To avoid the smallest strings to be replaced first,
                # the list is sorted by decreasing lengths
                # --
                l.sort(key=str.__len__)
                l.reverse()
                for name in l:
                    if aResult.find(name) >= 0:
                        val = self.get(name)
                        aResult = aResult.replace(name, "%s"%(val))
                        pass
                    pass
                try:
                    aResult = eval(aResult)
                except Exception, e:
                    msg = str(e)
                    msg += "\n"
                    msg += "A problem occurs while parsing "
                    msg += "the variable %s "%(variableName.__repr__())
                    msg += "with value %s ..."%(aResult_orig.__repr__())
                    msg += "\n"
                    msg += "Please, check your notebook !"
                    raise Exception(msg)
                pass
                
        return aResult
    
    def isVariable(self, variableName): 
        """
        Return true if variable with name "variableName" 
        exists in the study, otherwise return false.
        """
        return self.myStudy.IsVariable(variableName)

    def setAs(self, variableName, typ):
        value = self.get(variableName)
        value = float(typ(value))
        self.myStudy.SetStringAsDouble(variableName, value)
        return
    
    def setAsReal(self, variableName):
        self.setAs(variableName, float)
        return
    
    def setAsInteger(self, variableName):
        self.setAs(variableName, int)
        return
    
    def setAsBool(self, variableName):
        self.setAs(variableName, bool)
        return
    
    def check(self):
        for variableName in self.myStudy.GetVariableNames():
            self.get(variableName)
            pass
        return
    
    pass

def checkThisNoteBook(**kwargs):
    study = PseudoStudyForNoteBook(**kwargs)
    note_book = NoteBook(study)
    note_book.check()
    return

notebook = NoteBook(salome.myStudy)
