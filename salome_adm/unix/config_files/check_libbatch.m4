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

# CHECK_LIBBATCH
# This macro can be used to find libbatch and set the associated variables in
# a project based on autoconf. You can copy this file in your own project and
# eventually modify it according to your own needs.
# ----------------------------------------------------------------------------

AC_DEFUN([CHECK_LIBBATCH],[

AC_MSG_NOTICE(Checking for libbatch library)

AC_SUBST(LIBBATCH_INCLUDES)
AC_SUBST(LIBBATCH_LIBS)

LIBBATCH_INCLUDES=""
LIBBATCH_LIBS=""

libbatch_ok=no

AC_ARG_WITH([libbatch],
			[AS_HELP_STRING([--with-libbatch=DIR],
							[root directory path of libBatch installation])],
			[LIBBATCH_DIR="$withval"],
			[LIBBATCH_DIR=""])

if test "x$LIBBATCH_DIR" = "x" ; then
  # no --with-libbatch option used
  if test "x$LIBBATCH_ROOT_DIR" != "x" ; then
    # LIBBATCH_ROOT_DIR environment variable defined
    LIBBATCH_DIR=$LIBBATCH_ROOT_DIR
  fi
fi

LOCAL_INCLUDES=""
LOCAL_LIBS=""

if test "x$LIBBATCH_DIR" != "x" ; then
  LOCAL_INCLUDES="-I$LIBBATCH_DIR/include"
  if test "x$LIBBATCH_DIR" = "x/usr" ; then
    LOCAL_LIBS="-lbatch"
  else
    LOCAL_LIBS="-L$LIBBATCH_DIR/lib -lbatch"
  fi
else
  LOCAL_INCLUDES="-I/usr/include"
  LOCAL_LIBS="-lbatch"
fi

# libbatch headers

CPPFLAGS_old="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $LOCAL_INCLUDES"
AC_CHECK_HEADER(libbatch/BatchManager.hxx,
				libbatch_ok="yes",
				libbatch_ok="no")
CPPFLAGS="$CPPFLAGS_old"


if  test "x$libbatch_ok" = "xyes" ; then
  # libbatch library

  LIBS_old=$LIBS
  LIBS="$LIBS $LOCAL_LIBS"
  # Can't use AC_SEARCH_LIBS or AC_CHECK_LIB here because
  # they don't work with C++ namespaces
  # AC_SEARCH_LIBS(Batch::BatchManagerCatalog::getInstance,Batch,
  #				   libbatch_ok="yes",libbatch_ok="no")
  AC_MSG_CHECKING(
  			[for library containing Batch::BatchManagerCatalog::getInstance])
  AC_LINK_IFELSE([AC_LANG_PROGRAM([[
                                    namespace Batch {
                                      class BatchManagerCatalog {
                                        public: static void getInstance();
                                      };
                                    }
                                  ]],
                                  [using namespace Batch;
                                   BatchManagerCatalog::getInstance();])],
                 [libbatch_ok="yes"],
                 [libbatch_ok="no"])
  AC_MSG_RESULT($libbatch_ok)
  
  LIBS=$LIBS_old
fi

if test "x$libbatch_ok" = "xyes" ; then
  LIBBATCH_INCLUDES="$LOCAL_INCLUDES"
  LIBBATCH_LIBS="$LOCAL_LIBS"
fi

AC_MSG_RESULT(for libbatch: $libbatch_ok)

])
