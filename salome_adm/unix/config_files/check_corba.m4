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

AC_DEFUN([CHECK_CORBA],[

if test x"$DEFAULT_ORB" = x"omniORB"
then

  #  Contient le nom de l'ORB
  ORB=omniorb

  AC_MSG_RESULT(default orb : omniORB)
  IDL=$OMNIORB_IDL
  AC_SUBST(IDL)

  CORBA_ROOT=$OMNIORB_ROOT
  CORBA_INCLUDES=$OMNIORB_INCLUDES
  CORBA_CXXFLAGS=$OMNIORB_CXXFLAGS
  CORBA_LIBS=$OMNIORB_LIBS
  IDLCXXFLAGS=$OMNIORB_IDLCXXFLAGS
  PACOIDLCXXFLAGS=$OMNIORB_PACOIDLCXXFLAGS
  IDLPYFLAGS=$OMNIORB_IDLPYFLAGS

  AC_SUBST(CORBA_ROOT)
  AC_SUBST(CORBA_INCLUDES)
  AC_SUBST(CORBA_CXXFLAGS)
  AC_SUBST(CORBA_LIBS)
  AC_SUBST(IDLCXXFLAGS)
  AC_SUBST(PACOIDLCXXFLAGS)
  AC_SUBST(IDLPYFLAGS)

  IDL_CLN_H=$OMNIORB_IDL_CLN_H
  IDL_CLN_CXX=$OMNIORB_IDL_CLN_CXX
  IDL_CLN_OBJ=$OMNIORB_IDL_CLN_OBJ

  AC_SUBST(IDL_CLN_H)
  AC_SUBST(IDL_CLN_CXX)
  AC_SUBST(IDL_CLN_OBJ)

  IDL_SRV_H=$OMNIORB_IDL_SRV_H
  IDL_SRV_CXX=$OMNIORB_IDL_SRV_CXX
  IDL_SRV_OBJ=$OMNIORB_IDL_SRV_OBJ

  AC_SUBST(IDL_SRV_H)
  AC_SUBST(IDL_SRV_CXX)
  AC_SUBST(IDL_SRV_OBJ)

else
    AC_MSG_RESULT($DEFAULT_ORB unknown orb)

fi

])dnl
dnl
