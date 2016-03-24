#!/bin/bash

# Copyright (C) 2015-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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

echo "This is a script that can be run concurrently."
echo "It takes as single argument the number of concurrent executions:"
echo "Usage: " $0 "<nb_execution> <output_folder>"
echo
echo "Here is what executed code contents looks like:"
echo "   - do some stuff"
echo "   - run SALOME in terminal mode, and log port number to a file"
echo "   - run some scripts (Python, binary), each in a dedicated SALOME session on a specific port"
echo "   - do some stuff"
echo


if [ $# != 2 ]; then
    echo "Usage:" $0 "<nb_execution> <output_folder>"
    exit 1
fi

NB_COMP=$1
OUTPUT_FOLDER=$2
BASE_DIR=`pwd`
SALOME_PORT=-1

case ${OUTPUT_FOLDER} in
    /*) ;; # OUTPUT_FOLDER is given as an absolute path
    *) OUTPUT_FOLDER=${BASE_DIR}/${OUTPUT_FOLDER} ;; # build absolute path
esac

if [ "x${SALOME_APPLI_FOLDER}" == "x" ]; then
    echo "SALOME_APPLI_FOLDER variable is not set (or empty)."
    echo "Try to locate SALOME launcher in current directory."
    SALOME_APPLI_FOLDER=`pwd`
    if ! [ -f ${SALOME_APPLI_FOLDER}/salome ]; then
        echo "Unable to locate salome command."
        exit 1
    fi
fi

start_salome() {
    ${SALOME_APPLI_FOLDER}/salome start -t --ns-port-log=${OUTPUT_FOLDER}/session.log
    SALOME_PORT=`cat ${OUTPUT_FOLDER}/session.log`
    echo "SALOME is running on port" ${SALOME_PORT}
}

stop_salome() {
    ${SALOME_APPLI_FOLDER}/salome shell -p ${SALOME_PORT} ${SALOME_APPLI_FOLDER}/bin/salome/killSalomeWithPort.py args:${SALOME_PORT}
}

run_command() {
    WORK_DIR=`pwd` # a pushd has been done before calling this function
    echo "Run command in folder:" ${WORK_DIR}
    ${SALOME_APPLI_FOLDER}/salome shell -p ${SALOME_PORT} ${SALOME_APPLI_FOLDER}/bin/salome/waitContainers.py
    ${SALOME_APPLI_FOLDER}/salome shell -p ${SALOME_PORT} ${BASE_DIR}/myscript.py args:${WORK_DIR}
    echo "Execution terminated in folder:" ${WORK_DIR}
}


# make myscript.py
echo "
#!/usr/bin/env python
import os
import sys
import time
work_dir = sys.argv[1]
logfile = os.path.join(work_dir, 'myscript.log')
msg = 'Waiting for 3s in folder %s'%work_dir
time.sleep(3)
with open(logfile, 'w') as f:
  f.write(msg)
" > ${BASE_DIR}/myscript.py

# Build output folders
typeset -i i=${NB_COMP}
while (( $i > 0 ))
do
    mkdir -p ${OUTPUT_FOLDER}/execution_$i
    let "i=i-1"
done

# Start execution
start_salome
typeset -i i=${NB_COMP}
while (( $i > 0 ))
do
    pushd ${OUTPUT_FOLDER}/execution_$i > /dev/null
    run_command &
    popd > /dev/null
    let "i=i-1"
done
stop_salome
