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

"""
  This patch of omniORB is made to make it work with multiple interpreters
  and to correct the problem of incomplete import of CORBA packages
  in some situations common in SALOME

  This patch add or modify functions in omniORB module.

  In multiple interpreters context, omniORB module is meant to be shared among
  all interpreters
"""
import sys,string,imp
import omniORB
# Map of partially-opened modules
_partialModules = {}
# Map of modules to share
shared_imported={}

# Function to return a Python module for the required IDL module name
def openModule(mname, fname=None):
    # Salome modification start
    # Be sure to use the right module dictionnary
    import sys
    # Salome modification end

    if mname == "CORBA":
        mod = sys.modules["omniORB.CORBA"]
        # Salome modification start
        shared_imported[mname]=mod
        # Salome modification end

    elif sys.modules.has_key(mname):
        mod = sys.modules[mname]

        if _partialModules.has_key(mname):
            pmod = _partialModules[mname]
            mod.__dict__.update(pmod.__dict__)
            del _partialModules[mname]
        # Salome modification start
        shared_imported[mname]=mod
        # Salome modification end

    elif _partialModules.has_key(mname):
        mod = _partialModules[mname]

    # Salome modification start
    elif shared_imported.get(mname) :
        mod = shared_imported[mname]
    # Salome modification end

    else:
        mod = newModule(mname)


    if not hasattr(mod, "__doc__") or mod.__doc__ is None:
        mod.__doc__ = "omniORB IDL module " + mname + "\n\n" + \
                      "Generated from:\n\n"

    if fname is not None:
        mod.__doc__ = mod.__doc__ + "  " + fname + "\n"

    return mod

# Function to create a new module, and any parent modules which do not
# already exist
def newModule(mname):
    # Salome modification start
    # Be sure to use the right module dictionnary
    import sys
    # Salome modification end

    mlist   = string.split(mname, ".")
    current = ""
    mod     = None

    for name in mlist:
        current = current + name

        if sys.modules.has_key(current):
            mod = sys.modules[current]

        elif _partialModules.has_key(current):
            mod = _partialModules[current]

        else:
            newmod = imp.new_module(current)
            if mod: setattr(mod, name, newmod)
            _partialModules[current] = mod = newmod

        current = current + "."

    return mod

# Function to update a module with the partial module store in the
# partial module map
def updateModule(mname):
    # Salome modification start
    # Be sure to use the right module dictionnary
    import sys
    # Salome modification end
    if _partialModules.has_key(mname):
        pmod = _partialModules[mname]
        mod  = sys.modules[mname]
        mod.__dict__.update(pmod.__dict__)
        del _partialModules[mname]

    # Salome modification start
    shared_imported[mname]=sys.modules[mname]
    # Salome modification end

omniORB.updateModule=updateModule
omniORB.newModule=newModule
omniORB.openModule=openModule

