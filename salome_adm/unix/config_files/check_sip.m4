dnl Copyright (C) 2003  CEA/DEN, EDF R&D

AC_DEFUN([CHECK_SIP],[
AC_REQUIRE([CHECK_PYTHON])dnl

sip_ok=yes

AC_ARG_WITH(sip,
    [  --with-sip=EXEC sip executable ],
    [SIP="$withval"
      AC_MSG_RESULT("select $withval as sip executable")
    ], [
      AC_PATH_PROG(SIP, sip)
    ])

if test "x$SIP" = "x"
then
    sip_ok=no
    AC_MSG_RESULT(sip not in PATH variable)
else
    version=`sip -V`
    case "$version" in
         3.2*)
           SIP_VERS=old ;;
         3.3*)
           SIP_VERS=old ;;
         3.4*)
           SIP_VERS=new ;;
         3.5*)
           SIP_VERS=new ;;
         3.6*)
           SIP_VERS=new ;;
         3.7*)
           SIP_VERS=new ;;
         3.8*)
           SIP_VERS=new ;;
         3.9*)
           SIP_VERS=new ;;
         3.10*)
           SIP_VERS=new2 ;;
         *)
           AC_MSG_RESULT(sip version $version not supported)
           SIP_VERS=no ;;
    esac
    sip_ok=no

    if test "x$SIP_VERS" = "xold"
    then
        dnl old install : includes and libs are in python install or in SIPDIR
        AC_MSG_RESULT(old install detected)
        sip_ok=yes
	SIP_ROOT="$SIPDIR"
	SIP_INCLUDES="${PYTHON_INCLUDES} -I${SIPDIR}"
	SIP_LIBS="-L${PYTHON_PREFIX}/lib/python${PYTHON_VERSION}/site-packages -L${SIPDIR} -lsip"
    fi

    if test "x$SIP_VERS" = "xnew"
    then
	sip_ok=yes
	if test -d ${SIPDIR}/include/python${PYTHON_VERSION} ; then
           AC_MSG_RESULT(new install with include dir detected)
           dnl new install with include dir : includes and libs are in python install or in SIPDIR/include
	   SIP_ROOT="$SIPDIR"
	   SIP_INCLUDES="${PYTHON_INCLUDES} -I${SIPDIR}/include/python${PYTHON_VERSION}"
	   SIP_LIBS="-L${SIPDIR}/lib/python${PYTHON_VERSION}/site-packages -lsip"
	else
           dnl new install without include dir : includes and libs are in SIPDIR
           AC_MSG_RESULT(new install without include dir detected)
	   sip_ok=yes
	   SIP_ROOT="$SIPDIR"
	   SIP_INCLUDES="-I${SIPDIR}"
	   SIP_LIBS="-L${SIPDIR} -lsip"
        fi
    fi

    if test "x$SIP_VERS" = "xnew2"
        then
        dnl new2 install : includes and libs are in python install 
        AC_MSG_RESULT(new2 install detected)
	sip_ok=yes
        AC_CHECK_FILE($PYTHON_PREFIX/include/python$PYTHON_VERSION/sip.h,sip_ok=$sip_ok,sip_ok=no)
	SIP_INCLUDES="${PYTHON_INCLUDES}"
        AC_CHECK_FILE($PYTHON_PREFIX/lib/python$PYTHON_VERSION/site-packages/libsip.so,sip_ok=$sip_ok,sip_ok=no)
	SIP_LIBS="-L${PYTHON_PREFIX}/lib/python${PYTHON_VERSION}/site-packages -lsip"
    fi
fi

AC_SUBST(SIP)
AC_SUBST(SIP_INCLUDES)
AC_SUBST(SIP_LIBS)
AC_SUBST(SIP_VERS)

AC_MSG_RESULT(for sip: $sip_ok)

])dnl
dnl
