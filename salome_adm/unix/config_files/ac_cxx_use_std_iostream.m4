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

dnl @synopsis AC_CXX_USE_STD_IOSTREAM
dnl
dnl If the C++ library use std iostream
dnl
dnl @author Bernard Secher (CEA) - 05/12/2003

AC_DEFUN([AC_CXX_USE_STD_IOSTREAM],
[

AC_MSG_CHECKING(whether the compiler use std iostream)

cat > conftest.cxx <<EOF
#include <iostream>
int main(int argc, char **argv) {std::cout << "Hello" << std::endl; return 0;}
EOF

fUSE_STD_IOSTREAM=no
for ac_CXX_USE_STD_IOSTREAM in "" -D__USE_STD_IOSTREAM ; do
  if $CXX ${ac_CXX_USE_STD_IOSTREAM} conftest.cxx > /dev/null 2>&1; then
    CPPFLAGS="$CPPFLAGS ${ac_CXX_USE_STD_IOSTREAM}"
    if test x${ac_CXX_USE_STD_IOSTREAM} = x; then
      AC_MSG_RESULT(yes)
    else
      AC_MSG_RESULT(${ac_CXX_USE_STD_IOSTREAM})
    fi
    fUSE_STD_IOSTREAM=yes
    break
  fi
done
if test $fUSE_STD_IOSTREAM = no; then
  AC_MSG_RESULT(no)
fi

])
