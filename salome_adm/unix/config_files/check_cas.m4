AC_DEFUN([CHECK_CAS],[
AC_REQUIRE([AC_PROG_CXX])dnl
AC_REQUIRE([AC_PROG_CXXCPP])dnl

AC_CHECKING(for OpenCascade)

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_SUBST(CAS_CPPFLAGS)
AC_SUBST(CAS_CXXFLAGS)
AC_SUBST(CAS_LDFLAGS)

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
   osf4.*)
      casdir=Linux
      ;;
   solaris2.*)
      casdir=Linux
      ;;
   *)
      casdir=Linux
      ;;
esac

dnl were is OCC ?
if test -z $CASROOT; then
  AC_MSG_WARN(You must provide CASROOT variable : see OCC installation manual)
else
  occ_ok=yes
fi

if test "x$occ_ok" = "xyes"; then

dnl cascade headers

  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS -DLIN -DLINTEL -DCSFDB -DNO_CXX_EXCEPTION -DNo_exception -I$CASROOT/inc"
  CXXFLAGS_old="$CXXFLAGS"
  CXXFLAGS="$CXXFLAGS -funsigned-char"

  AC_CHECK_HEADER(Standard_Type.hxx,occ_ok=yes ,occ_ok=no)

  CPPFLAGS="$CPPFLAGS_old"
  CXXFLAGS="$CXXFLAGS_old"
fi

if test "x$occ_ok" = xyes ; then

  CAS_CPPFLAGS="-DLIN -DLINTEL -DCSFDB -DNO_CXX_EXCEPTION -DNo_exception -I$CASROOT/inc"
  CAS_CXXFLAGS="-funsigned-char"

  AC_MSG_CHECKING(for OpenCascade libraries)

  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $CAS_CPPFLAGS"
  CXXFLAGS_old="$CXXFLAGS"
  CXXFLAGS="$CXXFLAGS $CAS_CXXFLAGS"
  LIBS_old="$LIBS"
  LIBS="$LIBS -L$CASROOT/$casdir/lib -lTKernel"
  
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

  CPPFLAGS="$CPPFLAGS_old"
  CXXFLAGS="$CXXFLAGS_old"
  LIBS="$LIBS_old"
fi

if test "x$occ_ok" = xno ; then
  AC_MSG_RESULT(no)
  AC_MSG_WARN(Opencascade libraries not found)
else
  AC_MSG_RESULT(yes)
  CAS_LDFLAGS="-L$CASROOT/$casdir/lib -lTKPAppStd -lTKFillet -lTKService -lFWOSPlugin -lTKG2d -lTKShHealing -lTKShHealingStd -lTKOpenGl -lTKG3d -lTKTopAlgo -lTKGeomAlgo -lTKV2d -lPAppStdPlugin -lTKGeomBase -lTKV3d -lPTKernel -lTKHLR -lTKVRML -lTKShapeSchema -lTKIGES -lTKXSBase -lTKStdSchema -lTKMath -lTKBRep -lTKOffset -lTKernel -lTKBool -lTKPCAF -lTKCAF -lTKPShape -lTKCDF -lTKPrim -lTKSTEP -lTKFeat -lTKSTL"
fi

AC_LANG_RESTORE

])dnl
