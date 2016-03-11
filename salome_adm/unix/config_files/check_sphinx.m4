dnl Copyright (C) 2006-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
AC_DEFUN([CHECK_SPHINX],[

AC_CHECKING(for sphinx doc generator)

sphinx_ok=yes
dnl where is sphinx ?
AC_PATH_PROG(SPHINX,sphinx-build) 
if test "x$SPHINX" = "x"
then
  AC_MSG_WARN(sphinx not found)
  sphinx_ok=no
fi

AM_CONDITIONAL(SPHINX_IS_OK, [test x"$sphinx_ok" = xyes])

])dnl
dnl
