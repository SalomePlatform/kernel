include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = \
	ReceiverFactory.hxx \
	SenderFactory.hxx \
	SALOMEMultiComm.hxx \
	MultiCommException.hxx \
	SALOME_Comm_i.hxx \
	MatrixClient.hxx \
	SALOME_Matrix_i.hxx

# Scripts to be installed

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../Utils \
	-I$(top_builddir)/salome_adm/unix \
	-I$(top_builddir)/idl \
	@CORBA_CXXFLAGS@ @CORBA_INCLUDES@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS = \
	../Utils/libOpUtil.la \
	../SALOMELocalTrace/libSALOMELocalTrace.la \
	$(top_builddir)/idl/libSalomeIDLKernel.la

# _CS_gbo The need for these flags depends on wether the swig
# interface is generated and if MPI is activated.
OPT_CPPFLAGS = @PYTHON_INCLUDES@  @MPI_INCLUDES@
OPT_LIBS     = @PYTHON_LIBS@ @MPI_LIBS@
OPT_LDFLAGS  = -Xlinker -export-dynamic

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeCommunication.la 
libSalomeCommunication_la_SOURCES =\
	SALOME_Comm_i.cxx \
	SALOME_Matrix_i.cxx \
	SenderFactory.cxx \
	MultiCommException.cxx \
	SALOMEMultiComm.cxx \
	ReceiverFactory.cxx \
	MatrixClient.cxx \
	\
	MultiCommException.hxx \
	SALOME_Comm_i.hxx \
	SALOME_Matrix_i.hxx \
	SenderFactory.hxx \
	ReceiverFactory.hxx \
        MatrixClient.hxx \
	SALOMEMultiComm.hxx \
	Receivers.hxx \
	Receiver.hxx

# the following file is needed by an include file (VERY DIRTY!)
EXTRA_DIST = Receivers.cxx

libSalomeCommunication_la_CPPFLAGS = $(COMMON_CPPFLAGS) $(OPT_CPPFLAGS)
libSalomeCommunication_la_LDFLAGS  = -no-undefined -version-info=0:0:0 $(OPT_LDFLAGS)
libSalomeCommunication_la_LIBADD    = $(COMMON_LIBS) $(OPT_LIBS)

#
# ===============================================================
# Executables targets
# ===============================================================
#
# Nothing to build
