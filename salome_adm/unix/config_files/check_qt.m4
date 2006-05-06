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

AC_DEFUN([CHECK_QT],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CXX])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([AC_PROG_CXXCPP])dnl
AC_REQUIRE([CHECK_OPENGL])dnl
AC_REQUIRE([AC_LINKER_OPTIONS])dnl

AC_CHECKING(for Qt)

if test "x$OpenGL_ok" != "xyes" ; then
   AC_MSG_WARN(Qt needs OpenGL correct configuration, check configure output)
fi

qt_ok=yes

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

if test "x$QTDIR" = "x"
then
   AC_MSG_RESULT(please define QTDIR variable)
   qt_ok=no
else
   AC_MSG_RESULT(QTDIR is $QTDIR)
   qt_inc_ok=no
   QTINC=""
   AC_CHECK_FILE(${QTDIR}/include/qt3/qglobal.h,QTINC="/qt3",QTINC="")
   QT_VERS=`grep "QT_VERSION_STR" ${QTDIR}/include${QTINC}/qglobal.h | sed -e 's%^#define QT_VERSION_STR\([[:space:]]*\)%%g' -e 's%\"%%g'`
   AC_MSG_RESULT(Qt version is $QT_VERS)
   QT_VERS="Qt_"`echo $QT_VERS | sed -e 's%\"%%g' -e 's%\.%_%g'`
fi

if  test "x$qt_ok" = "xyes"
then
  if test -f ${QTDIR}/bin/moc
  then
    MOC=${QTDIR}/bin/moc
  else
    AC_PATH_PROG(MOC, moc)
  fi
  if test "x$MOC" = "x"
  then
    qt_ok=no
    AC_MSG_RESULT(moc qt-compiler not in PATH variable)
  else
    qt_ok=yes
    AC_MSG_RESULT(moc found)
  fi
fi

if  test "x$qt_ok" = "xyes"
then
  if test -f ${QTDIR}/bin/uic
  then
    UIC=${QTDIR}/bin/uic
  else
    AC_PATH_PROG(UIC, uic)
  fi
  if test "x$UIC" = "x"
  then
    qt_ok=no
    AC_MSG_RESULT(uic qt-interface compiler not in PATH variable)
  else
    qt_ok=yes
    AC_MSG_RESULT(uic found)
  fi
fi

AC_SUBST(QTDIR)
QT_ROOT=$QTDIR

if  test "x$qt_ok" = "xyes"
then
  CPPFLAGS_old=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS -I$QTDIR/include${QTINC}"

  AC_LANG_CPLUSPLUS
  AC_CHECK_HEADER(qaction.h,qt_ok=yes ,qt_ok=no)

  CPPFLAGS=$CPPFLAGS_old

  AC_MSG_CHECKING(include of qt headers)

  if  test "x$qt_ok" = "xno"
  then
    AC_MSG_RESULT(qt headers not found, or too old qt version, in $QTDIR/include)
    AC_MSG_RESULT(QTDIR environment variable may be wrong)
  else
    AC_MSG_RESULT(yes)
    QT_INCLUDES="-I${QT_ROOT}/include${QTINC} -DQT_THREAD_SUPPORT -DQT_CLEAN_NAMESPACE"
    QT_MT_INCLUDES="-I${QT_ROOT}/include${QTINC} -DQT_THREAD_SUPPORT -DQT_CLEAN_NAMESPACE"
  fi
fi

if  test "x$qt_ok" = "xyes"
then
  AC_MSG_CHECKING(linking qt library)
  LIBS_old=$LIBS
  if test "x$QTDIR" = "x/usr"
  then
    LIBS="$LIBS -lqt-mt $OGL_LIBS"
  else
    LIBS="$LIBS -L$QTDIR/lib${LIB_LOCATION_SUFFIX} -lqt-mt $OGL_LIBS"
  fi

  CXXFLAGS_old=$CXXFLAGS
  CXXFLAGS="$CXXFLAGS $QT_INCLUDES"

  AC_CACHE_VAL(salome_cv_lib_qt,[
    AC_TRY_LINK(
#include <qapplication.h>
,   int n;
    char **s;
    QApplication a(n, s);
    a.exec();,
    eval "salome_cv_lib_qt=yes",eval "salome_cv_lib_qt=no")
  ])
  qt_ok="$salome_cv_lib_qt"

  if  test "x$qt_ok" = "xno"
  then
    AC_MSG_RESULT(unable to link with qt library)
    AC_MSG_RESULT(QTDIR environment variable may be wrong)
  else
    AC_MSG_RESULT(yes)
    if test "x$QTDIR" = "x/usr"
    then
         QT_LIBS=" -lqt-mt"
      QT_MT_LIBS=" -lqt-mt"
    else
         QT_LIBS="-L$QTDIR/lib${LIB_LOCATION_SUFFIX} -lqt-mt"
      QT_MT_LIBS="-L$QTDIR/lib${LIB_LOCATION_SUFFIX} -lqt-mt"
    fi
  fi

  LIBS=$LIBS_old
  CXXFLAGS=$CXXFLAGS_old

fi

AC_SUBST(MOC)
AC_SUBST(UIC)

AC_SUBST(QT_ROOT)
AC_SUBST(QT_INCLUDES)
AC_SUBST(QT_MT_INCLUDES)
AC_SUBST(QT_LIBS)
AC_SUBST(QT_MT_LIBS)
AC_SUBST(QT_VERS)

AC_LANG_RESTORE

AC_MSG_RESULT(for Qt: $qt_ok)

# Save cache
AC_CACHE_SAVE

])dnl
dnl
