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

AC_DEFUN([CHECK_LAM],[

AC_REQUIRE([AC_PROG_CC])dnl

AC_ARG_WITH(lam,
  [AC_HELP_STRING([--with-lam=DIR],[root directory path of LAM installation])],
  WITHLAM="yes",WITHLAM="no")

MPI_INCLUDES=""
MPI_LIBS=""
if test "$WITHLAM" = yes; then

  MPIREQUESTED="yes"
  echo
  echo ---------------------------------------------
  echo testing lam
  echo ---------------------------------------------
  echo
  LAM_HOME=$withval

  if test "$LAM_HOME"; then
    MPI_INCLUDES="-I$LAM_HOME/include"
    if test "x$LAM_HOME" = "x/usr"
    then
      MPI_LIBS=""
    else
      MPI_LIBS="-L$LAM_HOME/lib"
    fi
  fi

  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$MPI_INCLUDES $CPPFLAGS"
  AC_CHECK_HEADER(mpi.h,WITHLAM="yes",WITHLAM="no")

  if test "$WITHLAM" = "yes";then
    AC_CHECK_LIB(util,openpty,,WITHLAM="no")
    LIBS_old="$LIBS"
    LDFLAGS_old="$LDFLAGS"
    LDFLAGS="$MPI_LIBS $LDFLAGS"
  fi

  if test "$WITHLAM" = "yes";then

    LIBS="$LIBS -lmpi++"
    AC_MSG_CHECKING(for MPI_Init in -lmpi++)
    AC_TRY_LINK([
  	  #include <mpi.h>
  	  ], [int argc=0; char **argv=0; MPI_Init(&argc,&argv);],
  	  WITHLAM="yes",WITHLAM="no")

    if test "$WITHLAM" = "yes";then

      AC_MSG_RESULT(yes)
      MPI_LIBS="$MPI_LIBS -lmpi++"
      AC_CHECK_LIB(mpi++,MPI_Publish_name,WITHMPI2="yes",WITHMPI2="no")

    else
      AC_MSG_RESULT(no)

      LIBS="$LIBS_old"
      AC_CHECK_LIB(lam,lam_mp_init,WITHLAM="yes",WITHLAM="no")
      if test "$WITHLAM" = "yes";then
        MPI_LIBS="$MPI_LIBS -llam"
        LIBS="$LIBS -llam"
      fi

      AC_CHECK_LIB(mpi,MPI_Init,WITHLAM="yes",WITHLAM="no")
      if test "$WITHLAM" = "yes";then
        MPI_LIBS="-lmpi $MPI_LIBS"
        LIBS="-lmpi $LIBS"
      fi

      AC_LANG_SAVE
      AC_LANG_CPLUSPLUS
      LIBS="-llammpi++ $LIBS"
      AC_MSG_CHECKING(for MPI_Init in -llammpi++)
      AC_TRY_LINK([
  	  #include <mpi.h>
  	  ], [MPI::Init();],
  	  WITHLAM="yes",WITHLAM="no")
      if test "$WITHLAM" = "yes";then
        AC_MSG_RESULT(yes)
        MPI_LIBS="-llammpi++ $MPI_LIBS"
      else
        AC_MSG_RESULT(no)
      fi
      AC_LANG_RESTORE

      AC_CHECK_LIB(mpi,MPI_Publish_name,WITHMPI2="yes",WITHMPI2="no")

    fi
  fi

  CPPFLAGS="$CPPFLAGS_old"
  LDFLAGS="$LDFLAGS_old"
  LIBS="$LIBS_old"

  if test "$WITHLAM" = "yes";then
     WITHMPI="yes"
     mpi_ok=yes
     CPPFLAGS="-DWITHLAM $CPPFLAGS"
  else
     mpi_ok=no
  fi

fi


])dnl
