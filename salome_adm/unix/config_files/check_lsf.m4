# Check for LSF
AC_DEFUN([CHECK_LSF],
[
 AC_ARG_WITH([lsf], 
             AC_HELP_STRING([--with-lsf[[[[[=path]      ]]]]],
                            [tell configure script to use LSF that is located at <path>. Default is no. If path is not given, it is automaticaly determined from LSF_ENVDIR variable.]),
             [],
             [withval=no])

 AC_MSG_CHECKING([for LSF])

 if test ! "x$withval" = "xno"
 then
   if test "x$withval" = "xyes"
   then
     if test ! "x$LSF_ENVDIR" = "x"
     then
       test -f "$LSF_ENVDIR/lsf.conf" && . "$LSF_ENVDIR/lsf.conf"
       LSF=$LSF_TOP
     else
       dnl some default values ...
       LSF=/usr
       LSF_INCLUDEDIR=$LSF/include
       LSF_LIBDIR=$LSF/lib
     fi
   else
     LSF=$withval
     LSF_INCLUDEDIR=$LSF/include
     LSF_LIBDIR=$LSF/lib
   fi

   AC_MSG_RESULT([$LSF])

   dnl INCLUDE directory

   LSF_INCLUDES="-I${LSF_INCLUDEDIR}"
   saved_CPPFLAGS=${CPPFLAGS}
   CPPFLAGS="${CPPFLAGS} ${LSF_INCLUDES}"
   AC_CHECK_HEADER([lsf/lsbatch.h], [lsf_header_found=yes], [lsf_header_found=no], [])
   test x${lsf_header_found} = xno && AC_MSG_WARN([LSF include file not found])
   CPPFLAGS=${saved_CPPFLAGS}
   AC_SUBST(LSF_INCLUDES)
 
   dnl LIB directory

   LSF_LDFLAGS="-L${LSF_LIBDIR}"
   LSF_LIBS=""
   saved_LDFLAGS=${LDFLAGS}
   saved_LIBS=${LIBS}
   LDFLAGS="${LDFLAGS} ${LSF_LDFLAGS}"
   lsf_lib_found=yes
   AC_CHECK_LIB([lsf], [ls_load],  [lsf_libs="-llsf ${lsf_libs}"], [lsf_lib_found=no], [])
   AC_CHECK_LIB([bat], [lsb_init], [lsf_libs="-lbat ${lsf_libs}"], [lsf_lib_found=no], [-llsf])
   test x${lsf_lib_found} = xno && AC_MSG_WARN([LSF library not found])
   LIBS="${LSF_LIBS} ${LIBS}"
   LDFLAGS=${saved_LDFLAGS}
   LIBS=${saved_LIBS}
   AC_SUBST(LSF_LDFLAGS)
   AC_SUBST(LSF_LIBS)

   test x${lsf_lib_found} = xyes && test x${lsf_header_found} = xyes && WITH_LSF=yes

 else
   WITH_LSF=no

   AC_MSG_RESULT([no])

 fi

 dnl AM_CONDITIONAL(WITH_LSF, test $WITH_LSF = 1)

 lsf_ok=$WITH_LSF
 if test $lsf_ok = no ; then
   lsf_ok=
 fi
 AC_SUBST(WITH_LSF)

])

