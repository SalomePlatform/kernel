import SALOME
import cPickle
import salome
import sys

salome.salome_init()

scopeName="Scope1"
varName="a"

def obj2Str(obj):
    return cPickle.dumps(obj,cPickle.HIGHEST_PROTOCOL)

def str2Obj(strr):
    return cPickle.loads(strr)

def waitKey():
    dsm=salome.naming_service.Resolve("/DataServerManager")
    dss,isCreated=dsm.giveADataScopeTransactionCalled(scopeName)
    assert(not isCreated)
    wk=dss.waitForKeyInVar(varName,obj2Str("ef"))
    return str2Obj(wk.waitFor())==[11,14,100]

if __name__=="__main__":
    sys.exit(not int(waitKey()))
