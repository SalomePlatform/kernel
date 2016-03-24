#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

# dict,list,tuple,int,float,str
import SALOME
import cPickle

class List(object):
    def __init__(self,varPtr,isTemporaryVar=False):
        assert(isinstance(varPtr,SALOME._objref_StringDataServer))
        self._var_ptr=varPtr
        if not isTemporaryVar:
            self._var_ptr.Register()
        self._is_temp=isTemporaryVar
        pass

    def __del__(self):
        self._var_ptr.UnRegister()
        pass

    def assign(self,elt):
        st=cPickle.dumps(elt,cPickle.HIGHEST_PROTOCOL)
        self._var_ptr.setSerializedContent(st)
        pass

    def __getitem__(self,*args):
        ret=Caller(self._var_ptr,"__getitem__")
        return ret(*args)

    def __setitem__(self,*args):
        ret=Caller(self._var_ptr,"__setitem__")
        return ret(*args)

    def append(self,*args):
        ret=Caller(self._var_ptr,"append")
        return ret(*args)

    def __str__(self):
        return self.local_copy().__str__()

    def __repr__(self):
        return self.local_copy().__repr__()

    def local_copy(self):
        return cPickle.loads(self._var_ptr.fetchSerializedContent())

    def __reduce__(self):
        return (list,(self.local_copy(),))

    pass

class Tuple(object):
    def __init__(self,varPtr,isTemporaryVar=False):
        assert(isinstance(varPtr,SALOME._objref_StringDataServer))
        self._var_ptr=varPtr
        if not isTemporaryVar:
            self._var_ptr.Register()
        self._is_temp=isTemporaryVar
        pass

    def __del__(self):
        self._var_ptr.UnRegister()
        pass

    def assign(self,elt):
        st=cPickle.dumps(elt,cPickle.HIGHEST_PROTOCOL)
        self._var_ptr.setSerializedContent(st)
        pass

    def __getitem__(self,*args):
        ret=Caller(self._var_ptr,"__getitem__")
        return ret(*args)

    def __setitem__(self,*args):
        ret=Caller(self._var_ptr,"__setitem__")
        return ret(*args)

    def __str__(self):
        return self.local_copy().__str__()

    def __repr__(self):
        return self.local_copy().__repr__()

    def local_copy(self):
        return cPickle.loads(self._var_ptr.fetchSerializedContent())

    def __reduce__(self):
        return (tuple,(self.local_copy(),))

    pass
        

class Int(object):
    def __init__(self,varPtr,isTemporaryVar=False):
        assert(isinstance(varPtr,SALOME._objref_StringDataServer))
        self._var_ptr=varPtr
        if not isTemporaryVar:
            self._var_ptr.Register()
        self._is_temp=isTemporaryVar
        pass

    def __del__(self):
        self._var_ptr.UnRegister()
        pass

    def __iadd__(self,*args):
        ret=Caller(self._var_ptr,"__add__")
        return ret(*args)

    def __isub__(self,*args):
        ret=Caller(self._var_ptr,"__sub__")
        return ret(*args)

    def assign(self,elt):
        st=cPickle.dumps(elt,cPickle.HIGHEST_PROTOCOL)
        self._var_ptr.setSerializedContent(st)
        pass

    def __str__(self):
        return self.local_copy().__str__()

    def __repr__(self):
        return self.local_copy().__repr__()

    def local_copy(self):
        return cPickle.loads(self._var_ptr.fetchSerializedContent())

    def __reduce__(self):
        return (int,(self.local_copy(),))

    pass

class Dict(object):
    def __init__(self,varPtr,isTemporaryVar=False):
        assert(isinstance(varPtr,SALOME._objref_StringDataServer))
        self._var_ptr=varPtr
        if not isTemporaryVar:
            self._var_ptr.Register()
        self._is_temp=isTemporaryVar
        pass

    def __del__(self):
        self._var_ptr.UnRegister()
        pass

    def assign(self,elt):
        st=cPickle.dumps(elt,cPickle.HIGHEST_PROTOCOL)
        self._var_ptr.setSerializedContent(st)
        pass

    def __getitem__(self,*args):
        ret=Caller(self._var_ptr,"__getitem__")
        return ret(*args)

    def __setitem__(self,*args):
        ret=Caller(self._var_ptr,"__setitem__")
        return ret(*args)

    def __len__(self):
        return len(self.local_copy())

    def __str__(self):
        return self.local_copy().__str__()

    def __repr__(self):
        return self.local_copy().__repr__()

    def local_copy(self):
        return cPickle.loads(self._var_ptr.fetchSerializedContent())

    def __reduce__(self):
        return (dict,(self.local_copy(),))

    pass

class Caller:
    def __init__(self,varPtr,meth):
        assert(isinstance(varPtr,SALOME._objref_StringDataServer))
        self._var_ptr=varPtr
        self._meth=meth
        pass

    def __call__(self,*args):
        ret=self._var_ptr.invokePythonMethodOn(self._meth,cPickle.dumps(args,cPickle.HIGHEST_PROTOCOL))
        return GetHandlerFromRef(ret,True)
    pass

PyHandlerTypeMap={int:Int,list:List,tuple:Tuple,dict:Dict}

def GetHandlerFromRef(objCorba,isTempVar=False):
    v=cPickle.loads(objCorba.fetchSerializedContent())
    if v is None:
        return None
    return PyHandlerTypeMap[v.__class__](objCorba,isTempVar)
