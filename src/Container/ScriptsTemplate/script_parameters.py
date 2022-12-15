class ScriptLocalParameters:
    def __init__(self, args):
        self.debug = False
        if args[0] == "-d":
            self.debug = True
            args = args[1:]

        self.nb_proc = self._read_arg(args[0], "NULL")
        self.workdir = self._read_arg(args[1], "NULL")
        self.isTmpDir = True if args[2] == "1" else False
        self.name_server = args[3]
        self.container = args[4]
        self.container_name = args[5]
        self.libbatch_nodefile = self._read_arg(args[6], "NULL")
        self.machine_file = self._read_arg(args[7], "NULL")
        self.ompi_uri_file = self._read_arg(args[8], "NULL")

        import platform
        self.Windows = platform.system() == "Windows"

    def _read_arg(self, value, null_value):
        if value == null_value:
            return None
        return value

    def __str__(self):
        str = []
        str.append("nb_proc: %s" % self.nb_proc)
        str.append("workdir: %s" % self.workdir)
        str.append("isTmpDir: %s" % self.isTmpDir)
        str.append("name_server: %s" % self.name_server)
        str.append("container: %s" % self.container)
        str.append("container_name: %s" % self.container_name)
        str.append("libbatch_nodefile: %s" % self.libbatch_nodefile)
        str.append("machine_file: %s" % self.machine_file)
        str.append("ompi_uri_file: %s" % self.ompi_uri_file)
        str.append("--")
        return "\n".join(str)