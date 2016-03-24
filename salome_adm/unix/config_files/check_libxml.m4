dnl Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

AC_DEFUN([CHECK_LIBXML],[

AC_CHECKING(for libxml library)

AC_LANG_SAVE
AC_LANG_C

AC_SUBST(LIBXML_INCLUDES)
AC_SUBST(LIBXML_LIBS)

LIBXML_INCLUDES=""
LIBXML_LIBS=""

AC_CHECKING(for libxml location)
AC_ARG_WITH(libxml,
            [  --with-libxml=DIR      root directory path to LIBXML library installation ],
            [LIBXML_DIR="$withval"
             AC_MSG_RESULT("select $withval as path to LIBXML library")])

AC_MSG_RESULT(\$LIBXML_DIR = ${LIBXML_DIR})

libxml_ok=no

LOCAL_INCLUDES=""
LOCAL_LIBS=""

if test "x$LIBXML_DIR" != "x"
then
  LOCAL_INCLUDES="-I$LIBXML_DIR/include/libxml2"
  if test "x$LIBXML_DIR" = "x/usr"
  then
    AC_MSG_NOTICE(Trying native Libxml2...)
    LOCAL_LIBS="-lxml2"
  else
    AC_MSG_NOTICE(Trying Libxml2 from $LIBXML_DIR ...)
    LOCAL_LIBS="-L$LIBXML_DIR/lib -lxml2"
  fi
else
  AC_MSG_NOTICE(Trying native Libxml2...)
  LOCAL_INCLUDES="-I/usr/include/libxml2"
  LOCAL_LIBS="-lxml2"
fi

dnl libxml2 headers

CPPFLAGS_old="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $LOCAL_INCLUDES"
AC_CHECK_HEADER(libxml/parser.h,libxml_ok="yes",libxml_ok="no")
CPPFLAGS="$CPPFLAGS_old"

if  test "x$libxml_ok" = "xyes"
then

dnl libxml2 library

  LIBS_old=$LIBS
  LIBS="$LIBS $LOCAL_LIBS"
  AC_CHECK_LIB(xml2,xmlInitParser,libxml_ok="yes",libxml_ok="no",)
  LIBS=$LIBS_old
fi

if test "x$libxml_ok" = "xyes"
then
  LIBXML_INCLUDES="$LOCAL_INCLUDES"
  LIBXML_LIBS="$LOCAL_LIBS"
fi

AC_MSG_RESULT(for libxml: $libxml_ok)

AC_LANG_RESTORE

])dnl
dnl

