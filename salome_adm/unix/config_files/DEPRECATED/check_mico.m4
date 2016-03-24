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

AC_DEFUN([CHECK_MICO],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CXX])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([AC_PROG_CXXCPP])dnl

AC_CHECKING(for mico)
mico_ok=yes

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_PATH_PROG(MICOD, micod)
if test "x$MICOD" = "x"
then
  mico_ok=no
  AC_MSG_RESULT(mico binaries not in PATH variable)
else
  mico_ok=yes
fi

if  test "x$mico_ok" = "xyes"
then
  AC_SUBST(MICO_IDL)

  MICO_BIN=`echo ${MICOD} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
  MICO_LIB=`echo ${MICO_BIN} | sed -e "s,bin,lib,"`
  MICO_ROOT=`echo ${MICO_BIN}  | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
  AC_SUBST(MICO_ROOT)

  MICO_IDL=${MICO_BIN}/idl
  AC_SUBST(MICO_IDL)

  MICO_INCLUDES="-I$MICO_ROOT/include"
  AC_SUBST(MICO_INCLUDES)

  MICO_VERSION=`sed -n -e 's/.define *MICO_VERSION *\"\([[^\"]]*\)\"/\1/p' $MICO_ROOT/include/mico/version.h`
  AC_SUBST(MICO_VERSION)
  AC_MSG_RESULT(using mico version $MICO_VERSION)

  MICO_CXXFLAGS="-ftemplate-depth-42 -fpermissive"
  AC_SUBST(MICO_CXXFLAGS)

  CXXFLAGS_old=$CXXFLAGS
  CXXFLAGS="$CXXFLAGS $MICO_CXXFLAGS $MICO_INCLUDES"

  AC_MSG_CHECKING(include of CORBA.h)
  AC_TRY_COMPILE(
#include "CORBA.h"
, ,mico_ok=yes ,mico_ok=no)

  if  test "x$mico_ok" = "xno"
  then
    AC_MSG_RESULT(CORBA.h not found in mico installation)
  else
    AC_MSG_RESULT(yes)
  fi

  CXXFLAGS=$CXXFLAGS_old

fi

if test "x$mico_ok" = "xyes" 
then
  if test "x$MICO_LIB" = "x/usr/lib"
  then
    MICO_LDFLAGS=""
  else
    MICO_LDFLAGS="-L$MICO_LIB"
  fi

  LIBS_old=$LIBS
  MICO_LIBS="$MICO_LDFLAGS -lmico$MICO_VERSION -ldl"
  AC_SUBST(MICO_LIBS)

  LIBS="$MICO_LIBS $LIBS"
  CXXFLAGS_old=$CXXFLAGS
  CXXFLAGS="$CXXFLAGS $MICO_CXXFLAGS $MICO_INCLUDES"

  AC_MSG_CHECKING(whether we can link with mico)
  AC_TRY_LINK(
#include "CORBA.h"
,CORBA::ORB_var orb,
    mico_ok=yes,mico_ok=no)

  if test "x$mico_ok" = "xno" 
  then
    AC_MSG_RESULT(mico library linking failed)
    mico_ok=no
  else
    AC_MSG_RESULT(yes)
  fi

  CXXFLAGS=$CXXFLAGS_old
  LIBS=$LIBS_old
fi


if test "x$mico_ok" = "xyes" 
then

  MICO_IDLCXXFLAGS="-bcxx -I$MICO_ROOT/idl"
  AC_SUBST(MICO_IDLCXXFLAGS)
  AC_SUBST(MICO_IDLPYFLAGS)

  MICO_IDL_CLN_H=.hh
  MICO_IDL_CLN_CXX=SK.cc
  MICO_IDL_CLN_OBJ=SK.o 
  AC_SUBST(MICO_IDL_CLN_H)
  AC_SUBST(MICO_IDL_CLN_CXX)
  AC_SUBST(MICO_IDL_CLN_OBJ)

  MICO_IDL_SRV_H=.hh
  MICO_IDL_SRV_CXX=SK.cc
  MICO_IDL_SRV_OBJ=SK.o
  AC_SUBST(MICO_IDL_SRV_H)
  AC_SUBST(MICO_IDL_SRV_CXX)
  AC_SUBST(MICO_IDL_SRV_OBJ)

  MICO_IDL_TIE_H=
  MICO_IDL_TIE_CXX=
  AC_SUBST(MICO_IDL_TIE_H)
  AC_SUBST(MICO_IDL_TIE_CXX)
  
  AC_DEFINE(MICO)

  CORBA_HAVE_POA=1
  AC_DEFINE(CORBA_HAVE_POA)

fi

AC_LANG_RESTORE

AC_MSG_RESULT(for mico: $mico_ok)

])dnl
dnl
