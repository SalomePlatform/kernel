# -*- coding: utf-8 -*-
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
# Author : Anthony Geay (EDF R&D)


import abc

def _raiseNow(strCont):
    raise Exception("The method %s has been called whereas it is an interface !"%strCont)

class WrappedType(object):
    """ Here definition of an interface in python."""
    __metaclass__=abc.ABCMeta
    @abc.abstractmethod
    def ptr(self):
        _raiseNow("ptr")
    @abc.abstractmethod
    def local_copy(self):
        _raiseNow("local_copy")
    @abc.abstractmethod
    def assign(self,elt):
        _raiseNow("assign")

class List(WrappedType):
    @abc.abstractmethod
    def __getitem__(self,*args):
        _raiseNow("__getitem__")
    @abc.abstractmethod
    def __setitem__(self,*args):
        _raiseNow("__setitem__")
    @abc.abstractmethod
    def __delitem__(self,*args):
        _raiseNow("__delitem__")
    @abc.abstractmethod
    def append(self,*args):
        _raiseNow("append")
    @abc.abstractmethod
    def extend(self,*args):
        _raiseNow("extend")
    @abc.abstractmethod
    def insert(self,*args):
        _raiseNow("insert")
    @abc.abstractmethod
    def pop(self,*args):
        _raiseNow("pop")
    @abc.abstractmethod
    def remove(self,*args):
        _raiseNow("remove")
    @abc.abstractmethod
    def reverse(self,*args):
        _raiseNow("reverse")
    @abc.abstractmethod
    def sort(self,*args):
        _raiseNow("sort")
    # work on local copy
    @abc.abstractmethod
    def count(self,*args):
        _raiseNow("count")
    @abc.abstractmethod
    def index(self,*args):
        _raiseNow("index")
    @abc.abstractmethod
    def __len__(self):
        _raiseNow("__len__")
    pass

class Dict(WrappedType):
    @abc.abstractmethod
    def __getitem__(self,*args):
        _raiseNow("__getitem__")
    @abc.abstractmethod
    def __setitem__(self,*args):
        _raiseNow("__setitem__")
    @abc.abstractmethod
    def __delitem__(self,*args):
        _raiseNow("__delitem__")
    @abc.abstractmethod
    def clear(self,*args):
        _raiseNow("clear")
    @abc.abstractmethod
    def get(self,*args):
        _raiseNow("get")
    @abc.abstractmethod
    def items(self,*args):
        _raiseNow("items")
    @abc.abstractmethod
    def pop(self,*args):
        _raiseNow("pop")
    @abc.abstractmethod
    def popitem(self,*args):
        _raiseNow("popitem")
    @abc.abstractmethod
    def setdefault(self,*args):
        _raiseNow("setdefault")
    @abc.abstractmethod
    def update(self,*args):
        _raiseNow("update")
    @abc.abstractmethod
    def values(self,*args):
        _raiseNow("values")
    # work on local copy
    @abc.abstractmethod
    def __contains__(self,*args):
        _raiseNow("__contains__")
    @abc.abstractmethod
    def has_key(self,*args):
        _raiseNow("has_key")
    @abc.abstractmethod
    def keys(self,*args):
        _raiseNow("keys")
    @abc.abstractmethod
    def copy(self,*args):
        _raiseNow("copy")
    @abc.abstractmethod
    def __len__(self):
        _raiseNow("__len__")
    pass

class Tuple(WrappedType):
    @abc.abstractmethod
    def __getitem__(self,*args):
        _raiseNow("__getitem__")
    # work on local copy
    @abc.abstractmethod
    def count(self,*args):
        _raiseNow("count")
    @abc.abstractmethod
    def index(self,*args):
        _raiseNow("index")
    @abc.abstractmethod
    def __len__(self):
        _raiseNow("__len__")
    pass

class Float(WrappedType):
    @abc.abstractmethod
    def __iadd__(self,*args):
        _raiseNow("__iadd__")
    @abc.abstractmethod
    def __isub__(self,*args):
        _raiseNow("__isub__")
    @abc.abstractmethod
    def __imul__(self,*args):
        _raiseNow("__imul__")
    @abc.abstractmethod
    def __idiv__(self,*args):
        _raiseNow("__idiv__")
    @abc.abstractmethod
    def __add__(self,*args):
        _raiseNow("__add__")
    @abc.abstractmethod
    def __sub__(self,*args):
        _raiseNow("__sub__")
    @abc.abstractmethod
    def __mul__(self,*args):
        _raiseNow("__mul__")
    @abc.abstractmethod
    def __div__(self,*args):
        _raiseNow("__div__")
    @abc.abstractmethod
    def __pow__(self,*args):
        _raiseNow("__pow__")
    @abc.abstractmethod
    def as_integer_ratio(self,*args):
        _raiseNow("as_integer_ratio")
    @abc.abstractmethod
    def conjugate(self,*args):
        _raiseNow("conjugate")
    @abc.abstractmethod
    def fromhex(self,*args):
        _raiseNow("fromhex")
    @abc.abstractmethod
    def hex(self,*args):
        _raiseNow("hex")
    @abc.abstractmethod
    def imag(self,*args):
        _raiseNow("imag")
    @abc.abstractmethod
    def is_integer(self,*args):
        _raiseNow("is_integer")
    @abc.abstractmethod
    def real(self,*args):
        _raiseNow("real")
    pass

