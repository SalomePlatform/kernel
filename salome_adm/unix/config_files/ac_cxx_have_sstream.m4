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

dnl @synopsis AC_CXX_HAVE_SSTREAM
dnl If the C++ library has a working stringstream, define HAVE_SSTREAM.
dnl @author Ben Stanley
dnl @version $Id$
dnl modified by Marc Tajchman (CEA) - 10/10/2002

AC_DEFUN([AC_CXX_HAVE_SSTREAM],[
  AC_CACHE_CHECK([whether the compiler has stringstream],
                 [ac_cv_prog_cxx_have_sstream],
                 [ac_cv_prog_cxx_have_sstream=no
    AC_REQUIRE([AC_CXX_NAMESPACES])
    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS
    AC_TRY_COMPILE([
      #include <sstream>
      #ifdef HAVE_NAMESPACES
      using namespace std;
      #endif
    ],
    [stringstream message; message << "Hello"; return 0;],
    [ac_cv_prog_cxx_have_sstream=yes],[ac_cv_prog_cxx_have_sstream=no])
    AC_LANG_RESTORE
  ])
  HAVE_SSTREAM=$ac_cv_prog_cxx_have_sstream
  AC_SUBST(HAVE_SSTREAM)
])
