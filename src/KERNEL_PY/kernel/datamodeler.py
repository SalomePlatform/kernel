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

## \defgroup datamodeler datamodeler
#  \{ 
#  \details Helper for modeling user data
#  \}

__author__="gboulant"
__date__ ="$15 avr. 2010 19:44:17$"

from uiexception import DevelException

# Most usable class types
TypeString= "".__class__
__ref_integer = 0
TypeInteger = __ref_integer.__class__
__ref_double = 0.0
TypeDouble = __ref_double.__class__
__ref_list = []
TypeList = __ref_list.__class__
__ref_dict = {}
TypeDictionnary = __ref_dict.__class__

# There is no control to do for these attributes. They are attributes for the
# class management and not data of the model.
UNCHECKED_ATTRIBUTES = [
    "_typemap",
    "_rangemap",
    "_defaultmap",
    "_voidmap"
]

## This class is a placeholder for modeling data. An object based on this class
#  (particular instance or specialized derived class) can defined attributes with
#  the following properties:
#  - a type : the class or the type of the attribute. Setting an attribute to
#    a value whose type is not the specified type raises an exception.
#  - a range : a list of the possible values for the attribute. Setting an
#    attribute to a value not in the range raises an exception
#  - a default: the default value of an attribute when an instance is created
#  - a void flag: the attribute can be authorized to be None or not using this
#    flag. Setting an attribute to a None value while the flag is not set to
#    True raises an exception. By default, a None value is not allowed.
#
#  These properties are dictionnaries mapping the attribute name to its
#  associated value for the property.
#  \n A typical usage is to derived this class in a specialized form where the
#  attributes names and there properties are defined in the constructor. See
#  use cases at the end of this file.
#  \ingroup datamodeler
class DataModeler:
    """
    This class is a placeholder for modeling data. An object based on this class
    (particular instance or specialized derived class) can defined attributes with
    the following properties:

    - a type : the class or the type of the attribute. Setting an attribute to
      a value whose type is not the specified type raises an exception.
    - a range : a list of the possible values for the attribute. Setting an
      attribute to a value not in the range raises an exception
    - a default: the default value of an attribute when an instance is created
    - a void flag: the attribute can be authorized to be None or not using this
      flag. Setting an attribute to a None value while the flag is not set to
      True raises an exception. By default, a None value is not allowed.

    These properties are dictionnaries mapping the attribute name to its
    associated value for the property.

    A typical usage is to derived this class in a specialized form where the
    attributes names and there properties are defined in the constructor. See
    use cases at the end of this file.

    """
    def __init__(self, typemap=None, rangemap=None, defaultmap=None, voidmap=None):
        self._typemap = {}
        self._rangemap   = {} # possible values
        self._defaultmap = {} # defaults values
        self._voidmap    = {}    # None values are allowed
        
        if typemap is not None:
            self._typemap.update(typemap)
        if rangemap is not None:
            self._rangemap.update(rangemap)
        if voidmap is not None:
            self._voidmap.update(voidmap)

        # Default initialization (if any)
        if defaultmap is not None:
            self._defaultmap.update(defaultmap)
            for name in self._defaultmap.keys():
                self.__setattr__(name,self._defaultmap[name])

    ## %A None argument means that no entry is created in the associated maps.
    def addAttribute(self, name, type=None, range=None, default=None, void=None):
        """
        A None argument means that no entry is created in the associated maps.
        """
        self._typemap[name] = type

        if range is not None:
            self._rangemap[name] = range

        if void is not None:
            self._voidmap[name] = void

        if (not void) and (default is None):
            return
        
        self.__setattr__(name,default)

    def __setattr__(self, name, val):
        if name in UNCHECKED_ATTRIBUTES:
            self.__dict__[name] = val
            return

        #__GBO_DEBUG_
        if name == "_typemap":
            print "WARNING WARNING WARNING : changing value of _typemap by ",val

        if name not in self._typemap.keys():
            raise DevelException("The class "+str(self.__class__)+" has no attribute "+str(name))

        if val is None:
            if not self.__isVoidAllowed(name):
                raise DevelException("The attribute "+str(name)+" can't be None")
            else:
                # We can stop here and set the value to None
                self.__dict__[name] = None
                return

        if self.__isNotValidType(name,val):
            raise DevelException("The attribute "+str(name)+" must be an instance of "+str(self._typemap[name]))

        if self.__isNotValidRange(name,val):
            raise DevelException("The attribute "+str(name)+" must be a value in :"+str(self._rangemap[name]))

        self.__dict__[name] = val
    
    def __getattribute__(self, name):
        if name in UNCHECKED_ATTRIBUTES:
            return self.__dict__[name]

        if name not in self._typemap.keys():
            raise DevelException("The class "+str(self.__class__)+" has no attribute "+str(name))
        # The attribute coulb be requested while it has not been created yet (for
        # example if we did't call the setter before).
        if not self.__dict__.has_key(name):
            return None
        
        return self.__dict__[name]

    def __isNotValidType(self, name, val):
        isNotValid = (
            ( self._typemap[name] is not None) and
            ( not isinstance(val,self._typemap[name]) ) )

        return isNotValid

    def __isNotValidRange(self, name, val):
        isNotValid = (
            ( self._rangemap is not None) and
            ( self._rangemap.has_key(name) ) and
            ( self._rangemap[name] is not None ) and
            ( val not in self._rangemap[name] ) )

        return isNotValid

    def __isVoidAllowed(self,name):
        isVoidAllowed = (
            ( self._voidmap is not None) and
            ( self._voidmap.has_key(name) ) and
            ( self._voidmap[name] is True ) )
            
        return isVoidAllowed

    def log(self):
        print "DATAMODELER ["+str(self.__class__)+"]: self._typemap.keys() = "+str(self._typemap.keys())




