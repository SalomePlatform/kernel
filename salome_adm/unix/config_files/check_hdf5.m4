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
#
AC_DEFUN([CHECK_HDF5],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl

AC_CHECKING(for HDF5)

AC_ARG_WITH(hdf5,
    [  --with-hdf5=DIR                 root directory path to hdf5 installation ],
    [HDF5HOME="$withval"
      AC_MSG_RESULT("select $withval as path to hdf5")
    ])

AC_SUBST(HDF5_INCLUDES)
AC_SUBST(HDF5_LIBS)
AC_SUBST(HDF5_MT_LIBS)

HDF5_INCLUDES=""
HDF5_LIBS=""
HDF5_MT_LIBS=""

hdf5_ok=no

LOCAL_INCLUDES=""
LOCAL_LIBS=""

if test -z $HDF5HOME
then
   AC_MSG_WARN(undefined HDF5HOME variable which specify hdf5 installation directory)
else
   LOCAL_INCLUDES="-I$HDF5HOME/include"
   LOCAL_LIBS="-L$HDF5HOME/lib"
fi

dnl hdf5 headers

CPPFLAGS_old="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $LOCAL_INCLUDES"
AC_CHECK_HEADER(hdf5.h,hdf5_ok=yes ,hdf5_ok=no)
CPPFLAGS="$CPPFLAGS_old"


if  test "x$hdf5_ok" = "xyes"
then

dnl hdf5 library

  LIBS_old="$LIBS"
  LIBS="$LIBS $LOCAL_LIBS"
  AC_CHECK_LIB(hdf5,H5open,hdf5_ok=yes,hdf5_ok=no)
  LIBS="$LIBS_old"

fi

if  test "x$hdf5_ok" = "xyes"
then
  HDF5_INCLUDES="$LOCAL_INCLUDES"
  HDF5_LIBS="$LOCAL_LIBS -lhdf5"
  HDF5_MT_LIBS="$LOCAL_LIBS -lhdf5"
fi

AC_MSG_RESULT(for hdf5: $hdf5_ok)

])dnl
