#!/usr/bin/env python

import socket
import os
import sys
import shutil

#####################################################################
def findFreePort(startPort, step) :

    currentPort = startPort;
    if step < 1:
        step = 1;

    serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM);

    portFound = 0;
    while (portFound != 1 and currentPort < 65536):
        try :
            serversocket.bind((socket.gethostname(), currentPort));
            portFound = 1;
            print str(currentPort) + ": " + "OK";
#            serversocket.shutdown(0);
#            serversocket.close();
        except Exception, inst:
            print str(currentPort) + ": " + str(inst.args);
            currentPort = currentPort+step;
    
    
    
    
    if (portFound != 1) :
        currentPort = -1;
    return currentPort;
#####################################################################



## 1. Generate config file for omniORB:
# content:
# InitRef = NameService=corbaname::[HOST_NAME]:[PORT_NUMBER]

freePort = findFreePort(2810, 1);
hostName = socket.gethostname();

if (len(sys.argv) > 1) and sys.argv[1] == "-nothing" :
    print "port:" + str(freePort);
    sys.exit(0)
    

        

if freePort < 0 :
    print
    print "Can't find a free port to launch omniNames"
    print "Try to kill the running servers and then launch SALOME again."
    print
    sys.exit(0)
    
else :
    omniCfgFileContent = "InitRef = NameService=corbaname::" + str(hostName) + ":" + str(freePort);
    omniCfgFilePath = os.environ.get('HOME') + "/" + ".omniORB_" + str(hostName) + "_" + str(freePort) + ".cfg";

    omni_file = file(omniCfgFilePath,'w+');
    omni_file.write(omniCfgFileContent);
    omni_file.write("\n");
    omni_file.close();

    if (len(sys.argv) > 1) and sys.argv[1] == "--save-config" :
        omniCfgCurrent = os.environ.get('HOME') + "/.omniORB_current.cfg";
        if os.name == "posix" :
            #make a symbolic link
            symlink(omniCfgFilePath, omniCfgCurrent);
        else :
            #copy the file
            shutil.copy(omniCfgFilePath, omniCfgCurrent);
            
        omniCfgFilePath = omniCfgCurrent;
            
    
    

    ## 2. Set the OMNIORB_CONFIG environment variable
    os.environ['OMNIORB_CONFIG']=omniCfgFilePath;


    ## 3. execute the file runSalome.py
    runSalomePyFilePath = os.environ.get('KERNEL_ROOT_DIR') + "/bin/salome/runSalome.py";

    if os.path.isfile(runSalomePyFilePath) :
        execfile(runSalomePyFilePath);
        
