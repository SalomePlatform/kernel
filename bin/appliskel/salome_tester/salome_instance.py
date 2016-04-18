# Copyright (C) 2015-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

import sys
import os

# Example of args:
#      args=["--gui", "--show-desktop=1", "--splash=0"]
#      args=["--terminal","--modules=MED,PARAVIS,GUI"]
class SalomeInstance(object):

  def __init__(self):
    self.port = None
  #

  def get_port(self):
    return self.port
  #

  @staticmethod
  def start(shutdown_servers=False, with_gui=False, args=[]):
    import tempfile
    log = tempfile.NamedTemporaryFile(suffix='_nsport.log', delete=False)
    log.close()

    instance_args = [
      "--ns-port-log=%s"%log.name,
      "--shutdown-servers=%d"%shutdown_servers
      ] + args

    salome_instance = SalomeInstance()
    salome_instance.__run(args=instance_args, with_gui=with_gui)

    with open(log.name) as f:
      salome_instance.port = int(f.readline())

    os.remove(log.name)
    return salome_instance
  #

  def __run(self, args=None, with_gui=False):
    if args is None:
      args = []

    sys.argv = ['runSalome'] + args

    if with_gui:
      # :WARNING: NOT TESTED YET
      sys.argv += ["--gui"]
      sys.argv += ["--show-desktop=1"]
      sys.argv += ["--splash=0"]
      #sys.argv += ["--standalone=study"]
      #sys.argv += ["--embedded=SalomeAppEngine,cppContainer,registry,moduleCatalog"]
    else:
      sys.argv += ["--terminal"]
      #sys.argv += ["--shutdown-servers=1"]
      #sys.argv += ["--modules=MED,PARAVIS,GUI"]
      pass

    import setenv
    setenv.main(True)
    import runSalome
    runSalome.runSalome()

    if not with_gui:
      import salome
      salome.salome_init()
      session_server = salome.naming_service.Resolve('/Kernel/Session')
      if session_server:
        session_server.emitMessage("connect_to_study")
        session_server.emitMessage("activate_viewer/ParaView")
        pass
  #

  def stop(self):
    from multiprocessing import Process
    from killSalomeWithPort import killMyPort
    import tempfile
    with tempfile.NamedTemporaryFile():
      p = Process(target = killMyPort, args=(self.port,))
      p.start()
      p.join()
    pass
  #

#

if __name__ == "__main__":
  print "##### Start instance..."
  salome_instance = SalomeInstance.start()
  port = salome_instance.get_port()
  print "#####    ...instance started on port %s"%port

  print "##### Terminate instance running on port %s"%port
  salome_instance.stop()
#
