dnl  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
dnl  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
dnl
dnl
dnl
AC_DEFUN([CHECK_CAS],[
AC_REQUIRE([AC_PROG_CXX])dnl
AC_REQUIRE([AC_PROG_CXXCPP])dnl

AC_CHECKING(for OpenCascade)

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_SUBST(CAS_CPPFLAGS)
AC_SUBST(CAS_CXXFLAGS)
AC_SUBST(CAS_KERNEL)
AC_SUBST(CAS_MATH)
AC_SUBST(CAS_VIEWER)
AC_SUBST(CAS_TKTopAlgo)
AC_SUBST(CAS_MODELER)
AC_SUBST(CAS_OCAF)
AC_SUBST(CAS_OCAFVIS)
AC_SUBST(CAS_DATAEXCHANGE)
AC_SUBST(CAS_LDFLAGS)
AC_SUBST(CAS_LDPATH)
AC_SUBST(CAS_STDPLUGIN)

CAS_CPPFLAGS=""
CAS_CXXFLAGS=""
CAS_LDFLAGS=""
occ_ok=no
config_h=no

dnl libraries directory location
case $host_os in
   linux*)
      casdir=Linux
      ;;
   freebsd*)
      casdir=Linux
      ;;
   irix5.*)
      casdir=Linux
      ;;
   irix6.*)
      casdir=Linux
      ;;
   osf*)
      casdir=Linux
      ;;
   solaris2.*)
      casdir=Linux
      ;;
   *)
      casdir=Linux
      ;;
esac

AC_MSG_CHECKING(for OpenCascade directories)

if test -z "$CASROOT"; then
  AC_MSG_RESULT(CASROOT not defined)
  for d in `echo $LD_LIBRARY_PATH | sed -e "s/:/ /g"` ; do
    if test -f $d/libTKernel.so ; then
      AC_MSG_RESULT(libTKernel.so detected in $d)
      CASROOT=$d
      CASROOT=`echo ${CASROOT} | sed -e "s,[[^/]]*$,,;s,/$,,;s,^$,.,"`
      break
    fi
  done
fi

if test -d ${CASROOT}/${casdir}/lib; then
  CAS_LDPATH="-L$CASROOT/$casdir/lib "
  AC_MSG_RESULT(yes)
else
  if test -d ${CASROOT}/lib; then
    CAS_LDPATH="-L$CASROOT/lib "
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
  fi
fi


dnl were is OCC ?
if test -z "$CASROOT"; then
  AC_MSG_WARN(You must provide CASROOT variable : see OCC installation manual)
else
  occ_ok=yes
  OCC_VERSION_MAJOR=0
  OCC_VERSION_MINOR=0
  OCC_VERSION_MAINTENANCE=0
  ff=$CASROOT/inc/Standard_Version.hxx
  if test -f $ff ; then
    grep "define OCC_VERSION_MAJOR" $ff > /dev/null
    if test $? = 0 ; then
      OCC_VERSION_MAJOR=`grep "define OCC_VERSION_MAJOR" $ff | awk '{i=3 ; print $i}'`
    fi
    grep "define OCC_VERSION_MINOR" $ff > /dev/null
    if test $? = 0 ; then
      OCC_VERSION_MINOR=`grep "define OCC_VERSION_MINOR" $ff | awk '{i=3 ; print $i}'`
    fi
    grep "define OCC_VERSION_MAINTENANCE" $ff > /dev/null
    if test $? = 0 ; then
      OCC_VERSION_MAINTENANCE=`grep "define OCC_VERSION_MAINTENANCE" $ff | awk '{i=3 ; print $i}'`
    fi
  fi
fi

if test "x$occ_ok" = "xyes"; then

dnl test c++ compiler flag for unsigned character
  for opt in -funsigned-char -unsigned ; do
    AC_CXX_OPTION($opt,CAS_CXXFLAGS,flag=yes,flag=no)
    if test "$flag" = "yes"; then
      break
    fi
  done
  
dnl cascade headers

  CPPFLAGS_old="$CPPFLAGS"
