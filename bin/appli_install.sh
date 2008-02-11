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

SALOME_ROOT=${HOME}/SALOME2
INSTALL_ROOT=${SALOME_ROOT}/Install

APPLI_ROOT=`pwd`

# --- clean appli
rm -rf bin lib share doc envd getAppliPath.py searchFreePort.sh runAppli runConsole runSession env.d

# --- install appli

mkdir -p env.d
ln -fs bin/salome/appliskel/envd .
ln -fs bin/salome/appliskel/getAppliPath.py .
ln -fs bin/salome/appliskel/searchFreePort.sh .
ln -fs bin/salome/appliskel/runRemote.sh .
ln -fs bin/salome/appliskel/runAppli .
ln -fs bin/salome/appliskel/runConsole .
ln -fs bin/salome/appliskel/runSession .
ln -fs bin/salome/appliskel/runTests .
ln -fs bin/salome/appliskel/.bashrc .

# --- prerequisites

if [ x${PREREQUISITE_SH} != x ]; then
  cp ${PREREQUISITE_SH} env.d/envProducts.sh;
else
  # --- unless PREREQUISITE_SH defines the prerequisite script, 
  #     edit and uncomment the following line to set it, and comment the echo and exit lines
  #ln -fs myPathForSalomePrerequisiteScriptToBeSourced env.d/envProducts.sh
  echo "The file to source for SALOME prerequisite definition is not known:"
  echo "--> Edit the corresponding line in " $0 ",Comment this message and the following exit command,"
  exit 1;
fi

# --- symbolic links creation, from modules_root_dir

VERSION=V3_2_0

for module in KERNEL MED GUI GEOM SMESH VISU SUPERV LIGHT NETGENPLUGIN PYCALCULATOR; 
do 
    echo " ========= " ${module};
    python virtual_salome.py -v --prefix="." --module=$INSTALL_ROOT/${module}_${VERSION}
    echo "export ${module}_ROOT_DIR=$APPLI_ROOT" >> env.d/configSalome.sh
done

# --- HELLO module

echo " =========  HELLO1";
python virtual_salome.py -v --prefix="." --module=$INSTALL_ROOT/HELLO1_${VERSION}
echo "export HELLO_ROOT_DIR=$APPLI_ROOT" >> env.d/configSalome.sh

# --- PYHELLO module

echo " ========= PYHELLO1";
python virtual_salome.py -v --prefix="." --module=$INSTALL_ROOT/PYHELLO1_${VERSION}
echo "export PYHELLO_ROOT_DIR=$APPLI_ROOT" >> env.d/configSalome.sh

# --- GUI config

echo "export config_var=$APPLI_ROOT:$APPLI_ROOT/share/salome/resources/gui" >> env.d/configGUI.sh

# --- SAMPLES directory

echo "export DATA_DIR=$INSTALL_ROOT/SAMPLES/${VERSION}/SAMPLES_SRC" >> env.d/configSalome.sh

# --- 
