# Copyright (C) 2005  OPEN CASCADE, CEA, EDF R&D, LEG
#           PRINCIPIA R&D, EADS CCR, Lip6, BV, CEDRAT
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either 
# version 2.1 of the License.
# 
# This library is distributed in the hope that it will be useful 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public  
# License along with this library; if not, write to the Free Software 
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
# 
# See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
# 
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
