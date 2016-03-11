# -*- coding: utf-8 -*-
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
# Author : Anthony Geay

import salome
import SALOME
import cPickle
import gc
import SalomeSDSClt

salome.salome_init()
st="jjj"
dsm=salome.naming_service.Resolve("/DataServerManager")
assert(dsm.isAliveAndKicking("Default"))
assert(isinstance(dsm,SALOME._objref_DataServerManager))
assert(isinstance(dsm.getDefaultScope(),SALOME._objref_DataScopeServer))
d2s=dsm.createDataScope("tonyy")
assert(isinstance(d2s,SALOME._objref_DataScopeServer))
a=d2s.createRdWrVar("str","c")
assert(a.getVarName()=="c")
#
a.setSerializedContent(cPickle.dumps(st,cPickle.HIGHEST_PROTOCOL))
assert(cPickle.loads(a.fetchSerializedContent())==st)
assert(cPickle.loads(a.fetchSerializedContent())==st)
assert(cPickle.loads(d2s.retrieveVar("c").fetchSerializedContent())==st)
assert(isinstance(d2s.retrieveVar("c"),SALOME._objref_PickelizedPyObjRdWrServer))
assert(dsm.listScopes()==['Default','tonyy'])
dsm.createDataScope("S2")
assert(dsm.retriveDataScope("S2").getScopeName()=="S2")
assert(dsm.listScopes()==['Default','tonyy','S2'])
dsm.removeDataScope("Default")
assert(dsm.listScopes()==['tonyy','S2'])
dsm.removeDataScope("tonyy")
assert(dsm.listScopes()==['S2'])
a=dsm.retriveDataScope("S2").createRdWrVar("int","a")
#
sname="S7"
dsm=salome.naming_service.Resolve("/DataServerManager")
st=cPickle.dumps([],cPickle.HIGHEST_PROTOCOL)
a=dsm.giveADataScopeCalled(sname)[0].createRdWrVar("list","a")
dsm.giveADataScopeCalled(sname)
a.setSerializedContent(cPickle.dumps([0,],cPickle.HIGHEST_PROTOCOL))
assert(cPickle.loads(a.fetchSerializedContent())==[0,])
a.setSerializedContent(st)
assert(cPickle.loads(a.fetchSerializedContent())==[])
tmp=a.invokePythonMethodOn("append",cPickle.dumps((0,),cPickle.HIGHEST_PROTOCOL))
assert(cPickle.loads(a.fetchSerializedContent())==[0])
for i in xrange(0,1000):
    tmp=a.invokePythonMethodOn("append",cPickle.dumps((i,),cPickle.HIGHEST_PROTOCOL))
    pass
dsm.removeDataScope(sname)
#
sname="S4"
d2s,_=dsm.giveADataScopeCalled(sname)
d2s.createRdWrVar("list","a")
a=SalomeSDSClt.GetHandlerFromRef(dsm.retriveDataScope(sname).retrieveVar("a"))
a.append(1)
for i in xrange(1000):
    a.append(i)
    pass
assert(sum(a.local_copy())==499501)
assert(dsm.isAliveAndKicking(sname))
#
dsm=salome.naming_service.Resolve("/DataServerManager")
sname="S11"
d2s,_=dsm.giveADataScopeCalled(sname)
d2s.createRdWrVar("dict","a")
a=SalomeSDSClt.GetHandlerFromName(sname,"a")
assert(isinstance(a,SalomeSDSClt.Dict))
a.assign({})
a["ab"]=45 ; gc.collect(0)
a["cd"]=[4,5] ; gc.collect(0)
a["cd"].append(77) ; gc.collect(0)
a["cd"].append([4,7,8]) ; gc.collect(0)
a["cd"][3].append(3) ; gc.collect(0)
a["ab"]+=2
assert(a.local_copy()=={'ab':47,'cd':[4,5,77,[4,7,8,3]]})
a["ab"]-=1
assert(a.local_copy()=={'ab':46,'cd':[4,5,77,[4,7,8,3]]})
assert(len(a)==2)
c=a["cd"] ; gc.collect(0)
assert(isinstance(c,SalomeSDSClt.List))
a["cd"]=26 ; gc.collect(0)
assert(a.local_copy()=={'ab':46,'cd':26})
a["cd"]=c
assert(a.local_copy()=={'ab':46,'cd':[4,5,77,[4,7,8,3]]})
