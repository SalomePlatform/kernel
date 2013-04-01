#! /usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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
from salome_utils import verbose
import Utils_Identity
import salome_utils

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
    from salome_utils import generateFileName, getTmpDir
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
            dir = getTmpDir()
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
    - ${HOME}/${APPLI}/USERS/.omniORB_${USER}_${HOSTNAME}_${NSPORT}.cfg
    - ${HOME}/${APPLI}/USERS/.omniORB_${USER}_last.cfg
    the last is removed only if the link points to the first file.
    """
    from salome_utils import generateFileName, getUserName
    home  = os.getenv("HOME")
    appli = os.getenv("APPLI")
    if appli is None:
        #Run outside application context
        pass
    else:
        dir = os.path.join(os.path.realpath(home), appli,"USERS")
        omniorb_config      = generateFileName(dir, prefix="omniORB",
                                               extension="cfg",
                                               hidden=True,
                                               with_username=True,
                                               with_hostname=True,
                                               with_port=port)
        last_running_config = generateFileName(dir, prefix="omniORB",
                                               with_username=True,
                                               suffix="last",
                                               extension="cfg",
                                               hidden=True)
        if os.access(last_running_config,os.F_OK):
            pointedPath = os.readlink(last_running_config)
            if pointedPath[0] != '/':
                pointedPath=os.path.join(os.path.dirname(last_running_config), pointedPath)
            if pointedPath == omniorb_config:
                os.unlink(last_running_config)
                pass
            pass
        if os.access(omniorb_config,os.F_OK):
            os.remove(omniorb_config)
            pass

        if os.path.lexists(last_running_config):return

        #try to relink last.cfg to an existing config file if any
        files = glob.glob(os.path.join(os.environ["HOME"],Utils_Identity.getapplipath(),
                                       "USERS",".omniORB_"+getUserName()+"_*.cfg"))
        current_config=None
        current=0
        for f in files:
          stat=os.stat(f)
          if stat.st_atime > current:
            current=stat.st_atime
            current_config=f
        if current_config:
          os.symlink(os.path.normpath(current_config), last_running_config)

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

    from salome_utils import generateFileName

    # set OMNIORB_CONFIG variable to the proper file
    home  = os.getenv("HOME")
    appli = os.getenv("APPLI")
    kwargs = {}
    if appli is not None:
        home = os.path.join(os.path.realpath(home), appli,"USERS")
        kwargs["with_username"]=True
        pass
    omniorb_config = generateFileName(home, prefix="omniORB",
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
        lcc = LifeCycleCORBA(orb)
        lcc.shutdownServers()
        # give some time to shutdown to complete
        time.sleep(1)
        # shutdown omniNames and notifd
        if cleanup:
            lcc.killOmniNames()
            time.sleep(1)
            pass
        pass
    except:
        pass
    pass

def killMyPort(port):
    """
    Kill SALOME session running on the specified port.
    Parameters:
    - port - port number
    """
    from salome_utils import getShortHostName, getHostName

    # try to shutdown session nomally
    import threading, time
    threading.Thread(target=shutdownMyPort, args=(port,False)).start()
    time.sleep(3) # wait a little, then kill processes (should be done if shutdown procedure hangs up)

    # new-style dot-prefixed pidict file
    filedict = getPiDict(port, hidden=True)
    # provide compatibility with old-style pidict file (not dot-prefixed)
    if not os.path.exists(filedict): filedict = getPiDict(port, hidden=False)
    # provide compatibility with old-style pidict file (short hostname)
    if not os.path.exists(filedict): filedict = getPiDict(port, hidden=True,  hostname=getShortHostName())
    # provide compatibility with old-style pidict file (not dot-prefixed, short hostname)
    if not os.path.exists(filedict): filedict = getPiDict(port, hidden=False, hostname=getShortHostName())
    # provide compatibility with old-style pidict file (long hostname)
    if not os.path.exists(filedict): filedict = getPiDict(port, hidden=True,  hostname=getHostName())
    # provide compatibility with old-style pidict file (not dot-prefixed, long hostname)
    if not os.path.exists(filedict): filedict = getPiDict(port, hidden=False, hostname=getHostName())
    #
    try:
        fpid = open(filedict, 'r')
        #
        from salome_utils import generateFileName
        if sys.platform == "win32":
            username = os.getenv( "USERNAME" )
        else:
            username = os.getenv('USER')
        path = os.path.join('/tmp/logs', username)
        fpidomniNames = generateFileName(path,
                                         prefix="",
                                         suffix="Pid_omniNames",
                                         extension="log",
                                         with_port=port)
        if not sys.platform == 'win32':
            cmd = 'pid=`ps -eo pid,command | egrep "[0-9] omniNames -start %s"` ; echo $pid > %s' % ( str(port), fpidomniNames )
            a = os.system(cmd)
            pass
        try:
            fpidomniNamesFile = open(fpidomniNames)
            lines = fpidomniNamesFile.readlines()
            fpidomniNamesFile.close()
            os.remove(fpidomniNames)
            for l in lines:
                try:
                    pidfield = l.split()[0] # pid should be at the first position
                    if sys.platform == "win32":
                        import win32pm
                        if verbose(): print 'stop process '+pidfield+' : omniNames'
                        win32pm.killpid(int(pidfield),0)
                    else:
                        if verbose(): print 'stop process '+pidfield+' : omniNames'
                        os.kill(int(pidfield),signal.SIGKILL)
                        pass
                    pass
                except:
                    pass
                pass
            pass
        except:
            pass
        #
        try:
            process_ids=pickle.load(fpid)
            fpid.close()
            for process_id in process_ids:
                for pid, cmd in process_id.items():
                    if verbose(): print "stop process %s : %s"% (pid, cmd[0])
                    try:
                        if sys.platform == "win32":
                            import win32pm
                            win32pm.killpid(int(pid),0)
                        else:
                            os.kill(int(pid),signal.SIGKILL)
                            pass
                        pass
                    except:
                        if verbose(): print "  ------------------ process %s : %s not found"% (pid, cmd[0])
                        pass
                    pass # for pid, cmd ...
                pass # for process_id ...
            pass # try...
        except:
            pass
        #
        os.remove(filedict)
        cmd='ps -eo pid,command | egrep "[0-9] omniNames -start '+str(port)+'" | sed -e "s%[^0-9]*\([0-9]*\) .*%\\1%g"'
        pid = commands.getoutput(cmd)
        a = ""
        while pid and len(a.split()) < 2:
            a = commands.getoutput("kill -9 " + pid)
            pid = commands.getoutput(cmd)
            #print pid
            pass
        pass
    except:
        print "Cannot find or open SALOME PIDs file for port", port
        pass
    #
    appliCleanOmniOrbConfig(port)
    pass

def killNotifdAndClean(port):
    """
    Kill notifd daemon and clean application running on the specified port.
    Parameters:
    - port - port number
    """
    try:
      filedict=getPiDict(port)
      f=open(filedict, 'r')
      pids=pickle.load(f)
      for d in pids:
        for pid,process in d.items():
          if 'notifd' in process:
            cmd='kill -9 %d'% pid
            os.system(cmd)
      os.remove(filedict)
    except:
      #import traceback
      #traceback.print_exc()
      pass

    appliCleanOmniOrbConfig(port)

def killMyPortSpy(pid, port):
    dt = 1.0
    while 1:
        if sys.platform == "win32":
            from win32pm import killpid
            if killpid(int(pid), 0) != 0:
                return
        else:
            from os import kill
            try:
                kill(int(pid), 0)
            except OSError, e:
                if e.errno != 3:
                    return
                break
            pass
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
        import SALOME
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
    for port in sys.argv[1:]:
        killMyPort(port)
        pass
    pass
