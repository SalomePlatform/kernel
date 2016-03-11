dnl Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
dnl File   : local_install.m4
dnl Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
dnl ---
dnl
dnl The purpose of below autoconf macro is to generate custom install script
dnl to be used instead of global /usr/bin/install one.
dnl The difference is that resulting install script does not install file passed
dnl as parameter if its target copy exists and the file is not changed, i.e 
dnl source and target files are the identical.
dnl The check is performed by the diff command.

AC_DEFUN([AC_LOCAL_INSTALL],
[
GLOBAL_INSTALL=$INSTALL
absdir=`pwd`
INSTALL=${absdir}/local-install.sh
cat > ${INSTALL} << EOF
#!/bin/sh

nb_args=\$[#]
args=\$[*]
if test \$nb_args -gt 1 ; then
    nb=\$(expr \$nb_args - 2)
    shift \$nb
    target=\$[2]
    if test -f \$target ; then
    orig=\$[1]
    if test -f \$orig ; then
        diff \$orig \$target > /dev/null 2>&1
        if test \$[?] = 0 ; then
        echo \$orig and \$target are identical
        exit 0
        fi
    fi
    fi
fi
${GLOBAL_INSTALL} \$args 
EOF
chmod +x ${INSTALL}
AC_SUBST(INSTALL)
AC_SUBST(GLOBAL_INSTALL)
])dnl
