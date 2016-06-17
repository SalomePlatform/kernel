#! /usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

## \file killSalomeWithPort.py
#  Stop all %SALOME servers from given sessions by killing them
#
#  The sessions are indicated by their ports on the command line as in :
#  \code
#  killSalomeWithPort.py 2811 2815
#  \endcode
#

import os, sys, pickle, signal, commands,glob
import subprocess
import shlex
from salome_utils import verbose


def getPiDict(port,appname='salome',full=True,hidden=True,hostname=None):
    """
    Get file with list of SALOME processes.
    This file is located in the user's home directory
    and named .<user>_<host>_<port>_SALOME_pidict
    where
    <user> is user name
    <host> is host name
    <port> is port number

    Parameters:
    - port    : port number
    - appname : application name (default is 'SALOME')
    - full    : if True, full path to the file is returned, otherwise only file name is returned
    - hidden  : if True, file name is prefixed with . (dot) symbol; this internal parameter is used
    to support compatibility with older versions of SALOME
    """
    # bug fix: ensure port is an integer
    # Note: this function is also called with port='#####' !!!
    try:
        port = int(port)
    except:
        pass

    from salome_utils import generateFileName, getLogDir
    dir = ""
    if not hostname:
        hostname = os.getenv("NSHOST")
        if hostname: hostname = hostname.split(".")[0]
        pass
    if full:
        # full path to the pidict file is requested
        if hidden:
            # new-style dot-prefixed pidict files
            # are in the system-dependant temporary diretory
            dir = getLogDir()
        else:
            # old-style non-dot-prefixed pidict files
            # are in the user's home directory
            dir = os.getenv("HOME")
            pass
        pass

    return generateFileName(dir,
                            suffix="pidict",
                            hidden=hidden,
                            with_username=True,
                            with_hostname=hostname or True,
                            with_port=port,
                            with_app=appname.upper())

def appliCleanOmniOrbConfig(port):
    """
    Remove omniorb config files related to the port in SALOME application:
    - ${OMNIORB_USER_PATH}/.omniORB_${USER}_${HOSTNAME}_${NSPORT}.cfg
    - ${OMNIORB_USER_PATH}/.omniORB_${USER}_last.cfg
    the last is removed only if the link points to the first file.
    """
    if verbose():
        print "clean OmniOrb config for port %s"%port

    from salome_utils import generateFileName, getUserName
    omniorbUserPath = os.getenv("OMNIORB_USER_PATH")
    if omniorbUserPath is None:
        #Run outside application context
        pass
    else:
        omniorb_config      = generateFileName(omniorbUserPath, prefix="omniORB",
                                               extension="cfg",
                                               hidden=True,
                                               with_username=True,
                                               with_hostname=True,
                                               with_port=port)
        last_running_config = generateFileName(omniorbUserPath, prefix="omniORB",
                                               with_username=True,
                                               suffix="last",
                                               extension="cfg",
                                               hidden=True)
        if os.access(last_running_config,os.F_OK):
            if not sys.platform == 'win32':
                pointedPath = os.readlink(last_running_config)
                if pointedPath[0] != '/':
                    pointedPath=os.path.join(os.path.dirname(last_running_config), pointedPath)
                    pass
                if pointedPath == omniorb_config:
                    os.unlink(last_running_config)
                    pass
                pass
            else:
                os.remove(last_running_config)
                pass
            pass

        if os.access(omniorb_config,os.F_OK):
            os.remove(omniorb_config)
            pass

        if os.path.lexists(last_running_config):return

        #try to relink last.cfg to an existing config file if any
        files = glob.glob(os.path.join(omniorbUserPath,".omniORB_"+getUserName()+"_*.cfg"))
        current_config=None
        current=0
        for f in files:
          stat=os.stat(f)
          if stat.st_atime > current:
            current=stat.st_atime
            current_config=f
        if current_config:
          if sys.platform == "win32":
            import shutil
            shutil.copyfile(os.path.normpath(current_config), last_running_config)
            pass
          else:
            os.symlink(os.path.normpath(current_config), last_running_config)
            pass
          pass
        pass
    pass

########## kills all salome processes with the given port ##########

def shutdownMyPort(port, cleanup=True):
    """
    Shutdown SALOME session running on the specified port.
    Parameters:
    - port - port number
    """
    if not port: return
    # bug fix: ensure port is an integer
    port = int(port)

    try:
        from PortManager import releasePort
        releasePort(port)
    except ImportError:
        pass

    from salome_utils import generateFileName

    # set OMNIORB_CONFIG variable to the proper file
    omniorbUserPath = os.getenv("OMNIORB_USER_PATH")
    kwargs = {}
    if omniorbUserPath is not None:
        kwargs["with_username"]=True
    else:
        omniorbUserPath = os.path.realpath(os.path.expanduser('~'))
    omniorb_config = generateFileName(omniorbUserPath, prefix="omniORB",
                                      extension="cfg",
                                      hidden=True,
                                      with_hostname=True,
                                      with_port=port,
                                      **kwargs)
    os.environ['OMNIORB_CONFIG'] = omniorb_config
    os.environ['NSPORT'] = str(port)

    # give the chance to the servers to shutdown properly
    try:
        import time
        from omniORB import CORBA

        from LifeCycleCORBA import LifeCycleCORBA
        # shutdown all
        orb = CORBA.ORB_init([''], CORBA.ORB_ID)
        lcc = LifeCycleCORBA(orb) # see (1)
        print "Terminating SALOME on port %s..."%(port)
        lcc.shutdownServers()
        # give some time to shutdown to complete
        time.sleep(1)
        # shutdown omniNames
        if cleanup:
            lcc.killOmniNames()
            time.sleep(1)
            pass
        pass
    except:
        pass
    exit(0) # see (1)
    pass
