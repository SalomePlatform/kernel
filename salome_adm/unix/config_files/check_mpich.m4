AC_DEFUN([CHECK_MPICH],[

AC_REQUIRE([AC_PROG_CC])dnl

AC_ARG_WITH(mpich,
	    --with-mpich=DIR root directory path of MPICH installation,
	    WITHMPICH="yes",WITHMPICH="no")

MPICH_INCLUDES=""
MPICH_LIBS=""
if test "$WITHMPICH" = yes; then

  echo
  echo ---------------------------------------------
  echo testing mpich
  echo ---------------------------------------------
  echo
  MPICH_HOME=$withval

  if test "$MPICH_HOME"; then
    MPICH_INCLUDES="-I$MPICH_HOME/include"
    MPICH_LIBS="-L$MPICH_HOME/lib"
  fi

  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$MPICH_INCLUDES $CPPFLAGS"
  AC_CHECK_HEADER(mpi.h,WITHMPICH="yes",WITHMPICH="no")
  CPPFLAGS="$CPPFLAGS_old"

  if test "$WITHMPICH" = "yes";then
    LDFLAGS_old="$LDFLAGS"
    LDFLAGS="$MPICH_LIBS $LDFLAGS"
    AC_CHECK_LIB(mpich,MPI_Init,
               AC_CHECK_LIB(pmpich, PMPI_Init,WITHMPICH="yes",WITHMPICH="no"),
               WITHMPICH="no")
    LDFLAGS="$LDFLAGS_old"
  fi

  MPICH_LIBS="$MPICH_LIBS -lpmpich -lmpich"

fi
AC_SUBST(MPICH_INCLUDES)
AC_SUBST(MPICH_LIBS)
AC_SUBST(WITHMPICH)

])dnl
