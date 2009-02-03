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
#  File   : Makefile.am
#  Author : André RIBES (EDF), Eric Fayolle (EDF)
#  Module : KERNEL
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

SUBDIRS = Palm Calcium

#
# ===============================================================
# Header to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = GenericPort.hxx \
			GenericProvidesPort.hxx \
			GenericUsesPort.hxx \
			DataIdFilter.hxx \
			CorbaTypeManipulator.hxx \
			CouplingPolicy.hxx \
			DisplayPair.hxx \
			FindKeyPredicate.hxx \
			AdjacentFunctor.hxx \
			IteratorTraits.hxx \
			ConstTraits.hxx

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS= @BOOST_CPPFLAGS@ -I$(top_srcdir)/src/DSC/DSC_User

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS = $(top_builddir)/src/DSC/DSC_User/Datastream/Palm/libSalomePalm.la \
	      $(top_builddir)/src/DSC/DSC_User/Datastream/Calcium/libSalomeCalcium.la \
				../libSalomeDSCSuperv.la

#
# ===============================================================
# Libraries targets
# ===============================================================
#
lib_LTLIBRARIES = libSalomeDatastream.la

libSalomeDatastream_la_SOURCES        = fake.cc

libSalomeDatastream_la_CXXFLAGS       = $(COMMON_CPPFLAGS)

libSalomeDatastream_la_LIBADD         = $(COMMON_LIBS)

libSalomeDatastream_la_LDFLAGS        = -no-undefined -version-info=0:0:0

