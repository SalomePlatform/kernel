#!/bin/sh

# --- example to adapt to your own configuration ---
#
# --- envSalome.sh gives a set of MODULE_ROOT_DIR, to source
# --- you must at least define and export KERNEL_ROOT_DIR

REPINST=/home/prascle/SALOME2/Install

export KERNEL_ROOT_DIR=${REPINST}/KERNEL_V301

export GUI_ROOT_DIR=${REPINST}/GUI_V301
#export SalomeAppConfig=${GUI_ROOT_DIR}/share/salome/resources
export SalomeAppConfig=${HOME}/${APPLI}
export SUITRoot=${GUI_ROOT_DIR}/share/salome

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

