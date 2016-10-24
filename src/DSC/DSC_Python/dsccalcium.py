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

## @package dsccalcium
# \brief Module that provides the base class for DSC components implemented in python
#

import calcium
import SALOME_ComponentPy
import SALOME_DriverPy
import Engines

## The SALOME base class for all DSC components implemented in python (interface Engines::Superv_Component).
#
#
class PyDSCComponent(SALOME_ComponentPy.SALOME_ComponentPy_i,
                     SALOME_DriverPy.SALOME_DriverPy_i):
  """
     A Python DSC component is implemented by a Python class that has
     the name of the component and is located in a python module that has the
     name of the component.

     You must derive it and implement init_service and those methods
     that are services of the component.
  """
  def __init__ ( self, orb, poa, contID, containerName, instanceName, interfaceName ):
    SALOME_ComponentPy.SALOME_ComponentPy_i.__init__(self, orb, poa,
                    contID, containerName, instanceName, interfaceName, False)
    SALOME_DriverPy.SALOME_DriverPy_i.__init__(self, interfaceName)
    # create the DSC proxy
    self.proxy=calcium.PySupervCompo(orb,poa,contID,instanceName,interfaceName )
    # Store a reference on naming service in _naming_service attribute 
    self._naming_service = SALOME_ComponentPy.SALOME_NamingServicePy_i( self._orb )

  def init_service(self,service):
    return True

  enums={Engines.DSC.RemovingConnection:calcium.DSC.RemovingConnection,
         Engines.DSC.AddingConnection:calcium.DSC.AddingConnection,
         Engines.DSC.ApplicationError:calcium.DSC.ApplicationError,
        }
  def get_provides_port(self,name,error):
    return self.proxy.get_provides_port(name,error)

  def connect_uses_port(self,name,port):
    self.proxy.connect_uses_port(name,port)

  def connect_provides_port(self,name):
    self.proxy.connect_provides_port(name)

  def disconnect_provides_port(self,name,message):
    self.proxy.disconnect_provides_port(name,message._v)
    #self.proxy.disconnect_provides_port(name,self.enums[message])

  def disconnect_uses_port(self,name,port,message):
    self.proxy.disconnect_uses_port(name,port,message._v)
    #self.proxy.disconnect_uses_port(name,port,self.enums[message])

  def get_port_properties(self,name):
    return self.proxy.get_port_properties(name)

  def setInputFileToService(self,service_name,Salome_file_name):
    return self.proxy.setInputFileToService(service_name,Salome_file_name)

  def checkInputFilesToService(self,service_name):
    return self.proxy.checkInputFilesToService(service_name)

  def getInputFileToService(self,service_name,Salome_file_name):
    return self.proxy.getInputFileToService(service_name,Salome_file_name)

  def setOutputFileToService(self,service_name,Salome_file_name):
    return self.proxy.setOutputFileToService(service_name,Salome_file_name)

  def getOutputFileToService(self,service_name,Salome_file_name):
    return self.proxy.getOutputFileToService(service_name,Salome_file_name)

  def checkOutputFilesToService(self,service_name):
    return self.proxy.checkOutputFilesToService(service_name)

  def beginService(self,service_name):
    SALOME_ComponentPy.SALOME_ComponentPy_i.beginService(self,service_name)
    self.proxy.setTimeOut()

