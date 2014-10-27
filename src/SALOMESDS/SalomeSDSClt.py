# -*- coding: utf-8 -*-
# Copyright (C) 2007-2014  CEA/DEN, EDF R&D
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
# Author : Anthony Geay

import SALOME
import cPickle

class WrappedType(object):
    def __init__(self,varPtr,isTemporaryVar=False):
        assert(isinstance(varPtr,SALOME._objref_PickelizedPyObjServer))
        self._var_ptr=varPtr
        if not isTemporaryVar:
            self._var_ptr.Register()
        self._is_temp=isTemporaryVar
        pass

    def local_copy(self):
        return cPickle.loads(self._var_ptr.fetchSerializedContent())

    def __str__(self):
        return self.local_copy().__str__()

    def __repr__(self):
        return self.local_copy().__repr__()

    def __reduce__(self):
        return (self._wrapped_type,(self.local_copy(),))

    def assign(self,elt):
        assert(isinstance(self._var_ptr,SALOME._objref_PickelizedPyObjRdWrServer))
        st=cPickle.dumps(elt,cPickle.HIGHEST_PROTOCOL)
        self._var_ptr.setSerializedContent(st)
        pass

    def __del__(self):
        self._var_ptr.UnRegister()
        pass
    pass

class List(WrappedType):
    def __init__(self,varPtr,isTemporaryVar=False):
        WrappedType.__init__(self,varPtr,isTemporaryVar)
        self._wrapped_type=list
        pass

    def __getitem__(self,*args):
        ret=Caller(self._var_ptr,"__getitem__")
        return ret(*args)

    def __setitem__(self,*args):
        ret=Caller(self._var_ptr,"__setitem__")
        return ret(*args)
    
    def __delitem__(self,*args):
        ret=Caller(self._var_ptr,"__delitem__")
        return ret(*args)

    def append(self,*args):
        ret=Caller(self._var_ptr,"append")
        return ret(*args)

    def extend(self,*args):
        ret=Caller(self._var_ptr,"extend")
        return ret(*args)

    def insert(self,*args):
        ret=Caller(self._var_ptr,"insert")
        return ret(*args)

    def pop(self,*args):
        ret=Caller(self._var_ptr,"pop")
        return ret(*args)

    def remove(self,*args):
        ret=Caller(self._var_ptr,"remove")
        return ret(*args)

    def reverse(self,*args):
        ret=Caller(self._var_ptr,"reverse")
        return ret(*args)

    def sort(self,*args):
        ret=Caller(self._var_ptr,"sort")
        return ret(*args)
    
    def count(self,*args):
        return self.local_copy().count(*args)

    def index(self,*args):
        return self.local_copy().index(*args)

    def __len__(self):
        return len(self.local_copy())
    pass

class Dict(WrappedType):
    def __init__(self,varPtr,isTemporaryVar=False):
        WrappedType.__init__(self,varPtr,isTemporaryVar)
        self._wrapped_type=dict
        pass

    def __getitem__(self,*args):
        ret=Caller(self._var_ptr,"__getitem__")
        return ret(*args)

    def __setitem__(self,*args):
        ret=Caller(self._var_ptr,"__setitem__")
        return ret(*args)

    def __delitem__(self,*args):
        ret=Caller(self._var_ptr,"__delitem__")
        return ret(*args)
    
    def clear(self,*args):
        ret=Caller(self._var_ptr,"clear")
        return ret(*args)

    def get(self,*args):
        ret=Caller(self._var_ptr,"get")
        return ret(*args)

    def items(self,*args):
        ret=Caller(self._var_ptr,"items")
        return ret(*args)

    def pop(self,*args):
        ret=Caller(self._var_ptr,"pop")
        return ret(*args)

    def popitem(self,*args):
        ret=Caller(self._var_ptr,"popitem")
        return ret(*args)

    def setdefault(self,*args):
        ret=Caller(self._var_ptr,"setdefault")
        return ret(*args)

    def update(self,*args):
        ret=Caller(self._var_ptr,"update")
        return ret(*args)

    def values(self,*args):
        ret=Caller(self._var_ptr,"values")
        return ret(*args)
 
    # work on local copy

    def __contains__(self,*args):
        return self.local_copy().__contains__(*args)

    def has_key(self,*args):
        return self.local_copy().has_key(*args)

    def keys(self,*args):
        return self.local_copy().keys(*args)

    def copy(self,*args):
        return self.local_copy().copy(*args)

    def __len__(self):
        return len(self.local_copy())

    pass

