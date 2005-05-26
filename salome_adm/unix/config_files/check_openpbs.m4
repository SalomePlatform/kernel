# Check for OpenPBS
AC_DEFUN([CHECK_OPENPBS],
[
 AC_ARG_VAR([OPENPBS], [OpenPBS home directory])
 AC_ARG_WITH([openpbs], 
             AC_HELP_STRING([--with-openpbs=<path>],
                            [tell configure script to use OpenPBS that is located at <path>]),
             [test ! x"${withval}" = xyes && OPENPBS=${withval}],
             [])

 dnl AC_ARG_VAR([OPENPBSSERVERNAME], [OpenPBS server_name file])
 dnl AC_ARG_WITH([openpbs-server-name], 
 dnl             AC_HELP_STRING([--with-openpbs-server-name=<path>],
 dnl                            [tell configure script to use OpenPBS server_name file that is located at <path>]),
 dnl             [test ! x"${withval}" = xyes && OPENPBSSERVERNAME=${withval}],
 dnl             [with_openpbs_server_name=""])


 if test -z "${OPENPBS}"
 then
   AC_MSG_WARN([Environment variable OPENPBS not set. Skipping OpenPBS configuration.])

 else

   OPENPBS_INCLUDES="-I${OPENPBS}/include"
   saved_CPPFLAGS=${CPPFLAGS}
   CPPFLAGS="${CPPFLAGS} ${OPENPBS_INCLUDES}"
   AC_CHECK_HEADER([pbs_ifl.h], [openpbs_header_found=yes], [openpbs_header_found=no], [])
   test x${openpbs_header_found} = xno && AC_MSG_WARN([OpenPBS include file not found])
   CPPFLAGS=${saved_CPPFLAGS}
   AC_SUBST(OPENPBS_INCLUDES)
 
   OPENPBS_LIBDIR="-L${OPENPBS}/lib"
   OPENPBS_LIBS="-lpbs"
   saved_LDFLAGS=${LDFLAGS}
   saved_LIBS=${LIBS}
   LDFLAGS="${LDFLAGS} ${OPENPBS_LIBDIR}"
   AC_CHECK_LIB([pbs], [pbs_connect], [openpbs_lib_found=yes], [openpbs_lib_found=no], [])
   test x${openpbs_lib_found} = xno && AC_MSG_WARN([OpenPBS library not found])
   LIBS="${LIBS} ${OPENPBS_LIBS}"
   LDFLAGS=${saved_LDFLAGS}
   LIBS=${saved_LIBS}
   AC_SUBST(OPENPBS_LIBDIR)
   AC_SUBST(OPENPBS_LIBS)
 
   dnl test -z "${OPENPBSSERVERNAME}" && OPENPBSSERVERNAME="/usr/spool/PBS/server_name"
   dnl AC_CHECK_FILE([${OPENPBSSERVERNAME}], [openpbs_server_name_found=yes], [openpbs_server_name_found=no])

   test x${openpbs_header_found} = xyes && test x${openpbs_lib_found} = xyes  && openpbs_ok="yes"

 fi

 WITHOPENPBS=$openpbs_ok
 if test x$WITHOPENPBS = x ; then
   WITHOPENPBS=no
 fi
 AC_SUBST(WITHOPENPBS)

])

