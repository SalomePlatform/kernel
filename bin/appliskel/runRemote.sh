#!/bin/bash
#
# --- run command in SALOME environment from remote call, ssh or rsh
#     - SALOME configuration is defined by :
#        - list of MODULE_ROOT_DIR
#        - prerequisite environment
#        - specific configuration for a particular SALOME application
#     - These elements of configuration are defined locally on each computer
#       in the directory APPLI:
#     - A SALOME application distributed on several computers needs APPLI
#       directories on the same path ($APPLI) relative to $HOME directory
#       of the user, on each computer.
#
# --- call example (from cli76cd to cli76ce):
# ssh cli76ce ${APPLI}/runRemote.sh cli76cd 2810 SALOME_Container myContainerName
#
# --- local arguments
#     $0 : ${APPLI}/runRemote.sh: from arg name, rebuild and export $APPLI variable
#     $1 : computer name for CORBA name service (where SALOME was launched)
#     $2 : port for CORBA name service
#     $3 and following : local command to execute, with args
#

# --- retrieve APPLI path, relative to $HOME, set ${APPLI}

. `dirname $0`/setAppliPath.sh

# --- set the SALOME environment (prerequisites, MODULES_ROOT_DIR...)

. ${HOME}/${APPLI}/envd ${HOME}/${APPLI}

# --- set the OMNIORB_CONFIG file and environment relative to this run of SALOME

export OMNIORB_CONFIG=${HOME}/${APPLI}/.omniORB_$1_$2.cfg
export NSHOST=$1
export NSPORT=$2
initref="NameService=corbaname::"$1":$2"
echo "ORBInitRef $initref" > $OMNIORB_CONFIG

# --- execute the command in the SALOME environment

shift 2

${KERNEL_ROOT_DIR}/bin/salome/envSalome.py /bin/bash --rcfile $HOME/$APPLI/.bashrc -c "$*"
