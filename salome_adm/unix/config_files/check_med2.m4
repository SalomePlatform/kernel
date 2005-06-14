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

AC_DEFUN([CHECK_MED2],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([CHECK_HDF5])dnl

AC_CHECKING(for MED2)

AC_ARG_WITH(med2,
    [  --with-med2=DIR                 root directory path to med2 installation ],
    [MED2HOME="$withval"
      AC_MSG_RESULT("select $withval as path to med2")
    ])

AC_SUBST(MED2_INCLUDES)
AC_SUBST(MED2_LIBS)
AC_SUBST(MED2_MT_LIBS)

MED2_INCLUDES=""
MED2_LIBS=""
MED2_MT_LIBS=""

med2_ok=no

LOCAL_INCLUDES="$HDF5_INCLUDES"
LOCAL_LIBS="-lmed $HDF5_LIBS"

if test -z $MED2HOME
then
   AC_MSG_WARN(undefined MED2HOME variable which specify med2 installation directory)
   AC_PATH_PROG(MDUMP, mdump)
   if test "xMDUMP" != "x" ; then
      MED2HOME=$MDUMP
      MED2HOME=`echo ${MED2HOME} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
      MED2HOME=`echo ${MED2HOME} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
   fi
fi
if test ! -z $MED2HOME
then
   LOCAL_INCLUDES="$LOCAL_INCLUDES -I$MED2HOME/include"
   if test "x$MED2HOME" = "x/usr"
   then
     LOCAL_LIBS="-lmed  $LOCAL_LIBS"
   else
     LOCAL_LIBS="-L$MED2HOME/lib $LOCAL_LIBS"
   fi
fi

dnl check med2 header

CPPFLAGS_old="$CPPFLAGS"
dnl we must test system : linux = -DPCLINUX
dnl we must test system : Alpha-OSF = -DOSF1
case $host_os in
   linux*)
      CPPFLAGS="$CPPFLAGS -DPCLINUX $LOCAL_INCLUDES"
      ;;
   osf*)
      CPPFLAGS="$CPPFLAGS -DOSF1 $LOCAL_INCLUDES"
      ;;
esac
AC_CHECK_HEADER(med.h,med2_ok=yes ,med2_ok=no)
CPPFLAGS="$CPPFLAGS_old"

if  test "x$med2_ok" = "xyes"
then

dnl check med2 library

  LIBS_old="$LIBS"
  LIBS="$LIBS $LOCAL_LIBS"
  AC_CHECK_LIB(med,MEDouvrir,med2_ok=yes,med2_ok=no)
  LIBS="$LIBS_old"

fi

if  test "x$med2_ok" = "xyes"
then
case $host_os in
   linux*)
        MED2_INCLUDES="-DPCLINUX $LOCAL_INCLUDES"
      ;;
   osf*)
      MED2_INCLUDES="-DOSF1 $LOCAL_INCLUDES"
      ;;
esac
  MED2_LIBS="$LOCAL_LIBS"
  MED2_MT_LIBS="$LOCAL_LIBS"
fi

AC_MSG_RESULT(for med2: $med2_ok)

])dnl
