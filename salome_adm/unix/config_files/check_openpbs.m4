dnl  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
dnl
dnl  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
dnl  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
dnl
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
 
   if test "x${OPENPBS}" = "x/usr"
   then
     OPENPBS_LIBDIR=""
   else
     OPENPBS_LIBDIR="-L${OPENPBS}/lib"
   fi
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

 AM_CONDITIONAL(WITH_OPENPBS, test $WITHOPENPBS = yes)

])

