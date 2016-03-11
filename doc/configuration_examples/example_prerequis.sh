# Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

# --- SALOME Required packages : configuration example,SALOME2 V2_2_0
# --- all path are relative to ${INSTALLROOT}
#
INSTALLROOT="__INSTALL_PATH__/SALOME2/prerequis"

#------ gcc 3.2.3 ---
export PATH=${INSTALLROOT}/gcc-3.2.3/bin:${PATH}
export LD_LIBRARY_PATH=${INSTALLROOT}/gcc-3.2.3/lib:${LD_LIBRARY_PATH}

#------ Tcl/tk ------
export PATH=${INSTALLROOT}/tcl8.3/bin:${INSTALLROOT}/tk8.3/bin:${PATH}
export LD_LIBRARY_PATH=${INSTALLROOT}/tcl8.3/lib:${INSTALLROOT}/tk8.3/lib:${LD_LIBRARY_PATH}

export TCLHOME=${INSTALLROOT}/tcl8.3
export TKHOME=${INSTALLROOT}/tk8.3

export TCL_LIBRARY=${INSTALLROOT}/tcl8.3/lib/tcl8.3
export TK_LIBRARY=${INSTALLROOT}/tk8.3/lib/tk8.3


#------ python ------
export PYTHONHOME=${INSTALLROOT}/Python-2.2.2
export PYTHON_INCLUDE=${PYTHONHOME}/include/python2.2
export PATH=${PYTHONHOME}/bin:${PATH}
export PYTHONSTARTUP=${PYTHONHOME}/.pythonrc

#------ qt ------
export QTDIR=${INSTALLROOT}/qt-x11-free-3.0.5
export PATH=$QTDIR/bin:${PATH} 
export LD_LIBRARY_PATH=$QTDIR/lib:${LD_LIBRARY_PATH} 

#------ sip ------
export SIPDIR=${INSTALLROOT}/sip-3.3.2
export PYTHONPATH=${SIPDIR}:${PYTHONPATH}
export PATH=${SIPDIR}:${PATH}
export LD_LIBRARY_PATH=${SIPDIR}:${LD_LIBRARY_PATH}

#------ pyqt ------
export PYQTDIR=${INSTALLROOT}/PyQt-3.3.2
# --- set PYQT_SIPS as below, presence required during pyqt check during configure
export PYQT_SIPS=${PYQTDIR}/sip
export PYTHONPATH=${PYQTDIR}:${PYTHONPATH}
export PATH=${PYQTDIR}:${PATH} 
export LD_LIBRARY_PATH=${PYQTDIR}:${LD_LIBRARY_PATH} 

#------ qwt ------
export QWTHOME=${INSTALLROOT}/qwt-0.4.1
export LD_LIBRARY_PATH=${QWTHOME}/lib:${LD_LIBRARY_PATH} 

#------ swig ------ 
export SWIG_ROOT=${INSTALLROOT}/SWIG-1.3.17
export PATH=${SWIG_ROOT}/bin:${PATH}
export SWIG_LIB=${SWIG_ROOT}/lib/swig1.3
export LD_LIBRARY_PATH=${SWIG_ROOT}/lib:${LD_LIBRARY_PATH}

#------ cas ------ 
export CASROOT=${INSTALLROOT}/CAS-5.2
export LD_LIBRARY_PATH=${CASROOT}/lin/lib:${LD_LIBRARY_PATH}
# Variable for Foundation Classes : 
export CSF_UnitsLexicon=${CASROOT}/src/UnitsAPI/Lexi_Expr.dat 
export CSF_UnitsDefinition=${CASROOT}/src/UnitsAPI/Units.dat 
# Variable for DataExchange : 
export CSF_SHMessageStd=${CASROOT}/src/SHMessageStd 
export CSF_XSMessage=${CASROOT}/src/XSMessage 
# Variable for Font : 
export CSF_MDTVFontDirectory=${CASROOT}/src/FontMFT 
export CSF_MDTVTexturesDirectory=${CASROOT}/src/Textures 

#------ hdf ------
export HDF5HOME=${INSTALLROOT}/hdf5-1.4.4
export PATH=${HDF5HOME}/bin:${PATH}
export LD_LIBRARY_PATH=${HDF5HOME}/lib:${LD_LIBRARY_PATH}

#------ med ------
export MED2HOME=${INSTALLROOT}/med-2.2.2.
export PATH=${MED2HOME}/bin:${PATH}
export LD_LIBRARY_PATH=${MED2HOME}/lib:${LD_LIBRARY_PATH}

#------ vtk ------
export VTKHOME=${INSTALLROOT}/VTK-4.2.2
export PYVTK_PATH=${VTKHOME}/lib/vtk
export LD_LIBRARY_PATH=${PYVTK_PATH}:${LD_LIBRARY_PATH} 
export PYTHONPATH=${VTKHOME}/Wrapping/Python:${PYVTK_PATH}:${PYTHONPATH}
export PATH=${VTKHOME}/bin:${PATH} 

#------ omni ------
export OMNIORBDIR=${INSTALLROOT}/omniORB-3.0.5
# --- OMNIORB_CONFIG is often modified by user
export OMNIORB_CONFIG=${HOME}/.salome.omniORB.cfg
export PYTHONPATH=${OMNIORBDIR}/lib/python:${OMNIORBDIR}/lib/i586_linux_2.0_glibc2.1:${PYTHONPATH}
export PATH=${OMNIORBDIR}/bin/i586_linux_2.0_glibc2.1:${PATH}
export LD_LIBRARY_PATH=${OMNIORBDIR}/lib/i586_linux_2.0_glibc2.1:${LD_LIBRARY_PATH} 

#------ boost ---------
export BOOSTDIR=${INSTALLROOT}/boost_1_31_0

#------ doxygen ------
export PATH=${INSTALLROOT}/doxygen-1.3-rc2/bin:${PATH}

#------ graphviz ------
export PATH=${INSTALLROOT}/graphviz-1.9/bin:${PATH}
export LD_LIBRARY_PATH=${INSTALLROOT}/graphviz-1.9/lib/graphviz:${LD_LIBRARY_PATH}


# ----- tix -----------
export PATH=${INSTALLROOT}/tix8.1.4/bin:${PATH}
export LD_LIBRARY_PATH=${INSTALLROOT}/tix8.1.4/lib:${LD_LIBRARY_PATH}
export TIXHOME=${INSTALLROOT}/tix8.1.4
export TIX_LIBRARY=${INSTALLROOT}/tix8.1.4/lib/tix8.1

#------ netgen ------
export NETGENHOME=${INSTALLROOT}/netgen4.3/install


