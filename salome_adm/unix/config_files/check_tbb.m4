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

AC_DEFUN([CHECK_TBB],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl

AC_CHECKING(for TBB)

AC_ARG_WITH(TBB,
    [  --with-TBB=DIR                 root directory path to TBB installation ],
    [TBBHOME="$withval"
      AC_MSG_RESULT("select $withval as path to TBB")
    ])

AC_SUBST(TBB_INCLUDES)
AC_SUBST(TBB_LIBS)

TBB_INCLUDES=""
TBB_LIBS=""

tbb_ok=no

LOCAL_INCLUDES=""
LOCAL_LIBS="-ltbb -ltbbmalloc"

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

CPPFLAGS_old=$CPPFLAGS

if test "x$TBBHOME" != "xno"; then
    if test "x$TBBHOME" == "xyes"; then
        TBBHOME=""
    fi
    if test -z $TBBHOME
    then
        AC_MSG_WARN(undefined TBBHOME variable which specify TBB library installation directory)
        AC_PATH_PROG(BINDIR, tbbvars.sh)
        if test "x$BINDIR" != "x" ; then
            dnl e.g. BINDIR = ./tbb/bin/intel64/cc4.1.0_libc2.4_kernel2.6.16.21/tbbvars.sh
            TBBHOME=$BINDIR
            TBBHOME=`echo ${TBBHOME} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
            TBBHOME=`echo ${TBBHOME} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
            TBBHOME=`echo ${TBBHOME} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
            TBBHOME=`echo ${TBBHOME} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
        fi
    fi
    if test ! -z $TBBHOME
    then
        TBBKERNEL='cc4.1.0_libc2.4_kernel2.6.16.21'
        LOCAL_INCLUDES="-I$TBBHOME/include"
        if test "x$TBBHOME" != "x/usr"; then
	    INTEL_LOCAL_LIB=""
            if test `uname -m` = "x86_64" ; then
                INTEL_LOCAL_LIB="$TBBHOME/lib/intel64/$TBBKERNEL"
            else
                INTEL_LOCAL_LIB="$TBBHOME/lib/ia32/$TBBKERNEL"
            fi
	    if test ! -d ${INTEL_LOCAL_LIB} ; then
	        INTEL_LOCAL_LIB="${TBBHOME}/lib"
	    fi
	    if test -d ${INTEL_LOCAL_LIB} ; then
                LOCAL_LIBS="-L${INTEL_LOCAL_LIB} $LOCAL_LIBS"
	    fi
        fi
    fi

dnl check tbb header

    CPPFLAGS="$CPPFLAGS $LOCAL_INCLUDES"

    AC_CHECK_HEADER(tbb/tbb.h,tbb_ok=yes ,tbb_ok=no)
fi

if  test "x$tbb_ok" = "xyes"
then

dnl check TBB library

  LIBS_old="$LIBS"
  LIBS="$LIBS $LOCAL_LIBS"
  AC_TRY_LINK([#include <tbb/tbb.h>],
              [tbb::simple_partitioner()],
              tbb_ok=yes,tbb_ok=no)

  LIBS="$LIBS_old"
 
fi

CPPFLAGS=$CPPFLAGS_old
AC_LANG_RESTORE

AC_MSG_RESULT(for TBB: $tbb_ok)

if  test "x$tbb_ok" = "xyes"
then
  TBB_LIBS="$LOCAL_LIBS"
  TBB_INCLUDES="$LOCAL_INCLUDES"
  CPPFLAGS="-DWITH_TBB $CPPFLAGS"
fi

AM_CONDITIONAL(WITH_TBB, [test x"$tbb_ok" = xyes])

])dnl
