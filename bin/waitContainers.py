#!/usr/bin/env python
import sys
import time
import orbmodule
import CosNaming
clt = orbmodule.client()
clt.waitNS("/ContainerManager")
obj = clt.orb.resolve_initial_references("NameService")
rootContext = obj._narrow(CosNaming.NamingContext)
cname = []
cname.append(CosNaming.NameComponent('Containers', 'dir'))

while(1):
  try:
    ccontext = rootContext.resolve(cname)
    break
  except CosNaming.NamingContext.NotFound, ex:
    time.sleep(1)
  except CosNaming.NamingContext.InvalidName, ex:
    time.sleep(1)
  except CosNaming.NamingContext.CannotProceed, ex:
    time.sleep(1)
  except (CORBA.TRANSIENT,CORBA.OBJECT_NOT_EXIST,CORBA.COMM_FAILURE):
    time.sleep(1)

def waitContainer(mycont):
  while(1):
    bl,bi=ccontext.list(0)
    if bi is not None:
      ok,b=bi.next_one()
      while(ok):
        for s in b.binding_name :
          if s.kind == "dir":
            obj=ccontext.resolve([s])
            scontext = obj._narrow(CosNaming.NamingContext)
            bll,bii=scontext.list(0)
            if bii is not None:
              ok,bb=bii.next_one()
              while(ok):
                for s in bb.binding_name :
                   if s.id == mycont:
                     print s.id
                     return
                ok,bb=bii.next_one()
        ok,b=bi.next_one()
    sys.stdout.write('+')
    sys.stdout.flush()
    time.sleep(1)

for cont in sys.argv:
  if cont != sys.argv[0]:
    waitContainer(cont)

