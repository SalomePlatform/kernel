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

import os, sys, re, socket
#import commands
from salome.kernel.server import Server
from salome.kernel.salome_utils import getHostName, makeDir
from salome.kernel.launchConfigureParser import verbose

# -----------------------------------------------------------------------------

class NamingServer(Server):
    #XTERM = ""
    #USER = os.getenv('USER')
    #if USER is None:
    #  USER = 'anonymous'
    #os.system("mkdir -m 777 -p /tmp/logs")
    #LOGDIR = "/tmp/logs/" + USER

    def initNSArgs(self):
        from salome.kernel.salome_utils import getLogDir
        upath = getLogDir()
        makeDir(upath)

        if verbose(): print("Name Service... ", end =' ')
        hname = getHostName()

        with open(os.environ["OMNIORB_CONFIG"]) as f:
          ss = re.findall("NameService=corbaname::" + hname + ":\d+", f.read())
          if verbose(): print("ss = ", ss, end=' ')
        sl = ss[0]
        ll = sl.split(':')
        aPort = ll[-1]
        #aPort=(ss.join().split(':'))[2];
        #aPort=re.findall("\d+", ss[0])[0]

        # \begin{E.A.}
        # put the log files of omniNames in different directory with port reference,
        # it is cleaner on linux and it is a fix for salome since it is impossible to
        # remove the log files if the corresponding omniNames has not been killed.
        # \end{E.A.}

        upath = os.path.join(upath, "omniNames_%s"%(aPort))
        try:
          os.mkdir(upath)
        except Exception:
          # print("Can't create " + upath)
          pass

        #os.system("touch " + upath + "/dummy")
        for fname in os.listdir(upath):
          try:
            os.remove(upath + "/" + fname)
          except Exception:
            pass
        #os.system("rm -f " + upath + "/omninames* " + upath + "/dummy " + upath + "/*.log")

        #aSedCommand="s/.*NameService=corbaname::" + hname + ":\([[:digit:]]*\)/\1/"
        # print("sed command = ", aSedCommand)
        #aPort = commands.getoutput("sed -e\"" + aSedCommand + "\"" + os.environ["OMNIORB_CONFIG"])
        # print("port=", aPort)
        if sys.platform == "win32":
          # print("start omniNames -start " + aPort + " -logdir " + upath)
          self.CMD = ['omniNames', '-start' , aPort , '-nohostname', '-logdir' , os.path.realpath(upath), '-errlog', os.path.realpath(os.path.join(upath,'omniNameErrors.log'))]
          #os.system("start omniNames -start " + aPort + " -logdir " + upath)
        else:
          # get ip address on default interface (for instance eth0) to limit listening on this interface (cyber security request)
          from subprocess import check_output
          ips = check_output(['hostname', '--all-ip-addresses'])
          if ips.strip():
            ipDefault = ips.split()[0].decode()
          else:
            ip = check_output(['hostname', '--ip-address'])
            ipDefault = ip.split()[-1].decode()
          self.CMD = ['omniNames','-start' , aPort]
          self.CMD += ['-logdir' , upath, '-errlog', upath+'/omniNameErrors.log']

        if verbose(): print("... ok")
        if verbose(): print("to list contexts and objects bound into the context with the specified name : showNS ")


    def initArgs(self):
        ld_path = "LD_LIBRARY_PATH"
        Server.initArgs(self)
        if sys.platform == "win32":
          env_ld_library_path = ['env', f'{ld_path}=' + os.getenv("PATH")]
        elif sys.platform == "darwin":
          env_ld_library_path = ['env', 'DYLD_LIBRARY_PATH=' + os.getenv("DYLD_LIBRARY_PATH"), 'DYLD_FALLBACK_LIBRARY_PATH=' + os.getenv("DYLD_FALLBACK_LIBRARY_PATH")]
        else:
          env_ld_library_path = []
          if ld_path in os.environ:
            env_ld_library_path += [ 'env', f'{ld_path}=' + os.getenv( ld_path ) ]
        self.CMD = ['xterm', '-e'] + env_ld_library_path + ['python3']
        self.initNSArgs()

# In LifeCycleCORBA, FactoryServer is started with rsh on the requested
#    computer if this Container does not exist. Default is localhost.
#    Others Containers are started with start_impl method of FactoryServer Container.
# For using rsh it is necessary to have in the ${HOME} directory a .rhosts file
# Warning : on RedHat the file /etc/hosts contains by default a line like :
# 127.0.0.1               bordolex bordolex.paris1.matra-dtv.fr localhost.localdomain localhost
#   (bordolex is the station name). omniNames on bordolex will be accessible from other
#   computers only if the computer name is removed on that line like :
#   127.0.0.1               bordolex.paris1.matra-dtv.fr localhost.localdomain localhost

# To start dynamically Containers on several computers you need to
# put in the ${OMNIORB_CONFIG} file a computer name instead of "localhost"
# example : ORBInitRef NameService=corbaname::dm2s0017

# If you need to use several omniNames running on the same computer, you have to :
#1. put in your ${OMNIORB_CONFIG} file a computer name and port number
# example : ORBInitRef NameService=corbaname::dm2s0017:1515
#2. start omninames with this port number like in orbmodile.py
# example : omniNames -start 1515 -logdir ${BaseDir}/logs/${Username} &
