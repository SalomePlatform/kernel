
AC_DEFUN([CHECK_SWIG],[
AC_REQUIRE([CHECK_PYTHON])dnl

swig_ok=yes

AC_ARG_WITH(swig,
    [  --with-swig=EXEC swig executable ],
    [SWIG="$withval"
      AC_MSG_RESULT("select $withval as swig executable")
    ], [
      AC_PATH_PROG(SWIG, swig)
    ])

if test "x$SWIG" = "x"
then
    swig_ok=no
    AC_MSG_RESULT(swig not in PATH variable)
fi

if  test "x$swig_ok" = "xyes"
then
   AC_MSG_CHECKING(python wrapper generation with swig)
   cat > conftest.h << EOF
int f(double);
EOF

   $SWIG -module conftest -python conftest.h >/dev/null 2>&1
   if test -f conftest_wrap.c
   then
      SWIG_FLAGS="-c++ -python -shadow"
   else
      swig_ok=no  
   fi
   rm -f conftest*
   AC_MSG_RESULT($swig_ok) 
fi

AC_SUBST(SWIG_FLAGS)
AC_SUBST(SWIG)

AC_MSG_RESULT(for swig: $swig_ok)

])dnl
dnl
