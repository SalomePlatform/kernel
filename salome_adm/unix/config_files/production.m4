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

if test "X$enable_production" = "Xyes"; then
  CFLAGS="$CFLAGS -O"
  CXXFLAGS="$CXXFLAGS -O"
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

