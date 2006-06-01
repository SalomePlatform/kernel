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

#------------------------------------------------
# to be sourced first, used in following scripts
#------------------------------------------------

export LD_LIBRARY_PATH="."
export PATH="/usr/kerberos/bin:/usr/local/bin:/usr/bin:/bin:/usr/X11R6/bin:/home/prascle/bin:."
export PYTHONPATH=""

export RAC=${HOME}
export PAKSAL=/opt
export PUB=${RAC}/public
export TARP=${RAC}/tarpublic
export PYPACKAGES=${PUB}/site_packages

# ------------------------------------------------
# ---- gdb 5.3

export PATH=${PAKSAL}/gdb53/bin:${PATH}
export LD_LIBRARY_PATH=${PAKSAL}/gdb53/lib:${LD_LIBRARY_PATH}

# ------------------------------------------------
# ---- ddd 3.3.7

export PATH=${PAKSAL}/ddd337/bin:${PATH}
export LD_LIBRARY_PATH=${PAKSAL}/ddd337/lib:${LD_LIBRARY_PATH}

# ------------------------------------------------
# --- Java 1.4

export JDKHOME=${PUB}/j2sdk_nb/j2sdk1.4.2
export JAVAHOME=${PUB}/j2sdk_nb/j2sdk1.4.2
export PATH=${JAVAHOME}/bin:${PATH}
export LD_LIBRARY_PATH=${JAVAHOME}/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${JAVAHOME}/jre/lib/i386:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH=${JAVAHOME}/jre/lib/i386/client:${LD_LIBRARY_PATH}

# ------------------------------------------------
# --- gsl -1.4

export GSLHOME=${PAKSAL}/gsl-1.4
export PATH=${GSLHOME}/bin:${PATH}
export LD_LIBRARY_PATH=${GSLHOME}/lib:${LD_LIBRARY_PATH}

# ------------------------------------------------
# --- fltk 1.1.4

export FLTKHOME=${PAKSAL}/fltk114
export PATH=${FLTKHOME}/bin:${PATH}
export LD_LIBRARY_PATH=${FLTKHOME}/lib:${LD_LIBRARY_PATH}

# ------------------------------------------------
# --- gmsh 1.48

export GMSHHOME=${PAKSAL}/gmsh148
export PATH=${GMSHHOME}/bin:${PATH}

# ------------------------------------------------
# ---- tkcvs, tkdiffb, cvs

export PATH=${PAKSAL}/tkcvs7.2/bin:${PAKSAL}/cvs1.11.14/bin:${PATH}
