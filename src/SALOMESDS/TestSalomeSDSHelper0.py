import SALOME
import pickle
import salome
import sys

salome.salome_init()

scopeName="Scope1"
varName="a"

def obj2Str(obj):
    return pickle.dumps(obj,pickle.HIGHEST_PROTOCOL)

def str2Obj(strr):
    return pickle.loads(strr)

def waitKey():
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    assert(not isCreated)
    wk=dss.waitForKeyInVar(varName,obj2Str("ef"))
    wk.waitFor()
    return str2Obj(dss.waitForMonoThrRev(wk))==[11,14,100]

if __name__=="__main__":
    sys.exit(not waitKey())
