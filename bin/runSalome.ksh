#!/bin/ksh
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

searchFreePort

if [[ "$*" = "-nothing" ]]; then
    echo "port:$NSPORT"
elif [ $# -ne 0 ] ; then
    python ${KERNEL_ROOT_DIR}/bin/salome/runSalome.py $*
else
    python ${KERNEL_ROOT_DIR}/bin/salome/runSalome.py
fi

# -----------------------------------------------------------------------------
# examples:
# ---------
#  $: ${KERNEL_ROOT_DIR}/bin/salome/runSalome
#
#     - parameters for launching are taken from SalomeApp.xml;
#     - if the config file does not exist, it is created with default values.
#
#
#  $: ${KERNEL_ROOT_DIR}/bin/salome/runSalome --modules=GEOM,SMESH,VISU,MED --embedded=registry,study,moduleCatalog,cppContainer --standalone=pyContainer --xterm --killall
#
#     parameters from command line supersede those from SalomeApp.xml
#
# Some CORBA servers can be launched in the SALOME_Session_Server's process
# (embedded = same process) or in a separate process (standalone):
# --> registry,study,moduleCatalog,cppContainer
# Other CORBA servers could only be launched in separate process (standalone):
# --> pyContainer
#
# $: ${KERNEL_ROOT_DIR}/bin/salome/runSalome -h
#    help
# -----------------------------------------------------------------------------
#
# l'option -i permet de garder l'interpreteur python ouvert :
# par defaut, les differents serveurs ouvrent des fen�tres xterm
# (cf. runSalome.py)
# le serveur Logger n'est pas obligatoire (comment� dans runSalome.py)
#
# -----------------------------------------------------------------------------
# Example on CCRT (without ihm) :
# ${KERNEL_ROOT_DIR}/bin/salome/runSalome.py --terminal --modules=MED,CALCULATOR,COMPONENT --standalone=registry,study,moduleCatalog,cppContainer,pyContainer --killall
