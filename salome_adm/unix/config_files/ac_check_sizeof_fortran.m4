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

AC_DEFUN([AC_CHECK_SIZEOF_FORTRAN], [
  AC_REQUIRE([AC_F77_WRAPPERS])
  AC_CACHE_CHECK([size of Fortran [$1]], [ac_cv_sizeof_fortran_[]translit($1, [A-Z *], [a-z_p])],[
    AC_LANG_SAVE()
    AC_LANG([Fortran 77])
    AC_COMPILE_IFELSE([
      AC_LANG_SOURCE([
        subroutine fsize()
        $1 i(2)
        call csize(i(1), i(2))
        end
      ])
    ],[
      AC_LANG([C])
      cp conftest.$ac_objext conftestf.$ac_objext
      old_LDFLAGS="$LDFLAGS"
      LDFLAGS="conftestf.$ac_objext $LDFLAGS"
#"$ac_cv_f77_libs"
      AC_TRY_RUN([
#       include <stdio.h>
#         define CSIZE F77_FUNC(csize,CSIZE)
#         define FSIZE F77_FUNC(fsize,FSIZE)
        static long size_val;
        /* Called by Fortran */
        void CSIZE (i1p, i2p)
        char *i1p, *i2p;
        {
          size_val = (i2p - i1p);
        }
        main() {
          FILE *f=fopen ("conftestval","w");
          if (!f) return 1;
          /* Call the Fortran function */
          FSIZE ();
          fprintf (f, "%d\n", size_val);
          return 0;
        }
     ])
     LDFLAGS="$old_LDFLAGS"
     ac_cv_sizeof_fortran_[]translit($1, [A-Z *], [a-z_p])=`cat conftestval`
     rm -f conftestval conftestf.$ac_objext
    ])
    AC_LANG_RESTORE()
  ])
  AC_DEFINE_UNQUOTED(SIZEOF_FORTRAN_[]translit($1, [a-z *], [A-Z_p]),
                     [$ac_cv_sizeof_fortran_[]translit($1, [A-Z *], [a-z_p])],
                     [The size of a Fortran `$1', as computed by sizeof.])

])
