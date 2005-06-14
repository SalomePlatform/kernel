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

AC_DEFUN([CHECK_VTK],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CXX])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([AC_PROG_CXXCPP])dnl

AC_CHECKING(for VTK)

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_SUBST(VTK_INCLUDES)
AC_SUBST(VTK_LIBS)
AC_SUBST(VTKPY_MODULES)

VTK_INCLUDES=""
VTK_LIBS=""
VTKPY_MODULES=""

vtk_ok=no

AC_PATH_X

if test "x$OpenGL_ok" != "xyes"
then
   AC_MSG_WARN(vtk needs OpenGL correct configuration, check configure output)
fi

if test "x$x_libraries" != "x"
then
   LXLIB="-L$x_libraries"
else
   LXLIB=""
fi

if test "x$x_libraries" = "x/usr/lib"
then
   LXLIB=""
fi

if test "x$x_libraries" = "x/usr/lib"
then
   LXLIB=""
fi

if test "x$x_libraries" = "x/usr/lib"
then
   LXLIB=""
fi

LOCAL_INCLUDES="$OGL_INCLUDES"
LOCAL_LIBS="-lvtkCommon -lvtkGraphics -lvtkImaging -lvtkFiltering -lvtkIO -lvtkRendering -lvtkHybrid $OGL_LIBS $LXLIB -lX11 -lXt"
TRY_LINK_LIBS="-lvtkCommon $OGL_LIBS $LXLIB -lX11 -lXt"

if test -z $VTKHOME
then 
   AC_MSG_WARN(undefined VTKHOME variable which specify where vtk was compiled)
   if test -f /usr/include/vtk/vtkPlane.h ; then
      AC_MSG_RESULT(trying /usr)
      VTKHOME="/usr"
   fi
fi

if test ! -z $VTKHOME
then
   LOCAL_INCLUDES="-I$VTKHOME/include/vtk $LOCAL_INCLUDES"
   LOCAL_LIBS="-L$VTKHOME/lib/vtk $LOCAL_LIBS"
   TRY_LINK_LIBS="-L$VTKHOME/lib/vtk $TRY_LINK_LIBS"
fi

dnl vtk headers
CPPFLAGS_old="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $LOCAL_INCLUDES"

AC_CHECK_HEADER(vtkPlane.h,vtk_ok="yes",vtk_ok="no")

 CPPFLAGS="$CPPFLAGS_old"

 if  test "x$vtk_ok" = "xyes"
 then
   VTK_INCLUDES="$LOCAL_INCLUDES"

 dnl vtk libraries

   AC_MSG_CHECKING(linking VTK library)

  LIBS_old="$LIBS"
#  LIBS="$LIBS $TRY_LINK_LIBS"
  LIBS="$LIBS $LOCAL_LIBS"
  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $VTK_INCLUDES"

 dnl  VTKPY_MODULES="$VTKHOME/python"

   AC_CACHE_VAL(salome_cv_lib_vtk,[
     AC_TRY_LINK(
#include "vtkPlane.h"
,   vtkPlane *p = vtkPlane::New();,
    eval "salome_cv_lib_vtk=yes",eval "salome_cv_lib_vtk=no")
  ])
  vtk_ok="$salome_cv_lib_vtk"
  LIBS="$LIBS_old"
  CPPFLAGS="$CPPFLAGS_old"

fi

if  test "x$vtk_ok" = "xno"
then
  AC_MSG_RESULT("no")
  AC_MSG_WARN(unable to link with vtk library)
else
  AC_MSG_RESULT("yes")
  VTK_LIBS="$LOCAL_LIBS"
  VTK_MT_LIBS="$LOCAL_LIBS"
fi

AC_MSG_RESULT("for vtk: $vtk_ok")

AC_LANG_RESTORE

# Save cache
AC_CACHE_SAVE

])dnl


