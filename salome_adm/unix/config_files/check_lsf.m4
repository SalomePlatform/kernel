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

   if test "x${LSF_LIBDIR}" = "x/usr/lib"
   then
     LSF_LDFLAGS=""
   else
     LSF_LDFLAGS="-L${LSF_LIBDIR}"
   fi
   LSF_LIBS="-lbat -llsf"
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

 AM_CONDITIONAL(WITH_LSF, test $WITH_LSF = yes)

 lsf_ok=$WITH_LSF
 if test $lsf_ok = no ; then
   lsf_ok=
 fi
 AC_SUBST(WITH_LSF)

])

