AC_DEFUN([CHECK_QWT],[
AC_REQUIRE([CHECK_QT])dnl

AC_CHECKING(for qwt)

qwt_ok=no

dnl were is qwt ?
if test -z $QWTHOME; then
  AC_MSG_WARN(You must provide QWTHOME variable)
else
  qwt_ok=yes
fi

if test "x$qwt_ok" = xno -o ! -d "$QWTHOME" ; then
  AC_MSG_RESULT(no)
  AC_MSG_WARN(qwt not found)
else
  
   AC_LANG_SAVE
   AC_LANG_CPLUSPLUS
   CPPFLAGS_old=$CPPFLAGS
   CPPFLAGS="$CPPFLAGS -I$QWTHOME/include"
   CPPFLAGS="$CPPFLAGS -I$QTDIR/include"

   AC_CHECK_HEADER(qwt.h,qwt_ok=yes,qwt_ok=no) 

   CPPFLAGS=$CPPFLAGS_old

   if test "x$qwt_ok" = xno ; then
     AC_MSG_RESULT(no)
     AC_MSG_WARN(qwt not found)
  else
     qwt_ok=yes
  fi

if  test "x$qwt_ok" = "xyes"
then
  AC_MSG_CHECKING(linking qwt library)
  LIBS_old=$LIBS
  LIBS="$LIBS -L$QTDIR/lib -lqt-mt -L$QWTHOME/lib -lqwt"

  CXXFLAGS_old=$CXXFLAGS
  CXXFLAGS="$CXXFLAGS -I$QTDIR/include -I$QWTHOME/include"

  AC_CACHE_VAL(salome_cv_lib_qwt,[
    AC_TRY_LINK(
#include <qapplication.h>
#include <qwt_plot.h>
,   int n;
    char **s;
    QApplication a(n, s);
    QwtPlot* p;
    a.setMainWidget(p);
    a.exec();,
    eval "salome_cv_lib_qwt=yes",eval "salome_cv_lib_qwt=no")
  ])
  qwt_ok="$salome_cv_lib_qwt"

  if  test "x$qwt_ok" = "xno"
  then
    AC_MSG_RESULT(unable to link with qwt library)
    AC_MSG_RESULT(QWTHOME environment variable may be wrong)
  else
    QWT_INCLUDES="-I$QWTHOME/include"
    QWT_LIBS="-L$QWTHOME/lib -lqwt"

    AC_SUBST(QWT_INCLUDES)
    AC_SUBST(QWT_LIBS)

    AC_MSG_RESULT(yes)
  fi

  LIBS=$LIBS_old
  CXXFLAGS=$CXXFLAGS_old

fi


  AC_LANG_RESTORE

fi


])dnl
dnl
