#!/usr/bin/env python
import salome
salome.salome_init()
salome.lcc.shutdownServers()
salome.SALOME_LifeCycleCORBA_killOmniNames()
