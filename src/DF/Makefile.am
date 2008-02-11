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
#  File   : Makefile.am
#  Author : Guillaume Boulant (CSSI)
#  Module : KERNEL

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Files to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS= \
	DF_Attribute.hxx \
	DF_Label.hxx \
	DF_Application.hxx \
	DF_Document.hxx \
	DF_ChildIterator.hxx \
	DF_Container.hxx \
	DF_definitions.hxx

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS=\
	-I$(srcdir)/../Basics \
	-I$(srcdir)/../SALOMELocalTrace \
	-I$(srcdir)/../HDFPersist \
	@HDF5_INCLUDES@ \
	@BOOST_CPPFLAGS@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS =\
	../HDFPersist/libSalomeHDFPersist.la \
	@HDF5_LIBS@

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libDF.la
libDF_la_SOURCES = \
	DF_Attribute.cxx \
	DF_Label.cxx \
	DF_Document.cxx \
	DF_Application.cxx \
	DF_ChildIterator.cxx \
	DF_Container.cxx \
\
	DF_Attribute.hxx \
	DF_Label.hxx \
	DF_Application.hxx \
	DF_Document.hxx \
	DF_ChildIterator.hxx \
	DF_Container.hxx \
	DF_definitions.hxx	

libDF_la_CPPFLAGS = $(COMMON_CPPFLAGS)
libDF_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libDF_la_LIBADD    = $(COMMON_LIBS)

#
# ===============================================================
# Executables targets
# ===============================================================
#
bin_PROGRAMS = testDF
testDF_SOURCES  = testDF.cxx
testDF_CPPFLAGS = $(COMMON_CPPFLAGS)
testDF_LDADD    = ./libDF.la \
                  $(HDF5_LIBS)

