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
dnl @synopsis AC_CXX_WARNINGS
dnl
dnl Check warning flags for C++ compiler to control warning messages
dnl
dnl @author Bernard Secher (CEA) - 04/12/2003
dnl
AC_DEFUN([AC_CXX_WARNINGS],[
  AC_CXX_OPTION(-Wno-deprecated,CXXFLAGS)
  AC_CXX_OPTION(-Wparentheses,CXXFLAGS)
  AC_CXX_OPTION(-Wreturn-type,CXXFLAGS)
  AC_CXX_OPTION(-Wmissing-declarations,CXXFLAGS)
  AC_CXX_OPTION(-Wunused,CXXFLAGS)
])
