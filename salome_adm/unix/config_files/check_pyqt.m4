dnl Copyright (C) 2003  CEA/DEN, EDF R&D

AC_DEFUN([CHECK_PYQT],[
AC_REQUIRE([CHECK_PYTHON])dnl

AC_ARG_WITH(pyqt,
    [  --with-pyqt=DIR      root directory path to PyQt installation ],
    [PYQTDIR="$withval"
      AC_MSG_RESULT("select $withval as path to PyQt")
    ])

AC_ARG_WITH(pyqt_sips,
    [  --with-pyqt_sips=DIR      a directory path to PyQt sips installation ],
    [PYQT_SIPS="$withval"
      AC_MSG_RESULT("select $withval as path to PyQt sips")
    ])


AC_ARG_WITH(pyuic,
    [  --with-pyuic=EXEC pyuic executable ],
    [PYUIC="$withval"
      AC_MSG_RESULT("select $withval as pyqt executable")
    ])

AC_CHECKING(for pyqt)

pyqt_ok=no

if test "x$PYQTDIR" = x; then
  PYQTDIR="/usr"
fi

if test "x$PYQT_SIPS" = x; then
  PYQT_SIPS="/usr/share/sip/qt"
fi

if test -d $PYQTDIR/lib/python${PYTHON_VERSION}/site-packages; then
  PYQTLIB=$PYQTDIR/lib/python${PYTHON_VERSION}/site-packages
else
  if test -d $PYQTDIR/lib; then
    PYQTLIB=$PYQTDIR/lib
  else
    PYQTLIB=$PYQTDIR
  fi
fi

if test -d $PYQTDIR/bin; then
  PYQTBIN=$PYQTDIR/bin
else
  PYQTBIN=$PYQTDIR
fi

AC_CHECK_FILE("$PYQTBIN/pyuic",pyqt_ok=yes,pyqt_ok=no)

if test "x$pyqt_ok" = xyes ; then
  AC_CHECK_FILES("$PYQTLIB/qt.py",pyqt_ok=yes,pyqt_ok=no)
fi

if test "x$pyqt_ok" = xno ; then
  AC_CHECK_FILES("$PYQTLIB/qt/qt.py",pyqt_ok=yes,pyqt_ok=no)
fi

if test "x$pyqt_ok" = xyes ; then
  AC_CHECK_FILE("$PYQTLIB/libqtcmodule.so",pyqt_ok=yes,pyqt_ok=no)
fi

if test "x$pyqt_ok" = xyes ; then
  AC_CHECK_FILES("$PYQT_SIPS/qtmod.sip",pyqt_ok=yes,pyqt_ok=no)
fi
if test "x$pyqt_ok" = xyes ; then
  AC_CHECK_FILE("$PYQT_SIPS/copying.sip",pyqt_ok=yes,pyqt_ok=no)
fi

if test "x$pyqt_ok" = xyes ; then
  PYQT_ROOT=$PYQTDIR
  PYQT_INCLUDES="-I$PYQT_SIPS"
  PYQT_LIBS="-L$PYQTLIB -lqtcmodule"
  AC_SUBST(PYQT_ROOT)
  AC_SUBST(PYQT_INCLUDES)
  AC_SUBST(PYQT_LIBS)
  AC_SUBST(PYQT_SIPS)
  AC_SUBST(PYUIC)
fi

])dnl
dnl


