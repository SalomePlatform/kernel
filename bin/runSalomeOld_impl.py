#!/usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

## @package runSalome
# \brief Module that provides services to launch SALOME
#

import sys, os, string, glob, time, pickle, re
from . import orbmodule
from . import setenv_impl
from .launchConfigureParser import verbose
from .server import process_id, Server
import json
import subprocess
from .salomeContextUtils import ScriptAndArgsObjectEncoder
import platform
from .runSalomeCommon import setVerbose, InterpServer, CatalogServer, SalomeDSServer, ConnectionManagerServer, RegistryServer, ContainerCPPServer, LoggerServer, CommonSessionServer, SessionServer, LauncherServer
# -----------------------------------------------------------------------------

from .killSalome_impl import killAllPorts

def killLocalPort():
    """
    kill servers from a previous SALOME execution, if needed,
    on the CORBA port given in args of runSalome
    """

    from .killSalomeWithPort_impl import killMyPort
    my_port=str(args['port'])
    try:
        killMyPort(my_port)
    except Exception:
        print("problem in killLocalPort()")
        pass
    pass

def givenPortKill(port):
    """
    kill servers from a previous SALOME execution, if needed,
    on the same CORBA port
    """

    from killSalomeWithPort import killMyPort
    my_port=port
    try:
        killMyPort(my_port)
    except Exception:
        print("problem in LocalPortKill(), killMyPort(%s)"%port)
        pass
    pass

def kill_salome(args):
    """
    Kill servers from previous SALOME executions, if needed;
    depending on args 'killall' or 'portkill', kill all executions,
    or only execution on the same CORBA port
    """

    if args['killall']:
        killAllPorts()
    elif args['portkill']:
        givenPortKill(str(args['port']))

def startGUI(clt):
    """Salome Session Graphic User Interface activation"""
    from . import Engines
    from . import SALOME
    from . import SALOMEDS
    from . import SALOME_ModuleCatalog
    from . import SALOME_Session_idl
    session=clt.waitNS("/Kernel/Session",SALOME.Session)
    session.GetInterface()

# -----------------------------------------------------------------------------

