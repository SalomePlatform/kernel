#  -*- coding: iso-8859-1 -*-
#  Copyright (C) 2007-2010  CEA/DEN, EDF R&D, OPEN CASCADE
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
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

import unittest
import salome
import Engines
import LifeCycleCORBA
salome.salome_init()
cm= salome.lcc.getContainerManager()
rm= salome.lcc.getResourcesManager()

class TestContainerManager(unittest.TestCase):
  """
Test with catalog :
<!DOCTYPE ResourcesCatalog>
<resources>
   <machine hostname="claui2c6" >
   </machine>
   <machine hostname="clt10br" >
   </machine>
</resources>
"""
  def setUp(self):
    pass

  def test0(self):
    """"""
    p=LifeCycleCORBA.MachineParameters(container_name="MyContainer",mode="start",
                                       policy="best",componentList=["PYHELLO"])
    co=cm.StartContainer( p )
    print "Container:",co,co.getHostName(), co.getPID(),co._get_name()
    self.assertEqual(co._get_name(), "/Containers/claui2c6/MyContainer")
    co=cm.StartContainer( p )
    self.assertEqual(co._get_name(), "/Containers/clt10br/MyContainer")

  def test1(self):
    """"""
    p=LifeCycleCORBA.MachineParameters(container_name="MyContainer",mode="get",
                                       policy="best",componentList=["PYHELLO"])
    co=cm.StartContainer( p )
    print "Container:",co,co.getHostName(), co.getPID(),co._get_name()
    self.assertEqual(co._get_name(), "/Containers/claui2c6/MyContainer")
    co=cm.StartContainer( p )
    self.assertEqual(co._get_name(), "/Containers/clt10br/MyContainer")

  def test2(self):
    """"""
    p=LifeCycleCORBA.MachineParameters(container_name="MyContainer",mode="getorstart",
                                       policy="best",componentList=["PYHELLO"])
    co=cm.StartContainer( p )
    print "Container:",co,co.getHostName(), co.getPID(),co._get_name()
    self.assertEqual(co._get_name(), "/Containers/claui2c6/MyContainer")
    co=cm.StartContainer( p )
    self.assertEqual(co._get_name(), "/Containers/clt10br/MyContainer")


if __name__ == '__main__':
  unittest.main()

