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

## @package SALOME_DriverPy
#  \brief python implementation of SALOMEDS::Driver interface
#
import SALOMEDS__POA

## Python implementation of generic SALOMEDS::Driver.
#
#
class SALOME_DriverPy_i(SALOMEDS__POA.Driver):
    """
    Should be inherited by any Python module's engine
    to provide persistence mechanism.
    """
    def __init__ (self, componentDataType):
        self._ComponentDataType = componentDataType

    def IORToLocalPersistentID(self, theSObject, IORString, isMultiFile, isASCII):
        return theSObject.GetID()

    def LocalPersistentIDToIOR(self, theSObject, PersistentID, isMultiFile, isASCII):
        return ""

    def ComponentDataType(self):
        return self._ComponentDataType

    def Version(self):
        try:
            return self.getVersion()
        except:
            return ""

    def Save(self, theComponent, theURL, isMultiFile):
        return ""

    def SaveASCII(self, theComponent, theURL, isMultiFile):
        return self.Save(theComponent, theURL, isMultiFile)

    def Load(self, theComponent, theStream, theURL, isMultiFile):
        return 1

    def LoadASCII(self, theComponent, theStream, theURL, isMultiFile):
        return self.Load(theComponent, theStream, theURL, isMultiFile)

    def Close(self, theComponent):
        pass

    def CanPublishInStudy(self, theIOR):
        return 1

    def PublishInStudy(self, theStudy, theSObject, theObject, theName):
        return None

    def CanCopy(self, theObject):
        return 0

