#!/bin/bash

# --- define port for CORBA naming service

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
            OMNIORB_CONFIG=${HOME}/${APPLI}/.omniORB_${myhost}_${NSPORT}.cfg
            export OMNIORB_CONFIG
	    export NSPORT
            NSHOST=${myhost}
            export NSHOST
            local initref="NameService=corbaname::"`hostname`":$NSPORT"
            #echo "ORBInitRef $initref" > $OMNIORB_CONFIG
            echo "InitRef = $initref" > $OMNIORB_CONFIG
            LAST_RUNNING_CONFIG=${HOME}/${APPLI}/.omniORB_${myhost}_last.cfg
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

