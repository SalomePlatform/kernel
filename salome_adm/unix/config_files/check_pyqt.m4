
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

  dnl were is pyqt ?

  if test -z $PYQTDIR; then
    AC_MSG_WARN(You must provide PYQTDIR variable)
  else
    pyqt_ok=yes
  fi

  version=`python -c "import qt;print qt.PYQT_VERSION"`
  case "$version" in
     3.2*)
       pyqt_vers=old ;;
     3.3*)
       pyqt_vers=old ;;
     3.4*)
       pyqt_vers=new ;;
     3.5*)
       pyqt_vers=new ;;
        *)
       pyqt_vers=no ;;
  esac

  if test "x$pyqt_ok" = xno -o ! -d "$PYQTDIR" ; then
    AC_MSG_RESULT(no)
    AC_MSG_WARN(pyqt not found)
  else

    pyqt_ok=no

    if test "x$pyqt_vers" = "xold"
    then
        AC_CHECK_FILES("$PYQTDIR"/qt.py "$PYQTDIR"/libqtcmodule.so,pyqt_ok=yes,pyqt_ok=no) 
        if test "x$pyqt_ok" = xno -o ! -d "$PYQTDIR"/sip ; then
           AC_MSG_RESULT(no)
           AC_MSG_WARN(pyqt not found)
        else
           PYQT_ROOT=$PYQTDIR
           PYQT_INCLUDES="-I$PYQTDIR/sip"
	   PYQT_LIBS="-L${PYTHON_PREFIX}/lib/python${PYTHON_VERSION}/site-packages -L$PYQTDIR -lqtcmodule"
           AC_SUBST(PYQT_ROOT)
           AC_SUBST(PYQT_INCLUDES)
           AC_SUBST(PYQT_LIBS)
           AC_MSG_RESULT(yes)
        fi
    fi

    if test "x$pyqt_vers" = "xnew"
    then
        AC_CHECK_FILES("$PYQTDIR"/lib/qt.py "$PYQTDIR"/lib/libqtcmodule.so,pyqt_ok=yes,pyqt_ok=no) 
        if test "x$pyqt_ok" = xno ; then
           AC_MSG_RESULT(no)
           AC_MSG_WARN(pyqt not found)
        else
           PYQT_ROOT=$PYQTDIR
           PYQT_INCLUDES="-I$PYQTDIR/sip"
           PYQT_LIBS="-L$PYQTDIR/lib -lqtcmodule"
           AC_SUBST(PYQT_ROOT)
           AC_SUBST(PYQT_INCLUDES)
           AC_SUBST(PYQT_LIBS)
           AC_MSG_RESULT(yes)
        fi
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
  fi

  AC_SUBST(PYUIC)
  AC_SUBST(PYQT_SIPS)

  AC_MSG_RESULT(for pyqt: $pyqt_ok)
fi

])dnl
dnl