def startSalome(args, modules_list, modules_root_dir):
    """Launch all SALOME servers requested by args"""
    init_time = os.times()

    if verbose(): print("startSalome ", args)

    #
    # Set server launch command
    #
    if 'server_launch_mode' in args:
        Server.set_server_launch_mode(args['server_launch_mode'])

    #
    # Wake up session option
    #
    if args['wake_up_session']:
        if "OMNIORB_CONFIG" not in os.environ:
            from salome.kernel.salome_utils import generateFileName
            omniorbUserPath = os.getenv("OMNIORB_USER_PATH")
            kwargs={}
            if omniorbUserPath is not None:
                kwargs["with_username"]=True

            last_running_config = generateFileName(omniorbUserPath, prefix="omniORB",
                                                   suffix="last",
                                                   extension="cfg",
                                                   hidden=True,
                                                   **kwargs)
            os.environ['OMNIORB_CONFIG'] = last_running_config
            pass
        pass

    #
    # Initialisation ORB and Naming Service
    #
    from . import orbmodule
    clt=orbmodule.client(args)
    addToPidict(args)

    #
    # Wake up session option
    #
    if args['wake_up_session']:
        from . import Engines
        from . import SALOME
        from . import SALOMEDS
        from . import SALOME_ModuleCatalog
        from . import SALOME_Session_idl
        session = clt.waitNS("/Kernel/Session",SALOME.Session)
        status = session.GetStatSession()
        if status.activeGUI:
            from salome.kernel.salome_utils import getPortNumber
            port = getPortNumber()
            msg  = "Warning :"
            msg += "\n"
            msg += "Session GUI for port number %s is already active."%(port)
            msg += "\n"
            msg += "If you which to wake up another session,"
            msg += "\n"
            msg += "please use variable OMNIORB_CONFIG"
            msg += "\n"
            msg += "to get the correct session object in naming service."
            sys.stdout.write(msg+"\n")
            sys.stdout.flush()
            return clt
        session.GetInterface()
        args["session_object"] = session
        return clt

    # Launch Logger Server (optional)
    # and wait until it is registered in naming service
    #

    if args['logger']:
        myServer=LoggerServer(args)
        myServer.run()
        clt.waitLogger("Logger")
        addToPidict(args)

    # Launch  Session Server (to show splash ASAP)
    #

    if args["gui"] and not args['launcher_only']:
        mySessionServ = SessionServer(args,args['modules'],modules_root_dir)
        mySessionServ.setpath(modules_list,modules_root_dir)
        mySessionServ.run()
        addToPidict(args)

    #
    # Launch Registry Server,
    # and wait until it is registered in naming service
    #

    if ('registry' not in args['embedded']) | (args["gui"] == 0) :
        myServer=RegistryServer(args)
        myServer.run()
        if sys.platform == "win32":
          clt.waitNS("/Registry")
        else:
          clt.waitNSPID("/Registry",myServer.PID)
        addToPidict(args)

    #
    # Launch Catalog Server,
    # and wait until it is registered in naming service
    #

    if ('moduleCatalog' not in args['embedded']) | (args["gui"] == 0):
        cataServer=CatalogServer(args)
        cataServer.setpath(modules_list,modules_root_dir)
        cataServer.run()
        from . import SALOME_ModuleCatalog
        if sys.platform == "win32":
          clt.waitNS("/Kernel/ModulCatalog",SALOME_ModuleCatalog.ModuleCatalog)
        else:
          clt.waitNSPID("/Kernel/ModulCatalog",cataServer.PID,SALOME_ModuleCatalog.ModuleCatalog)
        addToPidict(args)

    #
    # Launch SalomeDS Server,
    # and wait until it is registered in naming service
    #

    # print("ARGS = ",args)
    if ('study' not in args['embedded']) | (args["gui"] == 0):
        print("RunStudy")
        myServer=SalomeDSServer(args)
        myServer.run()
        if sys.platform == "win32":
          clt.waitNS("/Study")
        else:
          clt.waitNSPID("/Study",myServer.PID)
        addToPidict(args)

    #
    # Launch LauncherServer
    #

    if not 'launcher' in args:
      myCmServer = LauncherServer(args)
      myCmServer.setpath(modules_list,modules_root_dir)
      myCmServer.run()
      addToPidict(args)

    #
    # Launch ConnectionManagerServer
    #

    if not args['launcher_only']:
      myConnectionServer = ConnectionManagerServer(args)
      myConnectionServer.run()

    from .Utils_Identity import getShortHostName

    if os.getenv("HOSTNAME") == None:
        if os.getenv("HOST") == None:
            os.environ["HOSTNAME"]=getShortHostName()
        else:
            os.environ["HOSTNAME"]=os.getenv("HOST")

    theComputer = getShortHostName()

    #
    # Launch local C++ Container (FactoryServer),
    # and wait until it is registered in naming service
    #

    if ('cppContainer' in args['standalone']) | (args["gui"] == 0) :
        myServer=ContainerCPPServer(args, with_gui=args["gui"]!=0)
        myServer.run()
        if sys.platform == "win32":
          clt.waitNS("/Containers/" + theComputer + "/FactoryServer")
        else:
          clt.waitNSPID("/Containers/" + theComputer + "/FactoryServer",myServer.PID)
        addToPidict(args)

    if 'pyContainer' in args['standalone']:
        raise Exception('Python containers no longer supported')

    #
    # Wait until Session Server is registered in naming service
    #

    if args["gui"] and not args['launcher_only']:
##----------------
        from . import Engines
        from . import SALOME
        from . import SALOMEDS
        from . import SALOME_ModuleCatalog
        from . import SALOME_Session_idl
        if sys.platform == "win32":
          session=clt.waitNS("/Kernel/Session",SALOME.Session)
        else:
          session=clt.waitNSPID("/Kernel/Session",mySessionServ.PID,SALOME.Session)
        args["session_object"] = session
    end_time = os.times()
    if verbose(): print()
    print("Start SALOME, elapsed time : %5.1f seconds"% (end_time[4]
                                                         - init_time[4]))

    # ASV start GUI without Loader
    #if args['gui']:
    #    session.GetInterface()

    #
    # additional external python interpreters
    #
    nbaddi=0

    try:
        if 'interp' in args:
            nbaddi = args['interp']
    except Exception:
        import traceback
        traceback.print_exc()
        print("-------------------------------------------------------------")
        print("-- to get an external python interpreter:runSalome --interp=1")
        print("-------------------------------------------------------------")

    if verbose(): print("additional external python interpreters: ", nbaddi)
    if nbaddi:
        for i in range(nbaddi):
            print("i=",i)
            anInterp=InterpServer(args)
            anInterp.run()

    # set PYTHONINSPECT variable (python interpreter in interactive mode)
    if args['pinter']:
        os.environ["PYTHONINSPECT"]="1"
        try:
            import readline
        except ImportError:
            pass

    return clt

