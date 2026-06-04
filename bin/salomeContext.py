#! /usr/bin/env python3
# Copyright (C) 2013-2026  CEA, EDF, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#

""" This file must be compatible with python 3.6 as it is used by the salome boostrap."""

import logging
import os
import sys
import pickle
import platform
import subprocess
from typing import Any, List, Optional, Tuple, Union

from parseConfigFile import parseConfigFile
from salomeContextUtils import SalomeContextException # type: ignore


def usage(name: str = "salome", appended_cmd_doc: str = "", appended_opt_doc: str = "", do_print: bool = True) -> str:
    usage_str = f"""\
Usage: {name} [command] [options] [--config=<file,folder,...>] [--with-env-modules=<env_module1,env_module2,...>]

Commands:
=========
    start           Start a new SALOME instance. Start a single SALOME_Session_Server_No_Server
                    process with environment relevant to the application and hosting all servants in it.
    context         Initialize SALOME context. Current environment is extended.
    shell           Initialize SALOME context, attached to the last created SALOME
                    instance if any, and executes scripts passed as command arguments.
                    User works in a Shell terminal. SALOME environment is set but
                    application is not started.
    test            Run SALOME tests.
    info            Display some information about SALOME.
    doc <module(s)> Show online module documentation (if available).
                    Module names must be separated by blank characters.
    help            Show this message.
    remote          run command in SALOME environment from remote call, ssh or rsh.
    withsession     Start a new SWS SALOME instance with multiple servers hosting all servants.
    connect         In SWS context, Connect a Python console to the active SALOME instance.
    kill <port(s)>  In SWS context, Terminate SALOME instances running on given ports for current user.
                    Port numbers must be separated by blank characters.
    killall         Terminate *all* SALOME running SWS instances for current user.
                    Do not start a new one.

{appended_cmd_doc}
If no command is given, default is start.

Command options:
================
    Use salome <command> --help to show help on command. Available for the
    following commands: start, shell, connect, test, info.

--config=<file,folder,...>
==========================
    Initialize SALOME context from a list of context files and/or a list
    of folders containing context files. The list is comma-separated, without
    any blank characters.

--with-env-modules=<env_module1,env_module2,...>
================================================
    Initialize SALOME context with the provided additional environment modules.
    The list is comma-separated, without any blank characters.
{appended_opt_doc}
"""

    if do_print:
        print(usage_str)
    return usage_str


