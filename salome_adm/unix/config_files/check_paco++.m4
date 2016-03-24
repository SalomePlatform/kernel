dnl Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
dnl
dnl Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
dnl CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public
dnl License as published by the Free Software Foundation; either
dnl version 2.1 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public
dnl License along with this library; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
dnl
dnl See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
dnl

dnl  File   : check_paco++.m4
dnl  Author : André RIBES (EDF)
dnl  Module : KERNEL
dnl
AC_DEFUN([CHECK_PACO],[
PaCO_ok=yes

AC_ARG_WITH(paco,
	    [AC_HELP_STRING([--with-paco=DIR],[root directory path of PaCO++ installation])],
	    WITHPACO="yes",WITHPACO="no")

if test "x$WITHPACO" = "xyes"; 
then
  PACOPATH=$withval
  PACO_INCLUDES="-I${PACOPATH}/include"
  PACO_LIBS="-L${PACOPATH}/lib -lPaCO -lGaBro -lBasicBC"
  dnl a basic test to be sure that PaCO++
  dnl is correctly installed.
  AC_CHECK_PROG(PACO_IDL, PaCOIdlTool.sh, yes, no, ${PACOPATH}/bin)
  if test "x$PACO_IDL" = "xno";
  then
    PaCO_ok=no
    AC_MSG_RESULT(PaCO++ binary not in ${PACOPATH}/bin)
  fi
else
  PaCO_ok=no
fi

AC_MSG_CHECKING(for PaCO++)
if  test "x$PaCO_ok" = "xyes"
then
  AC_MSG_RESULT([yes])
  PACO_IDL=${PACOPATH}/bin/PaCOIdlTool.sh
  AC_SUBST(PACO_IDL)
  AC_SUBST(PACOPATH)
  AC_SUBST(PACO_INCLUDES)
  AC_SUBST(PACO_LIBS)
else  
  AC_MSG_RESULT([no])
fi
])dnl
dnl