# -----------------------------------------------------------------------------

def useSalome(args, modules_list, modules_root_dir):
    """
    Launch all SALOME servers requested by args,
    save list of process, give info to user,
    show registered objects in Naming Service.
    """
    global process_id

    clt=None
    try:
        clt = startSalome(args, modules_list, modules_root_dir)
    except Exception:
        import traceback
        traceback.print_exc()
        print()
        print()
        print("--- Error during Salome launch ---")

    # print(process_id)

    from .addToKillList_impl import addToKillList
    from .killSalomeWithPort_impl import getPiDict

    filedict = getPiDict(args['port'])
    for pid, cmd in list(process_id.items()):
        addToKillList(pid, cmd, args['port'])
        pass

    if verbose(): print("""
    Saving of the dictionary of Salome processes in %s
    To kill SALOME processes from a console (kill all sessions from all ports):
      python killSalome.py
    To kill SALOME from the present interpreter, if it is not closed :
      killLocalPort()      --> kill this session
                               (use CORBA port from args of runSalome)
      givenPortKill(port)  --> kill a specific session with given CORBA port
      killAllPorts()       --> kill all sessions

    runSalome, with --killall option, starts with killing
    the processes resulting from the previous execution.
    """%filedict)

    #
    #  Print Naming Service directory list
    #

    if clt != None:
        if verbose():
            print()
            print(" --- registered objects tree in Naming Service ---")
            clt.showNS()
            pass

        if not args['gui'] or not args['session_gui']:
            if args['shutdown_servers']:
                class __utils__:
                    def __init__(self, port):
                        self.port = port
                        from . import killSalomeWithPort_impl
                        self.killSalomeWithPort = killSalomeWithPort_impl
                        return
                    def __del__(self):
                        self.killSalomeWithPort.killMyPort(self.port)
                        return
                    pass
                def func(s):
                    del s
                import atexit
                atexit.register(func, __utils__(args['port']))
                pass
            pass

        # run python scripts, passed as command line arguments
        toimport = []
        if 'gui' in args and 'session_gui' in args:
            if not args['gui'] or not args['session_gui']:
                if 'study_hdf' in args:
                    toopen = args['study_hdf']
                    if toopen:
                        from . import salome
                        salome.salome_init_with_session(path=toopen)
                if 'pyscript' in args:
                    toimport = args['pyscript']
        from .salomeContextUtils import formatScriptsAndArgs
        command = formatScriptsAndArgs(toimport, escapeSpaces=True)
        if command:
            proc = subprocess.Popen(command, shell=True)
            addToKillList(proc.pid, command, args['port'])
            res = proc.wait()
            if res: sys.exit(1) # if there's an error when executing script, we should explicitly exit

    return clt

def execScript(script_path):
    print('executing', script_path)
    sys.path.insert(0, os.path.realpath(os.path.dirname(script_path)))
    exec(compile(open(script_path).read(), script_path, 'exec'),globals())
    del sys.path[0]

# -----------------------------------------------------------------------------

def registerEnv(args, modules_list, modules_root_dir):
    """
    Register args, modules_list, modules_root_dir in a file
    for further use, when SALOME is launched embedded in an other application.
    """
    from salome.kernel.salome_utils import getTmpDir
    fileEnv = getTmpDir()
    from salome.kernel.salome_utils import getUserName
    fileEnv += getUserName() + "_" + str(args['port']) \
            + '_' + args['appname'].upper() + '_env'
    fenv=open(fileEnv,'w')
    pickle.dump((args, modules_list, modules_root_dir),fenv)
    fenv.close()
    os.environ["SALOME_LAUNCH_CONFIG"] = fileEnv

# -----------------------------------------------------------------------------

