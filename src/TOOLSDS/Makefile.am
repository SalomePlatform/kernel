include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = SALOMEDS_Tool.hxx 

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../Utils \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@ \
	@CAS_CPPFLAGS@ @CAS_CXXFLAGS@ \
	@HDF5_INCLUDES@ \
	@BOOST_CPPFLAGS@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	../Utils/libOpUtil.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la\
	@CAS_KERNEL@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libTOOLSDS.la
libTOOLSDS_la_SOURCES = 	\
	SALOMEDS_Tool.cxx

libTOOLSDS_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libTOOLSDS_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libTOOLSDS_la_LIBADD    = $(COMMON_LIBS)

#
# ===============================================================
# Executables targets
# ===============================================================
#
# Nothing to build


