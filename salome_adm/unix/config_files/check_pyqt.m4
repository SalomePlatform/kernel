dnl Copyright (C) 2003  CEA/DEN, EDF R&D

AC_DEFUN([CHECK_PYQT],[
AC_REQUIRE([CHECK_PYTHON])dnl

AC_CHECKING(for pyqt)

pyqt_ok=no

PYTHON_SITE_PACKPYQT=$PYTHON_PREFIX/lib/python$PYTHON_VERSION/site-packages
AC_CHECK_FILES($PYTHON_SITE_PACKPYQT/qt.py $PYTHON_SITE_PACKPYQT/libqtcmodule.so,pyqt_ok=yes,pyqt_ok=no)
if test "x$pyqt_ok" = xyes ; then
   PYQT_INCLUDES="-I $PYTHON_SITE_PACKPYQT"
   PYQT_LIBS="-L$PYTHON_SITE_PACKPYQT -lqtcmodule"
fi

if test "x$pyqt_ok" = xyes ; then
   AC_CHECK_FILES(/usr/share/sip/qt/qtmod.sip,pyqt_ok=yes,pyqt_ok=no)
   if test "x$pyqt_ok" = xyes ; then
      PYQT_SIPS="/usr/share/sip/qt"
   fi
   AC_SUBST(PYQT_INCLUDES)
   AC_SUBST(PYQT_LIBS)
   AC_SUBST(PYQT_SIPS)
   AC_MSG_RESULT(yes)
else

  pyqt_ok=yes
  dnl were is pyqt ?

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


if test -z $PYQTDIR; then
   PYQTDIR="/usr/lib/python${PYTHON_VERSION}/site-packages"	
else
   if test -z $PYQT_SIPS; then
      PYQT_SIPS="$PYQTDIR/sip"	
   fi	
   if test -z $PYUIC; then
      AC_CHECK_FILE("$PYQTDIR/pyuic",pyqt_ok=yes,pyqt_ok=no)
      if test "x$pyqt_ok" = xyes ; then	
         PYUIC="$PYQTDIR/pyuic"
      else
         AC_PATH_PROG(PYUIC, pyuic)
      fi	
   fi	
fi

if test -z $PYQT_SIPS; then
   PYQT_SIPS="/usr/share/sip/qt"	
fi

if test -z $PYUIC; then
   PYUIC="/usr/bin/pyuic"	
fi

AC_CHECK_FILES("$PYQTDIR/qt.py","$PYQTDIR/qt/qt.py",pyqt_ok=yes,pyqt_ok=no)
if test "x$pyqt_ok" = xno ; then
   AC_MSG_RESULT(no)
   AC_MSG_WARN(pyqt not found)
else
   AC_CHECK_FILE("$PYQTDIR/libqtcmodule.so",pyqt_ok=yes,pyqt_ok=no)
   if test "x$pyqt_ok" = xno ; then
      AC_MSG_RESULT(no)
      AC_MSG_WARN(pyqt not found)
   else
      PYQT_ROOT=$PYQTDIR
      PYQT_LIBS="-L$PYQTDIR -lqtcmodule"
   fi
   AC_CHECK_FILE("$PYQT_SIPS/copying.sip",pyqt_ok=yes,pyqt_ok=no)
   if test "x$pyqt_ok" = xno ; then
      AC_MSG_RESULT(no)
      AC_MSG_WARN(pyqt not found)
   else
      PYQT_INCLUDES="-I$PYQT_SIPS"
   fi

   AC_SUBST(PYQT_ROOT)
   AC_SUBST(PYQT_INCLUDES)
   AC_SUBST(PYQT_LIBS)

   AC_MSG_RESULT(yes)
fi

PYQT_ROOT=$PYQTDIR
PYQT_INCLUDES="-I$PYQT_SIPS"
PYQT_LIBS="-L$PYQTDIR -lqtcmodule"

AC_SUBST(PYQT_ROOT)
AC_SUBST(PYQT_INCLUDES)
AC_SUBST(PYUIC)
AC_SUBST(PYQT_SIPS)
AC_SUBST(PYQT_LIBS)



AC_MSG_RESULT(for pyqt: $pyqt_ok)

fi
])dnl
dnl


