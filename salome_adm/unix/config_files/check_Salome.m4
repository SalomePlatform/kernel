#  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
# 
#  This library is free software; you can redistribute it and/or 
#  modify it under the terms of the GNU Lesser General Public 
#  License as published by the Free Software Foundation; either 
#  version 2.1 of the License. 
# 
#  This library is distributed in the hope that it will be useful, 
#  but WITHOUT ANY WARRANTY; without even the implied warranty of 
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
#  Lesser General Public License for more details. 
# 
#  You should have received a copy of the GNU Lesser General Public 
#  License along with this library; if not, write to the Free Software 
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
# 
#  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
#
#------------------------------------------------------------
#  Check availability of Salome binary distribution
#
#  Author : Marc Tajchman (CEA, 2002)
#------------------------------------------------------------

AC_DEFUN([CHECK_SALOME],[

AC_CHECKING(for Salome)

Salome_ok=no

AC_ARG_WITH(salome,
	    --with-salome=DIR root directory path of SALOME installation,
	    SALOME_DIR="$withval",SALOME_DIR="")

if test "x$SALOME_DIR" == "x" ; then

# no --with-salome-dir option used

   if test "x$SALOME_ROOT_DIR" != "x" ; then

    # SALOME_ROOT_DIR environment variable defined
      SALOME_DIR=$SALOME_ROOT_DIR

   else

    # search Salome binaries in PATH variable
      AC_PATH_PROG(TEMP, libMEDMEM_Swig.py)
      if test "x$TEMP" != "x" ; then
         SALOME_BIN_DIR=`dirname $TEMP`
         SALOME_DIR=`dirname $SALOME_BIN_DIR`
      fi
      
   fi
# 
fi

if test -f ${SALOME_DIR}/bin/libMEDMEM_Swig.py  ; then
   Salome_ok=yes
   AC_MSG_RESULT(Using Salome distribution in ${SALOME_DIR})

   if test "x$SALOME_ROOT_DIR" == "x" ; then
      SALOME_ROOT_DIR=${SALOME_DIR}
   fi
   if test "x$SALOME_SITE_DIR" == "x" ; then
      SALOME_SITE_DIR=${SALOME_ROOT_DIR}
   fi
   AC_SUBST(SALOME_ROOT_DIR)
   AC_SUBST(SALOME_SITE_DIR)

else
   AC_MSG_WARN("Cannot find compiled Salome distribution")
fi
  
AC_MSG_RESULT(for Salome: $Salome_ok)
 
])dnl
 
