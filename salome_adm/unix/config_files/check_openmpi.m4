dnl  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
dnl
dnl  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
dnl  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
dnl
dnl  This library is free software; you can redistribute it and/or
dnl  modify it under the terms of the GNU Lesser General Public
dnl  License as published by the Free Software Foundation; either
dnl  version 2.1 of the License.
dnl
dnl  This library is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl  Lesser General Public License for more details.
dnl
dnl  You should have received a copy of the GNU Lesser General Public
dnl  License along with this library; if not, write to the Free Software
dnl  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
dnl
dnl  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
dnl
AC_DEFUN([CHECK_OPENMPI],[
AC_ARG_WITH(openmpi,
	    AC_HELP_STRING([--with-openmpi=DIR],[root directory path of openmpi installation]),
	    WITHOPENMPI="yes",WITHOPENMPI="no")

MPI_INCLUDES=""
MPI_LIBS=""
mpi_ok=no

if test "$WITHOPENMPI" = yes; then

  OPENMPI_HOME=$withval

  if test "$OPENMPI_HOME"; then
    if test -f "${OPENMPI_HOME}/include/mpi.h"; then
      MPI_INCLUDES="-I${OPENMPI_HOME}/include"
    else  
      MPI_INCLUDES=`$OPENMPI_HOME/bin/mpicxx --showme:compile`
    fi
    MPI_LIBS=`$OPENMPI_HOME/bin/mpicxx --showme:link`
  fi

  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$MPI_INCLUDES $CPPFLAGS"
  AC_CHECK_HEADER(mpi.h,WITHOPENMPI="yes",WITHOPENMPI="no")
  CPPFLAGS="$CPPFLAGS_old"

  LIBS_old="$LIBS"
  LIBS="-L${OPENMPI_HOME}/lib $LIBS"
  AC_CHECK_LIB(mpi,MPI_Publish_name,WITHMPI2="yes",WITHMPI2="no")
  LIBS="$LIBS_old"

  AC_MSG_CHECKING(for openmpi)
  if test "$WITHOPENMPI" = "yes";then
     mpi_ok=yes
     WITHMPI=yes
     CPPFLAGS="-DOMPI_IGNORE_CXX_SEEK -DWITHOPENMPI $CPPFLAGS"
     AC_MSG_RESULT(yes)
  else
     mpi_ok=no
     WITHMPI=no
     AC_MSG_RESULT(no)
  fi
fi
])dnl

