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


LOCAL_INCLUDES="$OGL_INCLUDES"
LOCAL_LIBS="-lVTKCommon -lVTKGraphics -lVTKImaging -lVTKContrib $OGL_LIBS -L$x_libraries -lX11 -lXt"
TRY_LINK_LIBS="-lVTKCommon $OGL_LIBS -L$x_libraries -lX11 -lXt"

if test -z $VTKHOME
then 
   AC_MSG_WARN(undefined VTKHOME variable which specify where vtk was compiled)
else
   LOCAL_INCLUDES="-I$VTKHOME/common -I$VTKHOME/imaging -I$VTKHOME/graphics -I$VTKHOME/contrib $LOCAL_INCLUDES"
   LOCAL_LIBS="-L$VTKHOME/lib -L$VTKHOME/common -L$VTKHOME/graphics -L$VTKHOME/imaging -L$VTKHOME/contrib $LOCAL_LIBS"
   TRY_LINK_LIBS="-L$VTKHOME/lib -L$VTKHOME/common $TRY_LINK_LIBS"
fi

dnl vtk headers
CPPFLAGS_old="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $LOCAL_INCLUDES -Wno-deprecated"

AC_CHECK_HEADER(vtk.h,vtk_ok="yes",vtk_ok="no")

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
  CPPFLAGS="$CPPFLAGS $VTK_INCLUDES -Wno-deprecated"

dnl  VTKPY_MODULES="$VTKHOME/python"

  AC_CACHE_VAL(salome_cv_lib_vtk,[
    AC_TRY_LINK(
#include <vtk.h>
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
