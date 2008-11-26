dnl  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
dnl
dnl  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
dnl  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
dnl
# Check for Local
AC_DEFUN([CHECK_LOCAL],
[
 dnl AC_ARG_VAR([LOCAL], [])
 AC_ARG_WITH([local], 
             AC_HELP_STRING([--with-local],
                            [tell configure script to use local submission. Default is --with-local.]),
             [],
             [withval=yes])

 AC_MSG_CHECKING([for Local])

 if test "x$withval" = "xyes"
 then
   WITH_LOCAL=yes

   AC_MSG_RESULT([$withval])

   AC_ARG_VAR([RM], [A path to a rm-like command])
   AC_PATH_PROG([RM], [rm], [/bin/false])
   AC_DEFINE_UNQUOTED(RM, "$RM", [A path to a rm-like command])

   AC_ARG_VAR([SH], [A path to a sh-like command])
   AC_PATH_PROG([SH], [sh], [/bin/false])
   AC_DEFINE_UNQUOTED(SH, "$SH", [A path to a sh-like command])
   AC_ARG_VAR([CP], [A path to a cp-like command])
   AC_PATH_PROG([CP],  [cp],  [/bin/false])
   AC_DEFINE_UNQUOTED(CP, "$CP", [A path to a cp-like command])

   AC_ARG_VAR([RSH], [A path to a rsh-like command])
   AC_PATH_PROG([RSH], [rsh], [/bin/false])
   AC_DEFINE_UNQUOTED(RSH, "$RSH", [A path to a rsh-like command])
   AC_ARG_VAR([RCP], [A path to a rcp-like command])
   AC_PATH_PROG([RCP],  [rcp],  [/bin/false])
   AC_DEFINE_UNQUOTED(RCP, "$RCP", [A path to a rcp-like command])

   AC_ARG_VAR([SSH], [A path to a ssh-like command])
   AC_PATH_PROG([SSH], [ssh], [/bin/false])
   AC_DEFINE_UNQUOTED(SSH, "$SSH", [A path to a ssh-like command])
   AC_ARG_VAR([SCP], [A path to a scp-like command])
   AC_PATH_PROG([SCP],  [scp],  [/bin/false])
   AC_DEFINE_UNQUOTED(SCP, "$SCP", [A path to a scp-like command])


 else
   WITH_LOCAL=no

   AC_MSG_RESULT([no])

 fi

 AM_CONDITIONAL(WITH_LOCAL, test $WITH_LOCAL = yes)

 localbatch_ok=$WITH_LOCAL
 AC_SUBST(WITH_LOCAL)
 AC_SUBST(RM)
 AC_SUBST(SH)
 AC_SUBST(CP)
 AC_SUBST(RSH)
 AC_SUBST(RCP)
 AC_SUBST(SSH)
 AC_SUBST(SCP)

 dnl _CS_gbo Pour forcer l'utilisation du config.h, inclu
 dnl par les fichiers Batch_BatchManager_Local_*.cxx 
 dnl OWN_CONFIG_H=yes
 dnl AC_SUBST(OWN_CONFIG_H)
])

