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
AC_DEFUN([CHECK_OPENGL],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_ARG_WITH(opengl,
   [  --with-opengl=DIR root directory path of OpenGL installation ],
   [
      opengl_dir="$withval"
      local_opengl=yes
    ], [
      dirs="/usr/lib /usr/local/lib /opt/graphics/OpenGL/lib /usr/openwin/lib /usr/X11R6/lib"
    ])dnl

AC_CHECKING(for OpenGL)

AC_SUBST(OGL_INCLUDES)
AC_SUBST(OGL_LIBS)

OGL_INCLUDES=""
OGL_LIBS=""

OpenGL_ok=no

dnl openGL headers

# by default

if test "x$local_opengl" = "xyes" ; then
   if test -f "${opengl_dir}/include/GL/gl.h" ; then
      OpenGL_ok=yes
      OGL_INCLUDES="-I${opengl_dir}/include"
      OGL_LIBS="-L${opengl_dir}/lib"
      AC_MSG_RESULT(select OpenGL distribution in ${opengl_dir})
   else
      AC_MSG_RESULT(no gl.h header file in ${opengl_dir}/include/GL)
   fi
fi

if  test "x$OpenGL_ok" = "xno"
then
  AC_CHECK_HEADERS(GL/gl.h, [OpenGL_ok=yes])
fi

if  test "x$OpenGL_ok" = "xno"
then
# under SunOS ?
  AC_CHECK_HEADERS(/usr/openwin/share/include/GL/glxmd.h,
                  [OpenGL_ok=yes]
                  OGL_INCLUDES="-I/usr/openwin/share/include/")
fi

if  test "x$OpenGL_ok" = "xno"
then
# under IRIX ?
  AC_CHECK_HEADERS(/opt/graphics/OpenGL/include/GL/glxmd.h,
                  [OpenGL_ok=yes]
                  OGL_INCLUDES="-I/opt/graphics/OpenGL/include")
fi
if  test "x$OpenGL_ok" = "xno"
then
# some linux OpenGL servers hide the includes in /usr/X11R6/include/GL
  AC_CHECK_HEADERS(/usr/X11R6/include/GL/gl.h,
                  [OpenGL_ok=yes]
                  OGL_INCLUDES="-I/usr/X11R6/include")
fi

if  test "x$OpenGL_ok" = "xyes"
then
  AC_CHECKING(for OpenGL library)
  OpenGL_ok=no
  for i in $dirs; do
    if test -r "$i/libGL.so"; then
dnl      AC_MSG_RESULT(in $i)
      OGL_LIBS="-L$i"
      break
    fi
# under IRIX ?
    if test -r "$i/libGL.sl"; then
dnl      AC_MSG_RESULT(in $i)
      OGL_LIBS="-L$i"
      break
    fi
  done
  LDFLAGS_old="$LDFLAGS"
  LDFLAGS="$LDFLAGS $OGL_LIBS"
  AC_CHECK_LIB(GL,glBegin,OpenGL_ok=yes,OpenGL_ok=no)
  LDFLAGS="$LDFLAGS_old"
fi

if test "x$OpenGL_ok" = "xyes" ; then
  OGL_LIBS="$OGL_LIBS -lGL"
fi


OpenGLU_ok=no
LDFLAGS_old="$LDFLAGS"
LDFLAGS="$LDFLAGS $OGL_LIBS"
AC_CHECK_LIB(GLU,gluBeginSurface,OpenGLU_ok=yes,OpenGLU_ok=no)
LDFLAGS="$LDFLAGS_old"

if test "x$OpenGLU_ok" = "xyes" ; then
  OGL_LIBS="$OGL_LIBS -lGLU"
fi

# Save cache
AC_CACHE_SAVE

])dnl