# (1) If --shutdown-servers option is set to 1, session close procedure is
# called twice: first explicitely by salome command, second by automatic
# atexit to handle Ctrl-C. During second call, LCC does not exist anymore and
# a RuntimeError is raised; we explicitely exit this function with code 0 to
# prevent parent thread from crashing.

def __killMyPort(port, filedict):
    # bug fix: ensure port is an integer
    if port:
        port = int(port)

    try:
        with open(filedict, 'r') as fpid:
            process_ids=pickle.load(fpid)
            for process_id in process_ids:
                for pid, cmd in process_id.items():
                    if verbose(): print "stop process %s : %s"% (pid, cmd[0])
                    try:
                        from salome_utils import killpid
                        killpid(int(pid))
                    except:
                        if verbose(): print "  ------------------ process %s : %s not found"% (pid, cmd[0])
                        pass
                    pass # for pid ...
                pass # for process_id ...
            # end with
    except:
        print "Cannot find or open SALOME PIDs file for port", port
        pass
    os.remove(filedict)
    pass
#

def __guessPiDictFilename(port):
    from salome_utils import getShortHostName, getHostName
    filedicts = [
        # new-style dot-prefixed pidict file
        getPiDict(port, hidden=True),
        # provide compatibility with old-style pidict file (not dot-prefixed)
        getPiDict(port, hidden=False),
        # provide compatibility with old-style pidict file (short hostname)
        getPiDict(port, hidden=True, hostname=getShortHostName()),
        # provide compatibility with old-style pidict file (not dot-prefixed, short hostname
        getPiDict(port, hidden=False, hostname=getShortHostName()),
        # provide compatibility with old-style pidict file (long hostname)
        getPiDict(port, hidden=True, hostname=getHostName()),
        # provide compatibility with old-style pidict file (not dot-prefixed, long hostname)
        getPiDict(port, hidden=False, hostname=getHostName())
        ]

    log_msg = ""
    for filedict in filedicts:
        log_msg += "Trying %s..."%filedict
        if os.path.exists(filedict):
            log_msg += "   ... OK\n"
            break
        else:
            log_msg += "   ... not found\n"

    if verbose():
        print log_msg

    return filedict
#

def killMyPort(port):
    """
    Kill SALOME session running on the specified port.
    Parameters:
    - port - port number
    """
    # bug fix: ensure port is an integer
    if port:
        port = int(port)

    try:
        import PortManager # do not remove! Test for PortManager availability!
        filedict = getPiDict(port)
        if not os.path.isfile(filedict): # removed by previous call, see (1)
            if verbose():
                print "SALOME on port %s: already removed by previous call"%port
            # Remove port from PortManager config file
            try:
                from PortManager import releasePort
                if verbose():
                    print "Removing port from PortManager configuration file"
                releasePort(port)
            except ImportError:
                pass
            return
    except:
        pass

    # try to shutdown session normally
    import threading, time
    threading.Thread(target=shutdownMyPort, args=(port,True)).start()
    time.sleep(3) # wait a little, then kill processes (should be done if shutdown procedure hangs up)

    try:
        import PortManager # do not remove! Test for PortManager availability!
        filedict = getPiDict(port)
        #filedict = __guessPiDictFilename(port)
        import glob
        all_files = glob.glob("%s*"%filedict)
        for f in all_files:
            __killMyPort(port, f)
    except ImportError:
        filedict = __guessPiDictFilename(port)
        __killMyPort(port, filedict)
    #

    appliCleanOmniOrbConfig(port)
    pass

def cleanApplication(port):
    """
    Clean application running on the specified port.
    Parameters:
    - port - port number
    """
    # bug fix: ensure port is an integer
    if port:
        port = int(port)

    try:
        filedict=getPiDict(port)
        os.remove(filedict)
    except:
      #import traceback
      #traceback.print_exc()
      pass

    appliCleanOmniOrbConfig(port)
    pass

def killMyPortSpy(pid, port):
    dt = 1.0
    while 1:
        from salome_utils import killpid
        ret = killpid(int(pid), 0)
        if ret == 0:
            break
        elif ret < 0:
            return
        from time import sleep
        sleep(dt)
        pass
    filedict = getPiDict(port, hidden=True)
    if not os.path.exists(filedict):
        return
    try:
        import omniORB
        orb = omniORB.CORBA.ORB_init(sys.argv, omniORB.CORBA.ORB_ID)
        import SALOME_NamingServicePy
        ns = SALOME_NamingServicePy.SALOME_NamingServicePy_i(orb)
        import SALOME #@UnresolvedImport @UnusedImport
        session = ns.Resolve("/Kernel/Session")
        assert session
    except:
        return
    try:
        status = session.GetStatSession()
    except:
        # -- session is in naming service but has crash
        status = None
        pass
    if status:
        if not status.activeGUI:
            return
        pass
    killMyPort(port)
    return

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print "Usage: "
        print "  %s <port>" % os.path.basename(sys.argv[0])
        print
        print "Kills SALOME session running on specified <port>."
        sys.exit(1)
        pass
    if sys.argv[1] == "--spy":
        if len(sys.argv) > 3:
            pid = sys.argv[2]
            port = sys.argv[3]
            killMyPortSpy(pid, port)
            pass
        sys.exit(0)
        pass
    try:
        from salomeContextUtils import setOmniOrbUserPath #@UnresolvedImport
        setOmniOrbUserPath()
    except Exception, e:
        print e
        sys.exit(1)
    for port in sys.argv[1:]:
        killMyPort(port)
        pass
    pass
