#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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

def __prompt(vars = None, commands=[], message = "Connecting to SALOME"):
  if vars is None:
    vars = globals()
  import code
  import rlcompleter
  import readline
  readline.parse_and_bind("tab: complete")
  # calling this with globals ensures we can see the environment
  print message
  shell = code.InteractiveConsole(vars)
  for cmd in commands:
    shell.push(cmd)
  return shell.interact
#

def connect(args=[]):
  p = __prompt(commands=["import salomeConsole"])
  p()
#
