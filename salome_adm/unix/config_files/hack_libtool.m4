dnl Copyright (C) 2010-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
cat > hack_libtool <<EOF
#! /bin/sh

cmd=""
cmd_end=""

for param in "\$[@]"
do
    case \${param} in
	-L* ) where=\$(echo \${param} | cut -b3-) ; if test "\${where}" != "" && test -d \${where} ; then where=\$(cd \${where}; pwd) ; if test "\${where}" = "/usr/lib" || test "\${where}" = "/usr/lib64" ; then cmd_end="\${cmd_end} \${param}" ; else cmd="\${cmd} \${param}" ; fi ; fi ;;
	*   ) cmd="\${cmd} \${param}" ;;
    esac
done
cmd="\${cmd} \${cmd_end}"
\$cmd
EOF
chmod a+x hack_libtool
])
