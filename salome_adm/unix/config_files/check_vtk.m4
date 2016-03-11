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

dnl  OPTIONS_VTK
dnl  ------------------------------------------------------------------------
dnl  Adds the --with-vtk=path, --with-vtk-version and --with-paraview configure options
dnl
AC_DEFUN([OPTIONS_VTK], [
  AC_ARG_WITH([vtk],
              [AC_HELP_STRING([--with-vtk], [The prefix where VTK is installed (default "" means taking from environment variable unless VTK from ParaView is detected)])],
              [with_vtk=$withval], [with_vtk=""])

  AC_ARG_WITH([vtk-version],
              [AC_HELP_STRING([--with-vtk-version], [VTK include directory name is vtk-suffix, e.g. vtk-5.0/. What is the suffix? (Default "yes" means taking from environment variable)])],
              [vtk_suffix=$withval], [vtk_suffix="yes"])

  AC_ARG_WITH([paraview],
              [AC_HELP_STRING([--with-paraview], [ParaView from the specified location is used instead of VTK (default "" means taking from environment variable)])])
])

dnl
dnl  CHECK_VTK
dnl  ------------------------------------------------------------------------
dnl
AC_DEFUN([CHECK_VTK], [
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CXX])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([AC_PROG_CXXCPP])dnl
AC_REQUIRE([AC_LINKER_OPTIONS])dnl

AC_REQUIRE([OPTIONS_VTK])dnl

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_SUBST(VTK_INCLUDES)
AC_SUBST(VTK_LIBS)
AC_SUBST(VTK_SUFFIX)

VTK_INCLUDES=""
VTK_LIBS=""
VTK_SUFFIX=""

vtk_ok=no

if test "x$OpenGL_ok" != "xyes" ; then
   AC_MSG_WARN(VTK needs OpenGL correct configuration, check configure output)
fi

AC_PATH_X
if test "x$x_libraries" != "x"
then
   LXLIB="-L$x_libraries"
else
   LXLIB=""
fi

if test "x$x_libraries" = "x/usr/lib"
then
   LXLIB=""
fi

if test "x$x_libraries" = "x/usr/lib${LIB_LOCATION_SUFFIX}"
then
   LXLIB=""
fi

dnl VTK version suffix
if test -z $vtk_suffix ; then
  vtk_suffix="yes"
fi
if test "x$vtk_suffix" = "xno" ; then
  dnl in case user wrote --with-vtk-version=no, use empty suffix
  vtk_suffix=""
fi
if test "x$vtk_suffix" != "xyes" ; then
  VTKSUFFIX="$vtk_suffix"
else
  dnl in case user wrote --with-vtk-version=yes, get the suffix from env
  if test -z $VTKSUFFIX ; then
    VTKSUFFIX="-6.0"
  fi
fi

dnl
dnl Use VTK from ParaView unless --with-paraview=no is given
dnl

pv_vtk_ok=no

case "x$with_paraview" in

  xno )
    PVHOME=""
    ;;

  xyes | x )
  
    if test -z $PVHOME ; then
      AC_PATH_PROG(para_path, paraview)
      if test "x$para_path" != "x" ; then
         para_path=`dirname $para_path`
         PVHOME=`dirname $para_path`
      else
        for d in /usr/local /usr ; do
          if test -f ${d}/include/paraview${PVVERSION}/pqDialog.h ; then
            AC_MSG_RESULT(trying ${d})
            PVHOME="${d}"
            break
          fi
	  for suffix in 3.98 4.0; do
            if test -f ${d}/include/paraview-${suffix}/pqDialog.h ; then
              AC_MSG_RESULT(trying ${d})
              PVHOME="${d}"
              PVVERSION="-${suffix}"
              break
            fi
          done
          if test "x${PVHOME}" != "x" ; then break ; fi
          if test -f ${d}/include/paraview/pqDialog.h ; then
            AC_MSG_RESULT(trying ${d})
            PVHOME="${d}"
            PVVERSION=""
            break
          fi
        done
      fi
    fi
    ;;

  * )
    PVHOME=${with_paraview}
    ;;
esac

dnl Check VTK from ParaView.

