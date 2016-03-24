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

AC_DEFUN([CHECK_WITHIHM],[

AC_ARG_WITH(gui,
	    [  --with-gui              build package with GUI support [[default=yes]]])
AC_ARG_WITH(ihm,
	    [  --with-ihm              obsolete, use --with-gui instead])

if test "${with_gui}" == "yes" || test "${with_gui}" == "no" ; then
    WITHGUI="${with_gui}"
    WITHIHM=$WITHGUI
elif test "x${with_gui}" != "x" ; then
    AC_MSG_FAILURE(wrong value for --with-gui or --without-gui option)
elif test "${with_ihm}" == "yes" || test "${with_ihm}" == "no" ; then
    WITHIHM="${with_ihm}"
    WITHGUI=$WITHIHM
elif test "x${with_ihm}" != "x" ; then
    AC_MSG_FAILURE(wrong value for --with-ihm or --without-ihm option)
else
    WITHIHM="yes"
    WITHGUI="yes"
fi

echo
echo ---------------------------------------------
echo testing if GUI support is enabled : ${WITHGUI}
echo ---------------------------------------------
echo

withihm_ok=$WITHGUI
withgui_ok=$WITHGUI

AC_SUBST(WITHIHM)
AC_SUBST(WITHGUI)

])dnl

AC_DEFUN([CHECK_WITHGUI],[CHECK_WITHIHM])
