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
dnl define macros :
dnl AC_ENABLE_PRODUCTION AC_DISABLE_PRODUCTION
dnl and 
dnl AC_ENABLE_DEBUG AC_DISABLE_DEBUG
dnl
dnl version $Id$
dnl author Patrick GOLDBRONN
dnl
 
# AC_ENABLE_PRODUCTION
AC_DEFUN(AC_ENABLE_PRODUCTION, [dnl
define([AC_ENABLE_PRODUCTION_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(production,
changequote(<<, >>)dnl
<<  --enable-production[=PKGS]  build without debug information [default=>>AC_ENABLE_PRODUCTION_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_production=yes ;;
no) enable_production=no ;;
*)
  enable_production=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_production=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_production=AC_ENABLE_PRODUCTION_DEFAULT)dnl

AC_CXX_OPTION(-Wno-deprecated,CXXFLAGS)
AC_CXX_OPTION(-Wparentheses,CXXFLAGS)
AC_CXX_OPTION(-Wreturn-type,CXXFLAGS)
AC_CXX_OPTION(-Wmissing-declarations,CXXFLAGS)
AC_CXX_OPTION(-fmessage-length=0,CXXFLAGS)
AC_CXX_OPTION(-Wunused,CXXFLAGS)

if test "X$enable_production" = "Xyes"; then
  CFLAGS="$CFLAGS -O"
  AC_CXX_OPTION(-Wuninitialized,CXXFLAGS)
  CXXFLAGS="$CXXFLAGS -O "
fi
])

# AC_DISABLE_PRODUCTION - set the default flag to --disable-production
AC_DEFUN(AC_DISABLE_PRODUCTION, [AC_ENABLE_PRODUCTION(no)])

# AC_ENABLE_DEBUG
AC_DEFUN(AC_ENABLE_DEBUG, [dnl
define([AC_ENABLE_DEBUG_DEFAULT], ifelse($1, no, no, yes))dnl
AC_ARG_ENABLE(debug,
changequote(<<, >>)dnl
<<  --enable-debug[=PKGS]  build without debug information [default=>>AC_ENABLE_DEBUG_DEFAULT],
changequote([, ])dnl
[p=${PACKAGE-default}
case "$enableval" in
yes) enable_debug=yes ;;
no) enable_debug=no ;;
*)
  enable_debug=no
  # Look at the argument we got.  We use all the common list separators.
  IFS="${IFS= 	}"; ac_save_ifs="$IFS"; IFS="${IFS}:,"
  for pkg in $enableval; do
    if test "X$pkg" = "X$p"; then
      enable_debug=yes
    fi
  done
  IFS="$ac_save_ifs"
  ;;
esac],
enable_debug=AC_ENABLE_DEBUG_DEFAULT)dnl

if test "X$enable_debug" = "Xyes"; then
  CFLAGS="$CFLAGS -g -D_DEBUG_ "
  CXXFLAGS="$CXXFLAGS -g -D_DEBUG_ "
fi
])

# AC_DISABLE_DEBUG - set the default flag to --disable-debug
AC_DEFUN(AC_DISABLE_DEBUG, [AC_ENABLE_DEBUG(no)])

