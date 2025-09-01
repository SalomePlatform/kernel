class ScriptLocalParameters:
    def __init__(self, args):
        from salome.kernel import KernelBasis
        self.debug = KernelBasis.VerbosityActivated()
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
        str = """nb_proc: {self.nb_proc} workdir: {self.workdir} isTmpDir: {self.isTmpDir} name_server: {self.name_server} container: {self.container} container_name: {self.container_name} libbatch_nodefile: {self.libbatch_nodefile} machine_file: {self.machine_file} ompi_uri_file: {self.ompi_uri_file}""".format(**locals())
        return str
