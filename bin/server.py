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
        if self.args['xterm']:
            self.ARGS=['xterm', '-iconic', '-sb', '-sl', '500', '-hold']

    def __init__(self,args):
        self.args=args
        self.initArgs()


    def run(self):
        global process_id
        myargs=self.ARGS
        if self.args['xterm']:
            # (Debian) send LD_LIBRARY_PATH to children shells (xterm)
            env_ld_library_path=['env', 'LD_LIBRARY_PATH='
                                 + os.getenv("LD_LIBRARY_PATH")]
            myargs = myargs +['-T']+self.CMD[:1]+['-e'] + env_ld_library_path
        command = myargs + self.CMD
        print "command = ", command
	if sys.platform == "win32":
          import win32pm
          #cmd_str = "\"" + string.join(command, " ") + "\""
          #print cmd_str
          #pid = win32pm.spawnpid( cmd_str )
          pid = win32pm.spawnpid( string.join(command, " "), '-nc' )
          #pid = win32pm.spawnpid( string.join(command, " ") )
	else:
          pid = os.spawnvp(os.P_NOWAIT, command[0], command)
        process_id[pid]=self.CMD
        self.PID = pid
