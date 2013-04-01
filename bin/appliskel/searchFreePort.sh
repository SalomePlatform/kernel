#!/bin/bash
# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

# --- define port for CORBA naming service

DEFAULT=default

# call: searchFreePort [ save [test] ]
searchFreePort() {
    # if not specified by optional first parameter, savemode is set to default
    savemode=${1:-$DEFAULT}
    # if not specified by optional second parameter, testmode is set to default
    testmode=${2:-$DEFAULT}

    echo -n "Searching for a free port for naming service: "
    export NSPORT=2810
    local limit=$NSPORT
    let limit=limit+100

    while [ 1 ]
    do
        aRes=`netstat -ltn | grep -E :${NSPORT}`
        if [ -z "$aRes" ]; then
            echo ${NSPORT} - Ok
            export NSPORT
            export NSHOST=`hostname`

            RETURN_VALUES=$(${KERNEL_ROOT_DIR}/bin/salome/envSalome.py python ${KERNEL_ROOT_DIR}/bin/salome/ORBConfigFile.py ${HOME}/${APPLI}/USERS ${NSHOST} ${NSPORT} with_username=${USER})
            export OMNIORB_CONFIG=$(echo ${RETURN_VALUES} | cut -d' ' -f1)

            if [ "$savemode" = save ]
            then
                if [ "$testmode" = test ]
                then
                    export LAST_RUNNING_CONFIG=${HOME}/${APPLI}/USERS/.omniORB_${USER}_${NSHOST}_test.cfg
                else
                    export LAST_RUNNING_CONFIG=${HOME}/${APPLI}/USERS/.omniORB_${USER}_last.cfg
                fi

                rm ${LAST_RUNNING_CONFIG}
                ln -s ${OMNIORB_CONFIG} ${LAST_RUNNING_CONFIG}
            fi

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
