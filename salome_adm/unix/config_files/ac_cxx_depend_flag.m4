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

dnl @synopsis AC_C_DEPEND_FLAG
dnl define C_DEPEND_FLAG
dnl define CXX_DEPEND_FLAG
dnl @version $Id$
dnl @author Marc Tajchman
dnl
AC_DEFUN([AC_DEPEND_FLAG],[
 AC_MSG_CHECKING([which flag for dependency information generation])
 AC_LANG_SAVE
 AC_LANG_C
 echo "conftest.o: conftest.c" > conftest.verif
 echo "int  main() { return 0; }" > conftest.c

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
# porting on intel processor 64 bits: if 64 bits processor, by default compilation in 64 bits
        if test x"$linux64" = x"true"; then \
          MACHINE="PCLINUX64${SUFFIXES}";
	  CFLAGS=" -D_OCC64 ${CXXFLAGS}";
	  CXXFLAGS=" -D_OCC64 ${CXXFLAGS}";\
	else \
	  MACHINE=PCLINUX; \
	fi
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

dnl Evolution portage sur CCRT/osf system
 case $host_os in
   osf*)
dnl sur CCRT/osf pas d'equivalent de l'option -MG de gcc avec compilo natif
dnl on utilise donc gnu pour generer les dependances.
     DEPCC=gcc
     DEPCXX=g++
     DEPCXXFLAGS=
     DIFFFLAGS="-w"
dnl  MACHINE="OSF1"
     ;;
   *)
     DEPCC=${CC-cc}
     DEPCXX=${CXX-c++}
     DEPCXXFLAGS="\${CXXFLAGS}"
     DIFFFLAGS="-b -B"
dnl  MACHINE="PCLINUX"
     ;;
 esac
 C_DEPEND_FLAG=
 for ac_C_DEPEND_FLAG in -xM -MM -M ; do

    rm -f conftest.d conftest.err
    ${DEPCC} ${ac_C_DEPEND_FLAG} -c conftest.c 1> conftest.d 2> conftest.err
    if test -f conftest.u ; then
       mv conftest.u conftest.d
    fi

    rm -f conftest
    diff ${DIFFFLAGS} conftest.d conftest.verif > conftest
    if test ! -s conftest ; then
       C_DEPEND_FLAG=${ac_C_DEPEND_FLAG}
       break
    fi
 done

dnl use gcc option -MG : asume unknown file will be construct later
 rm -f conftest.d conftest.err
 ${DEPCC} ${C_DEPEND_FLAG} -MG -c conftest.c 1> conftest.d 2> conftest.err
 if test -f conftest.u ; then
    mv conftest.u conftest.d
 fi
 rm -f conftest
 diff ${DIFFFLAGS} conftest.d conftest.verif > conftest
 if test ! -s conftest ; then
    C_DEPEND_FLAG=${C_DEPEND_FLAG}" -MG"
 fi

 rm -f conftest*
 if test "x${C_DEPEND_FLAG}" = "x" ; then
    echo "cannot determine flag (C language)"
    exit
 fi

 printf " C :  ${DEPCC} ${C_DEPEND_FLAG}"

 AC_LANG_CPLUSPLUS
 echo "conftest.o: conftest.cxx" > conftest.verif
 echo "int  main() { return 0; }" > conftest.cxx

 CXX_DEPEND_FLAG=
 for ac_CXX_DEPEND_FLAG in -xM -MM -M ; do

    rm -f conftest.d conftest.err
    ${DEPCXX} ${ac_CXX_DEPEND_FLAG} -c conftest.cxx 1> conftest.d 2> conftest.err
    if test -f conftest.u ; then
       mv conftest.u conftest.d
    fi

    rm -f conftest
    diff ${DIFFFLAGS} conftest.d conftest.verif > conftest
    if test ! -s conftest ; then
       CXX_DEPEND_FLAG=${ac_CXX_DEPEND_FLAG}
       break
    fi
 done

dnl use g++ option -MG : asume unknown file will be construct later
 rm -f conftest.d conftest.err
 ${DEPCXX} ${CXX_DEPEND_FLAG} -MG -c conftest.cxx 1> conftest.d 2> conftest.err
 if test -f conftest.u ; then
    mv conftest.u conftest.d
 fi
 rm -f conftest
 diff ${DIFFFLAGS} conftest.d conftest.verif > conftest
 if test ! -s conftest ; then
    CXX_DEPEND_FLAG=${CXX_DEPEND_FLAG}" -MG"
 fi


 rm -f conftest*
 if test "x${CXX_DEPEND_FLAG}" = "x" ; then
    echo "cannot determine flag (C++ language)"
    exit
 fi

 printf " C++ : ${DEPCXX} ${CXX_DEPEND_FLAG}"
 AC_LANG_RESTORE

 AC_MSG_RESULT([ ... done])

 AC_SUBST(DEPCC)
 AC_SUBST(DEPCXX)
 AC_SUBST(DEPCXXFLAGS)
 AC_SUBST(C_DEPEND_FLAG)
 AC_SUBST(CXX_DEPEND_FLAG)
 AC_SUBST(MACHINE)
])
