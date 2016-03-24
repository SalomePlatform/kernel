# -*- coding: iso-8859-1 -*-
# Copyright (C) 2010-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

## \defgroup enumerate enumerate
#  \{ 
#  \details Emulates a C-like enum for python
#  \}

__author__ = "gboulant"
__date__ = "$1 avr. 2010 09:08:02$"

## This class emulates a C-like enum for python. It is initialized with a list
#  of strings to be used as the enum symbolic keys. The enum values are automatically
#  generated as sequencing integer starting at the specified offset value.
#  \ingroup enumerate
class Enumerate(object):
    """
    This class emulates a C-like enum for python. It is initialized with a list
    of strings to be used as the enum symbolic keys. The enum values are automatically
    generated as sequencing integer starting at the specified offset value.
    """
    
    ## Canonical constructor.
    #  \param keys a list of string to be used as the enum symbolic keys. The enum values
    #  are automatically generated as a sequence of integers starting at the specified
    #  offset value.
    def __init__(self, keys, offset=0):
        """
        Canonical constructor
        @keys a list of string to be used as the enum symbolic keys. The enum values
        are automatically generated as a sequence of integers starting at the specified
        offset value.
        """
        self._dict_keynumbers = {}
        for number, key in enumerate(keys):
            value = offset + number
            setattr(self, key, value)
            self._dict_keynumbers[key] = value

    ## Return true if this enumerate contains the specified key string
    #  \param key a key string to test
    def contains(self, key):
        """
        Return true if this enumerate contains the specified key string
        @key a key string to test
        """
        return (key in self._dict_keynumbers.keys())

    ## Returns true if the specified integer value is defined as an identifier
    #  in this enumarate.
    #  \param value a value to test
    def isValid(self, value):
        """
        Returns true if the specified integer value is defined as an identifier
        in this enumarate.
        @value a value to test
        """
        return (value in self._dict_keynumbers.values())

    ## Returns the list of keys in this enumerate.
    def listkeys(self):
        """
        Returns the list of keys in this enumerate.
        """
        list = self._dict_keynumbers.keys()
        list.sort()
        return list

    ## Returns the list of values specified to initiate this enumerate.
    def listvalues(self):
        """
        Returns the list of values specified to initiate this enumerate.
        """
        list = self._dict_keynumbers.values()
        list.sort()
        return list

    ## Returns the symbolic key string associated to the specified identifier value.
    #  \param value an integer value whose associated key string is requested.
    def keyOf(self, value):
        """
        Returns the symbolic key string associated to the specified identifier
        value.
        @param value : an integer value whose associated key string is requested.
        """
        if not self.isValid(value):
            return None
        # _MEM_ We assume here that the keys and associated values are in the
        # same order in their list.
        return self._dict_keynumbers.keys()[self._dict_keynumbers.values().index(value)]

        # If not, weshould use a longer implementation such that:
        #for key in self._dict_keynumbers.keys():
        #    if self._dict_keynumbers[key] == value:
        #        return key

#
# ==============================================================================
# Basic use cases and unit test functions
# ==============================================================================
#

def TEST_simple():
    TYPES_LIST = Enumerate([
        'SEP',
        'OTHER'
    ])
    print TYPES_LIST.listvalues()
    return True

def TEST_createFromList():
    codes = Enumerate([
        'KERNEL', # This should take the value 0
        'GUI', # This should take the value 1
        'GEOM', # ...
        'MED',
        'SMESH'])

    print codes.KERNEL
    print codes.GEOM
    if (codes.KERNEL == 0 and codes.GEOM == 2):
        return True
    else:
        return False

def TEST_createFromString():
    aList = "KERNEL GUI GEOM MED"

    codes = Enumerate(aList.split())

    print codes.KERNEL
    print codes.GEOM
    if (codes.KERNEL == 0 and codes.GEOM == 2):
        return True
    else:
        return False

def TEST_contains():
    codes = Enumerate([
        'KERNEL', # This should take the value 0
        'GUI', # This should take the value 1
        'GEOM', # ...
        'MED',
        'SMESH'])

    print "VISU in enumerate?", codes.contains("VISU")
    if (not codes.contains("VISU")):
        return True
    else:
        return False

def TEST_isValid():
    codes = Enumerate([
        'KERNEL', # This should take the value 0
        'GUI', # This should take the value 1
        'GEOM', # ...
        'MED',
        'SMESH'])

    if (not codes.isValid(23)):
        return True
    else:
        return False

def TEST_offset():
    codes = Enumerate([
        'KERNEL', # This should take the value 0
        'GUI', # This should take the value 1
        'GEOM', # ...
        'MED',
        'SMESH'], offset=20)

    print codes.KERNEL
    print codes.GEOM
    if (codes.KERNEL == 20 and codes.GEOM == 22):
        return True
    else:
        return False

def TEST_listvalues():
    codes = Enumerate([
        'KERNEL', # This should take the value 0
        'GUI', # This should take the value 1
        'GEOM', # ...
        'MED',
        'SMESH'], offset=20)

    print codes.listvalues()
    if codes.listvalues() != [20, 21, 22, 23, 24]:
        return False
    return True

def TEST_keyOf():
    codes = Enumerate([
        'KERNEL', # This should take the value 0
        'GUI', # This should take the value 1
        'GEOM', # ...
        'MED',
        'SMESH'])

    if ( codes.keyOf(codes.KERNEL) != 'KERNEL' or
         codes.keyOf(codes.GUI) != 'GUI' or
         codes.keyOf(codes.GEOM) != 'GEOM' or
         codes.keyOf(codes.MED) != 'MED' or
         codes.keyOf(codes.SMESH) != 'SMESH'):
            return False
    return True

if __name__ == "__main__":
    import unittester
    unittester.run("enumerate", "TEST_simple")
    unittester.run("enumerate", "TEST_createFromList")
    unittester.run("enumerate", "TEST_createFromString")
    unittester.run("enumerate", "TEST_contains")
    unittester.run("enumerate", "TEST_isValid")
    unittester.run("enumerate", "TEST_offset")
    unittester.run("enumerate", "TEST_listvalues")
    unittester.run("enumerate", "TEST_keyOf")
