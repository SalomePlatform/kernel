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

#  File   : salome_iapp.py
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
## @package salome_iapp
# Module salome gives access to Salome GUI ressources (if GUI has been launched).
#
import salome_ComponentGUI

    #--------------------------------------------------------------------------

IN_SALOME_GUI=None

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

    #--------------------------------------------------------------------------

def hasDesktop():
    return IN_SALOME_GUI

    #--------------------------------------------------------------------------

salome_iapp_initial = 1

class SalomeOutsideGUI(object):
    """
    Provides a replacement for class SalomeGUI outside GUI process.
    Do almost nothing
    """
    global myStudyId, myStudyName
    
    def hasDesktop(self):
        """Indicate if GUI is running"""
        return False
    
    def updateObjBrowser(self, bid):
        """update the GUI object browser"""
        print "SalomeOutsideGUI: no objectBrowser update outside GUI"
        pass
    
    def getActiveStudyId(self):
        """Get the active study id"""
        print "SalomeOutsideGUI.getActiveStudyId: avoid use outside GUI"
        return myStudyId
    
    def getActiveStudyName(self):
        """Get the active study name"""
        print "SalomeOutsideGUI.getActiveStudyName: avoid use outside GUI"
        return myStudyName
    
    def SelectedCount(self):
        """Get the number of active selections"""
        print "SalomeOutsideGUI: no selection mecanism available outside GUI"
        return 0
    
    def getSelected(self, i):
        """Get the selection number i """
        print "SalomeOutsideGUI: no selection mecanism available outside GUI"
        return none
    
    def AddIObject(self, Entry):
        """Add an entry"""
        print "SalomeOutsideGUI.AddIOObject: not available outside GUI"
        pass
    
    def RemoveIObject(self, Entry):
        """Remove an entry"""
        print "SalomeOutsideGUI.REmoveIOObject: not available outside GUI"
        pass
    
    def ClearIObjects(self):
        """Clear entries"""
        print "SalomeOutsideGUI.ClearIOObject: not available outside GUI"
        pass
    
    def Display(self, Entry):
        """Display an entry"""
        print "SalomeOutsideGUI.Display: not available outside GUI"
        pass
    
    def DisplayOnly(self, Entry):
        """Display only an entry"""
        print "SalomeOutsideGUI.DisplayOnly: not available outside GUI"
        pass
    
    def Erase(self, Entry):
        """Erase en entry"""
        print "SalomeOutsideGUI.Erase: not available outside GUI"
        pass
    
    def DisplayAll(self):
        """Display all"""
        print "SalomeOutsideGUI.Erase: not available outside GUI"
        pass
    
    def EraseAll(self):
        """Erase all"""
        print "SalomeOutsideGUI.EraseAll: not available outside GUI"
        pass

    def IsInCurrentView(self, Entry):
        """Indicate if an entry is in current view"""
        print "SalomeOutsideGUI.IsIncurentView: not available outside GUI"
        return False
        
    def getComponentName(self, ComponentUserName ):
        """Get component name from component user name"""
        print "SalomeOutsideGUI.getComponentName: not available outside GUI"
        return ""
   
    def getComponentUserName( self, ComponentName ):
        """Get component user name from component name"""
        print "SalomeOutsideGUI.getComponentUserName: not available outside GUI"
        return ""
        
    #--------------------------------------------------------------------------

    
def salome_iapp_init(embedded):
    global salome_iapp_initial
    global sg,IN_SALOME_GUI

    if salome_iapp_initial:
        salome_iapp_initial=0
        if embedded:
            import libSALOME_Swig
        
            class SalomeGUI(libSALOME_Swig.SALOMEGUI_Swig):
                getAllSelected = SalomeGUIgetAllSelected

            # create a SALOMEGUI_Swig instance
            sg = SalomeGUI()
            inSalomeGUI = sg.hasDesktop()
        else:
            # Not embedded in GUI
            sg=SalomeOutsideGUI()
            inSalomeGUI=0
        if IN_SALOME_GUI is None:
          IN_SALOME_GUI = inSalomeGUI
    return sg

def salome_iapp_close():
    global salome_iapp_initial
    salome_iapp_initial=1
    pass


