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
AC_DEFUN([CHECK_BOOST],[

AC_CHECKING(for BOOST Library)

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_SUBST(BOOST_CPPFLAGS)
BOOST_CPPFLAGS=""
boost_ok=no

if test -z ${BOOSTDIR}; then
  AC_CHECK_HEADER(boost/shared_ptr.hpp,boost_ok=yes,boost_ok=no)
  if test boost_ok = no ; then
    AC_MSG_WARN(You must provide BOOSTDIR variable)
  fi
else
  AC_MSG_RESULT(\$BOOSTDIR = ${BOOSTDIR})
  AC_CHECKING(for boost/shared_ptr.hpp header file)
  dnl BOOST headers
  CPPFLAGS_old="${CPPFLAGS}"
  BOOST_CPPFLAGS="-I${BOOSTDIR}"
  CPPFLAGS="${CPPFLAGS} ${BOOST_CPPFLAGS}"

  AC_CHECK_HEADER(boost/shared_ptr.hpp,boost_ok=yes,boost_ok=no)

  CPPFLAGS="${CPPFLAGS_old}"
fi

AC_LANG_RESTORE

])dnl


