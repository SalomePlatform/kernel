import calcium
import SALOME_ComponentPy
import SALOME_DriverPy
import Engines

class PyDSCComponent(SALOME_ComponentPy.SALOME_ComponentPy_i,
                     SALOME_DriverPy.SALOME_DriverPy_i):
  """
     A Python SALOME component is implemented by a Python class that has
     the name of the component and is located in a python module that has the
     name of the component.

     This class is a base class for Python DSC components.

     You must derive it and implement init_service and those methods
     that are services of the component.
  """
  def __init__ ( self, orb, poa, contID, containerName, instanceName, interfaceName ):
    SALOME_ComponentPy.SALOME_ComponentPy_i.__init__(self, orb, poa,
                    contID, containerName, instanceName, interfaceName, 0)
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

