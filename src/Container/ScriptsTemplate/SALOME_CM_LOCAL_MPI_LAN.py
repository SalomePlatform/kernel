import sys
from script_parameters import ScriptLocalParameters

def command(args):
    options = ScriptLocalParameters(args)
    if options.debug: print(options)

    cmd = []
    cmd.append("mpirun -np " + options.nb_proc)

    if options.libbatch_nodefile:
        cmd.append("-machinefile %s " % options.machine_file)

    cmd.append("-x PATH,LD_LIBRARY_PATH,OMNIORB_CONFIG,SALOME_trace ")

    cmd.append(options.container)
    cmd.append(options.container_name)

    return " ".join(cmd)