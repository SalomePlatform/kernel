#!/usr/bin/env python
import os, sys, pickle, signal, commands

########## kills all salome processes with the given port ##########
def killMyPort(port):
    filedict='/tmp/'+os.getenv('USER')+"_"+port+'_SALOME_pidict'
    found = 0
    try:
        fpid=open(filedict, 'r')
        found = 1
    except:
        print "le fichier %s des process SALOME n'est pas accessible"% filedict
        pass
        
    if found:
        a = os.system("pid=`ps -eo pid,command | egrep \"[0-9] omniNames -start "+str(port)+"\" | sed -e \"s%[^0-9]*\([0-9]*\) .*%\\1%g\"`; kill -9 $pid")
        try:
            process_ids=pickle.load(fpid)
            fpid.close()
            for process_id in process_ids:

                for pid, cmd in process_id.items():
                    print "stop process %s : %s"% (pid, cmd[0])
                    try:
                        os.kill(int(pid),signal.SIGKILL)
                    except:
                        print "  ------------------ process %s : %s inexistant"% (pid, cmd[0])
                        pass
                pass
        except:
            pass
        os.remove(filedict)
        pid = commands.getoutput("ps -eo pid,command | egrep \"[0-9] omniNames -start "+str(port)+"\" | sed -e \"s%[^0-9]*\([0-9]*\) .*%\\1%g\"")
        while pid != "":
            a = os.system("pid=`ps -eo pid,command | egrep \"[0-9] omniNames -start "+str(port)+"\" | sed -e \"s%[^0-9]*\([0-9]*\) .*%\\1%g\"`; kill -9 $pid")
            pid = commands.getoutput("ps -eo pid,command | egrep \"[0-9] omniNames -start "+str(port)+"\" | sed -e \"s%[^0-9]*\([0-9]*\) .*%\\1%g\"")
              

if __name__ == "__main__":
    for port in sys.argv[1:]:
        killMyPort(port)
