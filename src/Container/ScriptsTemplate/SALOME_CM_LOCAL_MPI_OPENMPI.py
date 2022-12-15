import sys
from script_parameters import ScriptLocalParameters

def command(args):
    options = ScriptLocalParameters(args)
    if options.debug: print(options)

    cmd = []
    cmd.append("mpirun -np " + options.nb_proc)

    if options.libbatch_nodefile:
        cmd.append("-machinefile %s " % options.machine_file)

    cmd.append("-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace ")
    if options.ompi_uri_file:
        cmd.append("-ompi-server file:%s" % options.ompi_uri_file)

    cmd.append(options.container)
    cmd.append(options.container_name)

    return " ".join(cmd)