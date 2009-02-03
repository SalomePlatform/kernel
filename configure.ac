#  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
#
#  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
#  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
#
#  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
#
# ================================================================
# Process this file with autoconf to produce a configure script
# ================================================================
#AC_PREREQ(2.59)
#
AC_INIT([Salome2 Project], [5.1.0], [paul.rascle@edf.fr], [SalomeKERNEL])

# AC_CONFIG_AUX_DIR defines an alternative directory where to find the auxiliary
# scripts such as config.guess, install-sh, ...
AC_CONFIG_AUX_DIR(salome_adm/unix/config_files)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([tar-pax])
#AC_CONFIG_HEADER([config.h])

XVERSION=`echo $VERSION | awk -F. '{printf("0x%02x%02x%02x",$1,$2,$3)}'`
AC_SUBST(XVERSION)

# set up MODULE_NAME variable for dynamic construction of directories (resources, etc.)
MODULE_NAME=kernel
AC_SUBST(MODULE_NAME)

echo
echo ---------------------------------------------
echo Initialize source and build root directories
echo ---------------------------------------------
echo

#
# This function return on stdout the absolute path of the filename in
# argument. Exemple:
# $ filename="../KERNEL_SRC/configure
# $ absfilename=`absolute_path $filename`
function absolute_path {
    filename=$1
    here=`pwd`
    apath=`dirname $filename`
    cd $apath
    apath=`pwd`
    cd $here
    echo $apath
}

# Build directory, where the configure script is executed.
ROOT_BUILDDIR=`pwd`
# Source directory, where the configure script is located.
ROOT_SRCDIR=`absolute_path $0`

AC_SUBST(ROOT_SRCDIR)
AC_SUBST(ROOT_BUILDDIR)

echo
echo Source root directory : $ROOT_SRCDIR
echo Build  root directory : $ROOT_BUILDDIR

# ----------------------------------------------------------------------------
# --- test options

AC_ARG_ENABLE(corba_gen,
  [AC_HELP_STRING([--enable-corba-gen],[Generate CORBA stuff [default=yes]])],
  [case "${enableval}" in
     yes) corba_gen=true ;;
     no)  corba_gen=false ;;
     *) AC_MSG_ERROR(bad value ${enableval} for --enable-corba-gen) ;;
  esac],[corba_gen=true])
AM_CONDITIONAL(CORBA_GEN, test x$corba_gen = xtrue)

# ---- option to build only launcher, resources manager and batch classes
AC_ARG_WITH(onlylauncher,
	    --with-onlylauncher,
	    WITHONLYLAUNCHER="yes",WITHONLYLAUNCHER="no")
AM_CONDITIONAL(WITHONLYLAUNCHER, test x$WITHONLYLAUNCHER = xyes)

# ----------------------------------------------------------------------------

echo
echo
echo ============================================================
echo testing general mandatory products - for all configurations
echo ============================================================
echo

dnl Modification B. Secher portage sur osf
AC_CHECK_PROG(SHELL,sh)
AC_SUBST(SHELL)

if test -z "$AR"; then
   AC_CHECK_PROGS(AR,ar xar,:,$PATH)
fi
AC_SUBST(AR)

echo
echo ---------------------------------------------
echo testing make
echo ---------------------------------------------
echo
AC_PROG_MAKE_SET
AC_PROG_INSTALL

echo
echo ---------------------------------------------
echo Configuring production
echo ---------------------------------------------
echo
# production.m4
AC_ENABLE_DEBUG(no)
AC_ENABLE_PRODUCTION(no)

echo
echo ---------------------------------------------
echo testing libtool
echo ---------------------------------------------
echo

dnl first, we set static to no!
dnl if we want it, use --enable-static
AC_ENABLE_STATIC(no)
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

echo
echo ---------------------------------------------
echo testing C/C++
echo ---------------------------------------------
echo

cc_ok=no
AC_PROG_CC
AC_PROG_CXX
AC_CXX_WARNINGS
AC_CXX_TEMPLATE_OPTIONS
AC_DEPEND_FLAG
# AC_CC_WARNINGS([ansi])
cc_ok=yes

dnl Library libdl :
AC_CHECK_LIB(dl,dlopen)

dnl Library librt : for alpha/osf
AC_CHECK_LIB(rt,nanosleep)

dnl add library libm :
AC_CHECK_LIB(m,ceil)

# _CS_gbo We should add all dependent libraries

