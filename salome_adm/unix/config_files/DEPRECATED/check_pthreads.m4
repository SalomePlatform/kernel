#@synonpsis CHECK_PTHREADS
dnl  check for pthreads system interfaces.
dnl  set CFLAGS_PTHREADS,  CXXFLAGS_PTHREADS and LIBS_PTHREADS to
dnl  flags to compiler flags for multithread program compilation (if exists),
dnl  and library, if one required.
dnl
dnl@author   (C) Ruslan Shevchenko <Ruslan@Shevchenko.Kiev.UA>, 1998
dnl@id $Id$
dnl ----------------------------------------------------------------
dnl CHECK_PTHREADS
AC_DEFUN([CHECK_PTHREADS],[
AC_CXX_OPTION(-pthread,CPPFLAGS,flag=yes,flag=no)

if test $flag = no; then
  AC_REQUIRE([AC_CANONICAL_SYSTEM])dnl
  AC_CHECK_HEADER(pthread.h,AC_DEFINE(HAVE_PTHREAD_H))
  AC_CHECK_LIB(posix4,nanosleep, LIBS_PTHREADS="-lposix4",LIBS_PTHREADS="")
  AC_CHECK_LIB(pthread,pthread_mutex_lock, 
               LIBS_PTHREADS="-lpthread $LIBS_PTHREADS",LIBS_PTHREADS="")
else
    case $host_os in
      osf*)
        LIBS_PTHREADS="-lpthread $LIBS_PTHREADS"
        ;;
    esac
fi

if test $flag = no && test x$LIBS_PTHREADS = x; then
  threads_ok=no
else
  threads_ok=yes
fi
])dnl
dnl
dnl
