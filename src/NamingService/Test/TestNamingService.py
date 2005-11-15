import sys, os,signal,string,commands
import runSalome
import orbmodule
import TestKiller

# get SALOME environment :

args, modules_list, modules_root_dir = runSalome.get_config()
runSalome.set_env(args, modules_list, modules_root_dir)

# set environment for trace in logger
# (with file, servers may be killed before the write to the file...)

#os.environ["SALOME_trace"] = "file:/tmp/traceUnitTest.log"
#os.environ["SALOME_trace"] = "local"
os.environ["SALOME_trace"] = "with_logger"

# launch CORBA naming server

clt=orbmodule.client()

# launch CORBA logger server

myServer=runSalome.LoggerServer(args)
myServer.run()
clt.waitLogger("Logger")

# execute Unit Test

command = ['TestNamingService']
ret = os.spawnvp(os.P_WAIT, command[0], command)

# kill Test process

TestKiller.killProcess(runSalome.process_id)
