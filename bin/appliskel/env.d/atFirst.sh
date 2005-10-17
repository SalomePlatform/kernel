#!/bin/sh

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
