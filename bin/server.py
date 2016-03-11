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

import os, sys, string
from salome_utils import getHostName
process_id = {}

# -----------------------------------------------------------------------------
#
# Definition des classes d'objets pour le lancement des Server CORBA
#

class Server:
    """Generic class for CORBA server launch"""

    server_launch_mode = "daemon"

    def initArgs(self):
        self.PID=None
        self.CMD=[]
        self.ARGS=[]
        if self.args.get('xterm'):
          if sys.platform != "win32":
            self.ARGS=['xterm', '-iconic', '-sb', '-sl', '500', '-hold']
          else:
            self.ARGS=['cmd', '/c', 'start  cmd.exe', '/K']

    def __init__(self,args):
        self.args=args
        self.initArgs()

    @staticmethod
    def set_server_launch_mode(mode):
      if mode == "daemon" or mode == "fork":
        Server.server_launch_mode = mode
      else:
        raise Exception("Unsupported server launch mode: %s" % mode)

    def run(self):
        global process_id
        myargs=self.ARGS
        if self.args.get('xterm'):
            # (Debian) send LD_LIBRARY_PATH to children shells (xterm)
            if sys.platform != "win32":
              env_ld_library_path=['env', 'LD_LIBRARY_PATH='
                                   + os.getenv("LD_LIBRARY_PATH")]
              myargs = myargs +['-T']+self.CMD[:1]+['-e'] + env_ld_library_path
        command = myargs + self.CMD
        #print "command = ", command
        if sys.platform == "win32":
          import subprocess
          pid = subprocess.Popen(command).pid
        elif Server.server_launch_mode == "fork":
          pid = os.spawnvp(os.P_NOWAIT, command[0], command)
        else: # Server launch mode is daemon
          pid=self.daemonize(command)
        if pid is not None:
          #store process pid if it really exists
          process_id[pid]=self.CMD
        self.PID = pid
        return pid

    def daemonize(self,args):
        # to daemonize a process need to do the UNIX double-fork magic
        # see Stevens, "Advanced Programming in the UNIX Environment" for details (ISBN 0201563177)
        # and UNIX Programming FAQ 1.7 How do I get my program to act like a daemon?
        #     http://www.erlenstar.demon.co.uk/unix/faq_2.html#SEC16
        #open a pipe
        c2pread, c2pwrite = os.pipe()
        #do first fork
        pid=os.fork()
        if pid > 0:
          #first parent
          os.close(c2pwrite)
          #receive real pid from child
          data=os.read(c2pread,24) #read 24 bytes
          os.waitpid(pid,0) #remove zombie
          os.close(c2pread)
          # return : first parent
          childpid=int(data)
          if childpid==-1:
            return None
          try:
            os.kill(childpid,0)
            return childpid
          except:
            return None

        #first child
        # decouple from parent environment
        os.setsid()
        os.close(c2pread)

        # do second fork : second child not a session leader
        try:
          pid = os.fork()
          if pid > 0:
            #send real pid to parent
            os.write(c2pwrite,"%d" % pid)
            os.close(c2pwrite)
            # exit from second parent
            os._exit(0)
        except OSError, e:
          print >>sys.stderr, "fork #2 failed: %d (%s)" % (e.errno, e.strerror)
          os.write(c2pwrite,"-1")
          os.close(c2pwrite)
          sys.exit(1)

        #I am a daemon
        os.close(0) #close stdin
        os.open("/dev/null", os.O_RDWR)  # redirect standard input (0) to /dev/null
        try:
          os.execvp(args[0], args)
        except OSError, e:
          print >>sys.stderr, "(%s) launch failed: %d (%s)" % (args[0],e.errno, e.strerror)
          os._exit(127)
