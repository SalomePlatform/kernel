#!/usr/bin/env python3
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2022-2026  CEA, EDF, OPEN CASCADE
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
from . import setenv_impl
from .server import process_id, Server
import json
import subprocess
from .salomeContextUtils import ScriptAndArgsObjectEncoder
from . import runSalomeNoServer_impl
from . import runSalomeCommon
import platform
from .launchConfigureParser import verbosity_nam
import logging
logger = logging.getLogger()

# -----------------------------------------------------------------------------

from .killSalome_impl import killAllPorts

def kill_salome(args):
    """
    Kill servers from previous SALOME executions, if needed;
    depending on args 'killall' or 'portkill', kill all executions,
    or only execution on the same CORBA port
    """

    if args['killall']:
        killAllPorts()
#
# -----------------------------------------------------------------------------

def startGUI(clt):
    """Salome Session Graphic User Interface activation"""
    from . import Engines
    from . import SALOME_CMOD
    from . import SALOMEDS
    from . import SALOME_ModuleCatalog
    from . import SALOME_Session_idl
    session=clt.waitNS("/Kernel/Session",SALOME.Session)
    session.GetInterface()

# -----------------------------------------------------------------------------

def startSalome(args, modules_list, modules_root_dir):
    """Launch all SALOME servers requested by args"""
    init_time = os.times()

    logger.debug("startSalome : {} ".format(args))

    ior_fakens_filename = None

    # Launch  Session Server (to show splash ASAP)
    #

    if args["gui"] and not args['launcher_only']:
        mySessionServ = runSalomeNoServer_impl.NoSessionServer(args,args['modules'],modules_root_dir)
        mySessionServ.setpath(modules_list,modules_root_dir)
        mySessionServ.run()
        ior_fakens_filename = mySessionServ.iorfakens
        logger.debug("Rendez-vous file for to retrieve IOR of session is \"{}\"".format(ior_fakens_filename))

    end_time = os.times()

    #
    # Wait until Session Server is registered in naming service
    #
    logger.debug("Start SALOME, elapsed time : %5.1f seconds"% (end_time[4] - init_time[4]))

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
        logger.error("-------------------------------------------------------------")
        logger.error("-- to get an external python interpreter:runSalome --interp=1")
        logger.error("-------------------------------------------------------------")

    logger.debug("additional external python interpreters: {}".format(nbaddi))
    if nbaddi:
        for i in range(nbaddi):
            anInterp=runSalomeCommon.InterpServer(args)
            anInterp.run()

    # set PYTHONINSPECT variable (python interpreter in interactive mode)
    if args['pinter']:
        os.environ["PYTHONINSPECT"]="1"
        try:
            import readline
        except ImportError:
            pass

    return ior_fakens_filename

# -----------------------------------------------------------------------------