if test "x$PVHOME" != "x" ; then

  if test "x$PVVERSION" = "x" ; then
    for suffix in 3.98 4.0; do
      if test -f $PVHOME/include/paraview-$suffix/vtkPVConfig.h ; then
       	PVVERSION=$suffix
        break;
      fi
    done
  fi	
  if test "x$PVVERSION" = "x" ; then
    ls -1d $PVHOME/include/paraview-* > /dev/null 2>&1
    if test $? = 0 ; then
      nb=$(ls -1d $PVHOME/include/paraview-* | wc -l)
      if test $nb = 1 ; then
        d=$(ls -1d $PVHOME/include/paraview-*)
        PVVERSION=$(echo $d | awk -F "-" '{print $(NF)}')
      fi
    fi
  fi
  if test "x$PVVERSION" = "x" ; then
    PVVERSION=`basename $PVHOME | sed -e "s,[[^-]]*,,"`
  else
    if test "${PVVERSION:0:1}" != "-" ; then
      PVVERSION="-$PVVERSION"
    fi
  fi

  AC_CHECKING(for VTK from ParaView)

  PV_LOCAL_INCLUDES="-I$PVHOME/include/paraview$PVVERSION $OGL_INCLUDES"

  dnl vtk headers
  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $PV_LOCAL_INCLUDES"
  
  AC_CHECK_HEADER(vtkPoints.h,pv_vtk_ok="yes",pv_vtk_ok="no")
  
  CPPFLAGS="$CPPFLAGS_old"
  
  if test "x$pv_vtk_ok" = "xyes"; then

     dnl vtk libraries
  
     AC_MSG_CHECKING(linking VTK library from ParaView)
  
     LIBS_old="$LIBS"
     CPPFLAGS_old="$CPPFLAGS"

     CPPFLAGS="$CPPFLAGS $PV_LOCAL_INCLUDES"

     dnl - try libs without suffix

     PV_LOCAL_LIBS="-L$PVHOME/lib/paraview$PVVERSION -lvtksys -lvtkmetaio -lvtkverdict -lvtkNetCDF -lvtkDICOMParser -lvtkftgl -lvtkexoIIc -lvtkCommonCore -lvtkCommonDataModel -lvtkCommonExecutionModel -lvtkFiltersFlowPaths -lvtkFiltersParallel -lvtkFiltersVerdict -lvtkInteractionStyle -lvtkInteractionWidgets -lvtkIOExport -lvtkIOXML -lvtkRenderingAnnotation -lvtkRenderingCore -lvtkRenderingFreeType -lvtkRenderingFreeTypeOpenGL -lvtkRenderingLabel -lvtkRenderingLOD -lvtkRenderingOpenGL -lvtkRenderingMatplotlib $LXLIB -lX11 -lXt"
     PV_TRY_LINK_LIBS="-L$PVHOME/lib/paraview$PVVERSION -lvtksys -lvtkCommonCore $LXLIB -lX11 -lXt"

     LIBS="${LIBS_old} $PV_TRY_LINK_LIBS"
  
#     AC_CACHE_VAL(salome_cv_lib_pvvtk,[
       AC_TRY_LINK([#include "vtkPoints.h"
                   ],
  		 [vtkPoints::New()],
  		 [salome_cv_lib_pvvtk=yes],
  		 [salome_cv_lib_pvvtk=no])
#     ])

     if test "x$salome_cv_lib_pvvtk" != "yes" -a "x$PVVERSION" != "x" ; then
         dnl - try libs with suffix
	 VTK_SUFFIX="-pv${PVVERSION:1:10}"
     	 PV_LOCAL_LIBS="-L$PVHOME/lib/paraview$PVVERSION -lvtksys${VTK_SUFFIX} -lvtkmetaio${VTK_SUFFIX} -lvtkverdict${VTK_SUFFIX} -lvtkNetCDF${VTK_SUFFIX} -lvtkDICOMParser${VTK_SUFFIX} -lvtkftgl${VTK_SUFFIX} -lvtkexoIIc${VTK_SUFFIX} -lvtkCommonCore${VTK_SUFFIX} -lvtkCommonDataModel${VTK_SUFFIX} -lvtkCommonExecutionModel${VTK_SUFFIX} -lvtkFiltersFlowPaths${VTK_SUFFIX} -lvtkFiltersParallel${VTK_SUFFIX} -lvtkFiltersVerdict${VTK_SUFFIX} -lvtkInteractionStyle${VTK_SUFFIX} -lvtkInteractionWidgets${VTK_SUFFIX} -lvtkIOExport${VTK_SUFFIX} -lvtkIOXML${VTK_SUFFIX} -lvtkRenderingAnnotation${VTK_SUFFIX} -lvtkRenderingCore${VTK_SUFFIX} -lvtkRenderingFreeType${VTK_SUFFIX} -lvtkRenderingFreeTypeOpenGL${VTK_SUFFIX} -lvtkRenderingLabel${VTK_SUFFIX} -lvtkRenderingLOD${VTK_SUFFIX} -lvtkRenderingOpenGL${VTK_SUFFIX} -lvtkRenderingMatplotlib${VTK_SUFFIX} $LXLIB -lX11 -lXt"
     	 PV_TRY_LINK_LIBS="-L$PVHOME/lib/paraview$PVVERSION -lvtksys${VTK_SUFFIX} -lvtkCommonCore${VTK_SUFFIX} $LXLIB -lX11 -lXt"

         LIBS="${LIBS_old} $PV_TRY_LINK_LIBS"
  
