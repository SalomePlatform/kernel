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
#  File   : salome_iapp.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$

from libSALOME_Swig import *
import salome_ComponentGUI

    #--------------------------------------------------------------------------

IN_SALOME_GUI=0

def ImportComponentGUI(ComponentName):
    if IN_SALOME_GUI:
        libName = "lib" + ComponentName + "_Swig"
        command = "from " + libName + " import *"
        exec ( command )
        constructor = ComponentName + "_Swig()"
        command = "gui = " + constructor
        exec ( command )
        return gui
    else:
        print "Warning: ImportComponentGUI(",ComponentName,") outside GUI !"
        print "calls to GUI methods may crash..."
        return salome_ComponentGUI

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

salome_iapp_initial = 1

def salome_iapp_init():
    global salome_iapp_initial
    global sg,IN_SALOME_GUI

    if salome_iapp_initial:
        salome_iapp_initial=0
        
        # create a SALOMEGUI_Swig instance
        sg = SalomeGUI()
        IN_SALOME_GUI=sg.hasDesktop()
    return sg
