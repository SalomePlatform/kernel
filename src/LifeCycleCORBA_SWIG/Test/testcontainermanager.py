#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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

import os
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
    self.container_name = "MyContainer-%s"%(os.getpid())
    pass

  def test0(self):
    """"""
    rp=LifeCycleCORBA.ResourceParameters(policy="best",componentList=["PYHELLO"])
    p=LifeCycleCORBA.ContainerParameters(container_name=self.container_name,mode="start",resource_params=rp)
    co=cm.GiveContainer( p )
    host1=co.getHostName()
    name1="/Containers/%s/%s" % (host1,self.container_name)
    self.assertEqual(co._get_name(), name1)
    co=cm.GiveContainer( p )
    host2=co.getHostName()
    name2="/Containers/%s/%s" % (host2,self.container_name)
    self.assertEqual(co._get_name(), name2)

  def test1(self):
    """"""
    rp=LifeCycleCORBA.ResourceParameters(policy="best",componentList=["PYHELLO"])
    p=LifeCycleCORBA.ContainerParameters(container_name=self.container_name,mode="get",resource_params=rp)
    co=cm.GiveContainer( p )
    host1=co.getHostName()
    name1="/Containers/%s/%s" % (host1,self.container_name)
    self.assertEqual(co._get_name(), name1)
    co=cm.GiveContainer( p )
    host2=co.getHostName()
    name2="/Containers/%s/%s" % (host2,self.container_name)
    self.assertEqual(co._get_name(), name2)

  def test2(self):
    """"""
    rp=LifeCycleCORBA.ResourceParameters(policy="best",componentList=["PYHELLO"])
    p=LifeCycleCORBA.ContainerParameters(container_name=self.container_name,mode="getorstart",resource_params=rp)
    co=cm.GiveContainer( p )
    host1=co.getHostName()
    name1="/Containers/%s/%s" % (host1,self.container_name)
    self.assertEqual(co._get_name(), name1)
    co=cm.GiveContainer( p )
    host2=co.getHostName()
    name2="/Containers/%s/%s" % (host2,self.container_name)
    self.assertEqual(co._get_name(), name2)


if __name__ == '__main__':
  #suite = unittest.TestLoader().loadTestsFromTestCase(TestContainerManager)
  #unittest.TextTestRunner().run(suite)
  unittest.main()
