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
    return string.split(socket.gethostname(),'.')[0]

def searchFreePort():
    print "Searching a free port for naming service:",
    NSPORT=2810
    limit=NSPORT
    limit=limit+100
    while 1:
        print "%s "%(NSPORT),
        status = os.system("netstat -ltn | grep -E :%s"%(NSPORT))
        if status:
            home = os.environ['HOME']
            appli=os.environ.get("APPLI")
            if appli is None:
                #Run outside application context
                home=home
            else:
                home='%s/%s'%(home,appli)
            hostname=getShortHostName()
            omniorb_config = '%s/.omniORB_%s_%s.cfg'%(home,hostname, NSPORT)
            os.environ['OMNIORB_CONFIG'] = omniorb_config
            f = open(omniorb_config, "w")
            f.write("ORBInitRef NameService=corbaname::%s:%s\n"%(hostname, NSPORT))
            f.close()
            last_running_config = '%s/.omniORB_%s_last.cfg'%(home, hostname)
            os.environ['LAST_RUNNING_CONFIG'] = last_running_config
            if os.access(last_running_config,os.F_OK):
                os.unlink(last_running_config)
                pass
            os.symlink(omniorb_config,last_running_config)
            #            LAST_RUNNING_CONFIG=${HOME}/${APPLI}/.omniORB_${myhost}_last.cfg
            print "- Ok"
            break
        if NSPORT == limit:
            msg  = ""
            msg += "I Can't find a free port to launch omniNames\n"
            msg += "I suggest you to kill the running servers and try again.\n"
            raise msg
        NSPORT=NSPORT+1
        pass
    os.environ['NSHOST']=hostname
    os.environ['NSPORT']=str(NSPORT)
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
