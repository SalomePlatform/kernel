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

# ------------------------------------------------
# ---- openpbs

export OPENPBS=/optroot/openpbs2316
export PATH=${OPENPBS}/bin:${PATH}
export LD_LIBRARY_PATH=${OPENPBS}/lib:${LD_LIBRARY_PATH}

#------------------------------------------------------------------------------

export INSTALL_ROOT=/home/prascle/PAL_SALOME141
#------ gcc ------
export GCC_ROOT=/usr
export CXX=${GCC_ROOT}/bin/g++
export CC=${GCC_ROOT}/bin/gcc
##
#------ boost ------ 
export BOOSTDIR=${INSTALL_ROOT}/boost-1.30.2
##

#------ tcltk ------
export TCLHOME=/usr

#------ Python ------
export PYTHONHOME=/usr
export PYTHONPATH=${PYPACKAGES}:${PYTHONPATH}
##
#------ Qt ------
#export QTDIR=/usr/lib/qt-3.0.5
export QTDIR=/usr/lib/qt3-gcc3.2
export PATH=${PATH}:${QTDIR}/bin
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${QTDIR}/lib
##
#------ Sip ------
export SIPDIR=/usr
export PYQTDIR=/usr
##
#------ PyQt ------
export PYQT_SIPS=/usr/share/sip/qt
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/lib/python2.2/site-packages
##
#------ Swig ------ 
export SWIG_ROOT=${INSTALL_ROOT}/SWIG-1.3.17
export PATH=${SWIG_ROOT}/bin:${PATH}
export LD_LIBRARY_PATH=${SWIG_ROOT}/lib:${LD_LIBRARY_PATH}
##
#------ OpenCascade ------
export CASROOT=/opt/CAS52patch
export LD_LIBRARY_PATH=${CASROOT}/Linux/lib:${LD_LIBRARY_PATH}
export PATH=${CASROOT}/Linux/bin:${PATH}
# Variable for Foundation Classes : 
export CSF_UnitsLexicon=${CASROOT}/src/UnitsAPI/Lexi_Expr.dat 
export CSF_UnitsDefinition=${CASROOT}/src/UnitsAPI/Units.dat 
# Variable for DataExchange : 
export CSF_SHMessage=${CASROOT}/src/SHMessage
export CSF_XSMessage=${CASROOT}/src/XSMessage 
# Variable for Font : 
export CSF_MDTVFontDirectory=${CASROOT}/src/FontMFT 
export CSF_MDTVTexturesDirectory=${CASROOT}/src/Textures 
##
export CSF_LANGUAGE=us
export MMGT_CLEAR=1
export CSF_EXCEPTION_PROMPT=1
export CSF_StandardDefaults=${CASROOT}/src/StdResource
export CSF_PluginDefaults=${CASROOT}/src/StdResource
export CSF_XCAFDefaults=${CASROOT}/src/StdResource
export CSF_GraphicShr=${CASROOT}/Linux/lib/libTKOpenGl.so
export CSF_IGESDefaults=${CASROOT}/src/XSTEPResource
export CSF_STEPDefaults=${CASROOT}/src/XSTEPResource
##
#export TCLLIBPATH=$TCLHOME/lib
##
#------ Qwt ------
export QWTHOME=${INSTALL_ROOT}/qwt-0.4.1
export LD_LIBRARY_PATH=${QWTHOME}/lib:${LD_LIBRARY_PATH} 
##
#------ Hdf ------
export HDF5HOME=${INSTALL_ROOT}/hdf5-1.4.4
export PATH=${HDF5HOME}/bin:$PATH
export LD_LIBRARY_PATH=${HDF5HOME}/lib:${LD_LIBRARY_PATH}
##
#------ Med ------
export MED2HOME=/opt/med2.2.2
export PATH=${MED2HOME}/bin:${PATH}
export LD_LIBRARY_PATH=${MED2HOME}/lib:${LD_LIBRARY_PATH}
##
#------ Vtk ------
export VTKHOME=${INSTALL_ROOT}/VTK-4.2.2
export PATH=${VTKHOME}/bin:${PATH}
export LD_LIBRARY_PATH=${VTKHOME}/lib/vtk:${LD_LIBRARY_PATH}
export PYTHONPATH=${VTKHOME}/lib/vtk:${VTKHOME}/lib/python2.2/site-packages/vtk_python:${PYTHONPATH}
##
#------ netgen ------
export NETGENROOT=${INSTALL_ROOT}/netgen4.3
export NETGEN_HOME=${INSTALL_ROOT}/netgen4.3
##
#------ OmniORB ------
export OMNIORBDIR=${INSTALL_ROOT}/omniORB-3.0.5
export OMNIORB_CONFIG=${OMNIORBDIR}/.omniORB.cfg
export PYTHONPATH=${OMNIORBDIR}/lib/python:${OMNIORBDIR}/lib/i586_linux_2.0_glibc2.1:${PYTHONPATH}
export PATH=$OMNIORBDIR/bin/i586_linux_2.0_glibc2.1:$PATH
export LD_LIBRARY_PATH=$OMNIORBDIR/lib/i586_linux_2.0_glibc2.1:$LD_LIBRARY_PATH 
##
#------ Numeric ------
export PYTHONPATH=${INSTALL_ROOT}/Numeric-22.0/lib/python2.2/site-packages/Numeric:${PYTHONPATH}
##
#------ HappyDoc ------
export PATH=${INSTALL_ROOT}/HappyDoc-r2_1/bin:${PATH}
export PYTHONPATH=${INSTALL_ROOT}/HappyDoc-r2_1/lib/python2.2/site-packages:${PYTHONPATH}
##
#------ Doxygen ------
export PATH=${INSTALL_ROOT}/doxygen-1.3-rc2/bin:${PATH}
##
#------ Graphviz ------
export PATH=${INSTALL_ROOT}/graphviz-1.9/bin:${PATH}
export LD_LIBRARY_PATH=${INSTALL_ROOT}/graphviz-1.9/lib/graphviz:${LD_LIBRARY_PATH}
##

#------------------------------------------------------------------------------
