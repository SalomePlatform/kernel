#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org
#
#
#
#  File   : test_remove_ref.py
#  Module : SALOMEDS     

from salome import *

myBuilder = myStudy.NewBuilder()


obj1 = myStudy.FindObjectID("0:1")
if obj1 is None: print "Is null obj1 "
else: print obj1.GetID()

obj2 = myBuilder.NewObject(obj1)
print "Obj2 ID = "+obj2.GetID()

myBuilder.Addreference(obj1, obj2)

(f, obj3) = obj1.ReferencedObject()

print "Ref obj ID = "+obj3.GetID()


myBuilder.RemoveReference(obj1)

(f, obj4) = obj1.ReferencedObject()

print "Ref is found ", f
