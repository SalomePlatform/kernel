dnl Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
dnl
dnl Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
dnl CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public
dnl License as published by the Free Software Foundation; either
dnl version 2.1 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public
dnl License along with this library; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
dnl
dnl See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
dnl

AC_DEFUN([CHECK_MPICH],[

AC_REQUIRE([AC_PROG_CC])dnl

AC_ARG_WITH(mpich,
  [AC_HELP_STRING([--with-mpich=DIR],[root directory path of MPICH installation])],
  WITHMPICH="yes",WITHMPICH="no")

MPI_INCLUDES=""
MPI_LIBS=""
if test "$WITHMPICH" = yes; then

  MPIREQUESTED="yes"
  echo
  echo ---------------------------------------------
  echo testing mpich
  echo ---------------------------------------------
  echo
  MPICH_HOME=$withval

  if test "$MPICH_HOME"; then
    MPI_INCLUDES="-I$MPICH_HOME/include"
    if test "x$MPICH_HOME" = "x/usr"
    then
      MPI_LIBS="-lmpichcxx -lmpich -lopa -lmpl -lrt -lpthread"
    else
      MPI_LIBS="-L$MPICH_HOME/lib -lmpichcxx -lmpich -lopa -lmpl -lrt -lpthread"
    fi
  fi

  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$MPI_INCLUDES $CPPFLAGS"
  AC_CHECK_HEADER(mpi.h,WITHMPICH="yes",WITHMPICH="no")
  CPPFLAGS="$CPPFLAGS_old"

  LIBS_old="$LIBS"
  LIBS="$MPI_LIBS $LIBS"
  AC_CHECK_LIB(mpich,MPI_Init,WITHMPICH="yes",WITHMPICH="no")
  AC_CHECK_LIB(mpich,MPI_Publish_name,WITHMPI2="yes",WITHMPI2="no")
  LIBS="$LIBS_old"

  AC_MSG_CHECKING(for mpich)
  if test "$WITHMPICH" = "yes";then
     mpi_ok=yes
     mpi2_ok=$WITHMPI2
     WITHMPI="yes"
     CPPFLAGS="-DWITHMPICH $CPPFLAGS"
     AC_MSG_RESULT(yes)
  else
     mpi_ok=no
     mpi2_ok=no
     WITHMPI=no
     AC_MSG_RESULT(no)
  fi

fi

])dnl
