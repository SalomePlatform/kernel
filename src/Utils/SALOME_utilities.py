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

#  SALOME Utils : general SALOME's definitions and tools
#  File   : SALOME_utilities.py
#  Module : SALOME
#
import SALOME_Trace
GLogger = SALOME_Trace.SALOME_Trace()

from launchConfigureParser import verbose

def MYTRACE ():
    if verbose():
        str = "- Trace "
        GLogger.putMessage(str + "  : ")
        

def REPERE():
    if verbose():
        GLogger.putMessage("   --------------  \n")


def BEGIN_OF(msg):
    if verbose():
        REPERE(); MYTRACE();
        GLogger.putMessage("Begin of : "+ str(msg) + "\n")
        REPERE();


def END_OF(msg):
    if verbose():
        REPERE(); MYTRACE();
        GLogger.putMessage("Normale end of : "+ str(msg) + "\n")
        REPERE();

def MESSAGE(msg):
    if verbose():
        MYTRACE()
        GLogger.putMessage(str(msg) + "\n")

def SCRUTE(var_name, var_value):
    MYTRACE();
    GLogger.putMessage(var_name + " = " + str(var_value) + "\n")

   
