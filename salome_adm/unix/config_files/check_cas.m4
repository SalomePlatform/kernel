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
AC_SUBST(CAS_VIEWER)
AC_SUBST(CAS_MODELER)
AC_SUBST(CAS_OCAF)
AC_SUBST(CAS_DATAEXCHANGE)
AC_SUBST(CAS_LDFLAGS)
AC_SUBST(CAS_LDPATH)

CAS_CPPFLAGS=""
CAS_CXXFLAGS=""
CAS_LDFLAGS=""
occ_ok=no

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
if test -z $CASROOT; then
  AC_MSG_WARN(You must provide CASROOT variable : see OCC installation manual)
else
  occ_ok=yes
  OCC_VERSION_MAJOR=0
  ff=$CASROOT/inc/Standard_Version.hxx
  if test -f $ff ; then
    grep "define OCC_VERSION_MAJOR" $ff > /dev/null
    if test $? = 0 ; then
      OCC_VERSION_MAJOR=`grep "define OCC_VERSION_MAJOR" $ff | awk '{i=3 ; print $i}'`
    fi
  fi
fi

if test "x$occ_ok" = "xyes"; then

dnl test c++ compiler flag for unsigned character
  for opt in -funsigned-char -unsigned ; do
    AC_CXX_OPTION($opt,CXXFLAGS,flag=yes,flag=no)
    if test "$flag" = "yes"; then
      break
    fi
  done
  
dnl cascade headers

  CPPFLAGS_old="$CPPFLAGS"
  CAS_CPPFLAGS="-DOCC_VERSION_MAJOR=$OCC_VERSION_MAJOR -DLIN -DLINTEL -DCSFDB -DNO_CXX_EXCEPTION -DNo_exception -I$CASROOT/inc"
  CPPFLAGS="$CPPFLAGS $CAS_CPPFLAGS"

  AC_CHECK_HEADER(Standard_Type.hxx,occ_ok=yes ,occ_ok=no)

fi

if test "x$occ_ok" = xyes ; then

  AC_MSG_CHECKING(for OpenCascade libraries)

  LIBS_old="$LIBS"
  LIBS="$LIBS $CAS_LDPATH -lTKernel"
  
  AC_CACHE_VAL(salome_cv_lib_occ,[
    AC_TRY_LINK(
#include <Standard_Type.hxx>
,   size_t size;
    const Standard_CString aName="toto";
    Standard_Type myST(aName) ; 
    myST.Find(aName);,
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
  CAS_KERNEL="$CAS_LDPATH -lTKernel -lTKMath"

  # E.A. compatibility version 4 and 5.x  
  CAS_OCAF="$CAS_LDPATH -lPTKernel -lTKCAF -lFWOSPlugin -lTKPShape -lTKPCAF -lTKStdSchema -lTKShapeSchema -lPAppStdPlugin"
  if test $OCC_VERSION_MAJOR -lt 5 ; then
    CAS_OCAF="$CAS_OCAF -lTKPAppStd"
  fi
  CAS_OCAF="$CAS_OCAF -lTKCDF"
  
  CAS_VIEWER="$CAS_LDPATH -lTKOpenGl -lTKV3d -lTKService"
  CAS_MODELER="$CAS_LDPATH -lTKG2d -lTKG3d -lTKGeomBase -lTKBRep -lTKGeomAlgo -lTKTopAlgo -lTKPrim -lTKBool -lTKHLR -lTKFillet -lTKOffset"

  # E.A. compatibility version 4 and 5.x  
  CAS_DATAEXCHANGE="$CAS_LDPATH -lTKXSBase -lTKIGES -lTKSTEP -lTKShHealing"
  if test $OCC_VERSION_MAJOR -lt 5 ; then
    CAS_DATAEXCHANGE="$CAS_DATAEXCHANGE -lTKShHealingStd"
  fi


  CAS_LDFLAGS="$CAS_KERNEL $CAS_OCAF $CAS_VIEWER $CAS_MODELER $CAS_DATAEXCHANGE"  

fi

AC_LANG_RESTORE

])dnl


