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

AC_DEFUN([CHECK_MED3],[
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([CHECK_HDF5])dnl
#AC_REQUIRE([MED_CHECK_SIZEOF_MEDINT]) # seems not required

AC_CHECKING(for MED3)

AC_ARG_WITH(med3,
    [  --with-med3=DIR                 root directory path to med3 installation ],
    [MED3HOME="$withval"
      AC_MSG_RESULT("select $withval as path to med3")
    ])

AC_SUBST(MED3_INCLUDES)
AC_SUBST(MED3_LIBS)
AC_SUBST(MED3_LIBS_C_ONLY)
AC_SUBST(MED3_MT_LIBS)
AC_SUBST(MED_CPPFLAGS)
AC_SUBST(MED3HOME)

MED3_INCLUDES=""
MED3_LIBS=""
MED3_MT_LIBS=""
#MED_CPPFLAGS="$DEFINED_F77INT64"

med3_ok=no

LOCAL_INCLUDES="$HDF5_INCLUDES"
LOCAL_LIBS="-lmed -lmedimport $HDF5_LIBS"
LOCAL_LIBS_C_ONLY="-lmedC $HDF5_LIBS"

if test -z $MED3HOME
then
   AC_MSG_WARN(undefined MED3HOME variable which specify med3 installation directory)
   AC_PATH_PROG(MDUMP, mdump)
   if test "xMDUMP" != "x" ; then
      MED3HOME=$MDUMP
      MED3HOME=`echo ${MED3HOME} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
      MED3HOME=`echo ${MED3HOME} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
   fi
fi
if test ! -z $MED3HOME
then
   LOCAL_INCLUDES="$LOCAL_INCLUDES -I$MED3HOME/include"
   if test "x$MED3HOME" = "x/usr"
   then
     LOCAL_LIBS="-lmed  $LOCAL_LIBS"
     LOCAL_LIBS_C_ONLY="-lmedC $LOCAL_LIBS_C_ONLY"
   else
     LOCAL_LIBS="-L$MED3HOME/lib $LOCAL_LIBS"
     LOCAL_LIBS_C_ONLY="-L$MED3HOME/lib $LOCAL_LIBS_C_ONLY"
   fi
fi

dnl check med3 header

f77int="F77INT32"
case  $host_os in
   irix5.* | irix6.* | osf4.* | osf5.* | linux*  )

        linux64="true"
# porting on intel processor 64 bits
        expr "$host_os" : 'linux' >/dev/null && ( test ! x"$host_cpu" = x"x86_64" && test ! x"$host_cpu" = x"ia64" ) && linux64="false"
	if test ! x"$linux64" = "xfalse" ; then
	  echo "$as_me:$LINENO: checking for 64bits integers size in F77/F90" >&5
echo $ECHO_N "checking for 64bits integers size in F77/F90... $ECHO_C" >&6
	  # Check whether --enable-int64 or --disable-int64 was given.
if test "${enable_int64+set}" = set; then
  enableval="$enable_int64"

fi;
	  case "X-$enable_int64" in
	    X-no)
	     echo "$as_me:$LINENO: result: \"disabled\"" >&5
echo "${ECHO_T}\"disabled\"" >&6
	     SUFFIXES="_32"
	     ;;
	    *)
	     echo "$as_me:$LINENO: result: \"enabled\"" >&5
echo "${ECHO_T}\"enabled\"" >&6
	     SUFFIXES=""
	     f77int="F77INT64"
	     ;;
	  esac
	fi
     ;;
   *)
     ;;
esac

case $host_os in
    linux*)
        test x"$linux64" = x"true" && \
          MACHINE="PCLINUX64${SUFFIXES}" || \
	MACHINE=PCLINUX
	;;
    hpux*)
	MACHINE=HP9000
	;;
    aix4.*)
	MACHINE=RS6000
	host_os_novers=aix4.x
	;;
    irix5.*)
	MACHINE="IRIX64${SUFFIXES}"
	host_os_novers=irix5.x
	;;
    irix6.*)
	MACHINE="IRIX64${SUFFIXES}"
	host_os_novers=irix6.x
	;;
    osf4.*)
	MACHINE="OSF1${SUFFIXES}"
	host_os_novers=osf4.x
	;;
    osf5.*)
	MACHINE="OSF1${SUFFIXES}"
	 host_os_novers=osf5.x
	 ;;
    solaris2.*)
	MACHINE=SUN4SOL2
	 host_os_novers=solaris2.x
	 ;;
    uxpv*)
	MACHINE=VPP5000
	 ;;
    *)
	MACHINE=
	 host_os_novers=$host_os
	 ;;
esac

CPPFLAGS_old="$CPPFLAGS"
dnl we must test system : linux = -DPCLINUX
dnl we must test system : Alpha-OSF = -DOSF1
case $host_os in
   linux*)
      CPPFLAGS="$CPPFLAGS -D$MACHINE $LOCAL_INCLUDES"
dnl      CPPFLAGS="$CPPFLAGS -DPCLINUX $LOCAL_INCLUDES"
      ;;
   osf*)
      CPPFLAGS="$CPPFLAGS -DOSF1 $LOCAL_INCLUDES"
      ;;
esac
AC_CHECK_HEADER(med.h,med3_ok=yes ,med3_ok=no)
CPPFLAGS="$CPPFLAGS_old"

if  test "x$med3_ok" = "xyes"
then

dnl check med3 library

  LIBS_old="$LIBS"
  # $LOCAL_LIBS_C_ONLY is added to next line because on Fedora, check error :
  #/usr/bin/ld: /tmp/ccQHybYu.o: undefined reference to symbol 'MEDfileOpen'                                                     
  #/usr/bin/ld: note: 'MEDfileOpen' is defined in DSO /data/tmplgls/salome/prerequis/install/FD18_64/med-dev/FROM_hdf5-1.8.10/lib/libmedC.so so try adding it to the linker command line
  # See http://comments.gmane.org/gmane.linux.redhat.fedora.devel/160632
  LIBS="$LIBS $LOCAL_LIBS $LOCAL_LIBS_C_ONLY"
  AC_CHECK_LIB(med,MEDfileOpen,med3_ok=yes,med3_ok=no)

  if  test "x$med3_ok" = "xyes"
  then
   AC_CHECK_LIB(medimport,HAVE_MEDimport,med3_ok=yes,med3_ok=no)
  fi
  LIBS="$LIBS_old"
 
fi

if  test "x$med3_ok" = "xyes"
then
case $host_os in
   linux*)
      MED3_INCLUDES="-D$MACHINE $LOCAL_INCLUDES"
dnl      MED3_INCLUDES="-DPCLINUX $LOCAL_INCLUDES"
      ;;
   osf*)
      MED3_INCLUDES="-DOSF1 $LOCAL_INCLUDES"
      ;;
esac
  MED3_LIBS="$LOCAL_LIBS"
  MED3_LIBS_C_ONLY="$LOCAL_LIBS_C_ONLY"
  MED3_MT_LIBS="$LOCAL_LIBS"
fi


AC_MSG_RESULT(for med3: $med3_ok)

])dnl
