#!/bin/bash
#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
# --- define port for CORBA naming service
#
searchFreePort() {
    echo -n "Searching for a free port for naming service: "
    NSPORT=2810
    export NSPORT
    local limit=$NSPORT
    let limit=limit+100
    while [ 1 ]
    do
        aRes=`netstat -ltn | grep -E :${NSPORT}`
        if [ -z "$aRes" ]; then
            echo ${NSPORT} - Ok
	    local myhost=`hostname`
            OMNIORB_CONFIG=${HOME}/${APPLI}/USERS/.omniORB_${USER}_${myhost}_${NSPORT}.cfg
            export OMNIORB_CONFIG
	    export NSPORT
            NSHOST=${myhost}
            export NSHOST
            local initref="NameService=corbaname::"`hostname`":$NSPORT"
            #echo "ORBInitRef $initref" > $OMNIORB_CONFIG
            echo "InitRef = $initref" > $OMNIORB_CONFIG
            LAST_RUNNING_CONFIG=${HOME}/${APPLI}/USERS/.omniORB_${USER}_last.cfg
            export LAST_RUNNING_CONFIG
	    rm ${LAST_RUNNING_CONFIG}
            ln -s ${OMNIORB_CONFIG} ${LAST_RUNNING_CONFIG}
            break
        fi
        echo -n "${NSPORT} "
        if [[ $NSPORT -eq $limit ]] ; then
            echo
            echo "Can't find a free port to launch omniNames"
            echo "Try to kill the running servers and then launch SALOME again."
            exit
        fi
        let NSPORT=NSPORT+1
    done
}

