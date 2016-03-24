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

dnl This macro defines the PYTHON_EMBED_{CFLAGS,LDFLAGS,LIBS} substitutions
dnl that should be used when embedding the python interpreter into a program.
dnl AM_INIT_PYTHON_EMBED
dnl
AC_DEFUN([AM_INIT_PYTHON_EMBED],
[AC_REQUIRE([AM_PATH_PYTHON])
AC_REQUIRE([AM_INIT_PYEXEC_MOD])

AC_MSG_CHECKING(for flags used to embed python interpreter)
changequote(,)dnl
py_makefile="`$PYTHON -c '
import sys
print \"%s/lib/python%s/config/Makefile\"%(sys.exec_prefix, sys.version[:3])'`"
changequote([,])dnl
if test ! -f "$py_makefile"; then
   AC_MSG_ERROR([*** Couldn't find the python config makefile.  Maybe you are
*** missing the development portion of the python installation])
fi

changequote(,)dnl
py_lib="`$PYTHON -c '
import sys
ver = sys.version[:3]
pre = sys.exec_prefix
print \"-L%s/lib/python%s/config\" % (pre, ver),
if ver == \"1.4\":
        print \"-lPython -lObjects -lParser\"
else:
        print \"-lpython\" + ver

changequote([,])dnl

py_ldflags=`sed -n -e 's/^LDFLAGS=\(.*\)/\1/p' $py_makefile`
py_linkforshared=`sed -n -e 's/^LINKFORSHARED=\(.*\)/\1/p' $py_makefile`

PYTHON_EMBED_LDFLAGS="$py_ldflags $py_linkforshared"

py_localmodlibs=`sed -n -e 's/^LOCALMODLIBS=\(.*\)/\1/p' $py_makefile`
py_basemodlibs=`sed -n -e 's/^BASEMODLIBS=\(.*\)/\1/p' $py_makefile`
py_other_libs=`sed -n -e 's/^LIBS=\(.*\)/\1/p' $py_makefile`

PYTHON_EMBED_LIBS="$py_lib $py_localmodlibs $py_basemodlibs $py_other_libs"

PYTHON_EMBED_CFLAGS="$PYTHON_INCLUDES"

AC_MSG_RESULT(done)

AC_SUBST(PYTHON_EMBED_CFLAGS)
AC_SUBST(PYTHON_EMBED_LDFLAGS)
AC_SUBST(PYTHON_EMBED_LIBS)
])

dnl creates a config.c file, which holds the builtin module initialisation
dnl table for python.  The first argument should be the output filename.
dnl The second argument gives the names of all the modules you want to build
dnl into the executable.
dnl AM_PYTHON_CREATE_CONFIG_C(CONFIG_C, MODULE ...)
AC_DEFUN([AM_PYTHON_CREATE_CONFIG_C],
[AC_REQUIRE([AM_INIT_PYTHON_EMBED])
AC_MSG_CHECKING(for config.c.in)
changequote(,)dnl
py_config_in="`$PYTHON -c '
import sys
print \"%s/lib/python%s/config/config.c.in\"%(sys.exec_prefix, sys.version[:3])'`"
changequote([,])dnl
if test ! -f "$py_config_in"; then
   AC_MSG_ERROR([*** Couldn't find the config.c.in file.  Maybe you are
*** missing the development portion of the python installation])
fi

py_cnf_decls=""
py_cnf_inits=""
py_nl='\
'
for mod in . $2; do
  if test "$mod" != .; then
    py_cnf_decls="${py_cnf_decls}extern void init$mod();$py_nl"
    py_cnf_inits="${py_cnf_inits}  {\"$mod\", init$mod},$py_nl"
  fi
done
sed -e "
  /MARKER 1/i$py_nl$py_cnf_decls
  /MARKER 2/i$py_nl$py_cnf_inits
" $py_config_in > $1
AC_MSG_RESULT(created $1)
])

s page was automatically generated Contact Matt Wilson with any comments
the LXR engine written by Arne     or questions about LXR on gnome.org.
rg Gleditsch and Per Kristian      Do not contact Matt Wilson concerning
rmshus.                            any software on this site.
