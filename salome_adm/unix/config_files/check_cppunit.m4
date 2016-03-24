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

AC_DEFUN([CHECK_CPPUNIT],[

AC_CHECKING(for cppunit)

cppunit_ok=yes

dnl were is cppunit ?

AC_ARG_WITH(cppunit,
    [AC_HELP_STRING([--with-cppunit=DIR],[directory path to CPPUNIT installation])],
    [CPPUNITHOME="$withval"
      AC_MSG_RESULT("select $withval as path to CPPUNIT")
    ])

AC_ARG_WITH(cppunit_inc,
    [AC_HELP_STRING([--with-cppunit_inc=DIR],[directory path to CPPUNIT includes])],
    [CPPUNIT_INCLUDES="$withval"
      AC_MSG_RESULT("select $withval as path to CPPUNIT includes")
    ])

if test -z "$CPPUNITHOME"; then
  AC_MSG_RESULT(CPPUNITHOME not defined)
  exits_ok=no	
  if test "x$exits_ok" = "xno"; then
     for d in /usr/local /usr ; do
        AC_CHECK_FILE(${d}/lib64/libcppunit.so,exits_ok=yes,exits_ok=no)
        if test "x$exits_ok" = "xyes"; then
           CPPUNITHOME=$d
           AC_MSG_RESULT(libcppunit.so detected in $d/lib64)
	   break
        fi
	AC_CHECK_FILE(${d}/lib/libcppunit.so,exits_ok=yes,exits_ok=no)
        if test "x$exits_ok" = "xyes"; then
           CPPUNITHOME=$d
           AC_MSG_RESULT(libcppunit.so detected in $d/lib)
	   break
        fi
     done
  fi
  if test "x$exits_ok" = "xno"; then
     for d in `echo $LD_LIBRARY_PATH | sed -e "s/:/ /g"` ; do
        if test -f $d/libcppunit.so ; then
           AC_MSG_RESULT(libcppunit.so detected in $d)
           CPPUNITHOME=$d
           CPPUNITHOME=`echo ${CPPUNITHOME} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
           exits_ok=yes
           break
        fi
     done
  fi
  if test "x$exits_ok" = "xyes"; then
     if test -z "$CPPUNIT_INCLUDES"; then
        CPPUNIT_INCLUDES=$CPPUNITHOME"/include"
     fi
  fi
else
  if test -z "$CPPUNIT_INCLUDES"; then
     CPPUNIT_INCLUDES="$CPPUNITHOME/include"
  fi   	
fi

if test "x$cppunit_ok" = xno -o ! -d "$CPPUNITHOME" ; then
  AC_MSG_RESULT(no)
  AC_MSG_WARN(cppunit not found)
  cppunit_ok=no
else
   AC_LANG_SAVE
   AC_LANG_CPLUSPLUS
   CPPFLAGS_old=$CPPFLAGS
   CPPFLAGS="$CPPFLAGS -I$CPPUNIT_INCLUDES"
   
   AC_CHECK_HEADER(cppunit/extensions/HelperMacros.h,cppunit_ok=yes,cppunit_ok=no) 

   CPPFLAGS=$CPPFLAGS_old

   if test "x$cppunit_ok" = xno ; then
     AC_MSG_RESULT(no)
     AC_MSG_WARN(cppunit not found)
   else
     cppunit_ok=yes
   fi

  if  test "x$cppunit_ok" = "xno"
  then
    AC_MSG_RESULT(unable to found cppunit headers and library)
    AC_MSG_RESULT(CPPUNITHOME environment variable may be wrong)
  else
    if test "x$CPPUNIT_INCLUDES" = "x/usr/include"
    then
      CPPUNIT_INCLUDES=""
    else
      CPPUNIT_INCLUDES="-I$CPPUNIT_INCLUDES"
    fi
    if test "x$CPPUNITHOME" = "x/usr"
    then
      CPPUNIT_LIBS=" -lcppunit"
    else
      CPPUNIT_LIBS="-L$CPPUNITHOME/lib -lcppunit"
    fi

    AC_SUBST(CPPUNIT_INCLUDES)
    AC_SUBST(CPPUNIT_LIBS)
    AC_SUBST(cppunit_ok)
    AC_MSG_RESULT(yes)
  fi
fi

AM_CONDITIONAL(CPPUNIT_IS_OK, [test x"$cppunit_ok" = xyes])

AC_LANG_RESTORE

])dnl
dnl