#
# ==============================================================================
# Basic use cases and unit tests
# ==============================================================================
#
def TEST_usecase():
    typemap={}
    typemap["stringdata"] = TypeString
    typemap["integerdata"] = TypeInteger
    typemap["anydata"] = None # can be anything

    data = DataModeler(typemap)

    sdata = "toto"
    idata = 3
    data.stringdata = sdata
    data.integerdata = idata

    data.anydata = 5.3
    data.anydata = "any value"
    data.anydata = True

    print data.integerdata
    return True

def TEST_addAttribute():
    typemap={}
    typemap["stringdata"] = TypeString
    typemap["integerdata"] = TypeInteger
    data = DataModeler(typemap)
    data.stringdata = "a string value"

    ref_value = 1.3
    data.addAttribute(
        name    = "myAttr",
        type    = TypeDouble,
        range   = None,
        default = ref_value,
        void    = False)

    try:
        if data.myAttr != ref_value:
            return False
        data.myAttr = 5.3
        #data.myAttr = 5
    except Exception, e:
        print e
        return False

    try:
        data.myAttr = "bad type value"
        return False
    except Exception, e:
        print e
        return True

def TEST_badAttributeName():
    map={}
    map["stringdata"] = TypeString
    map["integerdata"] = TypeInteger

    data = DataModeler(map)

    # this should raise an exception
    try:
        data.myatt = 3
        return False
    except Exception, e:
        print "OK : "+str(e)
        return True

def TEST_badAttributeType():
    map={}
    map["stringdata"] = TypeString
    map["integerdata"] = TypeInteger

    data = DataModeler(map)
    # this should raise an exception
    try:
        data.stringdata = 2
        return False
    except Exception, e:
        print "OK : "+str(e)
        return True

def TEST_badAttributeRange():
    map={}
    map["stringdata"] = TypeString
    map["integerdata"] = TypeInteger

    range={}
    ref_integervalue = 3
    range["integerdata"] = [1,ref_integervalue,7]

    data = DataModeler(map,range)
    # this should not raise an exception
    try:
        data.integerdata = ref_integervalue
        data.stringdata = "anything (no restriction has been defined)"
    except Exception, e:
        print e
        return False

    # this should raise an exception
    try:
        data.integerdata = 9999 # a value not in the range
        return False
    except Exception, e:
        print e
        return True

def TEST_voidAttributeAllowed():
    map={}
    map["stringdata"] = TypeString
    map["integerdata"] = TypeInteger

    voidmap={}
    voidmap["stringdata"] = True

    data = DataModeler(typemap=map,voidmap=voidmap)
    try:
        # this should not raise an exception
        data.stringdata = None
        print data.stringdata
    except Exception, e:
        print e
        return False
    
    try:
        # this should raise an exception
        data.integerdata = None
        return False
    except Exception, e:
        print e
        return True

def TEST_defaultValues():
    typemap={}
    typemap["stringdata"] = TypeString
    typemap["integerdata"] = TypeInteger

    ref_value = "my initial value"
    defaultmap={}
    defaultmap["stringdata"] = ref_value

    data = DataModeler(typemap=typemap,defaultmap=defaultmap)
    print data.stringdata
    if data.stringdata != ref_value:
        return False
    else:
        return True

if __name__ == "__main__":
    from unittester import run
    run("salome/kernel/datamodeler","TEST_usecase")
    run("salome/kernel/datamodeler","TEST_addAttribute")
    run("salome/kernel/datamodeler","TEST_badAttributeName")
    run("salome/kernel/datamodeler","TEST_badAttributeType")
    run("salome/kernel/datamodeler","TEST_badAttributeRange")
    run("salome/kernel/datamodeler","TEST_voidAttributeAllowed")
    run("salome/kernel/datamodeler","TEST_defaultValues")
