#!/usr/bin/env python
import os, sys, pickle, signal, commands

def getPiDict(port,appname='salome',full=True):
    filedict = []
    filedict.append( os.getenv('USER') )          # user name
    filedict.append( os.getenv('HOSTNAME') )      # host name
    filedict.append( str(port) )                  # port number
    filedict.append( appname.upper() )            # application name
    filedict.append( 'pidict' )                   # constant part

    filedict = '_'.join(filedict)
    if full:
        filedict = os.getenv("HOME") + '/' + filedict
    return filedict

########## kills all salome processes with the given port ##########
def killMyPort(port):
    filedict=getPiDict(port)
    found = 0
    try:
        fpid=open(filedict, 'r')
        found = 1
    except:
        print "le fichier %s des process SALOME n'est pas accessible"% filedict
        pass
        
    if found:
        a = os.system("pid=`ps -eo pid,command | egrep \"[0-9] omniNames -start "+str(port)+"\" | sed -e \"s%[^0-9]*\([0-9]*\) .*%\\1%g\"`; kill -9 $pid >& /dev/null")
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
	a = ""
        while pid != "" and len(a.split(" ")) < 2:
            a = commands.getoutput("pid=`ps -eo pid,command | egrep \"[0-9] omniNames -start "+str(port)+"\" | sed -e \"s%[^0-9]*\([0-9]*\) .*%\\1%g\"`; kill -9 $pid")
            pid = commands.getoutput("ps -eo pid,command | egrep \"[0-9] omniNames -start "+str(port)+"\" | sed -e \"s%[^0-9]*\([0-9]*\) .*%\\1%g\"")
	    print pid
              

if __name__ == "__main__":
    for port in sys.argv[1:]:
        killMyPort(port)
