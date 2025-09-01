import sys

class ScriptRemoteParameters:
    def __init__(self, args):
        from salome.kernel import KernelBasis
        self.debug = KernelBasis.VerbosityActivated()
        self.protocol = args[0]
        self.user = self._read_arg(args[1], "NULL")
        self.host = self._read_arg(args[2], "NULL")
        self.appli = self._read_arg(args[3], "NULL")
        self.workdir = self._read_arg(args[4], "NULL")
        self.ssl = True if args[5] == "1" else False
        self.nshost = args[6]
        self.nsport = args[7]
        self.remote_script = self._read_arg(args[8], "NULL")
        self.naming_service = self._read_arg(args[9], "NULL")
        self.appli_mode = args[10]

        import platform
        self.Windows = platform.system() == "Windows"

    def _read_arg(self, value, null_value):
        if value == null_value:
            return None
        return value

    def __str__(self):
        str = []
        str.append("protocol: %s" % self.protocol)
        str.append("user: %s" % self.user)
        str.append("hostname: %s" % self.host)
        str.append("appli: %s" % self.appli)
        str.append("workdir: %s" % self.workdir)
        str.append("ssl: %s" % self.ssl)
        str.append("nshost: %s" % self.nshost)
        str.append("nsport: %s" % self.nsport)
        str.append("remote_script: %s" % self.remote_script)
        str.append("naming_service: %s" % self.naming_service)
        str.append("appil_mode: %s" % self.appli_mode)
        str.append("--")
        return "\n".join(str)

# ----------------------------------------------
def command(args):
    from salome.kernel import KernelBasis
    options = ScriptRemoteParameters(args)
    if options.debug:
        KernelBasis.WriteInStdout( str(options) )

    # build command depending on protocol
    cmd = []
    envd = (options.protocol != "srun")

    if options.protocol == "rsh":
        # RSH command
        cmd.append("rsh")
        if options.user:
            cmd.append("-l " + options.user)
        cmd.append(options.host)

    elif options.protocol == "ssh":
        # SSH command
        cmd.append("ssh")
        if options.user:
            cmd.append("-l " + options.user)
        cmd.append(options.host)

    elif options.protocol == "srun":
        # srun command
        cmd.append("srun")
        cmd.append("-n 1 -N 1 --overlap --mem-per-cpu=0 --cpu-bind=none")
        cmd.append("--nodelist=" + options.host)

    elif options.protocol == "pbsdsh":
        # pbsdh command
        cmd.append("pbsdsh")
        cmd.append("-o -h")
        cmd.append(options.host)

    elif options.protocol == "blaunch":
        # blaunch command
        cmd.append("blaunch")
        cmd.append("-no-shell")
        cmd.append(options.host)

    else:
        # unknown protocol
        raise ValueError("Unknown protocol: %s" % options.protocol)


    if options.appli_mode == "dir":
        cmd.append(options.appli + "/" + options.remote_script)
        if not envd:
            cmd.append("--noenvd")
        if options.ssl:
            cmd.append(options.naming_service)
        else:
            cmd.append(options.nshost)
            cmd.append(options.nsport)

        if options.workdir:
            cmd.append(" WORKINGDIR '%s'" % options.workdir)

    elif options.appli_mode == "launcher":
        cmd.append(options.appli + " remote")
        if not options.ssl:
            cmd.append("-m %s -p %s" % (options.nshost, options.nsport))

        if options.workdir:
            cmd.append("-d " + options.workdir)
        cmd.append("--")

    # elif ignore other appli_mode value

    return " ".join(cmd)