#         AC_CACHE_VAL(salome_cv_lib_pvvtk,[
           AC_TRY_LINK([#include "vtkPoints.h"
                       ],
  		     [vtkPoints::New()],
  		     [salome_cv_lib_pvvtk=yes],
  		     [salome_cv_lib_pvvtk=no])
#         ])
     fi

     pv_vtk_ok="$salome_cv_lib_pvvtk"

     LIBS="$LIBS_old"
     CPPFLAGS="$CPPFLAGS_old"
     AC_MSG_RESULT($pv_vtk_ok)
  fi

  dnl Find out version of VTK from ParaView
  PVVTKVERSION=" Undefined"
  if test "x$pv_vtk_ok" = "xyes"; then
     AC_MSG_CHECKING(VTK version)
     PVVTKVERSION=`grep VTK_VERSION $PVHOME/include/paraview$PVVERSION/vtkConfigure.h`
     AC_MSG_RESULT(${PVVTKVERSION:20:10})
  fi
fi

dnl
dnl Use regular VTK if no ParaView found or a newer version is provided via --with-vtk
dnl 

try_regular_vtk=no
if test "$pv_vtk_ok" = "no"; then
  try_regular_vtk=yes
fi

if test -z $with_vtk ; then
  with_vtk=""
fi
case "x$with_vtk" in
  xyes)
    dnl in case user wrote --with-vtk=yes
    with_vtk=""
    ;;
  xno)
    dnl in case user wrote --with-vtk=no
    with_vtk=""
    AC_MSG_WARN(Value "no", specified for option --with-vtk, is not supported)
    ;;
  x)
    ;;
  *)
    try_regular_vtk=yes
    ;;
esac

if test "$try_regular_vtk" = "yes"; then

  dnl Check regular VTK installation
  AC_CHECKING(for regular VTK)
  
  dnl VTK install dir

  if test "x$with_vtk" != "x" ; then
    VTKHOME="$with_vtk"
  else
    if test -z $VTKHOME ; then
      AC_MSG_WARN(undefined VTKHOME variable which specify where vtk was compiled)
      for d in /usr/local /usr ; do
        if test -f ${d}/include/vtk${VTKSUFFIX}/vtkPlane.h ; then
          AC_MSG_RESULT(trying ${d})
          VTKHOME="${d}"
          break
        fi
	for suffix in 5.8 5.9 6.0 ; do
          if test -f ${d}/include/vtk-${suffix}/vtkPlane.h ; then
            AC_MSG_RESULT(trying ${d})
            VTKHOME="${d}"
            VTKSUFFIX="-${suffix}"
            break
          fi
        done
        if test "x${VTKHOME}" != "x" ; then break ; fi
        if test -f ${d}/include/vtk/vtkPlane.h ; then
          AC_MSG_RESULT(trying ${d})
          VTKHOME="${d}"
          VTKSUFFIX=""
          break
        fi
      done
    fi
  fi
  
  # VSR: is it necessary to check with suffix as for ParaView?
  
  VTK_LOCAL_INCLUDES="-I$VTKHOME/include/vtk${VTKSUFFIX} $OGL_INCLUDES"
  VTK_LOCAL_LIBS="-L$VTKHOME/lib${LIB_LOCATION_SUFFIX}/vtk${VTKSUFFIX} -lvtkCommonCore -lvtkCommonDataModel -lvtkCommonExecutionModel -lvtkFiltersFlowPaths -lvtkFiltersParallel -lvtkFiltersVerdict -lvtkInteractionStyle -lvtkInteractionWidgets -lvtkIOExport -lvtkIOXML -lvtkRenderingAnnotation -lvtkRenderingCore -lvtkRenderingFreeType -lvtkRenderingFreeTypeOpenGL -lvtkRenderingLabel -lvtkRenderingLOD -lvtkRenderingOpenGL -lvtkRenderingMatplotlib $LXLIB -lX11 -lXt"
  VTK_TRY_LINK_LIBS="-L$VTKHOME/lib${LIB_LOCATION_SUFFIX} -L$VTKHOME/lib${LIB_LOCATION_SUFFIX}/vtk${VTKSUFFIX} -lvtkCommonCore $LXLIB -lX11 -lXt"
  
  dnl vtk headers
  CPPFLAGS_old="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $VTK_LOCAL_INCLUDES"
  
  AC_CHECK_HEADER(vtkPlane.h,vtk_ok="yes",vtk_ok="no")
  
  CPPFLAGS="$CPPFLAGS_old"
  
  if test "x$vtk_ok" = "xyes"; then
  
  #   VTK_INCLUDES="$OGL_INCLUDES"
  
     dnl vtk libraries
  
     AC_MSG_CHECKING(linking regular VTK library)
  
     LIBS_old="$LIBS"
     LIBS="$LIBS $VTK_TRY_LINK_LIBS"
     CPPFLAGS_old="$CPPFLAGS"
     CPPFLAGS="$CPPFLAGS $VTK_LOCAL_INCLUDES"
  
     AC_CACHE_VAL(salome_cv_lib_vtk,[
       AC_TRY_LINK([#include "vtkPlane.h"
                   ],
  		 [vtkPlane::New()],
  		 [salome_cv_lib_vtk=yes],
  		 [salome_cv_lib_vtk=no])
     ])
     vtk_ok="$salome_cv_lib_vtk"
     LIBS="$LIBS_old"
     CPPFLAGS="$CPPFLAGS_old"
     AC_MSG_RESULT($vtk_ok)
  fi
  
  VTKVERSION=" Undefined"
  if test "x$vtk_ok" = "xyes"; then
     AC_MSG_CHECKING(VTK version)
     VTKVERSION=`grep VTK_VERSION $VTKHOME/include/vtk${VTKSUFFIX}/vtkConfigure.h`
     AC_MSG_RESULT(${VTKVERSION:20:10})
  fi
