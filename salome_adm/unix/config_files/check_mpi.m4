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

AC_DEFUN([CHECK_MPI],[

AC_REQUIRE([AC_PROG_CC])dnl

AC_ARG_WITH(mpi_lib,
   [AC_HELP_STRING([--with-mpi_lib=DIR],[directory path of MPICH lib installation])],
   MPILIBREQUESTED="$withval")

AC_ARG_WITH(mpi_include,
   [AC_HELP_STRING([--with-mpi_include=DIR],[directory path of MPICH header file installation])],
   MPIINCLUDEREQUESTED="$withval")

AC_ARG_WITH(mpi,
   [AC_HELP_STRING([--with-mpi=DIR],[root directory path of MPICH installation])],
   MPIREQUESTED="yes",MPIREQUESTED="no")

MPI_INCLUDES=""
MPI_LIBS=""
WITHMPI=no
mpi_ok=no
if test x"$MPIREQUESTED" = xyes; then

  echo
  echo ---------------------------------------------
  echo testing mpi
  echo ---------------------------------------------
  echo

  if test "x$withval" != "xyes"; then
    MPI_HOME=$withval
    MPI_INCLUDES="-I$MPI_HOME/include"
    if test "x$MPI_HOME" = "x/usr"
    then
      MPI_LIBS=""
    else
      MPI_LIBS="-L$MPI_HOME/lib"
    fi
  fi

  if test x"$MPILIBREQUESTED" != x; then
    MPI_LIBS="-L$MPILIBREQUESTED"
  fi

  if test x"$MPIINCLUDEREQUESTED" != x; then
    MPI_INCLUDES="-I$MPIINCLUDEREQUESTED"
  fi

  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$MPI_INCLUDES $CPPFLAGS"
  AC_CHECK_HEADER(mpi.h,WITHMPI="yes",WITHMPI="no")
  CPPFLAGS="$CPPFLAGS_old"

  if test "$WITHMPI" = "yes";then
    LIBS_old="$LIBS"
    LDFLAGS_old="$LDFLAGS"
    LDFLAGS="$MPI_LIBS $LDFLAGS"
    AC_CHECK_LIB(elan,elan_init,MPI_LIBS="$MPI_LIBS -lelan")
    AC_CHECK_LIB(mpi,MPI_Init,WITHMPI="yes",WITHMPI="no")
    AC_CHECK_LIB(mpi,MPI_Publish_name,WITHMPI2="yes",WITHMPI2="no")
    LDFLAGS="$LDFLAGS_old"
    LIBS="$LIBS_old"
  fi

  if test "$WITHMPI" = "yes";then
    mpi_ok=yes
    MPI_LIBS="$MPI_LIBS -lmpi -lmpio -lmpiCC"
  else
    mpi_ok=no
  fi

  if test "$WITHMPI2" = "yes";then
    mpi2_ok=yes
  else
    mpi2_ok=no
  fi

fi

if test "$WITHMPI" = no; then
dnl
dnl ---------------------------------------------
dnl testing MPICH
dnl ---------------------------------------------
dnl

  CHECK_MPICH

  if test "$WITHMPICH" = no; then
dnl
dnl ---------------------------------------------
dnl testing LAM
dnl ---------------------------------------------
dnl

    CHECK_LAM

  fi

  if test "$WITHLAM" = no; then
dnl
dnl ---------------------------------------------
dnl testing OPENMPI
dnl ---------------------------------------------
dnl

    CHECK_OPENMPI

  fi

fi

if test x$WITHMPI2 = xyes; then
  CPPFLAGS="-DHAVE_MPI2 $CPPFLAGS"
  CORBA_IDLCXXFLAGS="-DHAVE_MPI2 $CORBA_IDLCXXFLAGS"
  CORBA_IDLPYFLAGS="-DHAVE_MPI2 $CORBA_IDLPYFLAGS"
fi

AC_SUBST(WITHMPI)
AC_SUBST(MPI_INCLUDES)
AC_SUBST(MPI_LIBS)
AC_SUBST(mpi_ok)
AC_SUBST(mpi2_ok)
AM_CONDITIONAL(MPI_IS_OK, [test x"$mpi_ok" = xyes])
AM_CONDITIONAL(MPI2_IS_OK, [test x"$mpi2_ok" = xyes])

])dnl