AC_CXX_USE_STD_IOSTREAM

AC_CXX_HAVE_SSTREAM

# Testing linker
AC_LINKER_OPTIONS

echo
echo ---------------------------------------------
echo testing threads
echo ---------------------------------------------
echo

ENABLE_PTHREADS

echo
echo ---------------------------------------------
echo testing python
echo ---------------------------------------------
echo

CHECK_PYTHON

echo
echo ---------------------------------------------
echo testing swig
echo ---------------------------------------------
echo

dnl _CS_gbo We should use here a variable given from the CHECK_PYTHON
AM_PATH_PYTHON(2.3)
CHECK_SWIG

echo
echo ---------------------------------------------
echo Testing libxml2
echo ---------------------------------------------
echo

CHECK_LIBXML

if test x$WITHONLYLAUNCHER = xno; then
echo
echo ---------------------------------------------
echo testing HDF5
echo ---------------------------------------------
echo

CHECK_HDF5

# ----------------------------------------------------------------------------
# --- test corba

if test x$corba_gen = xtrue; then

echo
echo ============================================================
echo testing mandatory products for CORBA configurations
echo ============================================================
echo

dnl
dnl ---------------------------------------------
dnl testing sockets
dnl ---------------------------------------------
dnl

CHECK_SOCKETS

echo
echo ---------------------------------------------
echo Configuration of the ORB
echo ---------------------------------------------
echo

echo Testing OMNIORB ...
CHECK_OMNIORB

echo Selecting the default ORB
DEFAULT_ORB=omniORB
echo DEFAULT_ORB : $DEFAULT_ORB
CHECK_CORBA

CORBA=salome_adm/unix/make_$ORB
AC_SUBST_FILE(CORBA)

# _CS_gbo:
# The CORBA variable defines the file name that
# implements the building rules to be included by Makefile to process
# idl files and CORBA specific dependencies (ex: the file
# salome_adm/unix/make_omniorb is the default for SALOME). The value
# of ORB is defined by CHECK_CORBA.

# Note that this checking process is no longer relevant for SALOME
# because the possibility of changing the ORB as a plugin is probably
# deprecated. The software configuration is clearly omniorb for CORBA
# communication. This could be simplify in a near futur.

echo
echo ---------------------------------------------
echo BOOST Library
echo ---------------------------------------------
echo

CHECK_BOOST

fi 
# --- end test corba
# ----------------------------------------------------------------------------

fi 
# --- end test ONLYLAUNCHER
# ----------------------------------------------------------------------------

echo
echo ============================================================
echo testing parallel products
echo ============================================================
echo

echo
echo ---------------------------------------------
echo checking if MPI is requested by user
echo ---------------------------------------------
echo

CHECK_MPI

echo
echo ---------------------------------------------
echo checking if PaCO++ is requested by user
echo ---------------------------------------------
echo

CHECK_PACO

echo
echo ------------------------------------------------------------
echo checking if parallel kernel extensions are requested by user
echo ------------------------------------------------------------
echo

AC_MSG_CHECKING(whether to enable parallel kernel extension)
AC_ARG_ENABLE(parallel_extension, 
	      AC_HELP_STRING([--enable-parallel_extension], [parallel kernel extension = [no/yes] (default is no)]),
	      parallel_extension_ok=$enableval,		       
	      parallel_extension_ok=no)
if test "x$parallel_extension_ok" = "xyes"
then
  if test "x$PaCO_ok" = "xno"
  then
    parallel_extension_ok=no
  fi
fi

if test "x$parallel_extension_ok" = "xyes"
then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi

AM_CONDITIONAL([WITH_PACO_PARALLEL], [test "x$parallel_extension_ok" = "xyes"])

echo
echo ============================================================
echo testing optionnal products
echo ============================================================
echo

echo
echo ---------------------------------------------
echo Testing Batch
echo ---------------------------------------------
echo

dnl Several batch systems (OpenPBS, LSF, ...) can be operated using
dnl the Batch classes library integrated in the KERNEL module. The
dnl checking process tests here the presence of underlying batch
dnl softwares. If none is detected, the KERNEL is declared to be
dnl "without batch". 

echo testing OpenPBS
echo ---------------
openpbs_ok=no
CHECK_OPENPBS
dnl openpbs_ok is set to yes by CHECK_OPENPBS

echo testing LSF
echo -----------
lsf_ok=no
CHECK_LSF
dnl lsf_ok is set to yes by CHECK_LSF
echo lsf = $lsf_ok

