dnl  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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

AC_DEFUN([CHECK_MPICH],[

AC_REQUIRE([AC_PROG_CC])dnl

AC_ARG_WITH(mpich,
	    --with-mpich=DIR root directory path of MPICH installation,
	    WITHMPICH="yes",WITHMPICH="no")

MPICH_INCLUDES=""
MPICH_LIBS=""
if test "$WITHMPICH" = yes; then

  echo
  echo ---------------------------------------------
  echo testing mpich
  echo ---------------------------------------------
  echo
  MPICH_HOME=$withval

  if test "$MPICH_HOME"; then
    MPICH_INCLUDES="-I$MPICH_HOME/include"
    MPICH_LIBS="-L$MPICH_HOME/lib"
  fi

  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$MPICH_INCLUDES $CPPFLAGS"
  AC_CHECK_HEADER(mpi.h,WITHMPICH="yes",WITHMPICH="no")
  CPPFLAGS="$CPPFLAGS_old"

  if test "$WITHMPICH" = "yes";then
    LDFLAGS_old="$LDFLAGS"
    LDFLAGS="$MPICH_LIBS $LDFLAGS"
    AC_CHECK_LIB(mpich,MPI_Init,WITHMPICH="yes",WITHMPICH="no")
    LDFLAGS="$LDFLAGS_old"
  fi

  if test "$WITHMPICH" = "yes";then
    mpich_ok=yes
    MPICH_LIBS="$MPICH_LIBS -lmpich"
  fi


fi
AC_SUBST(MPICH_INCLUDES)
AC_SUBST(MPICH_LIBS)
AC_SUBST(WITHMPICH)

])dnl
