#!/usr/bin/env python

import os, sys, string
process_id = {}

# -----------------------------------------------------------------------------
#
# Definition des classes d'objets pour le lancement des Server CORBA
#

class Server:
    """Generic class for CORBA server launch"""

    def initArgs(self):
        self.PID=None
        self.CMD=[]
        self.ARGS=[]
        if self.args.get('xterm'):
            self.ARGS=['xterm', '-iconic', '-sb', '-sl', '500', '-hold']

    def __init__(self,args):
        self.args=args
        self.initArgs()


    def run(self):
        global process_id
        myargs=self.ARGS
        if self.args.get('xterm'):
            # (Debian) send LD_LIBRARY_PATH to children shells (xterm)
            env_ld_library_path=['env', 'LD_LIBRARY_PATH='
                                 + os.getenv("LD_LIBRARY_PATH")]
            myargs = myargs +['-T']+self.CMD[:1]+['-e'] + env_ld_library_path
        command = myargs + self.CMD
        #print "command = ", command
        if sys.platform == "win32":
          import win32pm
          #cmd_str = "\"" + string.join(command, " ") + "\""
          #print cmd_str
          #pid = win32pm.spawnpid( cmd_str )
          pid = win32pm.spawnpid( string.join(command, " "), '-nc' )
          #pid = win32pm.spawnpid( string.join(command, " ") )
        else:
          #pid = os.spawnvp(os.P_NOWAIT, command[0], command)
          pid=self.daemonize(command)
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
          return int(data)

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
