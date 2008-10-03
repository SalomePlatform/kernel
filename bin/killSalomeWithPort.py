#!/usr/bin/env python

# Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
#           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either 
# version 2.1 of the License.
# 
# This library is distributed in the hope that it will be useful 
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

import os, sys, pickle, signal, commands
from launchConfigureParser import verbose

def getPiDict(port,appname='salome',full=True,hidden=True):
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
                            with_hostname=True,
                            with_port=port,
                            with_app=appname.upper())

def appliCleanOmniOrbConfig(port):
    """
    Remove omniorb config files related to the port in SALOME application:
    - ${HOME}/${APPLI}/.omniORB_${HOSTNAME}_${NSPORT}.cfg
    - ${HOME}/${APPLI}/.omniORB_last.cfg
    the last is removed only if the link points to the first file.
    """
    from salome_utils import generateFileName
    home  = os.getenv("HOME")
    appli = os.getenv("APPLI")
    if appli is None:
        #Run outside application context
        pass
    else:
        dir = os.path.join(home, appli)
        omniorb_config      = generateFileName(dir, prefix="omniORB",
                                               extension="cfg",
                                               hidden=True,
                                               with_hostname=True,
                                               with_port=port)
        last_running_config = generateFileName(dir, prefix="omniORB",
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
        pass
    pass

########## kills all salome processes with the given port ##########

def killMyPort(port):
    """
    Kill SALOME session running on the specified port.
    Parameters:
    - port - port number
    """
    # new-style dot-prefixed pidict file
    filedict = getPiDict(port, hidden=True)
    # provide compatibility with old-style pidict file (not dot-prefixed)
    if not os.path.exists(filedict): filedict = getPiDict(port, hidden=False)
    #
    try:
        fpid = open(filedict, 'r')
        #
        from salome_utils import generateFileName
        fpidomniNames = generateFileName(os.path.join('/tmp/logs', os.getenv('USER')),
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
            
if __name__ == "__main__":
    for port in sys.argv[1:]:
        killMyPort(port)
        pass
    pass