case $host_os in
   linux*)
      CAS_CPPFLAGS="-DOCC_VERSION_MAJOR=$OCC_VERSION_MAJOR -DOCC_VERSION_MINOR=$OCC_VERSION_MINOR -DOCC_VERSION_MAINTENANCE=$OCC_VERSION_MAINTENANCE -DLIN -DLINTEL -DCSFDB -DNo_exception -DHAVE_CONFIG_H -DHAVE_LIMITS_H -DHAVE_WOK_CONFIG_H"

      OCC_VERSION_STRING="$OCC_VERSION_MAJOR.$OCC_VERSION_MINOR.$OCC_VERSION_MAINTENANCE"
      case $OCC_VERSION_STRING in
        [[0-5]].* | 6.0.* | 6.1.0) # catch versions < 6.1.1
          CAS_CPPFLAGS="$CAS_CPPFLAGS -DNO_CXX_EXCEPTION"
          ;;
        *)
          CAS_CPPFLAGS="$CAS_CPPFLAGS -DOCC_CONVERT_SIGNALS"
          ;;
      esac
      CAS_CPPFLAGS="$CAS_CPPFLAGS -I$CASROOT/inc"
      ;;
   osf*)
      CAS_CPPFLAGS="-DOCC_VERSION_MAJOR=$OCC_VERSION_MAJOR -DOCC_VERSION_MINOR=$OCC_VERSION_MINOR -DOCC_VERSION_MAINTENANCE=$OCC_VERSION_MAINTENANCE -DLIN -DLINTEL -DCSFDB -DNo_exception -DHAVE_CONFIG_H -DHAVE_LIMITS_H -DHAVE_WOK_CONFIG_H -I$CASROOT/inc"
      ;;
esac
  CPPFLAGS="$CPPFLAGS $CAS_CPPFLAGS"

  echo
  echo testing config.h

  AC_CHECK_HEADER(config.h, config_h=yes, [
	echo "config.h file not found!"
  ])

  if test "x$config_h" = xno ; then
      AC_MSG_WARN(config.h file not found)
      dnl There is no consequence for SALOME building because
      dnl this file is not used. SALOME uses SALOMEconfig.h instead!
  else
      AC_MSG_RESULT(config.h file ok)    
  fi

  AC_CHECK_HEADER(Standard_Type.hxx,occ_ok=yes ,occ_ok=no)

fi

if test "x$occ_ok" = xyes ; then

  AC_MSG_CHECKING(for OpenCascade libraries)

  LIBS_old="$LIBS"
  LIBS="$LIBS $CAS_LDPATH -lTKernel"
  
  AC_CACHE_VAL(salome_cv_lib_occ,[
    AC_TRY_LINK(
#include <TCollection_AsciiString.hxx>
,    TCollection_AsciiString aStr ("toto");
    aStr.Capitalize();, 
    eval "salome_cv_lib_occ=yes",eval "salome_cv_lib_occ=no")
  ])
  occ_ok="$salome_cv_lib_occ"

fi
CPPFLAGS="$CPPFLAGS_old"
LIBS="$LIBS_old"

if test "x$occ_ok" = xno ; then
  AC_MSG_RESULT(no)
  AC_MSG_WARN(Opencascade libraries not found)
else
  AC_MSG_RESULT(yes)
  CAS_KERNEL="$CAS_LDPATH -lTKernel"
  CAS_MATH="$CAS_LDPATH -lTKMath"

  if test -f $CASROOT/$casdir/lib/libStdPlugin.so ; then
    # this libraries are only for CASCADE 5.2.3
    CAS_STDPLUGIN="StdPlugin"
  fi

  CAS_OCAF="$CAS_LDPATH -lPTKernel -lTKernel -lTKCDF -lTKLCAF -lTKPCAF -lTKStdSchema"
  CAS_OCAFVIS="$CAS_LDPATH -lTKCAF -lStdPlugin -lStdLPlugin -lTKPLCAF -lTKPShape -lTKStdLSchema -lTKShapeSchema"
  
  CAS_TKV3d="$CAS_LDPATH -lTKV3d"
  CAS_VIEWER="$CAS_TKV3d -lTKService"

  CAS_TKBRep="$CAS_LDPATH -lTKG2d -lTKG3d -lTKGeomBase -lTKBRep"

  CAS_TKTopAlgo="$CAS_TKBRep -lTKGeomAlgo -lTKTopAlgo"
  CAS_TKPrim="$CAS_TKTopAlgo -lTKPrim"
  
  CAS_MODELER="$CAS_TKPrim -lTKBO -lTKBool -lTKHLR -lTKFillet -lTKOffset -lTKFeat"

  CAS_DATAEXCHANGE="$CAS_LDPATH -lTKIGES -lTKSTEP"

  CAS_LDFLAGS="$CAS_KERNEL $CAS_MATH $CAS_OCAF $CAS_OCAFVIS $CAS_VIEWER $CAS_MODELER $CAS_DATAEXCHANGE"  

fi

AC_LANG_RESTORE

])dnl


