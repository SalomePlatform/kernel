
AC_DEFUN([CHECK_PYQT],[
AC_REQUIRE([CHECK_PYTHON])dnl

AC_CHECKING(for pyqt)

pyqt_ok=no

dnl were is pyqt ?

if test -z $PYQTDIR; then
  AC_MSG_WARN(You must provide PYQTDIR variable)
else
  pyqt_ok=yes
fi

if test "x$pyqt_ok" = xno -o ! -d "$PYQTDIR" -o ! -d "$PYQTDIR"/sip ; then
  AC_MSG_RESULT(no)
  AC_MSG_WARN(pyqt not found)
else
  AC_CHECK_FILES("$PYQTDIR"/qt.py "$PYQTDIR"/libqtcmodule.so,pyqt_ok=yes,pyqt_ok=no) 
  if test "x$pyqt_ok" = xno ; then
     AC_MSG_RESULT(no)
     AC_MSG_WARN(pyqt not found)
  else
     PYQT_ROOT=$PYQTDIR
     PYQT_INCLUDES="-I$PYQTDIR/sip"
     PYQT_LIBS="-L$PYQTDIR -lqtcmodule"

     AC_SUBST(PYQT_ROOT)
     AC_SUBST(PYQT_INCLUDES)
     AC_SUBST(PYQT_LIBS)

     AC_MSG_RESULT(yes)
  fi
fi

AC_ARG_WITH(pyuic,
    [  --with-pyuic=EXEC pyuic executable ],
    [PYUIC="$withval"
      AC_MSG_RESULT("select $withval as pyqt executable")
    ], [
      AC_PATH_PROG(PYUIC, pyuic)
    ])

if test "x$PYUIC" = "x"
then
    pyqt_ok=no
    AC_MSG_RESULT(pyuic not in PATH variable)
else
	if test -z $PYQT_SIPS; then
	  AC_MSG_WARN(You must provide PYQT_SIPS variable)
	  pyqt_ok=no
	else
	  PYQT_SIPS=${PYQT_SIPS}
	fi
	PYQT_LIBS="-L${PYTHON_PREFIX}/lib/python${PYTHON_VERSION}/site-packages ${PYQT_LIBS}"
fi

AC_SUBST(PYUIC)
AC_SUBST(PYQT_SIPS)
AC_SUBST(PYQT_LIBS)



AC_MSG_RESULT(for pyqt: $pyqt_ok)

])dnl
dnl