class Int(WrappedType):
    @abc.abstractmethod
    def __iadd__(self,*args):
        _raiseNow("__iadd__")
    @abc.abstractmethod
    def __isub__(self,*args):
        _raiseNow("__isub__")
    @abc.abstractmethod
    def __imul__(self,*args):
        _raiseNow("__imul__")
    @abc.abstractmethod
    def __imod__(self,*args):
        _raiseNow("__imod__")
    @abc.abstractmethod
    def __idiv__(self,*args):
        _raiseNow("__idiv__")
    @abc.abstractmethod
    def __add__(self,*args):
        _raiseNow("__add__")
    @abc.abstractmethod
    def __sub__(self,*args):
        _raiseNow("__sub__")
    @abc.abstractmethod
    def __mul__(self,*args):
        _raiseNow("__mul__")
    @abc.abstractmethod
    def __mod__(self,*args):
        _raiseNow("__mod__")
    @abc.abstractmethod
    def __div__(self,*args):
        _raiseNow("__div__")
    @abc.abstractmethod
    def __pow__(self,*args):
        _raiseNow("__pow__")
    @abc.abstractmethod
    def bit_length(self,*args):
        _raiseNow("bit_length")
    @abc.abstractmethod
    def conjugate(self,*args):
        _raiseNow("conjugate")
    @abc.abstractmethod
    def denominator(self,*args):
        _raiseNow("denominator")
    @abc.abstractmethod
    def imag(self,*args):
        _raiseNow("imag")
    @abc.abstractmethod
    def numerator(self,*args):
        _raiseNow("numerator")
    @abc.abstractmethod
    def real(self,*args):
        _raiseNow("real")
    pass

class String(WrappedType):
    @abc.abstractmethod
    def __add__(self,*args):
        _raiseNow("__add__")
    @abc.abstractmethod
    def __iadd__(self,*args):
        _raiseNow("__iadd__")
    @abc.abstractmethod
    def __getitem__(self,*args):
        _raiseNow("__getitem__")
    @abc.abstractmethod
    def capitalize(self,*args):
        _raiseNow("capitalize")
    @abc.abstractmethod
    def center(self,*args):
        _raiseNow("center")
    @abc.abstractmethod
    def count(self,*args):
        _raiseNow("count")
    @abc.abstractmethod
    def decode(self,*args):
        _raiseNow("decode")
    @abc.abstractmethod
    def encode(self,*args):
        _raiseNow("encode")
    @abc.abstractmethod
    def endswith(self,*args):
        _raiseNow("endswith")
    @abc.abstractmethod
    def expandtabs(self,*args):
        _raiseNow("expandtabs")
    @abc.abstractmethod
    def find(self,*args):
        _raiseNow("find")
    @abc.abstractmethod
    def format(self,*args):
        _raiseNow("format")
    @abc.abstractmethod
    def index(self,*args):
        _raiseNow("index")
    @abc.abstractmethod
    def isalnum(self,*args):
        _raiseNow("isalnum")
    @abc.abstractmethod
    def isalpha(self,*args):
        _raiseNow("isalpha")
    @abc.abstractmethod
    def isdigit(self,*args):
        _raiseNow("isdigit")
    @abc.abstractmethod
    def islower(self,*args):
        _raiseNow("islower")
    @abc.abstractmethod
    def isspace(self,*args):
        _raiseNow("isspace")
    @abc.abstractmethod
    def istitle(self,*args):
        _raiseNow("istitle")
    @abc.abstractmethod
    def isupper(self,*args):
        _raiseNow("isupper")
    @abc.abstractmethod
    def join(self,*args):
        _raiseNow("join")
    @abc.abstractmethod
    def ljust(self,*args):
        _raiseNow("ljust")
    @abc.abstractmethod
    def lower(self,*args):
        _raiseNow("lower")
    @abc.abstractmethod
    def lstrip(self,*args):
        _raiseNow("lstrip")
    @abc.abstractmethod
    def partition(self,*args):
        _raiseNow("partition")
    @abc.abstractmethod
    def replace(self,*args):
        _raiseNow("replace")
    @abc.abstractmethod
    def rfind(self,*args):
        _raiseNow("rfind")
    @abc.abstractmethod
    def rindex(self,*args):
        _raiseNow("rindex")
    @abc.abstractmethod
    def rjust(self,*args):
        _raiseNow("rjust")
    @abc.abstractmethod
    def rpartition(self,*args):
        _raiseNow("rpartition")
    @abc.abstractmethod
    def rsplit(self,*args):
        _raiseNow("rsplit")
    @abc.abstractmethod
    def rstrip(self,*args):
        _raiseNow("rstrip")
    @abc.abstractmethod
    def split(self,*args):
        _raiseNow("split")
    @abc.abstractmethod
    def splitlines(self,*args):
        _raiseNow("splitlines")
    @abc.abstractmethod
    def startswith(self,*args):
        _raiseNow("startswith")
    @abc.abstractmethod
    def strip(self,*args):
        _raiseNow("strip")
    @abc.abstractmethod
    def swapcase(self,*args):
        _raiseNow("swapcase")
    @abc.abstractmethod
    def title(self,*args):
        _raiseNow("title")
    @abc.abstractmethod
    def translate(self,*args):
        _raiseNow("translate")
    @abc.abstractmethod
    def upper(self,*args):
        _raiseNow("upper")
    @abc.abstractmethod
    def zfill(self,*args):
        _raiseNow("zfill")
    @abc.abstractmethod
    def __len__(self):
        _raiseNow("__len__")
    pass
