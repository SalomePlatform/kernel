#!/usr/bin/env python
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

import os, sys, re, socket
#import commands
from server import Server
from Utils_Identity import getShortHostName
from launchConfigureParser import verbose

# -----------------------------------------------------------------------------

class NamingServer(Server):
    #XTERM = ""
    #USER = os.getenv('USER')
    #if USER is None:
    #  USER = 'anonymous'
    #os.system("mkdir -m 777 -p /tmp/logs")
    #LOGDIR = "/tmp/logs/" + USER

    def initNSArgs(self):
        from salome_utils import getLogDir
        upath = getLogDir()
        try:
            os.makedirs(upath, mode=0777)
        except:
            pass

        if verbose(): print "Name Service... ",
        #hname=os.environ["HOST"] #commands.getoutput("hostname")
        if sys.platform == "win32":
          hname = getShortHostName();
        else:
          hname = socket.gethostname();
        #print "hname=",hname

        with open(os.environ["OMNIORB_CONFIG"]) as f:
          ss = re.findall("NameService=corbaname::" + hname + ":\d+", f.read())
          if verbose(): print "ss = ", ss,
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
        except:
          #print "Can't create " + upath
          pass

        #os.system("touch " + upath + "/dummy")
        for fname in os.listdir(upath):
          try:
            os.remove(upath + "/" + fname)
          except:
            pass
        #os.system("rm -f " + upath + "/omninames* " + upath + "/dummy " + upath + "/*.log")

        #aSedCommand="s/.*NameService=corbaname::" + hname + ":\([[:digit:]]*\)/\1/"
        #print "sed command = ", aSedCommand
        #aPort = commands.getoutput("sed -e\"" + aSedCommand + "\"" + os.environ["OMNIORB_CONFIG"])
        #print "port=", aPort
        if sys.platform == "win32":
          #print "start omniNames -start " + aPort + " -logdir " + upath
          self.CMD = ['omniNames', '-start' , aPort , '-nohostname', '-logdir' , os.path.realpath(upath), '-errlog', os.path.realpath(os.path.join(upath,'omniNameErrors.log'))]
          #os.system("start omniNames -start " + aPort + " -logdir " + upath)
        else:
          #self.CMD=['omniNames -start ' , aPort , ' -logdir ' , upath , ' &']
          self.CMD = ['omniNames','-start' , aPort, '-logdir' , upath, '-errlog', upath+'/omniNameErrors.log']
          #os.system("omniNames -start " + aPort + " -logdir " + upath + " &")

        if verbose(): print "... ok"
        if verbose(): print "to list contexts and objects bound into the context with the specified name : showNS "


    def initArgs(self):
        Server.initArgs(self)
        if sys.platform == "win32":
          env_ld_library_path = ['env', 'LD_LIBRARY_PATH=' + os.getenv("PATH")]
        else:
          env_ld_library_path = ['env', 'LD_LIBRARY_PATH=' + os.getenv("LD_LIBRARY_PATH")]
        self.CMD = ['xterm', '-e']+ env_ld_library_path + ['python']
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