echo testing Local batch system
echo --------------------------
localbatch_ok=no
CHECK_LOCAL
dnl localbatch_ok is set to yes by CHECK_LOCAL

WITH_BATCH=no
test x$openpbs_ok = xyes || test x$lsf_ok = xyes || test x$localbatch_ok = xyes && WITH_BATCH=yes

AC_ARG_ENABLE(batch,
  [AC_HELP_STRING([--enable-batch],[Batch [default=yes]])],
  [case "${enableval}" in
     yes) test x$openpbs_ok = xyes || test x$lsf_ok = xyes || test x$localbatch_ok = xyes && WITH_BATCH=yes;;
     no)  WITH_BATCH=no ;;
     *) AC_MSG_ERROR(bad value ${enableval} for --enable-batch) ;;
  esac],[test x$openpbs_ok = xyes || test x$lsf_ok = xyes || test x$localbatch_ok = xyes && WITH_BATCH=yes])

echo Batch mode = $WITH_BATCH

AC_SUBST(WITH_BATCH)
AM_CONDITIONAL(WITH_BATCH, [test x"$WITH_BATCH" = xyes])

echo
echo ----------------------------------------------
echo testing CPPUNIT only required for unit testing
echo ----------------------------------------------
echo
CHECK_CPPUNIT

echo
echo ============================================================
echo testing products required only for documentation generation
echo ============================================================
echo

echo
echo ---------------------------------------------
echo Testing html generators
echo ---------------------------------------------
echo

CHECK_HTML_GENERATORS

echo
echo ============================================================
echo Summary
echo ============================================================
echo

# This function displays the values of each variable given in arguments 
function summary {
  variables=$*
  for var in $variables
  do
    eval toto=\$$var
    if test x$toto != "x"; then
      printf "   %10s : " `echo \$var | sed -e "s,_ok,,"`
      eval echo \$$var
    fi
  done
}

# --------------------------------------

function check_fatal_error {
  variables=$*
  for var in $variables
  do
    eval toto=\$$var
    if test x$toto != "xyes"; then
        echo "FATAL ERROR: some mandatory products are missing."
	echo "Installing the missing products is required before running the configure script".
    	exit 1
    fi
  done  
}

# --------------------------------------

if test x$WITHONLYLAUNCHER = xno; then
  if test x$corba_gen = xtrue; then
    basic_mandatory_products="cc_ok threads_ok python_ok swig_ok hdf5_ok libxml_ok"
  else
    basic_mandatory_products="cc_ok threads_ok hdf5_ok libxml_ok"
  fi
else
  basic_mandatory_products="cc_ok threads_ok libxml_ok"
fi

echo --- General mandatory products - Light configuration:
summary $basic_mandatory_products
check_fatal_error $basic_mandatory_products
echo

if test x$WITHONLYLAUNCHER = xno; then
  corba_mandatory_products="omniORB_ok omniORBpy_ok boost_ok"
  if test x$corba_gen = xtrue; then
    echo --- CORBA mandatory products - default configuration:
    summary $corba_mandatory_products
    check_fatal_error $corba_mandatory_products
  else
    echo --- CORBA products not required - option --disable-corba-gen
    if test x"$WITH_BATCH" = xyes; then
      echo --- BATCH mode mandatory products - default configuration:
      summary "python_ok"
      check_fatal_error "python_ok"
    else 
    echo --- Python not required - option --enable-batch=no
    fi
  fi
fi
echo

parallel_products="mpi_ok PaCO_ok parallel_extension_ok"
echo --- Kernel parallel extensions:
summary $parallel_products
echo

optional_products="cppunit_ok openpbs_ok lsf_ok numpy_ok"
echo --- Optional products:
echo ["    These products are optional because the KERNEL functions"]
echo ["    using them are built only if the products are detected."]
summary $optional_products
echo

htmldoc_products="doxygen_ok graphviz_ok rst2html_ok"

echo --- Html documentation products: only required for doc production
summary $htmldoc_products
echo

if test x$WITHONLYLAUNCHER = xno; then
echo
echo "Default ORB   : $DEFAULT_ORB"
echo
fi

dnl generals files which could be included in every makefile

AC_SUBST_FILE(COMMENCE) COMMENCE=salome_adm/unix/make_commence
AC_SUBST_FILE(CONCLUDE) CONCLUDE=salome_adm/unix/make_conclude
AC_SUBST_FILE(MODULE) MODULE=salome_adm/unix/make_module

