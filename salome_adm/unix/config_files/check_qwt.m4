dnl Copyright (C) 2003  CEA/DEN, EDF R&D

AC_DEFUN([CHECK_QWT],[
AC_REQUIRE([CHECK_QT])dnl

AC_CHECKING(for qwt)

qwt_ok=yes

dnl were is qwt ?

AC_ARG_WITH(qwt,
    [  --with-qwt=DIR     directory path to QWT installation ],
    [QWTHOME="$withval"
      AC_MSG_RESULT("select $withval as path to QWT")
    ])

AC_ARG_WITH(qwt_inc,
    [  --with-qwt_inc=DIR   directory path to QWT includes ],
    [QWT_INCLUDES="$withval"
      AC_MSG_RESULT("select $withval as path to QWT includes")
    ])

if test -z $QWTHOME; then
  AC_MSG_RESULT(QWTHOME not defined)
  exits_ok=no	
  if test "x$exits_ok" = "xno"; then
     for d in /usr/local /usr ; do
        AC_CHECK_FILE(${d}/lib/libqwt.so,exits_ok=yes,exits_ok=no)
        if test "x$exits_ok" = "xyes"; then
           QWTHOME=$d
           AC_MSG_RESULT(libqwt.so detected in $d/lib)
        fi
     done
  fi
  if test "x$exits_ok" = "xno"; then
     for d in `echo $LD_LIBRARY_PATH | sed -e "s/:/ /g"` ; do
        if test -f $d/libqwt.so ; then
           AC_MSG_RESULT(libqwt.so detected in $d)
           QWTHOME=$d
           QWTHOME=`echo ${QWTHOME} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
           exits_ok=yes
           break
        fi
     done
  fi
  if test "x$exits_ok" = "xyes"; then
     if test -z $QWT_INCLUDES; then
        QWT_INCLUDES=$QWTHOME"/include/qwt"
     fi
  fi
else
  if test -z $QWT_INCLUDES; then
     QWT_INCLUDES="$QWTHOME/include"
  fi   	
fi

if test "x$qwt_ok" = xno -o ! -d "$QWTHOME" ; then
  AC_MSG_RESULT(no)
  AC_MSG_WARN(qwt not found)
  qwt_ok=no
else
   AC_LANG_SAVE
   AC_LANG_CPLUSPLUS
   CPPFLAGS_old=$CPPFLAGS
   CPPFLAGS="$CPPFLAGS -I$QWT_INCLUDES"
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
  CXXFLAGS="$CXXFLAGS -I$QTDIR/include -I$QWT_INCLUDES"

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
    QWT_INCLUDES="-I$QWT_INCLUDES"
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
