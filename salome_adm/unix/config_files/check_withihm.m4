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

AC_DEFUN([CHECK_WITHIHM],[

AC_ARG_WITH(ihm,
	    --with-ihm [default=yes],
	    WITHIHM="yes",WITHIHM="no")

if test "${with_ihm}" = yes; then

  echo
  echo ---------------------------------------------
  echo testing WITH_IHM : yes
  echo ---------------------------------------------
  echo
  WITHIHM="yes"
  if test "${with_ihm}" = "yes";then
    withihm_ok=yes
  fi

elif test "${with_ihm}" = no; then

  echo
  echo ---------------------------------------------
  echo testing WITH_IHM : no
  echo ---------------------------------------------
  echo
  WITHIHM="no"
  if test "${with_ihm}" = "no";then
    withihm_ok=no
  fi

else

  echo
  echo ---------------------------------------------
  echo testing WITH_IHM : yes
  echo ---------------------------------------------
  echo
  WITHIHM="yes"
  if test "${with_ihm}" = "yes";then
    withihm_ok=yes
  fi

fi
AC_SUBST(WITHIHM)

])dnl
