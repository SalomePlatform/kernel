#!/usr/bin/env python
import time
import salome
salome.salome_init()
salome.lcc.shutdownServers()
#give some time to shutdown to complete
time.sleep(1)
salome.LifeCycleCORBA.killOmniNames()