class Tuple(WrappedType):
    def __init__(self,varPtr,isTemporaryVar=False):
        WrappedType.__init__(self,varPtr,isTemporaryVar)
        self._wrapped_type=tuple
        pass

    def __getitem__(self,*args):
        ret=Caller(self._var_ptr,"__getitem__")
        return ret(*args)
    
    # work on local copy

    def count(self,*args):
        return self.local_copy().count(*args)

    def index(self,*args):
        return self.local_copy().index(*args)
    
    def __len__(self):
        return len(self.local_copy())

    pass

class Float(WrappedType):
    def __init__(self,varPtr,isTemporaryVar=False):
        WrappedType.__init__(self,varPtr,isTemporaryVar)
        self._wrapped_type=float
        pass

    def __iadd__(self,*args):
        return self.local_copy().__add__(*args)

    def __isub__(self,*args):
        return self.local_copy().__sub__(*args)

    def __imul__(self,*args):
        return self.local_copy().__mul__(*args)

    def __idiv__(self,*args):
        return self.local_copy().__div__(*args)

    def __add__(self,*args):
        return self.local_copy().__add__(*args)

    def __sub__(self,*args):
        return self.local_copy().__sub__(*args)

    def __mul__(self,*args):
        return self.local_copy().__mul__(*args)

    def __div__(self,*args):
        return self.local_copy().__div__(*args)
    
    def __pow__(self,*args):
        return self.local_copy().__pow__(*args)

    def as_integer_ratio(self,*args):
        return self.local_copy().as_integer_ratio(*args)

    def conjugate(self,*args):
        return self.local_copy().conjugate(*args)

    def fromhex(self,*args):
        return self.local_copy().fromhex(*args)

    def hex(self,*args):
        return self.local_copy().hex(*args)

    def imag(self,*args):
        return self.local_copy().imag(*args)

    def is_integer(self,*args):
        return self.local_copy().is_integer(*args)

    def real(self,*args):
        return self.local_copy().real(*args)
    pass

class Int(WrappedType):
    def __init__(self,varPtr,isTemporaryVar=False):
        WrappedType.__init__(self,varPtr,isTemporaryVar)
        self._wrapped_type=int
        pass

    def __iadd__(self,*args):
        return self.local_copy().__add__(*args)

    def __isub__(self,*args):
        return self.local_copy().__sub__(*args)

    def __imul__(self,*args):
        return self.local_copy().__mul__(*args)

    def __imod__(self,*args):
        return self.local_copy().__mod__(*args)
    
    def __idiv__(self,*args):
        return self.local_copy().__div__(*args)

    def __add__(self,*args):
        return self.local_copy().__add__(*args)

    def __sub__(self,*args):
        return self.local_copy().__sub__(*args)

    def __mul__(self,*args):
        return self.local_copy().__mul__(*args)

    def __mod__(self,*args):
        return self.local_copy().__mod__(*args)

    def __div__(self,*args):
        return self.local_copy().__div__(*args)
    
    def __pow__(self,*args):
        return self.local_copy().__pow__(*args)

    def bit_length(self,*args):
        return self.local_copy().bit_length(*args)

    def conjugate(self,*args):
        return self.local_copy().conjugate(*args)

    def denominator(self,*args):
        return self.local_copy().denominator(*args)

    def imag(self,*args):
        return self.local_copy().imag(*args)
    
    def numerator(self,*args):
        return self.local_copy().numerator(*args)

    def real(self,*args):
        return self.local_copy().real(*args)
    pass

