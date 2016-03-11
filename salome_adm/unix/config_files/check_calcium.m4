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

AC_DEFUN([CHECK_CALCIUM], [

## Guess where as cal_int type is provided by --with-cal_int option
## or auto-detection must be used

AC_ARG_WITH([cal_int],
            AC_HELP_STRING([--with-cal_int=<C type>],
	                   [Use <C type> for mapping a Fortran integer in C within CALCIUM C/F77 interfaces.]),
	    [],
	    [withval=no])

AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(int)

if test "x$withval" = "xno"
then
  AC_REQUIRE([CHECK_F77])
  AC_CHECK_SIZEOF_FORTRAN(integer)

  if test "x$ac_cv_sizeof_fortran_integer" = "x8" ; then
     AC_DEFINE(HAVE_F77INT64,[],
                       [The size of a Fortran integer, as computed by sizeof.])
     test "x$ac_cv_sizeof_long" = "x8" || AC_MSG_ERROR([Size of C type long expected to be eight bytes])
     LONG_OR_INT="long"
     CALCIUM_IDL_INT_F77="long long"
     CALCIUM_CORBA_INT_F77="CORBA::LongLong"
  elif test "x$ac_cv_sizeof_fortran_integer" = "x4" ; then
     test "x$ac_cv_sizeof_int" = "x4" || AC_MSG_ERROR([Size of C type int expected to be four bytes])
     LONG_OR_INT="int"
     CALCIUM_IDL_INT_F77="long"
     CALCIUM_CORBA_INT_F77="CORBA::Long"
  else
     AC_MSG_ERROR([Size of Fortran type integer is neither four nor eigth bytes])
  fi

elif test "x$withval" = "xint"
then
  LONG_OR_INT="int" 
  CALCIUM_IDL_INT_F77="long"
  CALCIUM_CORBA_INT_F77="CORBA::Long"
  AC_MSG_NOTICE([Using C type int for cal_int])

elif test "x$withval" = "xlong"
then
  LONG_OR_INT="long"
  CALCIUM_IDL_INT_F77="long long"
  CALCIUM_CORBA_INT_F77="CORBA::LongLong"
  AC_MSG_NOTICE([Using C type long for cal_int])
else
  AC_MSG_ERROR([Fortran type integer must be mapped to C type int or C type long]) 
fi

AC_SUBST(CALCIUM_IDL_INT_F77)
AC_SUBST(CALCIUM_CORBA_INT_F77)
AC_SUBST(LONG_OR_INT)
AC_DEFINE_UNQUOTED([CAL_INT],[$LONG_OR_INT],
	 [The C type to be used for mapping a Fortran integer in C within CALCIUM C/F77 interfaces.])
])
