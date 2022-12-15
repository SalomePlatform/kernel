import sys
from script_parameters import ScriptLocalParameters

def command(args):
    options = ScriptLocalParameters(args)
    if options.debug: print(options)

    cmd = []
    if options.workdir:
        if options.isTmpDir:
            if options.Windows:
                cmd.append("cd /d %s\n" % options.workdir)
            else:
                cmd.append("cd %s;" % options.workdir)
        else:
            if options.Windows:
                cmd.append("mkdir %s" % options.workdir);
                cmd.append("cd /d %s\n" % options.workdir)
            else:
                cmd.append("mkdir -p %s && cd %s;" % (options.workdir, options.workdir))

    cmd.append(options.container)
    cmd.append(options.container_name)

    return " ".join(cmd)