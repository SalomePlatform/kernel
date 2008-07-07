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

import os
import sys
import string
import socket

_session = None

def startSession(modules=[]):
    global _session
    if _session: return
    searchFreePort()
    _session = SalomeSession(modules)
    return

def getShortHostName():
    """
    gives Hostname without domain extension.
    SALOME naming service needs short Hostnames (without domain extension).
    HOSTNAME is not allways defined in environment,
    socket.gethostname() gives short or complete Hostname, depending on
    defined aliases.
    """
    from salome_utilities import getShortHostName
    return getShortHostName()

def searchFreePort():
    """
    Search free port for SALOME session.
    Returns first found free port number.
    """
    print "Searching a free port for naming service:",
    from salome_utilities import generateFileName, getHostName
    hostname = getHostName()
    NSPORT = 2810
    limit  = NSPORT+100
    while 1:
        print "%s "%(NSPORT),
        status = os.system("netstat -ltn | grep -E :%s"%(NSPORT))
        if status:
            home  = os.getenv("HOME")
            appli = os.getenv("APPLI")
            if appli is not None: home = os.path.join(home, appli)
            omniorb_config = generateFileName(home, prefix="omniORB",
                                              extension="cfg",
                                              hidden=True,
                                              with_hostname=True,
                                              with_port=NSPORT)
            f = open(omniorb_config, "w")
            f.write("ORBInitRef NameService=corbaname::%s:%s\n"%(hostname, NSPORT))
            f.close()
            os.environ['OMNIORB_CONFIG'] = omniorb_config
            last_running_config = generateFileName(home, prefix="omniORB",
                                                   suffix="last",
                                                   extension="cfg",
                                                   hidden=True)
            os.environ['LAST_RUNNING_CONFIG'] = last_running_config
            if os.access(last_running_config,os.F_OK):
                os.unlink(last_running_config)
                pass
            os.symlink(omniorb_config,last_running_config)
            print "- Ok"
            break
        if NSPORT == limit:
            msg  = ""
            msg += "Can not find a free port to launch omniNames\n"
            msg += "Kill the running servers and try again.\n"
            raise msg
        NSPORT = NSPORT+1
        pass
    os.environ['NSHOST'] = hostname
    os.environ['NSPORT'] = str(NSPORT)
    return NSPORT


class SalomeSession(object):
    import runSalome
    import killSalomeWithPort
    import killSalome
    def __init__(self, modules):
        import runSalome
        sys.argv  = ["dummy.py"]
        sys.argv += ["--terminal"]
        if modules:
            sys.argv += ['--modules=%s'%(",".join(modules))]
            pass
        runSalome.clt, runSalome.args = runSalome.main()
        import salome
        salome.salome_init()
        return
    def __del__(self):
        import runSalome
        runSalome.killLocalPort()
        return
    pass
