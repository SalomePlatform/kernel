
AC_DEFUN([CHECK_OMNIORB],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CXX])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([AC_PROG_CXXCPP])dnl

AC_CHECKING(for omniORB)
omniORB_ok=yes

if test "x$PYTHON" = "x" 
then
  CHECK_PYTHON
fi

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_PATH_PROG(OMNIORB_IDL, omniidl)
if test "xOMNIORB_IDL" = "x"
then
  omniORB_ok=no
  AC_MSG_RESULT(omniORB binaries not in PATH variable)
else
  omniORB_ok=yes
fi

if  test "x$omniORB_ok" = "xyes"
then
  AC_SUBST(OMNIORB_IDL)

  OMNIORB_BIN=`echo ${OMNIORB_IDL} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
  OMNIORB_LIB=`echo ${OMNIORB_BIN} | sed -e "s,bin,lib,"`

  OMNIORB_ROOT=`echo ${OMNIORB_BIN}  | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
  OMNIORB_ROOT=`echo ${OMNIORB_ROOT} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
  AC_SUBST(OMNIORB_ROOT)

  OMNIORB_INCLUDES="-I$OMNIORB_ROOT/include -I$OMNIORB_ROOT/include/omniORB3 -I$OMNIORB_ROOT/include/COS"
dnl  OMNIORB_INCLUDES="-I$OMNIORB_ROOT/include -I$OMNIORB_ROOT/include/omniORB4 -I$OMNIORB_ROOT/include/COS"
  AC_SUBST(OMNIORB_INCLUDES)

  ENABLE_PTHREADS

  OMNIORB_CXXFLAGS=
  case $build_cpu in
    sparc*)
      AC_DEFINE(__sparc__)
      OMNIORB_CXXFLAGS="$OMNIORB_CXXFLAGS -D__sparc__"
      ;;
   *86*)
      AC_DEFINE(__x86__)
      OMNIORB_CXXFLAGS="$OMNIORB_CXXFLAGS -D__x86__"
      ;;
  esac
  case $build_os in
    solaris*)
      AC_DEFINE(__sunos__)
      __OSVERSION__=5
      AC_DEFINE(__OSVERSION__)
      OMNIORB_CXXFLAGS="$OMNIORB_CXXFLAGS -D__sunos__"
      ;;
   linux*)
      AC_DEFINE(__linux__)
      __OSVERSION__=2
      AC_DEFINE(__OSVERSION__)
      OMNIORB_CXXFLAGS="$OMNIORB_CXXFLAGS -D__linux__"
      ;;
  esac
  AC_SUBST(OMNIORB_CXXFLAGS)

  CPPFLAGS_old=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $OMNIORB_CXXFLAGS $OMNIORB_INCLUDES"

  AC_LANG_CPLUSPLUS
  AC_CHECK_HEADER(CORBA.h,omniORB_ok="yes",omniORB_ok="no")

  CPPFLAGS=$CPPFLAGS_old

fi

dnl omniORB_ok=yes

if test "x$omniORB_ok" = "xyes" 
then
  OMNIORB_LDFLAGS="-L$OMNIORB_LIB"

  LIBS_old=$LIBS
  LIBS="$LIBS $OMNIORB_LDFLAGS -lomnithread"

  CXXFLAGS_old=$CXXFLAGS
  CXXFLAGS="$CXXFLAGS $OMNIORB_CXXFLAGS $OMNIORB_INCLUDES"

  AC_MSG_CHECKING(whether we can link with omnithreads)
  AC_CACHE_VAL(salome_cv_lib_omnithreads,[
    AC_TRY_LINK(
#include <omnithread.h>
,   omni_mutex my_mutex,
    eval "salome_cv_lib_omnithreads=yes",eval "salome_cv_lib_omnithreads=no")
  ])

  omniORB_ok="$salome_cv_lib_omnithreads"
  if  test "x$omniORB_ok" = "xno"
  then
    AC_MSG_RESULT(omnithreads not found)
  else
    AC_MSG_RESULT(yes)
  fi

  LIBS=$LIBS_old
  CXXFLAGS=$CXXFLAGS_old
fi


dnl omniORB_ok=yes
if test "x$omniORB_ok" = "xyes" 
then

  AC_CHECK_LIB(socket,socket, LIBS="-lsocket $LIBS",,)
  AC_CHECK_LIB(nsl,gethostbyname, LIBS="-lnsl $LIBS",,)

  LIBS_old=$LIBS
  OMNIORB_LIBS="$OMNIORB_LDFLAGS -lomniORB3 -ltcpwrapGK -lomniDynamic3 -lomnithread -lCOS3 -lCOSDynamic3"
dnl  OMNIORB_LIBS="$OMNIORB_LDFLAGS -lomniORB4 -lomniDynamic4 -lomnithread -lCOS4 -lCOSDynamic4"
  AC_SUBST(OMNIORB_LIBS)

  LIBS="$OMNIORB_LIBS $LIBS"
  CXXFLAGS_old=$CXXFLAGS
  CXXFLAGS="$CXXFLAGS $OMNIORB_CXXFLAGS $OMNIORB_INCLUDES"

  AC_MSG_CHECKING(whether we can link with omniORB3)
  AC_CACHE_VAL(salome_cv_lib_omniorb3,[
    AC_TRY_LINK(
#include <CORBA.h>
,   CORBA::ORB_var orb,
    eval "salome_cv_lib_omniorb3=yes",eval "salome_cv_lib_omniorb3=no")
  ])
  omniORB_ok="$salome_cv_lib_omniorb3"

  omniORB_ok=yes
  if test "x$omniORB_ok" = "xno" 
  then
    AC_MSG_RESULT(omniORB library linking failed)
    omniORB_ok=no
  else
    AC_MSG_RESULT(yes)
  fi
  LIBS="$LIBS_old"
  CXXFLAGS=$CXXFLAGS_old
fi


if test "x$omniORB_ok" = "xyes" 
then

  OMNIORB_IDLCXXFLAGS="-I$OMNIORB_ROOT/idl"
  OMNIORB_IDLPYFLAGS="-bpython -I$OMNIORB_ROOT/idl"
  AC_SUBST(OMNIORB_IDLCXXFLAGS)
  AC_SUBST(OMNIORB_IDLPYFLAGS)

  OMNIORB_IDL_CLN_H=.hh
  OMNIORB_IDL_CLN_CXX=SK.cc
  OMNIORB_IDL_CLN_OBJ=SK.o 
  AC_SUBST(OMNIORB_IDL_CLN_H)
  AC_SUBST(OMNIORB_IDL_CLN_CXX)
  AC_SUBST(OMNIORB_IDL_CLN_OBJ)

  OMNIORB_IDL_SRV_H=.hh
  OMNIORB_IDL_SRV_CXX=SK.cc
  OMNIORB_IDL_SRV_OBJ=SK.o
  AC_SUBST(OMNIORB_IDL_SRV_H)
  AC_SUBST(OMNIORB_IDL_SRV_CXX)
  AC_SUBST(OMNIORB_IDL_SRV_OBJ)

  OMNIORB_IDL_TIE_H=
  OMNIORB_IDL_TIE_CXX=
  AC_SUBST(OMNIORB_IDL_TIE_H)
  AC_SUBST(OMNIORB_IDL_TIE_CXX)
  
  AC_DEFINE(OMNIORB)

  CORBA_HAVE_POA=1
  AC_DEFINE(CORBA_HAVE_POA)

  CORBA_ORB_INIT_HAVE_3_ARGS=1
  AC_DEFINE(CORBA_ORB_INIT_HAVE_3_ARGS)
  CORBA_ORB_INIT_THIRD_ARG='"omniORB3"'
  AC_DEFINE(CORBA_ORB_INIT_THIRD_ARG, "omniORB3")

fi

omniORBpy_ok=no
if  test "x$omniORB_ok" = "xyes"
then
  AC_MSG_CHECKING(omniORBpy (CORBA.py file available))
  if test -f ${OMNIORB_ROOT}/lib/python/CORBA.py
  then
    omniORBpy_ok=yes
    PYTHONPATH=${OMNIORB_ROOT}/lib/python:${OMNIORB_LIB}:${PYTHON_PREFIX}/lib/python${PYTHON_VERSION}:${PYTHONPATH}
    AC_SUBST(PYTHONPATH)
    AC_MSG_RESULT(yes)
  fi
fi

AC_LANG_RESTORE

AC_MSG_RESULT(for omniORBpy: $omniORBpy_ok)
AC_MSG_RESULT(for omniORB: $omniORB_ok)

# Save cache
AC_CACHE_SAVE

])dnl
dnl