def no_main():
    """Salome Launch, when embedded in other application"""
    fileEnv = os.environ["SALOME_LAUNCH_CONFIG"]
    fenv=open(fileEnv,'r')
    args, modules_list, modules_root_dir = pickle.load(fenv)
    fenv.close()
    kill_salome(args)
    from .searchFreePort import searchFreePort
    searchFreePort(args, 0)
    clt = useSalome(args, modules_list, modules_root_dir)
    return clt

# -----------------------------------------------------------------------------

def addToPidict(args):
    global process_id
    from .addToKillList_impl import addToKillList
    for pid, cmd in list(process_id.items()):
        addToKillList(pid, cmd, args['port'])

# -----------------------------------------------------------------------------

def main(exeName=None):
    """Salome launch as a main application"""

    # define folder to store omniorb config (initially in virtual application folder)
    try:
        from .salomeContextUtils import setOmniOrbUserPath
        setOmniOrbUserPath()
    except Exception as e:
        print(e)
        sys.exit(1)

    from salome.kernel.salome_utils import getHostName
    keep_env = not os.getenv('SALOME_PLEASE_SETUP_ENVIRONMENT_AS_BEFORE')
    args, modules_list, modules_root_dir = setenv_impl.get_config(exeName=exeName, keepEnvironment=keep_env)
    print("runSalome running on %s" % getHostName())

    kill_salome(args)
    save_config = True
    if 'save_config' in args:
        save_config = args['save_config']
    # --
    test = True
    if args['wake_up_session']:
        test = False
        pass
    if test and not 'launcher' in args:
        from .searchFreePort import searchFreePort
        searchFreePort(args, save_config, args.get('useport'))
        pass
    # --
    #setenv_impl.main()
    setenv_impl.set_env(args, modules_list, modules_root_dir, keepEnvironment=keep_env)
    clt = useSalome(args, modules_list, modules_root_dir)
    return clt,args

# -----------------------------------------------------------------------------

def foreGround(clt, args):
    # --
    if "session_object" not in args:
        return
    session = args["session_object"]
    # --
    # Wait until gui is arrived
    # tmax = nbtot * dt
    # --
    gui_detected = False
    dt = 0.1
    nbtot = 100
    nb = 0
    session_pid = None
    while 1:
        try:
            status = session.GetStatSession()
            gui_detected = status.activeGUI
            session_pid = session.getPID()
        except Exception:
            pass
        if gui_detected:
            break
        from time import sleep
        sleep(dt)
        nb += 1
        if nb == nbtot:
            break
        pass
    # --
    if not gui_detected:
        return
    # --
    from salome.kernel.salome_utils import getPortNumber
    port = getPortNumber()
    # --
    server = Server({})
    if sys.platform == "win32":
      server.CMD = [os.getenv("PYTHONBIN"), "-m", "killSalomeWithPort", "--spy", "%s"%(session_pid or os.getpid()), "%s"%(port)]
    else:
      server.CMD = ["killSalomeWithPort.py", "--spy", "%s"%(session_pid or os.getpid()), "%s"%(port)]
    server.run(True)
    # os.system("killSalomeWithPort.py --spy %s %s &"%(os.getpid(), port))
    # --
    dt = 1.0
    try:
        while 1:
            try:
                status = session.GetStatSession()
                assert status.activeGUI
            except Exception:
                break
            from time import sleep
            sleep(dt)
            pass
        pass
    except KeyboardInterrupt:
        from killSalomeWithPort import killMyPort
        killMyPort(port)
        pass
    return
#

def runSalome():
    clt,args = main()
    # --
    test = args['gui'] and args['session_gui']
    test = test or args['wake_up_session']
    # --
    # The next test covers the --pinter option or if var PYTHONINSPECT is set
    # --
    test = test and not os.environ.get('PYTHONINSPECT')
    # --
    # The next test covers the python -i $KERNEL_ROOT_DIR/bin/salome/runSalome.py case
    # --
    try:
        from ctypes import POINTER, c_int, cast, pythonapi
        iflag_ptr = cast(pythonapi.Py_InteractiveFlag, POINTER(c_int))
        test = test and not iflag_ptr.contents.value
    except Exception:
        pass
    # --
#    test = test and os.getenv("SALOME_TEST_MODE", "0") != "1"
    test = test and args['foreground']
    # --
    if test:
        from time import sleep
        sleep(3.0)
        foreGround(clt, args)
        pass
    pass
#
