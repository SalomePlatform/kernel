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

import salome
import string
import SALOME
import SALOMEDS
import SALOME_Session_idl

PT_INTEGER = 0
PT_REAL = 1
PT_BOOLEAN = 2
PT_STRING = 3
PT_REALARRAY = 4
PT_INTARRAY = 5
PT_STRARRAY = 6

_AP_LISTS_LIST_ = "AP_LISTS_LIST"
_AP_ENTRIES_LIST_ = "AP_ENTRIES_LIST"
_AP_PROPERTIES_LIST_ = "AP_PROPERTIES_LIST"
_AP_DUMP_PYTHON_ = "AP_DUMP_PYTHON"

vp_session = None

def getSession():
    global vp_session
    if vp_session is None:
        vp_session = salome.naming_service.Resolve("/Kernel/Session")
        vp_session = vp_session._narrow(SALOME.Session)
        pass
    return vp_session 

class IParameters:
    """
    Interface IParameters was designed to provide a common way to set up
    named parameters. The interface is initialized by AttributeParameter that
    is used as a container of stored data.
    The interface supports 3 types of named parameters:
    1. Named list - a named list consists of string values.
       One can append value to list (method 'append'), get a number of values
       in the list (method 'nbValues'), get all values of the given list
       (method 'getValues') and get names of all created named lists.
    2. Parameters - that is a set of named string values associated with some named
       entity. Parameter consists of tree elements: entity name, a parameter name
       and a parameter value. Thus for one named entity there are an arbitrary number
       of pair 'name parameter : value'.
       One can add a new parameter to entry (method 'setParameter'), get a value of
       a given named parameter of the given entry (method 'getParameter'), get a number
       of parameters of the given entry (method 'getNbParameters'), get all names of
       parameters for the given entry (method 'getAllParameterNames'), get all
       values of parameters for the entry (method 'getAllParameterValues') and get all
       stored entries (method 'getEntries')
    3. Property - a property has a name and a string value.
       One can set property (method 'setProperty'), getProperty (method 'getProperty') and
       get a list of all stored properties (method 'getProperties').

    Note:   
          Methods not mentioned above are private and is not supposed to be used
          by module's developers.
       
    """
    def __init__(self, attributeParameter, clr=False):
        """Initializes the instance. If clr parameter is True, all IAPP attribute values are cleared."""
        self._ap = attributeParameter
        if ( clr ): self.clear()
        pass
	
    def clear(self):
        """Clear parameters"""
	self._ap.Clear()

    def append(self, listName, value):
        """Appends a value to the named list"""
        if self._ap is None: return -1
        v = []
        if self._ap.IsSet(listName, PT_STRARRAY) == 0:
            if self._ap.IsSet(_AP_LISTS_LIST_, PT_STRARRAY) == 0: self._ap.SetStrArray(_AP_LISTS_LIST_, v);
            if listName != _AP_ENTRIES_LIST_ and listName != _AP_PROPERTIES_LIST_:
                self.append(_AP_LISTS_LIST_, listName)
                pass
            self._ap.SetStrArray(listName, v)
            pass
        
        v = self._ap.GetStrArray(listName)
        v.append(value)
        self._ap.SetStrArray(listName, v)
        return (len(v)-1)
    
    def nbValues(self, listName):
        """Returns a number of values in the named list"""
        if self._ap is None: return -1
        if self._ap.IsSet(listName, PT_STRARRAY) == 0: return 0
        v = self._ap.GetStrArray(listName)
        return len(v)

    def getValues(self, listName):
        """Returns a list of values in the named list"""
        v = []
        if self._ap is None: return v
        if self._ap.IsSet(listName, PT_STRARRAY) == 0: return v
        return self._ap.GetStrArray(listName)

    def getLists(self):
        """Returns a list of named lists' names"""
        v = []
        if self._ap is None: return v
        if self._ap.IsSet(_AP_LISTS_LIST_, PT_STRARRAY) == 0: return v
        return self._ap.GetStrArray(_AP_LISTS_LIST_)

    def setParameter(self, entry, parameterName, value):
        """Sets a value of the named parameter for the entry"""
        if self._ap is None: return
        v = []
        if self._ap.IsSet(entry, PT_STRARRAY) ==0: 
            self.append(_AP_ENTRIES_LIST_, entry) #Add the entry to the internal list of entries
            self._ap.SetStrArray(entry, v)
            pass
        
        v = self._ap.GetStrArray(entry)
        v.append(parameterName)
        v.append(value)
        self._ap.SetStrArray(entry, v)
        pass

    def getParameter(self, entry, parameterName):
        """Returns a value of the named parameter for the entry"""
        if self._ap is None: return ""
        if self._ap.IsSet(entry, PT_STRARRAY) == 0: return ""
        v = self._ap.GetStrArray(entry)
        length = len(v);
        i = 0
        while i<length:
            if v[i] == parameterName: return v[i+1]
            i+=1
            pass
        
        return ""

    def getAllParameterNames(self, entry):
        """Returns all parameter names of the given entry"""
        v = []
        names = []
        if self._ap is None: return v
        if self._ap.IsSet(entry, PT_STRARRAY) == 0: return v
        v = self._ap.GetStrArray(entry)
        length = len(v)
        i = 0
        while i<length:
            names.append(v[i])
            i+=2
            pass
        
        return names

    def getAllParameterValues(self, entry):
        """Returns all parameter values of the given entry"""
        v = []
        values = []
        if self._ap is None: return v
        if self._ap.IsSet(entry, PT_STRARRAY) == 0: return v
        v = self._ap.GetStrArray(entry)
        length = len(v)
        i = 1
        while i<length:
            values.append(v[i]+1)
            i+=2
            pass
        
        return values

    def getNbParameters(self, entry):
        """Returns a number of parameters of the entry"""
        if self._ap is None: return -1
        if self._ap.IsSet(entry, PT_STRARRAY) == 0: return -1
        return len(self._ap.GetStrArray(entry))/2

    def getEntries(self):
        """Returns all entries"""
        v = []
        if self._ap is None: return v
        if self._ap.IsSet(_AP_ENTRIES_LIST_, PT_STRARRAY) == 0: return v
        return self._ap.GetStrArray(_AP_ENTRIES_LIST_)

    def setProperty(self, name, value):
        """Sets a property value"""
        if self._ap is None: return
        if self._ap.IsSet(name, PT_STRING) == 0: 
            self.append(_AP_PROPERTIES_LIST_, name) #Add the property to the internal list of properties
            pass
        self._ap.SetString(name, value)
        pass

    def getProperty(self, name):
        """Returns a value of the named property"""
        if self._ap is None: return ""
        if self._ap.IsSet(name, PT_STRING) == 0: return ""
        return self._ap.GetString(name)

    def getProperties(self):
        """Returns all propetries"""
        v = []
        if self._ap is None: return v
        if self._ap.IsSet(_AP_PROPERTIES_LIST_, PT_STRARRAY) == 0: return v
        return self._ap.GetStrArray(_AP_PROPERTIES_LIST_)

    def parseValue(self, value, separator, fromEnd):
        """Breaks a value string in two parts which is divided by separator."""
        v = []
        pos = - 1
        if fromEnd == 1: pos = value.rfind(separator)
        else: pos = value.find(separator)

        if pos < 0: 
            v.append(value)
            return v
        
        part1 = value[0:pos]
        part2 = value[pos+1:len(value)]
        v.append(part1)
        v.append(part2)
        return v

    def setDumpPython(self, isDumping):
        """Enables/Disables the dumping to Python"""
        if self._ap is None: return
        _ap.SetBool(_AP_DUMP_PYTHON_, isDumping)
        pass

    def isDumpPython(self):
        """Returns whether there is the dumping to Python"""
        if self._ap is None: return 0
        if self._ap.IsSet(_AP_DUMP_PYTHON_, PT_BOOLEAN) == 0: return 0
        return self._ap.GetBool(_AP_DUMP_PYTHON_)

    pass
