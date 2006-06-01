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

import sys,os
import string

def getNSparams(info=""):
    """
    check environment for omniORB configuration file.
    parse the file to find the line defining naming service  host and port,
    set environment variables NSPORT and NSHOST,
    get host and port,
    if   info==host print host
    elif info==port print host
    else    print 2 strings on stdout on one line: host port
    """
    my_port=""
    my_host=""
    if os.environ.has_key("OMNIORB_CONFIG"):
        file = open(os.environ["OMNIORB_CONFIG"], "r")
        s = file.read()
        while len(s):
            l = string.split(s, ":")
            if string.split(l[0], " ")[0] == "ORBInitRef" or \
               string.split(l[0], " ")[0] == "InitRef" :
                my_port = l[len(l)-1]
                if my_port[-1] == '\n':
                    my_port = my_port[:-1]
                    pass
                my_host = l[len(l)-2]
                break;
                pass
            s = file.read()
            pass
        pass
    if info=='host':
        # keep print, stdout used in shell
        print my_host
        os.environ['NSHOST']=my_host
        return my_host
        pass
    elif info=='port':
        # keep print, stdout used in shell
        print my_port
        os.environ['NSPORT']=my_port
        return my_port
        pass
    else:
        # keep print, stdout used in shell
        print  my_host, my_port
        return my_host, my_port
    pass

# ------------------------------------------------------------------------

if __name__ == "__main__":
    if len(sys.argv) >1:        
        if sys.argv[1]=='host':
            getNSparams('host')
            pass
        elif sys.argv[1]=='port':
            getNSparams('port')
            pass
        else:
            getNSparams('')
            pass
        pass
    else:
        getNSparams('')
        pass