class SalomeContext:
    """
    The SalomeContext class in an API to configure SALOME context then
    start SALOME using a single python command.

    Initialize context from a list of configuration files
    identified by their names.
    These files should be in appropriate .cfg format.
    """

    def __init__(self, configFileNames: Optional[List[str]] = None, name: str = "salome"):
        self.getLogger().setLevel(logging.INFO)
        # it could be None explicitly (if user use multiples setVariable...for standalone)
        self.configFileNames = None
        self.name = name
        if configFileNames is None:
            return
        configFileNames = configFileNames or []
        if len(configFileNames) == 0:
            raise SalomeContextException("No configuration files given")
        self.configFileNames = configFileNames

    def __setContextFromConfigFiles(self) -> None:
        if self.configFileNames is None:
            return
        reserved = [
            "PATH",
            "DYLD_FALLBACK_LIBRARY_PATH",
            "DYLD_LIBRARY_PATH",
            "LD_LIBRARY_PATH",
            "PYTHONPATH",
            "MANPATH",
            "PV_PLUGIN_PATH",
            "INCLUDE",
            "LIBPATH",
            "SALOME_PLUGINS_PATH",
            "LIBRARY_PATH",
            "QT_PLUGIN_PATH",
        ]
        for filename in self.configFileNames:
            extension = os.path.splitext(filename)[-1]
            if extension == ".cfg":
                self.__setContextFromConfigFile(filename, reserved)
            else:
                self.getLogger().error(
                    "Unrecognized extension for configuration file: %s", filename
                )

    def __loadEnvModules(self, env_modules: List[str]) -> None:
        modulecmd = os.getenv("LMOD_CMD")
        if not modulecmd:
            raise SalomeContextException("Lmod module environment not present")
        try:
            out, err = subprocess.Popen(
                [modulecmd, "python", "try-load"] + env_modules, stdout=subprocess.PIPE
            ).communicate()
            exec(out)  # define specific environment variables
        except Exception:
            raise SalomeContextException(
                f"Failed to load env modules: {' '.join(env_modules)} with error {err.decode()}"
            )

    def runSalome(self, args: List[str]) -> Tuple[bytes, bytes, Union[int, Any]]:
        # Run this module as a script, in order to use appropriate Python interpreter
        # according to current path (initialized from context files).
        env_modules_option = "--with-env-modules="
        env_modules_l = [x for x in args if x.startswith(env_modules_option)]
        if env_modules_l:
            env_modules = env_modules_l[-1][len(env_modules_option) :].split(",")
            self.__loadEnvModules(env_modules)
            args = [x for x in args if not x.startswith(env_modules_option)]
        else:
            env_modules = os.getenv("SALOME_ENV_MODULES", None)
            if env_modules:
                self.__loadEnvModules(env_modules.split(","))

        self.__setContextFromConfigFiles()
        env_copy = os.environ.copy()
        selfBytes = pickle.dumps(self, protocol=0)
        argsBytes = pickle.dumps(args, protocol=0)
        #
        absoluteAppliPath = os.getenv('ABSOLUTE_APPLI_PATH','')
        _script = os.path.join(absoluteAppliPath,"bin","salome","salomeContext.py")
        proc = subprocess.Popen(
            [
                "python3",
                __file__,
                selfBytes.decode("latin1"),
                argsBytes.decode("latin1"),
            ],
            shell=False,
            close_fds=True,
            env=env_copy,
        )
        out, err = proc.communicate()
        return out, err, proc.returncode

    def addToPath(self, value: str) -> None:
        """Append value to PATH environment variable"""
        self.addToVariable("PATH", value)

    def addToLdLibraryPath(self, value: str) -> None:
        """Append value to LD_LIBRARY_PATH environment variable"""
        if sys.platform == "win32":
            self.addToVariable("PATH", value)
        elif sys.platform == "darwin":
            if "LAPACK" in value:
                self.addToVariable("DYLD_FALLBACK_LIBRARY_PATH", value)
            else:
                self.addToVariable("DYLD_LIBRARY_PATH", value)
        else:
            self.addToVariable("LD_LIBRARY_PATH", value)

    def addToDyldLibraryPath(self, value: str) -> None:
        """Append value to DYLD_LIBRARY_PATH environment variable"""
        self.addToVariable("DYLD_LIBRARY_PATH", value)

    def addToPythonPath(self, value: str) -> None:
        """Append value to PYTHONPATH environment variable"""
        self.addToVariable("PYTHONPATH", value)

    def addToTestsPath(self, value: str) -> None:
        """Append value to SALOME_TESTS_PATH environnement variable"""
        self.appendVariable("SALOME_TESTS_PATH", value, separator=":")

    def setVariable(self, name, value: str, overwrite: bool = False) -> None:
        """Set environment variable to value"""
        env = os.getenv(name, "")
        if env and not overwrite:
            self.getLogger().error(
                "Environment variable already existing (and not overwritten): %s=%s",
                name,
                value,
            )
            return

        if env:
            self.getLogger().debug(
                "Overwriting environment variable: %s=%s", name, value
            )

        value = os.path.expandvars(value)  # expand environment variables
        self.getLogger().debug("Set environment variable: %s=%s", name, value)
        os.environ[name] = value

    def setDefaultValue(self, name: str, value: Any) -> None:
        """Set environment variable only if it is undefined."""
        env = os.getenv(name, "")
        if not env:
            value = os.path.expandvars(value)  # expand environment variables
            self.getLogger().debug("Set environment variable: %s=%s", name, value)
            os.environ[name] = value

    def unsetVariable(self, name: str) -> None:
        """Unset environment variable"""
        if name in os.environ:
            self.getLogger().debug("Unset environment variable: %s", name)
            del os.environ[name]

    def addToVariable(self, name: str, value: Any, separator: str = os.pathsep) -> None:
        """Prepend value to environment variable"""
        if value == "":
            return

        value = os.path.expandvars(value)  # expand environment variables
        env = os.getenv(name, None)
        env_list = env.split(separator) if env is not None else []
        self.getLogger().debug("Prepend to environment variable: %s : %s", name, value)
        value_list = []
        for current_val in value.split(separator):
            if current_val not in value_list+env_list:
                value_list.append(current_val)
        env_list = value_list + env_list
        self.getLogger().debug("Add to %s: %s", name, separator.join(value_list))
        os.environ[name] = separator.join(env_list)

    def appendVariable(self, name, value, separator=os.pathsep):
        """Append a variable"""
        if value == "":
            return

        value = os.path.expandvars(value)  # expand environment variables
        env = os.getenv(name, None)
        env_list = env.split(separator) if env is not None else []
        self.getLogger().debug("Append to environment variable: %s : %s", name, value)
        value_list = []
        for current_val in value.split(separator):
            if current_val not in value_list+env_list:
                value_list.append(current_val)
        env_list += value_list
        self.getLogger().debug("Add to %s: %s", name, separator.join(value_list))
        os.environ[name] = separator.join(env_list)

    def removeFromVariable(self, name, value, separator=os.pathsep):
        """Remove value from environment variable"""
        if value == "":
            return

        value = os.path.expandvars(value)  # expand environment variables
        self.getLogger().debug("Remove from %s: %s", name, value)
        env = os.getenv(name, None)
        if env == value:
            env = ""
        else:
            # env = env.removeprefix(value + separator) (Python >= 3.9)
            str = value + separator
            if env.startswith(str):
                env = env[len(str) :]
            # env = env.removesuffix(separator + value) (Python >= 3.9)
            str = separator + value
            if env.endswith(str):
                env = env[: -len(str)]
            env = env.replace(separator + value + separator, ":")

        os.environ[name] = env

    ###################################
    # This begins the private section #
    ###################################

    def __parseArguments(self, args: List[str]) -> Tuple[Optional[str], List[str]]:
        if len(args) == 0 or args[0].startswith("-"):
            return None, args

        command = args[0]
        options = args[1:]

        availableCommands = {
            "start": "_sessionless",
            "withsession": "_runAppli",
            "context": "_setContext",
            "shell": "_runSession",
            "remote": "_runRemote",
            "connect": "_runConsole",
            "kill": "_kill",
            "killall": "_killAll",
            "test": "_runTests",
            "info": "_showInfo",
            "doc": "_showDoc",
            "help": "_usage",
            "coffee": "_makeCoffee",
            "car": "_getCar",
        }

        if command not in availableCommands:
            command = "start"
            options = args

        return availableCommands[command], options

    def _startSalome(self, args: List[str]) -> int:
        """
        Run SALOME!
        Args consist in a mandatory command followed by optional parameters.
        See usage for details on commands.
        """
        try:
            from setenv import add_path

            absoluteAppliPath = os.getenv("ABSOLUTE_APPLI_PATH")
            path = os.path.realpath(os.path.join(absoluteAppliPath, "bin", "salome"))
            add_path(path, "PYTHONPATH")
            path = os.path.realpath(
                os.path.join(absoluteAppliPath, "bin", "salome", "appliskel")
            )
            add_path(path, "PYTHONPATH")

        except Exception:
            pass

        command, options = self.__parseArguments(args)
        sys.argv = options

        if command is None:
            if args and args[0] in ["-h", "--help", "help"]:
                usage(name=self.name)
                return 0
            # try to default to "start" command
            command = "_sessionless"

        try:
            res = getattr(self, command)(options)  # run appropriate method
            return res or 0
        except SystemExit as ex:
            if ex.code != 0:
                self.getLogger().error("SystemExit %s in method %s.", ex.code, command)
            return ex.code
        except SalomeContextException as e:
            self.getLogger().error(e)
            return 1
        except Exception:
            self.getLogger().error("Unexpected error:")
            import traceback

            traceback.print_exc()
            return 1

    def __setContextFromConfigFile(
        self, filename: str, reserved: Optional[List[str]] = None
    ) -> int:
        mesa_root_dir = "MESA_ROOT_DIR"
        if reserved is None:
            reserved = []
        try:
            configInfo = parseConfigFile(filename, reserved)
            unsetVars = configInfo.unsetVariables
            configVars = configInfo.outputVariables
            reservedDict = configInfo.reservedValues
            defaultValues = configInfo.defaultValues
        except SalomeContextException as e:
            self.getLogger().error(str(e))
            return 1

        # unset variables
        for var in unsetVars:
            self.unsetVariable(var)

        # mesa stuff
        if "MESA_GL_VERSION_OVERRIDE" in os.environ:
            configVarsDict = {k: v for (k, v) in configVars}
            if mesa_root_dir in configVarsDict:
                path_to_mesa_lib = os.path.join(configVarsDict[mesa_root_dir], "lib")
                if os.name == "posix":
                    self.addToVariable("LD_LIBRARY_PATH", path_to_mesa_lib)
                else:
                    self.addToVariable("PATH", path_to_mesa_lib)

        # set context
        for reserved in reservedDict:
            a = [_f for _f in reservedDict[reserved] if _f]  # remove empty elements
            a = [os.path.realpath(x) for x in a]
            reformattedVals = os.pathsep.join(a)
            if reserved in ["INCLUDE", "LIBPATH"]:
                self.addToVariable(reserved, reformattedVals, separator=" ")
            else:
                self.addToVariable(reserved, reformattedVals)

        for key, val in configVars:
            self.setVariable(key, val, overwrite=True)

        for key, val in defaultValues:
            self.setDefaultValue(key, val)

        pythonpath = os.getenv("PYTHONPATH", "").split(os.pathsep)
        pythonpath = [os.path.realpath(x) for x in pythonpath]
        sys.path[:0] = pythonpath
        return 0

    def _runAppli(self, args: Optional[List[str]] = None) -> int:
        if args is None:
            args = []
        # Initialize SALOME environment
        sys.argv = ["runSalomeOld"] + args
        import setenv

        setenv.main(True, exeName=f"{self.name} withsession")

        import runSalomeOld

        runSalomeOld.runSalome()
        return 0

    def _sessionless(self, args: Optional[List[str]] = None) -> int:
        if args is None:
            args = []
        sys.argv = ["runSalome"] + args
        import setenv

        setenv.main(True, exeName=f"{self.name} withsession")

        import runSalome

        runSalome.runSalome()
        return 0

    def _setContext(self, args: Optional[List[str]] = None) -> int:
        salome_context_set = os.getenv("SALOME_CONTEXT_SET")
        if salome_context_set:
            print("***")
            print("*** SALOME context has already been set.")
            print("*** Enter 'exit' (only once!) to leave SALOME context.")
            print("***")
            return 0

        os.environ["SALOME_CONTEXT_SET"] = "yes"
        print("***")
        print("*** SALOME context is now set.")
        print("*** Enter 'exit' (only once!) to leave SALOME context.")
        print("***")

        if sys.platform == "win32":
            cmd = ["cmd.exe"]
        else:
            cmd = ["/bin/bash"]
        proc = subprocess.Popen(cmd, shell=False, close_fds=True)
        proc.communicate()
        return proc.returncode

    def _runSession(self, args: Optional[List[str]] = None) -> int:
        if args is None:
            args = []
        sys.argv = ["runSession"] + args
        import runSession

        params, args = runSession.configureSession(args, exe="salome shell")

        sys.argv = ["runSession"] + args
        import setenv

        setenv.main(True)

        return runSession.runSession(params, args)

    def _runRemote(self, args: Optional[List[str]] = None) -> int:
        if args is None:
            args = []
        #   complete salome environment
        sys.argv = ["runRemote"]
        import setenv

        setenv.main(True)

        import runRemote

        return runRemote.runRemote(args)

    def _runConsole(self, args: Optional[List[str]] = None) -> int:
        if args is None:
            args = []
        # Initialize SALOME environment
        sys.argv = ["runConsole"]
        import setenv

        setenv.main(True)

        import runConsole

        return runConsole.connect(args)

    def _kill(self, args: Optional[List[str]] = None) -> int:
        if args is None:
            args = []
        ports = args
        if not ports:
            print(f"Port number(s) not provided to command: {self.name} kill <port(s)>")
            return 1

        sys.argv = ["kill"]
        import setenv

        setenv.main(True)
        if os.getenv("NSHOST") == "no_host":
            os.unsetenv("NSHOST")
        for port in ports:
            if sys.platform == "win32":
                proc = subprocess.Popen(
                    [os.getenv("PYTHONBIN"), "-m", "killSalomeWithPort", str(port)]
                )
            else:
                proc = subprocess.Popen(["killSalomeWithPort.py", str(port)])
            proc.communicate()
        return 0

    def _killAll(self, args: Optional[List[str]] = None) -> int:
        sys.argv = ["killAll"]
        import setenv

        setenv.main(True)
        if os.getenv("NSHOST") == "no_host":
            os.unsetenv("NSHOST")
        try:
            import PortManager  # mandatory

            ports = PortManager.getBusyPorts()["this"]

            if ports:
                for port in ports:
                    if sys.platform == "win32":
                        proc = subprocess.Popen(
                            [
                                os.getenv("PYTHONBIN"),
                                "-m",
                                "killSalomeWithPort",
                                str(port),
                            ]
                        )
                    else:
                        proc = subprocess.Popen(["killSalomeWithPort.py", str(port)])
                    proc.communicate()
        except ImportError:
            # :TODO: should be declared obsolete
            from killSalome import killAllPorts

            killAllPorts()
            pass
        from addToKillList import killList

        killList()
        return 0

    def _runTests(self, args: Optional[List[str]] = None) -> int:
        if args is None:
            args = []
        sys.argv = ["runTests"]
        import setenv

        setenv.main(True)

        import runTests

        return runTests.runTests(args, exe="salome test")

    def _showSoftwareVersions(self, softwares: Optional[List[str]] = None) -> None:
        absoluteAppliPath = os.getenv("ABSOLUTE_APPLI_PATH")
        filename = os.path.join(absoluteAppliPath, "sha1_collections.txt")
        if not os.path.exists(filename):
            return
        versions = {}
        max_len = 0
        with open(filename) as f:
            for line in f:
                try:
                    software, version, _ = line.split()
                    versions[software.upper()] = version
                    if len(software) > max_len:
                        max_len = len(software)
                except Exception:
                    pass
        if softwares:
            for soft in softwares:
                if soft.upper() in versions:
                    print(soft.upper().rjust(max_len), versions[soft.upper()])
        else:
            import collections

            od = collections.OrderedDict(sorted(versions.items()))
            for name, version in od.items():
                print(name.rjust(max_len), versions[name])

    def _showInfo(self, args: Optional[List[str]] = None) -> int:
        if args is None:
            args = []

        usage = f"Usage: {self.name} info [options]"
        epilog = f"""\n
Display some information about {self.name.upper()}.\n
Available options are:
    -p,--ports                     Show the list of busy ports (running SALOME instances).
    -s,--softwares [software(s)]   Show the list and versions of SALOME softwares.
                                   Software names must be separated by blank characters.
                                   If no software is given, show version of all softwares.
    -v,--version                   Show running SALOME version.
    -h,--help                      Show this message.
"""
        if not args:
            args = ["--version"]

        if "-h" in args or "--help" in args:
            print(usage + epilog)
            return 0

        if "-p" in args or "--ports" in args:
            import PortManager

            ports = PortManager.getBusyPorts()
            this_ports = ports["this"]
            other_ports = ports["other"]
            if this_ports or other_ports:
                print("SALOME instances are running on the following ports:")
                if this_ports:
                    print("   This application:", this_ports)
                else:
                    print("   No SALOME instances of this application")
                if other_ports:
                    print("   Other applications:", other_ports)
                else:
                    print("   No SALOME instances of other applications")
            else:
                print("No SALOME instances are running")

        if "-s" in args or "--softwares" in args:
            if "-s" in args:
                index = args.index("-s")
            else:
                index = args.index("--softwares")
            indexEnd = index + 1
            while indexEnd < len(args) and args[indexEnd][0] != "-":
                indexEnd = indexEnd + 1
            self._showSoftwareVersions(softwares=args[index + 1 : indexEnd])

        if "-v" in args or "--version" in args:
            print("Running with python", platform.python_version())
            return self._sessionless(["--version"])

        return 0

    def _showDoc(self, args: Optional[List[str]] = None) -> int:
        if args is None:
            args = []

        modules = args
        if not modules:
            print(f"Module(s) not provided to command: {self.name} doc <module(s)>")
            return 1

        appliPath = os.getenv("ABSOLUTE_APPLI_PATH")
        if not appliPath:
            raise SalomeContextException(
                "Unable to find application path. Please check that the variable ABSOLUTE_APPLI_PATH is set."
            )
        baseDir = os.path.join(appliPath, "share", "doc", "salome")
        for module in modules:
            docfile = os.path.join(baseDir, "gui", module.upper(), "index.html")
            if not os.path.isfile(docfile):
                docfile = os.path.join(baseDir, "tui", module.upper(), "index.html")
            if not os.path.isfile(docfile):
                docfile = os.path.join(baseDir, "dev", module.upper(), "index.html")
            if os.path.isfile(docfile):
                subprocess.Popen(["xdg-open", docfile]).communicate()
            else:
                print("Online documentation is not accessible for module:", module)

    def _usage(self, unused: Optional[List[str]] = None) -> None:
        usage()

    def _makeCoffee(self, unused: Optional[List[str]] = None) -> None:
        print("                        (")
        print("                          )     (")
        print("                   ___...(-------)-....___")
        print("               .-\"\"       )    (          \"\"-.")
        print("         .-\'``\'|-._             )         _.-|")
        print("        /  .--.|   `\"\"---...........---\"\"`   |")
        print("       /  /    |                             |")
        print("       |  |    |                             |")
        print("        \\  \\   |                             |")
        print("         `\\ `\\ |                             |")
        print("           `\\ `|            SALOME           |")
        print("           _/ /\\            4 EVER           /")
        print("          (__/  \\             <3            /")
        print("       _..---\"\"` \\                         /`\"\"---.._")
        print("    .-\'           \\                       /          \'-.")
        print("   :               `-.__             __.-\'              :")
        print("   :                  ) \"\"---...---\"\" (                 :")
        print("    \'._               `\"--...___...--\"`              _.\'")
        print("      \\\"\"--..__                              __..--\"\"/")
        print("       \'._     \"\"\"----.....______.....----\"\"\"     _.\'")
        print("          `\"\"--..,,_____            _____,,..--\"\"`")
        print("                        `\"\"\"----\"\"\"`")
        print("")
        print("                    SALOME is working for you; what else?")
        print("")

    def _getCar(self, unused: Optional[List[str]] = None) -> None:
        print("                                              _____________")
        print("                                  ..---:::::::-----------. ::::;;.")
        print("                               .\'\"\"\"\"\"\"                  ;;   \\  \":.")
        print("                            .\'\'                          ;     \\   \"\\__.")
        print("                          .\'                            ;;      ;   \\\\\";")
        print("                        .\'                              ;   _____;   \\\\/")
        print("                      .\'                               :; ;\"     \\ ___:\'.")
        print("                    .\'--...........................    : =   ____:\"    \\ \\")
        print("               ..-\"\"                               \"\"\"\'  o\"\"\"     ;     ; :")
        print("          .--\"\"  .----- ..----...    _.-    --.  ..-\"     ;       ;     ; ;")
        print("       .\"\"_-     \"--\"\"-----\'\"\"    _-\"        .-\"\"         ;        ;    .-.")
        print("    .\'  .\'   SALOME             .\"         .\"              ;       ;   /. |")
        print("   /-./\'         4 EVER <3    .\"          /           _..  ;       ;   ;;;|")
        print("  :  ;-.______               /       _________==.    /_  \\ ;       ;   ;;;;")
        print("  ;  / |      \"\"\"\"\"\"\"\"\"\"\".---.\"\"\"\"\"\"\"          :    /\" \". |;       ; _; ;;;")
        print(" /\"-/  |                /   /                  /   /     ;|;      ;-\" | ;\';")
        print(":-  :   \"\"\"----______  /   /              ____.   .  .\"\'. ;;   .-\"..T\"   .")
        print("\'. \"  ___            \"\":   \'\"\"\"\"\"\"\"\"\"\"\"\"\"\"    .   ; ;    ;; ;.\" .\"   \'--\"")
        print(" \",   __ \"\"\"  \"\"---... :- - - - - - - - - \' \'  ; ;  ;    ;;\"  .\"")
        print("  /. ;  \"\"\"---___                             ;  ; ;     ;|.\"\"")
        print(" :  \":           \"\"\"----.    .-------.       ;   ; ;     ;:")
        print("  \\  \'--__               \\   \\        \\     /    | ;     ;;")
        print("   \'-..   \"\"\"\"---___      :   .______..\\ __/..-\"\"|  ;   ; ;")
        print("       \"\"--..       \"\"\"--\"        m l s         .   \". . ;")
        print("             \"\"------...                  ..--\"\"      \" :")
        print("                        \"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"    \\        /")
        print("                                               \"------\"")
        print("")
        print("                                Drive your simulation properly with SALOME!")
        print("")

    # Add the following two methods since logger is not pickable
    # Ref: http://stackoverflow.com/questions/2999638/how-to-stop-attributes-from-being-pickled-in-python
    def __getstate__(self):
        d = dict(self.__dict__)
        if hasattr(self, "_logger"):
            del d["_logger"]
        return d

    def __setstate__(self, d):
        self.__dict__.update(d)  # I *think* this is a safe way to do it

    # Excluding self._logger from pickle operation imply using the following method to access logger
    def getLogger(self):
        if not hasattr(self, "_logger"):
            self._logger = logging.getLogger(__name__)
            # self._logger.setLevel(logging.DEBUG)
            # self._logger.setLevel(logging.WARNING)
            self._logger.setLevel(logging.ERROR)
        return self._logger


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(usage=usage(do_print=False))
    parser.add_argument(
        "context",
        # type=bytes,
        help="The pickled context class object")
    parser.add_argument(
        "args",
        # type=bytes,
        help="The pickled args")
    args = parser.parse_args()

    context : SalomeContext = pickle.loads(args.context.encode("latin1"))
    cargs: str= pickle.loads(args.args.encode("latin1"))
    status = context._startSalome(cargs)
    sys.exit(status)
