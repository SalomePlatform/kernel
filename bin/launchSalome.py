#! /usr/bin/env python
#  -*- coding: iso-8859-1 -*-
# Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
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
    from ORBConfigFile import writeORBConfigFile
    writeORBConfigFile(os.environ.get('HOME'), hostName, freePort)

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
