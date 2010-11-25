dnl  Copyright (C) 2010  CEA/DEN, EDF R&D, OPEN CASCADE
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

dnl ---
dnl File   : hack_libtool.m4
dnl Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
dnl ---
dnl
dnl The purpose of below autoconf macro is to workaround very annoying problem
dnl of the GNU libtool program. The problem leads to the incorrect linking
dnl to the native libraries (installed in /usr/lib[64]) instead of those supplied 
dnl with specific -Ldir options.

AC_DEFUN([AC_HACK_LIBTOOL],[
sed -i "s%^CC=\"\(.*\)\"%hack_libtool (){ \n\
  if test \"\$(echo \$[@] | grep -E '\\\-L/usr/lib(/../lib)?(64)? ')\" == \"\" \n\
  then\n\
    cmd=\"\1 \$[@]\"\n\
  else\n\
    cmd=\"\1 \"\`echo \$[@] | sed -r -e 's|(.*)-L/usr/lib(/../lib)?(64)? (.*)|\\\1\\\4 -L/usr/lib\\\3|g'\`\n\
  fi\n\
  \$cmd\n\
}\n\
CC=\"hack_libtool\"%g" libtool
])
