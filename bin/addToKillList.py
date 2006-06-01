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

import os, sys, pickle, string, signal

########## adds to the kill list of SALOME one more process ##########

def findFileDict():
    if os.environ.has_key('NSPORT'):
        my_port = os.environ['NSPORT']
        pass
    else:
        my_port = 2809
        try:
            file = open(os.environ["OMNIORB_CONFIG"], "r")
            s = file.read()
            while len(s):
                l = string.split(s, ":")
                if string.split(l[0], " ")[0] == "ORBInitRef" or string.split(l[0], " ")[0] == "InitRef" :
                    my_port = int(l[len(l)-1])
                    pass
                s = file.read()
                pass
            pass
        except:
            pass
        pass
    print "myport = ", my_port
    return my_port
    
def addToKillList(command_pid, command):
    my_port = findFileDict()
    from killSalomeWithPort import getPiDict
    filedict=getPiDict(my_port)
    try:
        fpid=open(filedict, 'r')
        process_ids=pickle.load(fpid)
        fpid.close()
    except:
        process_ids=[{}]
        pass
        
    already_in=0
    for process_id in process_ids:
        print process_id
        for pid, cmd in process_id.items():
            #print "see process %s : %s"% (pid, cmd[0])
	    if pid == command_pid:
		already_in=1
                pass
            pass
        pass

    command=(command.split(" "))[0]
    if already_in == 0:
        try:
	    process_ids.append({command_pid: [command]})
    	    fpid=open(filedict,'w')
            pickle.dump(process_ids, fpid)
	    fpid.close()
        except:
	    print "addToKillList: can not add command %s to the kill list"% filedict
    	    pass
	pass
    pass

def killList():
    my_port = findFileDict()
    from killSalomeWithPort import getPiDict
    filedict=getPiDict(my_port)
    try:
        fpid=open(filedict, 'r')
        process_ids=pickle.load(fpid)
        fpid.close()
    except:
        process_ids=[{}]
        pass

    for process_id in process_ids:
        print process_id
        for pid, cmd in process_id.items():
            print "stop process %s : %s"% (pid, cmd[0])
            try:
                os.kill(int(pid),signal.SIGKILL)
            except:
                print "  ------------------ process %s : %s inexistant"% (pid, cmd[0])
                pass
            pass
        pass
    os.remove(filedict)
    pass
  


if __name__ == "__main__":
    print sys.argv
    addToKillList(sys.argv[1], sys.argv[2])
