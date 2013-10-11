`dnl Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
dnl
dnl Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
dnl CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public
dnl License as published by the Free Software Foundation; either
dnl version 2.1 of the License.
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

AC_DEFUN([CHECK_SIMANIO],[

SIMANIO_CPPFLAGS=""
SIMANIO_LIBS=""

AC_CHECKING(for SimanIO location)
AC_ARG_WITH(simanio,
   [AC_HELP_STRING([--with-simanio=DIR],[root directory path to SimanIO installation])],
   [SIMANIODIR="$withval"
    AC_MSG_RESULT("select $withval as path to SimanIO installation")
   ])

if test "x$SIMANIODIR" = "x" ; then
  # no --with-simanio option used
  if test "x$SIMANIO_ROOT_DIR" != "x" ; then
    # SIMANIO_ROOT_DIR environment variable defined
    SIMANIODIR=$SIMANIO_ROOT_DIR
  fi
fi

AC_MSG_RESULT(\$SIMANIODIR = ${SIMANIODIR})

CPPFLAGS_old="${CPPFLAGS}"
LIBS_old=$LIBS

LIB_SUFFIX="${LIB_LOCATION_SUFFIX}"

if test "x${SIMANIODIR}" != "x" ; then
  SIMANIO_CPPFLAGS="-I${SIMANIODIR}/include"
  SIMANIO_LIBS="-L${SIMANIODIR}/lib"
fi

if test "x${SIMANIODIR}" = "x/usr" ; then
  SIMANIO_CPPFLAGS=""
  SIMANIO_LIBS=""
fi

simanio_ok=no
simanio_headers_ok=no
simanio_binaries_ok=no

dnl SIMANIO headers
#AC_CHECKING(for SimanIO headers)

simanio_headers_ok=yes
if test "x${SIMANIODIR}" != "x" ; then
  AC_CHECK_FILE(${SIMANIODIR}/include/SimanIO_Link.hxx,
                simanio_headers_ok=yes,
                simanio_headers_ok=no)
fi

if test "x${simanio_headers_ok}" = "xno" ; then
  SIMANIO_CPPFLAGS="SIMANIO_CPPFLAGS_NOT_DEFINED"
else
  #AC_MSG_RESULT(\$SIMANIO_CPPFLAGS = ${SIMANIO_CPPFLAGS})
  AC_CHECK_FILE(${SIMANIODIR}/lib/libSimanIO.so,
                simanio_lib_ok=yes,
                simanio_lib_ok=no)
fi
#AC_MSG_RESULT(for SimanIO libraries: $simanio_lib_ok)

if test "x${simanio_headers_ok}" = "xyes" ; then
  if test "x${simanio_lib_ok}" = "xyes" ; then
    SIMANIO_LIBS="-L${SIMANIODIR}/lib -lSimanIO"
    simanio_ok=yes
  fi
fi

AC_MSG_RESULT(for simanio: $simanio_ok)

AC_SUBST(SIMANIO_CPPFLAGS)
AC_SUBST(SIMANIO_LIBS)

])dnl
