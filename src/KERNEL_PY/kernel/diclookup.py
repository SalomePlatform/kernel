# -*- coding: iso-8859-1 -*-
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

## \defgroup diclookup diclookup
#  \{ 
#  \details Smart dictionnary with key/value lookup
#  \}

__author__="gboulant"
__date__ ="$21 mai 2010 18:00:23$"


# search a dictionary for key or value
# using named functions or a class
# tested with Python25   by Ene Uran    01/19/2008

## return the key of dictionary dic given the value
#  \ingroup diclookup
def find_key(dic, val):
    """return the key of dictionary dic given the value"""
    return [k for k, v in dic.iteritems() if v == val][0]

## return the value of dictionary dic given the key
#  \ingroup diclookup
def find_value(dic, key):
    """return the value of dictionary dic given the key"""
    return dic[key]

## a dictionary which can lookup value by key, or keys by value
#  \ingroup diclookup
class Lookup(dict):
    """
    a dictionary which can lookup value by key, or keys by value
    """
    ## items can be a list of pair_lists or a dictionary
    def __init__(self, items=None):
        """items can be a list of pair_lists or a dictionary"""
	if items is None:
	    items = []
        dict.__init__(self, items)

    ## find the key(s) as a list given a value
    def get_keys(self, value):
        """find the key(s) as a list given a value"""
        return [item[0] for item in self.items() if item[1] == value]

    ## find the key associated to the given a value. If several keys exist,
    #  only the first is given. To get the whole list, use get_keys instead.
    def get_key(self, value):
        """
        find the key associated to the given a value. If several keys exist,
        only the first is given. To get the whole list, use get_keys instead.
        """
        list = self.get_keys(value)
        if len(list) == 0:
            return None
        return list[0]

    ## find the value given a key
    def get_value(self, key):
        """find the value given a key"""
        return self[key]

#
# ==============================================================================
# Use cases and unit tests
# ==============================================================================
#
def TEST_getTestDictionnary():
    # dictionary of chemical symbols
    symbol_dic = {
    'C': 'carbon',
    'H': 'hydrogen',
    'N': 'nitrogen',
    'Li': 'lithium',
    'Be': 'beryllium',
    'B': 'boron'
    }
    return symbol_dic

def TEST_find_value():
    symbol_dic = TEST_getTestDictionnary()
    print find_key(symbol_dic, 'boron')  # B
    print find_value(symbol_dic, 'B')    # boron
    print find_value(symbol_dic, 'H')    # hydrogen
    if find_key(symbol_dic, 'nitrogen') != 'N':
        return False
    return True

def TEST_lookup():
    symbol_dic = TEST_getTestDictionnary()

    name = 'lithium'
    symbol = 'Li'
    # use a dictionary as initialization argument
    look = Lookup(symbol_dic)
    print look.get_key(name)      # [Li']
    if look.get_key(name) != symbol:
        print "get "+str(look.get_key(name))+" while "+str(symbol)+" was expected"
        return False
    print look.get_value(symbol)  # lithium

    # use a list of pairs instead of a dictionary as initialization argument
    # (will be converted to a dictionary by the class internally)
    age_list = [['Fred', 23], ['Larry', 28], ['Ene', 23]]
    look2 = Lookup(age_list)
    print look2.get_keys(23)        # ['Ene', 'Fred']
    if look2.get_keys(23)[0] != 'Ene' or look2.get_keys(23)[1] != 'Fred':
        print "get "+str(look2.get_keys(23))+" while ['Ene', 'Fred'] was expected"
        return False
    print look2.get_value('Fred')  # 23
    return True

if __name__ == '__main__':
    import unittester
    unittester.run("diclookup", "TEST_find_value")
    unittester.run("diclookup", "TEST_lookup")
