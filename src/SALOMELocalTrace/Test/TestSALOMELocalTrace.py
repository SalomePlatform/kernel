
import sys, os,signal,string,commands
import runSalome

# get SALOME environment :
# here we need KERNEL_ROOT_DIR, PATH, LD_LIBRARY_PATH

args, modules_list, modules_root_dir = runSalome.get_config()
runSalome.set_env(args, modules_list, modules_root_dir)

# execute Unit Test

command = ['TestSALOMELocalTrace']
ret = os.spawnvp(os.P_WAIT, command[0], command)

# no process to kill
