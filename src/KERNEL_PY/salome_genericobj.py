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

"""
 When imported this module adds to CORBA objref from GenericObj type
 automatic management of reference count
 The reference count is incremented when the local proxy is created (assignment of __omni_obj attribute)
 and is decremented when the local proxy is deleted (call of __del__) 

 The GenericObj class for proxy is modified by adding two methods : __del__ and __setattr__
 The module must be imported before any other import of SALOME CORBA module
"""
import omniORB
import SALOME

def mydel(self):
    self.UnRegister()
    omniORB.CORBA.Object.__del__(self)

def mysetattr(self,attr,value):
    self.__dict__[attr]=value
    if attr == "__omni_obj":
      self.Register()

SALOME._objref_GenericObj.__del__=mydel
SALOME._objref_GenericObj.__setattr__=mysetattr

