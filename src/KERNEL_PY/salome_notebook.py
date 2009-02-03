#  Copyright (C) 2008  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
#
#
#  File   : salome_notebook.py
#  Author : Roman NIKOLAEV, Open CASCADE S.A.S.
#  Module : SALOME
#  $Header:
"""
Module salome_notebook gives access to Salome Notebook.
"""

import salome

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
                
        return aResult
    
    def isVariable(self, variableName): 
	"""
	Return true if variable with name "variableName" 
	exists in the study, otherwise return false.
	"""
        return self.myStudy.IsVariable(variableName)
                
notebook = NoteBook(salome.myStudy)