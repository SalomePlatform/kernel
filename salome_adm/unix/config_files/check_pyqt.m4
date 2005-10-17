dnl Copyright (C) 2003  CEA/DEN, EDF R&D

AC_DEFUN([CHECK_PYQT],[
AC_REQUIRE([CHECK_PYTHON])dnl
AC_REQUIRE([CHECK_QT])dnl
AC_REQUIRE([CHECK_SIP])dnl

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

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

pyqt_ok=yes

dnl look for pyuic
pyqt_uic_ok=no
if test "x$PYUIC" != x; then
  dnl try withval value
  AC_CHECK_FILE($PYUIC,pyqt_uic_ok=yes,pyqt_uic_ok=no)
fi
if test "x$pyqt_uic_ok" == "xno"; then
  dnl try ${PYQTDIR}
  if test "x${PYQTDIR}" != "x"; then
    if test -d ${PYQTDIR} ; then
      AC_CHECK_FILE(${PYQTDIR}/pyuic,pyqt_uic_ok=yes,pyqt_uic_ok=no)
      if test "x$pyqt_uic_ok" == "xyes"; then
        PYUIC="${PYQTDIR}/pyuic"
      fi
    fi
  fi	
fi
if test "x$pyqt_uic_ok" == "xno"; then
  dnl try ${SIPDIR}
  if test "x${SIPDIR}" != "x"; then
    if test -d ${SIPDIR} ; then
      AC_CHECK_FILE(${SIPDIR}/pyuic,pyqt_uic_ok=yes,pyqt_uic_ok=no)
      if test "x$pyqt_uic_ok" == "xyes"; then
        PYUIC="${SIPDIR}/pyuic"
      fi
    fi
  fi	
fi
if test "x$pyqt_uic_ok" == "xno"; then
  dnl try ${PYTHONHOME}/bin
  if test "x${PYTHONHOME}" != "x"; then
    if test -d ${PYTHONHOME}/bin ; then
      AC_CHECK_FILE(${PYTHONHOME}/bin/pyuic,pyqt_uic_ok=yes,pyqt_uic_ok=no)
      if test "x$pyqt_uic_ok" == "xyes"; then
        PYUIC="${PYTHONHOME}/bin/pyuic"
      fi
    fi
  fi	
fi
if test "x$pyqt_uic_ok" == "xno"; then
  dnl try /usr/bin
  AC_CHECK_FILE(/usr/bin/pyuic,pyqt_uic_ok=yes,pyqt_uic_ok=no)
  if test "x$pyqt_uic_ok" == "xyes"; then
    PYUIC="/usr/bin/pyuic"
  fi
fi
if test "x$pyqt_uic_ok" == "xno"; then
  pyqt_ok=no
fi

dnl look for PyQt libs
pyqt_lib_ok=no
if test "x${PYQTDIR}" != "x"; then
  dnl try {PYQTDIR}
  AC_CHECK_FILE(${PYQTDIR}/libqtcmodule.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
  if test "x$pyqt_lib_ok" == "xyes"; then
    if test "x${PYQTDIR}" = "x/usr/lib"
    then
      PYQT_LIBS="-lqtcmodule"
    else
      PYQT_LIBS="-L${PYQTDIR} -lqtcmodule"
    fi
  else 
    AC_CHECK_FILE(${PYQTDIR}/qt.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
    if test "x$pyqt_lib_ok" == "xyes"; then
      if test "x${PYQTDIR}" = "x/usr/lib"
      then
        PYQT_LIBS=""
      else
        PYQT_LIBS="-L${PYQTDIR}"
      fi
    fi
  fi
  if test "x$pyqt_lib_ok" == "xno"; then
    dnl try {PYQTDIR}/lib
    if test -d {PYQTDIR}/lib; then
      AC_CHECK_FILE(${PYQTDIR}/lib/libqtcmodule.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
      if test "x$pyqt_lib_ok" == "xyes"; then
        PYQT_LIBS="-L${PYQTDIR}/lib -lqtcmodule"
      else 
        AC_CHECK_FILE(${PYQTDIR}/lib/qt.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
        if test "x$pyqt_lib_ok" == "xyes"; then
          PYQT_LIBS="-L${PYQTDIR}/lib"
        fi
      fi
    fi
  fi
  if test "x$pyqt_lib_ok" == "xno"; then
    dnl try {PYQTDIR}/lib/python${PYTHON_VERSION}/site-packages
    if test -d {PYQTDIR}/lib/python${PYTHON_VERSION}/site-packages; then
      AC_CHECK_FILE(${PYQTDIR}/lib/python${PYTHON_VERSION}/site-packages/libqtcmodule.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
      if test "x$pyqt_lib_ok" == "xyes"; then
        PYQT_LIBS="-L${PYQTDIR}/lib/python${PYTHON_VERSION}/site-packages -lqtcmodule"
      else 
        AC_CHECK_FILE(${PYQTDIR}/lib/python${PYTHON_VERSION}/site-packages/qt.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
        if test "x$pyqt_lib_ok" == "xyes"; then
          PYQT_LIBS="-L${PYQTDIR}/lib/python${PYTHON_VERSION}/site-packages"
        fi
      fi
    fi
  fi
fi
if test "x$pyqt_lib_ok" == "xno"; then
  dnl try ${SIPDIR}
  if test "x${SIPDIR}" != "x"; then
    if test -d ${SIPDIR} ; then
      AC_CHECK_FILE(${SIPDIR}/libqtcmodule.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
      if test "x$pyqt_lib_ok" == "xyes"; then
        if test "x${SIPDIR}" = "x/usr/lib"
        then
          PYQT_LIBS="-lqtcmodule"
        else
          PYQT_LIBS="-L${SIPDIR} -lqtcmodule"
        fi
      else 
        AC_CHECK_FILE(${SIPDIR}/qt.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
        if test "x$pyqt_lib_ok" == "xyes"; then
          if test "x${SIPDIR}" = "x/usr/lib"
          then
            PYQT_LIBS=""
          else
            PYQT_LIBS="-L${SIPDIR}"
          fi
        fi
      fi
    fi
  fi
fi
if test "x$pyqt_lib_ok" == "xno"; then
  dnl try ${PYTHONHOME}/lib/python${PYTHON_VERSION}/site-packages
  if test "x${PYTHONHOME}" != "x"; then
    if test -d ${PYTHONHOME}/lib/python${PYTHON_VERSION}/site-packages ; then
      AC_CHECK_FILE(${PYTHONHOME}/lib/python${PYTHON_VERSION}/site-packages/libqtcmodule.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
      if test "x$pyqt_lib_ok" == "xyes"; then
        PYQT_LIBS="-L${PYTHONHOME}/lib/python${PYTHON_VERSION}/site-packages -lqtcmodule"
      else 
        AC_CHECK_FILE(${PYTHONHOME}/lib/python${PYTHON_VERSION}/site-packages/qt.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
        if test "x$pyqt_lib_ok" == "xyes"; then
          PYQT_LIBS="-L${PYTHONHOME}/lib/python${PYTHON_VERSION}/site-packages"
        fi
      fi
    fi
  fi	
fi
if test "x$pyqt_lib_ok" == "xno"; then
  dnl try /usr/lib/python${PYTHON_VERSION}/site-packages
  AC_CHECK_FILE(/usr/lib/python${PYTHON_VERSION}/site-packages/libqtcmodule.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
  if test "x$pyqt_lib_ok" == "xyes"; then
    PYQT_LIBS="-L/usr/lib/python${PYTHON_VERSION}/site-packages -lqtcmodule"
  else 
    AC_CHECK_FILE(/usr/lib/python${PYTHON_VERSION}/site-packages/qt.so,pyqt_lib_ok=yes,pyqt_lib_ok=no)
    if test "x$pyqt_lib_ok" == "xyes"; then
      PYQT_LIBS="-L/usr/lib/python${PYTHON_VERSION}/site-packages"
    fi
  fi
fi
if test "x$pyqt_lib_ok" == "xno"; then
  pyqt_ok=no
fi

dnl look for PyQt sips
pyqt_sips_ok=no
dnl try ${PYQT_SIPS} or ${PYQT_SIPS}/qt
if test "x${PYQT_SIPS}" != "x"; then
  AC_CHECK_FILE(${PYQT_SIPS}/qglobal.sip,pyqt_sips_ok=yes,pyqt_sips_ok=no)
  if test "x$pyqt_sips_ok" == "xno"; then
    AC_CHECK_FILE(${PYQT_SIPS}/qt/qglobal.sip,pyqt_sips_ok=yes,pyqt_sips_ok=no)
    if test "x$pyqt_sips_ok" == "xyes"; then
      PYQT_SIPS="${PYQT_SIPS}/qt"
    fi
  fi
fi
if test "x$pyqt_sips_ok" == "xno"; then
  dnl try ${PYQTDIR}/sip
  if test "x${PYQTDIR}" != "x"; then
    if test -d ${PYQTDIR}/sip ; then
      AC_CHECK_FILE(${PYQTDIR}/sip/qglobal.sip,pyqt_sips_ok=yes,pyqt_sips_ok=no)
      if test "x$pyqt_sips_ok" == "xyes"; then
        PYQT_SIPS="${PYQTDIR}/sip"
      else
        AC_CHECK_FILE(${PYQTDIR}/sip/qt/qglobal.sip,pyqt_sips_ok=yes,pyqt_sips_ok=no)
        if test "x$pyqt_sips_ok" == "xyes"; then
          PYQT_SIPS="${PYQTDIR}/sip/qt"
        fi
      fi
    fi
  fi
fi
if test "x$pyqt_sips_ok" == "xno"; then
  dnl try ${SIPDIR}/sip
  if test "x${SIPDIR}" != "x"; then
    if test -d ${SIPDIR}/sip ; then
      AC_CHECK_FILE(${SIPDIR}/sip/qglobal.sip,pyqt_sips_ok=yes,pyqt_sips_ok=no)
      if test "x$pyqt_sips_ok" == "xyes"; then
        PYQT_SIPS="${SIPDIR}/sip"
      else
        AC_CHECK_FILE(${SIPDIR}/sip/qt/qglobal.sip,pyqt_sips_ok=yes,pyqt_sips_ok=no)
        if test "x$pyqt_sips_ok" == "xyes"; then
          PYQT_SIPS="${SIPDIR}/sip/qt"
        fi
      fi
    fi
  fi
fi
if test "x$pyqt_sips_ok" == "xno"; then
  dnl try /usr/share/sip
  if test -d /usr/share/sip ; then
    AC_CHECK_FILE(/usr/share/sip/qglobal.sip,pyqt_sips_ok=yes,pyqt_sips_ok=no)
    if test "x$pyqt_sips_ok" == "xyes"; then
      PYQT_SIPS="/usr/share/sip"
    else
      AC_CHECK_FILE(/usr/share/sip/qt/qglobal.sip,pyqt_sips_ok=yes,pyqt_sips_ok=no)
      if test "x$pyqt_sips_ok" == "xyes"; then
        PYQT_SIPS="/usr/share/sip/qt"
      fi
    fi
  fi
fi
if test "x$pyqt_sips_ok" == "xno"; then
  pyqt_ok=no
else
  PYQT_INCLUDES="-I$PYQT_SIPS"
  
  dnl Additional sip flags required for correct wrappers compilation
  AC_MSG_CHECKING(which qt classes should be excluded)

  PYQT_SIPFLAGS=""

  CXXFLAGS_old=$CXXFLAGS
  CXXFLAGS="$CXXFLAGS $QT_INCLUDES"
  LIBS_old=$LIBS
  LIBS="$LIBS $QT_LIBS"

  AC_TRY_COMPILE([#include <qcdestyle.h>],
                 [new QCDEStyle();],,PYQT_SIPFLAGS="$PYQT_SIPFLAGS -x Qt_STYLE_CDE")
  AC_TRY_COMPILE([#include <qinterlacestyle.h>],
                 [new QInterlaceStyle();],,PYQT_SIPFLAGS="$PYQT_SIPFLAGS -x Qt_STYLE_INTERLACE")
  AC_TRY_COMPILE([#include <qmotifstyle.h>],
                 [new QMotifStyle();],,PYQT_SIPFLAGS="$PYQT_SIPFLAGS -x Qt_STYLE_MOTIF")
  AC_TRY_COMPILE([#include <qmotifplusstyle.h>],
                 [new QMotifPlusStyle();],,PYQT_SIPFLAGS="$PYQT_SIPFLAGS -x Qt_STYLE_MOTIFPLUS")
  AC_TRY_COMPILE([#include <qplatinumstyle.h>],
                 [new QPlatinumStyle();],,PYQT_SIPFLAGS="$PYQT_SIPFLAGS -x Qt_STYLE_PLATINUM")
  AC_TRY_COMPILE([#include <qsgistyle.h>],
                 [new QSGIStyle();],,PYQT_SIPFLAGS="$PYQT_SIPFLAGS -x Qt_STYLE_SGI")
  AC_TRY_COMPILE([#include <qwindowsstyle.h>],
                 [new QWindowsStyle();],,PYQT_SIPFLAGS="$PYQT_SIPFLAGS -x Qt_STYLE_WINDOWS")
  AC_TRY_COMPILE([#include <qwindowsxpstyle.h>],
                 [new QWindowsXPStyle();],,PYQT_SIPFLAGS="$PYQT_SIPFLAGS -x Qt_STYLE_WINDOWSXP")

  LIBS="$LIBS -lqassistantclient"
  AC_TRY_LINK([#include <qassistantclient.h>],
              [new QAssistantClient("foo");],,PYQT_SIPFLAGS="$PYQT_SIPFLAGS -x Qt_ASSISTANTCLIENT")

  AC_MSG_RESULT(done)

  CXXFLAGS=$CXXFLAGS_old
  LIBS=$LIBS_old
fi

AC_SUBST(PYQT_INCLUDES)
AC_SUBST(PYQT_LIBS)
AC_SUBST(PYQT_SIPS)
AC_SUBST(PYUIC)
AC_SUBST(PYQT_SIPFLAGS)

AC_LANG_RESTORE

AC_MSG_RESULT(for pyqt: $pyqt_ok)

])dnl
dnl