class String(WrappedType):
    def __init__(self,varPtr,isTemporaryVar=False):
        WrappedType.__init__(self,varPtr,isTemporaryVar)
        self._wrapped_type=int
        pass

    def __add__(self,*args):
        return self.local_copy().__add__(*args)

    def __iadd__(self,*args):
        return self.local_copy().__add__(*args)

    def __getitem__(self,*args):
        return self.local_copy().__getitem__(*args)

    def capitalize(self,*args):
        return self.local_copy().capitalize(*args)

    def center(self,*args):
        return self.local_copy().center(*args)

    def count(self,*args):
        return self.local_copy().count(*args)

    def decode(self,*args):
        return self.local_copy().decode(*args)

    def encode(self,*args):
        return self.local_copy().encode(*args)

    def endswith(self,*args):
        return self.local_copy().endswith(*args)

    def expandtabs(self,*args):
        return self.local_copy().expandtabs(*args)

    def find(self,*args):
        return self.local_copy().find(*args)

    def format(self,*args):
        return self.local_copy().format(*args)

    def index(self,*args):
        return self.local_copy().index(*args)

    def isalnum(self,*args):
        return self.local_copy().isalnum(*args)

    def isalpha(self,*args):
        return self.local_copy().isalpha(*args)

    def isdigit(self,*args):
        return self.local_copy().isdigit(*args)

    def islower(self,*args):
        return self.local_copy().islower(*args)

    def isspace(self,*args):
        return self.local_copy().isspace(*args)

    def istitle(self,*args):
        return self.local_copy().istitle(*args)

    def isupper(self,*args):
        return self.local_copy().isupper(*args)

    def join(self,*args):
        return self.local_copy().join(*args)

    def ljust(self,*args):
        return self.local_copy().ljust(*args)

    def lower(self,*args):
        return self.local_copy().lower(*args)

    def lstrip(self,*args):
        return self.local_copy().lstrip(*args)

    def partition(self,*args):
        return self.local_copy().partition(*args)

    def replace(self,*args):
        return self.local_copy().replace(*args)

    def rfind(self,*args):
        return self.local_copy().rfind(*args)

    def rindex(self,*args):
        return self.local_copy().rindex(*args)

    def rjust(self,*args):
        return self.local_copy().rjust(*args)

    def rpartition(self,*args):
        return self.local_copy().rpartition(*args)

    def rsplit(self,*args):
        return self.local_copy().rsplit(*args)

    def rstrip(self,*args):
        return self.local_copy().rstrip(*args)

    def split(self,*args):
        return self.local_copy().split(*args)

    def splitlines(self,*args):
        return self.local_copy().splitlines(*args)

    def startswith(self,*args):
        return self.local_copy().startswith(*args)

    def strip(self,*args):
        return self.local_copy().strip(*args)

    def swapcase(self,*args):
        return self.local_copy().swapcase(*args)

    def title(self,*args):
        return self.local_copy().title(*args)

    def translate(self,*args):
        return self.local_copy().translate(*args)

    def upper(self,*args):
        return self.local_copy().upper(*args)

    def zfill(self,*args):
        return self.local_copy().zfill(*args)

    def __len__(self):
        return len(self.local_copy())
    pass

class Caller:
    def __init__(self,varPtr,meth):
        assert(isinstance(varPtr,SALOME._objref_PickelizedPyObjServer))
        self._var_ptr=varPtr
        self._meth=meth
        pass

    def __call__(self,*args):
        ret=self._var_ptr.invokePythonMethodOn(self._meth,cPickle.dumps(args,cPickle.HIGHEST_PROTOCOL))
        return GetHandlerFromRef(ret,True)
    pass

PyHandlerTypeMap={int:Int,float:Float,str:String,list:List,tuple:Tuple,dict:Dict}

def GetHandlerFromRef(objCorba,isTempVar=False):
    """ Returns a client that allows to handle a remote corba ref of a global var easily.
    """
    assert(isinstance(objCorba,SALOME._objref_PickelizedPyObjServer))
    v=cPickle.loads(objCorba.fetchSerializedContent())
    if v is None:
        objCorba.UnRegister()
        return None
    return PyHandlerTypeMap[v.__class__](objCorba,isTempVar)
    
    
def CreateRdOnlyGlobalVar(value,varName,scopeName):
    import salome
    dsm=salome.naming_service.Resolve("/DataServerManager")
    d2s,isCreated=dsm.giveADataScopeCalled(scopeName)
    return GetHandlerFromRef(d2s.createRdOnlyVar(varName,cPickle.dumps(value,cPickle.HIGHEST_PROTOCOL)),False)
    
def CreateRdExtGlobalVar(value,varName,scopeName):
    import salome
    dsm=salome.naming_service.Resolve("/DataServerManager")
    d2s,isCreated=dsm.giveADataScopeCalled(scopeName)
    return GetHandlerFromRef(d2s.createRdExtVar(varName,cPickle.dumps(value,cPickle.HIGHEST_PROTOCOL)),False)

def GetHandlerFromName(scopeName,varName):
    import salome
    dsm=salome.naming_service.Resolve("/DataServerManager")
    d2s=dsm.retriveDataScope(scopeName)
    return GetHandlerFromRef(d2s.retrieveVar(varName),False)
