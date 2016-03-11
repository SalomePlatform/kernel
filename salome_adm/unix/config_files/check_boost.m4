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

AC_DEFUN([CHECK_BOOST],[

AC_CHECKING(for BOOST Library)

AC_REQUIRE([ENABLE_PTHREADS])dnl

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

gccver=`$CC -dumpversion | sed 's/^\([[0-9]]\+\)\.\([[0-9]]\+\).*/\1\2/g'`
SUFFIXES="empty -mt -gcc -gcc-mt -gcc${gccver} -gcc${gccver}-mt"

BOOST_CPPFLAGS=""
BOOST_LIBSUFFIX=""
BOOST_LIBS=""

AC_CHECKING(for BOOST location)
AC_ARG_WITH(boost,
   [AC_HELP_STRING([--with-boost=DIR],[root directory path to BOOST library installation])],
   [BOOSTDIR="$withval"
    AC_MSG_RESULT("select $withval as path to BOOST library")
   ])
   
if test "x${BOOSTDIR}" = "x" ; then
  BOOSTDIR="/usr"
fi

AC_MSG_RESULT(\$BOOSTDIR = ${BOOSTDIR})

CPPFLAGS_old="${CPPFLAGS}"
LIBS_old=$LIBS

LIB_SUFFIX="${LIB_LOCATION_SUFFIX}"

if test "x${BOOSTDIR}" != "x" ; then
  BOOST_CPPFLAGS="-I${BOOSTDIR}/include"
  BOOST_LIBS="-L${BOOSTDIR}/lib${LIB_SUFFIX}"
fi

if test "x${BOOSTDIR}" = "x/usr" ; then
  BOOST_CPPFLAGS=""
  BOOST_LIBS=""
fi

boost_ok=no
boost_headers_ok=no
boost_binaries_ok=no

dnl BOOST headers
AC_CHECKING(for BOOST headers)
CPPFLAGS="${CPPFLAGS_old} ${BOOST_CPPFLAGS}"

boost_include_dir_ok=yes
if test "x${BOOSTDIR}" != "x" ; then
  AC_CHECK_FILE(${BOOSTDIR}/include/boost/shared_ptr.hpp,
                boost_include_dir_ok=yes,
                boost_include_dir_ok=no)
fi

BOOST_PROGRAM_OPTIONS_LIB=no
if test "x${boost_include_dir_ok}" = "xyes" ; then
  AC_CHECK_FILE(${BOOSTDIR}/include/boost/program_options.hpp,
                BOOST_PROGRAM_OPTIONS_LIB=yes,
                BOOST_PROGRAM_OPTIONS_LIB=no)
fi
AC_MSG_RESULT(for boost program_options tool: $BOOST_PROGRAM_OPTIONS_LIB)

if test "x${boost_include_dir_ok}" = "xyes" ; then
  AC_TRY_COMPILE([#include <boost/shared_ptr.hpp>],
                 [boost::shared_ptr<int>(new int)],
                 boost_headers_ok=yes,
                 boost_headers_ok=no)
fi

if test "x${boost_headers_ok}" = "xno" ; then
  BOOST_CPPFLAGS="BOOST_CPPFLAGS_NOT_DEFINED"
else
  AC_MSG_RESULT(\$BOOST_CPPFLAGS = ${BOOST_CPPFLAGS})
fi
AC_MSG_RESULT(for boost headers: $boost_headers_ok)

if test "x${boost_headers_ok}" = "xyes" ; then
  dnl BOOST binaries
  AC_CHECKING(for BOOST binaries)
  boost_lib_dir_ok=yes
  if test "x${BOOSTDIR}" != "x" ; then
    for BOOST_LIBSUFFIX in ${SUFFIXES} ; do
      test "${BOOST_LIBSUFFIX}" == "empty" && BOOST_LIBSUFFIX=""
      AC_CHECK_FILE([${BOOSTDIR}/lib${LIB_SUFFIX}/libboost_thread${BOOST_LIBSUFFIX}.so],
                    [boost_lib_dir_ok=yes],
                    [AC_CHECK_FILE([${BOOSTDIR}/lib64/libboost_thread${BOOST_LIBSUFFIX}.so],
		                   [boost_lib_dir_ok=yes; LIB_SUFFIX=64],
		                   [boost_lib_dir_ok=no])
                    ])
      if test "x${boost_lib_dir_ok}" = "xyes" ; then
        break
      fi
    done
  fi
  if test "x${boost_lib_dir_ok}" = "xyes" ; then
    LIBS="${LIBS_old} ${BOOST_LIBS} -lboost_thread${BOOST_LIBSUFFIX} -lboost_system${BOOST_LIBSUFFIX}"
    AC_TRY_LINK([#include <boost/thread/thread.hpp>],
                [struct TBody{ void operator()(){} }; boost::thread(TBody())],
                boost_binaries_ok=yes,
                boost_binaries_ok=no)
    if test "x${boost_binaries_ok}" = "xyes" ; then
      break
    fi
  fi
fi

if test "x${boost_binaries_ok}" = "xno" ; then
  BOOST_LIBS="BOOST_LIBS_NOT_FOUND"
  BOOST_LIBSUFFIX="-not-defined"
else
  AC_MSG_RESULT(\$BOOST_LIBSUFFIX = ${BOOST_LIBSUFFIX})
  AC_MSG_RESULT(\$BOOST_LIBS = ${BOOST_LIBS})
  AC_CHECK_FILE(${BOOSTDIR}/lib${LIB_SUFFIX}/libboost_thread${BOOST_LIBSUFFIX}.so,
                BOOST_LIB_THREAD="${BOOST_LIBS} -lboost_thread${BOOST_LIBSUFFIX} -lboost_system${BOOST_LIBSUFFIX}",
                BOOST_LIB_THREAD="")
  AC_CHECK_FILE(${BOOSTDIR}/lib${LIB_SUFFIX}/libboost_signals${BOOST_LIBSUFFIX}.so,
                BOOST_LIB_SIGNALS="${BOOST_LIBS} -lboost_signals${BOOST_LIBSUFFIX}",
                BOOST_LIB_SIGNALS="")
  AC_CHECK_FILE(${BOOSTDIR}/lib${LIB_SUFFIX}/libboost_system${BOOST_LIBSUFFIX}.so,
                BOOST_LIB_SYSTEM="${BOOST_LIBS} -lboost_system${BOOST_LIBSUFFIX}",
                BOOST_LIB_SYSTEM="")
  AC_CHECK_FILE(${BOOSTDIR}/lib${LIB_SUFFIX}/libboost_regex${BOOST_LIBSUFFIX}.so,
                BOOST_LIB_REGEX="${BOOST_LIBS} -lboost_regex${BOOST_LIBSUFFIX}",
                BOOST_LIB_REGEX="")
  AC_CHECK_FILE(${BOOSTDIR}/lib${LIB_SUFFIX}/libboost_filesystem${BOOST_LIBSUFFIX}.so,
                BOOST_LIB_FILESYSTEM="${BOOST_LIBS} -lboost_filesystem${BOOST_LIBSUFFIX}",
                BOOST_LIB_FILESYSTEM="")
fi
AC_MSG_RESULT(for boost binaries: $boost_binaries_ok)

CPPFLAGS="${CPPFLAGS_old}"
LIBS="${LIBS_old}"

if test "x${boost_headers_ok}" = "xyes" ; then
  if test "x${boost_binaries_ok}" = "xyes" ; then
    boost_ok=yes
  fi
fi

AC_MSG_RESULT(for boost: $boost_ok)

AC_SUBST(BOOST_CPPFLAGS)
AC_SUBST(BOOST_LIBSUFFIX)
AC_SUBST(BOOST_LIBS)
AC_SUBST(BOOST_LIB_THREAD)
AC_SUBST(BOOST_LIB_SIGNALS)
AC_SUBST(BOOST_LIB_SYSTEM)
AC_SUBST(BOOST_LIB_REGEX)
AC_SUBST(BOOST_LIB_FILESYSTEM)
AC_SUBST(BOOST_PROGRAM_OPTIONS_LIB)

AC_LANG_RESTORE

])dnl


