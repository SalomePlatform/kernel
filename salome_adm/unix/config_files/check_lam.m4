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

AC_DEFUN([CHECK_LAM],[

AC_REQUIRE([AC_PROG_CC])dnl

AC_ARG_WITH(lam,
	    --with-lam=DIR root directory path of LAM installation,
	    WITHLAM="yes",WITHLAM="no")

MPI_INCLUDES=""
MPI_LIBS=""
if test "$WITHLAM" = yes; then

  echo
  echo ---------------------------------------------
  echo testing lam
  echo ---------------------------------------------
  echo
  LAM_HOME=$withval

  if test "$LAM_HOME"; then
    MPI_INCLUDES="-I$LAM_HOME/include"
    MPI_LIBS="-L$LAM_HOME/lib"
  fi

  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$MPI_INCLUDES $CPPFLAGS"
  AC_CHECK_HEADER(mpi.h,WITHLAM="yes",WITHLAM="no")
  CPPFLAGS="$CPPFLAGS_old"

  if test "$WITHLAM" = "yes";then
    AC_CHECK_LIB(util,openpty,,WITHLAM="no")
    LIBS_old="$LIBS"
    LDFLAGS_old="$LDFLAGS"
    LDFLAGS="$MPI_LIBS $LDFLAGS"
    AC_CHECK_LIB(lam,lam_mp_init,,WITHLAM="no")
    AC_CHECK_LIB(mpi,MPI_Init,WITHLAM="yes",WITHLAM="no")
    AC_CHECK_LIB(mpi,MPI_Publish_name,WITHMPI2="yes",WITHMPI2="no")
    LDFLAGS="$LDFLAGS_old"
    LIBS="$LIBS_old"
  fi

  if test "$WITHLAM" = "yes";then
     WITHMPI="yes"
     mpi_ok=yes
     MPI_LIBS="$MPI_LIBS -lmpi -llam"
  else
     mpi_ok=no
  fi

fi


])dnl