fi

dnl Select either of VTKs
if  test "x$pv_vtk_ok" = "xyes" ; then
  if test "x$vtk_ok" = "xyes" ; then
     if test "$VTKVERSION" \> "$PVVTKVERSION" ; then
       AC_MSG_RESULT([VTK from ParaView is older, ignored])
       pv_vtk_ok=no
     else
       AC_MSG_RESULT([regular VTK is older, ignored])
       vtk_ok=no
     fi
  fi
fi  

if  test "x$pv_vtk_ok" = "xyes" ; then
  AC_MSG_RESULT(for VTK: yes)
  VTK_INCLUDES="$PV_LOCAL_INCLUDES -DVTK_EXCLUDE_STRSTREAM_HEADERS"
  VTK_LIBS="$PV_LOCAL_LIBS"
  vtk_ok=yes
else
    if  test "x$vtk_ok" = "xyes" ; then
      AC_MSG_RESULT(for VTK: yes)
      VTK_INCLUDES="$VTK_LOCAL_INCLUDES -DVTK_EXCLUDE_STRSTREAM_HEADERS"
      VTK_LIBS="$VTK_LOCAL_LIBS"
    else
      AC_MSG_RESULT(for VTK: no)
      AC_MSG_WARN(unable to link with vtk library)
    fi
fi

AC_LANG_RESTORE

# Save cache
AC_CACHE_SAVE

])dnl


dnl
dnl  CHECK_PYVTK (Python wrappings for VTK)
dnl  ------------------------------------------------------------------------
dnl
AC_DEFUN([CHECK_PYVTK], [
AC_REQUIRE([AC_PROG_CC])dnl
AC_REQUIRE([AC_PROG_CXX])dnl
AC_REQUIRE([AC_PROG_CPP])dnl
AC_REQUIRE([AC_PROG_CXXCPP])dnl
AC_REQUIRE([AC_LINKER_OPTIONS])dnl
AC_REQUIRE([CHECK_VTK])dnl
AC_REQUIRE([CHECK_PYTHON])dnl

AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_SUBST(VTK_PYLIBS)
VTK_PYLIBS=""

if test "x$vtk_ok" != "x" ; then
    PYVERSION=`echo ${PYTHON_VERSION} | sed -e "s%\.%%g"`
    VTK_PYLIBS="-lvtkWrappingPython${PYVERSION}${VTK_SUFFIX}"
fi

AC_LANG_RESTORE

# Save cache
AC_CACHE_SAVE

])dnl

