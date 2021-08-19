# Copyright (C) 2015-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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
import SALOME
import pickle
import salome
import sys

scopeName="Scope1"
varName="a"

def obj2Str(obj):
    return pickle.dumps(obj,pickle.HIGHEST_PROTOCOL)

def str2Obj(strr):
    return pickle.loads(strr)

def waitKey(IORNS):
    import Engines
    import CORBA
    orb = CORBA.ORB_init([''])
    ns = orb.string_to_object(IORNS)
    import SALOME
    dsm = orb.string_to_object(ns.Resolve("/DataServerManager").decode())
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    assert(not isCreated)
    wk=dss.waitForKeyInVar(varName,obj2Str("ef"))
    wk.waitFor()
    return str2Obj(dss.waitForMonoThrRev(wk))==[11,14,100]

if __name__=="__main__":
    IORNS = sys.argv[-1]
    with salome.SessionContextManager():
        if waitKey(IORNS) is not True:
            raise AssertionError("Not the expected result")