dnl les dependences
AC_SUBST_FILE(DEPEND) DEPEND=salome_adm/unix/depend

dnl We don t need to say when we re entering directories if we re using
dnl GNU make becuase make does it for us.
if test "X$GMAKE" = "Xyes"; then
   AC_SUBST(SETX) SETX=":"
else
   AC_SUBST(SETX) SETX="set -x"
fi

echo
echo ---------------------------------------------
echo generating Makefiles and configure files
echo ---------------------------------------------
echo

# This list is initiated using autoscan and must be updated manually
# when adding a new file <filename>.in to manage. When you execute
# autoscan, the Makefile list is generated in the output file configure.scan.
# This could be helpfull to update de configuration.
AC_OUTPUT([ \
	./KERNEL_version.h \
	./salome_adm/unix/SALOMEconfig.ref \
	./salome_adm/Makefile \
	./salome_adm/cmake_files/Makefile \
	./salome_adm/unix/Makefile \
	./salome_adm/unix/config_files/Makefile \
	Makefile \
	./bin/Makefile \
	./bin/VERSION \
	./doc/Makefile \
	./doc/salome/Makefile \
	./doc/salome/tui/Makefile \
	./doc/salome/tui/INPUT/doxyuser:./doc/salome/tui/KERNEL/doxyuser.in \
	./doc/salome/tui/INPUT/doxydev:./doc/salome/tui/KERNEL/doxydev.in \
	./doc/salome/tui/INPUT/sources/static/tree.js:./doc/salome/tui/KERNEL/sources/static/tree.js.in \
	./idl/Makefile \
	./resources/Makefile \
	./resources/KERNELCatalog.xml \
	./resources/CatalogResources.xml \
	./src/Makefile \
	./src/Basics/Makefile \
	./src/Basics/Test/Makefile \
	./src/Batch/Makefile \
	./src/Batch_SWIG/Makefile \
	./src/Communication/Makefile \
	./src/Communication_SWIG/Makefile \
	./src/Container/Makefile \
	./src/ParallelContainer/Makefile \
	./src/DF/Makefile \
	./src/DSC/Makefile \
	./src/DSC/DSC_Basic/Makefile \
	./src/DSC/DSC_User/Makefile \
	./src/DSC/DSC_User/Basic/Makefile \
	./src/DSC/DSC_User/Datastream/Makefile \
	./src/DSC/DSC_User/Datastream/Palm/Makefile \
	./src/DSC/DSC_User/Datastream/Calcium/Makefile \
	./src/DSC/ParallelDSC/Makefile \
	./src/DSC/DSC_Python/Makefile \
	./src/GenericObj/Makefile \
	./src/HDFPersist/Makefile \
	./src/KERNEL_PY/Makefile \
	./src/Launcher/Makefile \
	./src/LifeCycleCORBA/Makefile \
	./src/LifeCycleCORBA/Test/Makefile \
	./src/LifeCycleCORBA_SWIG/Makefile \
	./src/LifeCycleCORBA_SWIG/Test/Makefile \
	./src/Logger/Makefile \
	./src/Logger/Test/Makefile \
	./src/ModuleCatalog/Makefile \
	./src/ModuleGenerator/Makefile \
	./src/ModuleGenerator/testIDLparser \
	./src/MPIContainer/Makefile \
	./src/NamingService/Makefile \
	./src/NamingService/Test/Makefile \
	./src/Notification/Makefile \
	./src/NOTIFICATION_SWIG/Makefile \
	./src/Registry/Makefile \
	./src/ResourcesManager/Makefile \
	./src/SALOMEDS/Makefile \
	./src/SALOMEDS/Test/Makefile \
	./src/SALOMEDSClient/Makefile \
	./src/SALOMEDSImpl/Makefile \
	./src/SALOMEDSImpl/Test/Makefile \
	./src/SALOMELocalTrace/Makefile \
	./src/SALOMELocalTrace/Test/Makefile \
	./src/SALOMETraceCollector/Makefile \
	./src/SALOMETraceCollector/Test/Makefile \
	./src/TestContainer/Makefile \
	./src/TestMPIContainer/Makefile \
	./src/TOOLSDS/Makefile \
	./src/UnitTests/Makefile \
	./src/Utils/Makefile \
	./src/Utils/Test/Makefile \
])

