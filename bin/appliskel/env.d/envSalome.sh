#!/bin/sh

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

# --- example to adapt to your own configuration ---
#
# --- envSalome.sh gives a set of MODULE_ROOT_DIR, to source
# --- you must at least define and export KERNEL_ROOT_DIR

REPINST=/home/prascle/SALOME2/Install

export KERNEL_ROOT_DIR=${REPINST}/KERNEL_V301

export GUI_ROOT_DIR=${REPINST}/GUI_V301
#export SalomeAppConfig=${GUI_ROOT_DIR}/share/salome/resources/gui
export SalomeAppConfig=${HOME}/${APPLI}

export GEOM_ROOT_DIR=${REPINST}/GEOM_V301
export MED_ROOT_DIR=${REPINST}/MED_V301
export SMESH_ROOT_DIR=${REPINST}/SMESH_V301
export SUPERV_ROOT_DIR=${REPINST}/SUPERV_V301
export VISU_ROOT_DIR=${REPINST}/VISU_V301
export NETGENPLUGIN_ROOT_DIR=${REPINST}/NETGENPLUGIN_V301
export COMPONENT_ROOT_DIR=${REPINST}/COMPONENT_V301
export PYCALCULATOR_ROOT_DIR=${REPINST}/PYCALCULATOR_V301
export PYHELLO_ROOT_DIR=${REPINST}/PYHELLO_V301
export PYTIX_ROOT_DIR=${REPINST}/PYTIX_V301

