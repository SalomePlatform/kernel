#!/usr/bin/env python
import orbmodule
import Engines
import Registry
import SALOME
import SALOMEDS
import SALOME_ModuleCatalog
clt=orbmodule.client()
obj = clt.Resolve('Kernel/Session')
if obj != None:
  ses = obj._narrow(SALOME.Session)
  ses.StopSession()
obj = clt.Resolve('SalomeLauncher')
if obj != None:
  cm = obj._narrow(Engines.SalomeLauncher)
  cm.Shutdown()
obj = clt.Resolve('Kernel/ModulCatalog')
if obj != None:
  mc = obj._narrow(SALOME_ModuleCatalog.ModuleCatalog)
  mc.shutdown()
obj = clt.Resolve('Registry')
if obj != None:
  reg = obj._narrow(Registry.Components)
  reg.Shutdown()
obj = clt.Resolve('myStudyManager')
if obj != None:
  sm = obj._narrow(SALOMEDS.StudyManager)
  sm.Shutdown()