def useSalome(args, modules_list, modules_root_dir):
    """
    Launch all SALOME servers requested by args,
    save list of process, give info to user,
    show registered objects in Naming Service.
    """
    global process_id
    ior_fakens_filename = None
    try:
        ior_fakens_filename = startSalome(args, modules_list, modules_root_dir)
    except Exception:
        import traceback
        traceback.print_exc()
        logger.error("--- Error during Salome launch ---")

    # print(process_id)

    from addToKillList import addToKillList
    from killSalomeWithPort import getPiDict

    filedict = getPiDict(args['port'])
    for pid, cmd in list(process_id.items()):
        addToKillList(pid, cmd, args['port'])
        pass

    logger.debug("""
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

    return ior_fakens_filename

def execScript(script_path):
    print('executing', script_path)
    sys.path.insert(0, os.path.realpath(os.path.dirname(script_path)))
    exec(compile(open(script_path).read(), script_path, 'exec'),globals())
    del sys.path[0]

# -----------------------------------------------------------------------------

def main(exeName=None):
    """Salome launch as a main application"""
    keep_env = not os.getenv('SALOME_PLEASE_SETUP_ENVIRONMENT_AS_BEFORE')
    args, modules_list, modules_root_dir = setenv_impl.get_config(exeName=exeName, keepEnvironment=keep_env)
    runSalomeCommon.setVerbose(args[verbosity_nam])

    kill_salome(args)
    # --

    setenv_impl.set_env(args, modules_list, modules_root_dir, keepEnvironment=keep_env)
    ior_fakens_filename = useSalome(args, modules_list, modules_root_dir)
    # Management of -t <script.py>
    toimport = []
    env = os.environ
    if 'gui' in args and 'session_gui' in args:
        if not args['gui'] or not args['session_gui']:
            if 'study_hdf' in args:
                toopen = args['study_hdf']
                if toopen:
                    os.environ["PATH_TO_STUDY_FILE_TO_INITIATE"] = toopen
                    logger.debug("An input Study has been specified {} -> pass it with PATH_TO_STUDY_FILE_TO_INITIATE env var".format(toopen))
            if 'pyscript' in args:
                toimport = args['pyscript']
    from .salomeContextUtils import formatScriptsAndArgs
    from .addToKillList_impl import addToKillList
    command = formatScriptsAndArgs(toimport, escapeSpaces=True)
    if command:
        logger.debug("Launching following shell command : {}".format(str(command)))
        proc = subprocess.Popen(command, shell=True, env = env)
        addToKillList(proc.pid, command)
        res = proc.wait()
        if res: sys.exit(1)
    return args, ior_fakens_filename

# -----------------------------------------------------------------------------

def foreGround(args, ior_fakens_filename):
    # --
    import os
    gui_detected = False
    dt = 0.1
    nbtot = 200
    nb = 0
    if ior_fakens_filename is None:
        logger.warning("No file set to host IOR of the fake naming server")
        return
    if not os.path.exists(ior_fakens_filename):
        logger.warning("No file {} set to host IOR of the fake naming server does not exit !")
        return
    import CORBA
    # do not comment following lines to downcast safely orb.string_to_object(ior_fakens.Resolve("/Kernel/Session").decode())
    from . import Engines
    from . import SALOME_CMOD
    from time import sleep
    orb = CORBA.ORB_init([''], CORBA.ORB_ID)
    ior_fakens = None
    session = None
    while True:
        try:
            ior_fakens = orb.string_to_object(open(ior_fakens_filename).read())
            session = orb.string_to_object(ior_fakens.Resolve("/Kernel/Session").decode())
        except Exception:
            pass
        if ( session is not None ) and (not CORBA.is_nil(session)):
            try:
                os.remove(ior_fakens_filename)
                logger.debug("File {} has been removed".format(ior_fakens_filename))
            except:
                pass
            logger.debug("Session in child process has been found ! yeah ! {}".format(str(session)))
            break
        sleep(dt)
        nb += 1
        logger.debug("Unfortunately Session not found into {} : Sleep and retry. {}/{}".format(ior_fakens_filename,nb,nbtot))
        if nb == nbtot:
            break

    if session is None:
        logger.debug("Couldn't find /Kernel/Session in the child process. Return.")
        return

    nb = 0
    # --
    # Wait until gui is arrived
    # tmax = nbtot * dt
    # --
    session_pid = None
    while 1:
        try:
            status = session.GetStatSession()
            gui_detected = status.activeGUI
            session_pid = session.getPID()
            logger.debug("Process of the session under monitoring {}".format(session_pid))
        except Exception:
            pass
        if gui_detected:
            break
        sleep(dt)
        nb += 1
        if nb == nbtot:
            break
        pass
    # --
    if not gui_detected:
        logger.debug("Couldn't find active GUI in the current session. Return.")
        return
    # --
    from .salome_utils import getPortNumber
    port = getPortNumber()
    # --
    dt = 1.0
    try:
        while 1:
            try:
                status = session.GetStatSession()
                assert status.activeGUI
            except Exception:
                logger.debug("Process of the session under monitoring {} has vanished !".format(session_pid))
                break
            from time import sleep
            sleep(dt)
            pass
        pass
    except KeyboardInterrupt:
        logger.debug("Keyboard requested : killing all process attached to port {}".format(port))
    finally:
        from killSalomeWithPort import killProcessSSL
        killProcessSSL(port,[session_pid])
    return
#

def runSalome():
    args, ior_fakens_filename = main()
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
        foreGround(args, ior_fakens_filename)
        pass
    pass
#
