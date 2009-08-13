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
#  Author : André RIBES (EDF)
#  Module : KERNEL
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

#
# ===============================================================
# Header to be installed
# ===============================================================
#
# header files  
salomeinclude_HEADERS = basic_port_factory.hxx \
			data_short_port_provides.hxx \
			data_short_port_uses.hxx

#
# ===============================================================
# Local definitions
# ===============================================================
#

# This local variable defines the list of CPPFLAGS common to all target in this package.
COMMON_CPPFLAGS= -I$(top_builddir)/idl \
		 -I$(top_srcdir)/src/DSC/DSC_Basic \
		 -I$(top_srcdir)/src/DSC/DSC_User \
		 -I$(top_srcdir)/src/Container \
		 -I$(top_srcdir)/src/Notification \
		 -I$(top_srcdir)/src/SALOMELocalTrace \
		 -I$(top_srcdir)/src/Basics \
		 -I$(top_srcdir)/src/Utils \
		 @CORBA_CXXFLAGS@ @CORBA_INCLUDES@

# This local variable defines the list of dependant libraries common to all target in this package.
COMMON_LIBS = 

#
# ===============================================================
# Libraries targets
# ===============================================================
#

lib_LTLIBRARIES = libSalomeDSCSupervBasic.la

libSalomeDSCSupervBasic_la_SOURCES  = basic_port_factory.cxx \
				      data_short_port_uses.cxx \
				      data_short_port_provides.cxx 

libSalomeDSCSupervBasic_la_CXXFLAGS = $(COMMON_CPPFLAGS)

libSalomeDSCSupervBasic_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSalomeDSCSupervBasic_la_LIBADD  = ../libSalomeDSCSuperv.la

