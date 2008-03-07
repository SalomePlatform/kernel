dnl  Copyright (C) 2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
dnl  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
dnl
dnl
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
    MPI_INCLUDES=`$OPENMPI_HOME/bin/mpicxx --showme:compile`
    MPI_LIBS=`$OPENMPI_HOME/bin/mpicxx --showme:link`
  fi

  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$MPI_INCLUDES $CPPFLAGS"
  AC_CHECK_HEADER(mpi.h,WITHOPENMPI="yes",WITHOPENMPI="no")
  CPPFLAGS="$CPPFLAGS_old"

  AC_MSG_CHECKING(for openmpi)
  if test "$WITHOPENMPI" = "yes";then
     mpi_ok=yes
     AC_MSG_RESULT(yes)
  else
     mpi_ok=no
     AC_MSG_RESULT(no)
  fi
fi
])dnl

