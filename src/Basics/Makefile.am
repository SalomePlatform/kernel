# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
#
# Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
# CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
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

#  Basics: General purpose C++ 
#  File   : Makefile.in
#  Author : Paul RASCLE (EDF)
#  Module : SALOME
#  $Header$

include $(top_srcdir)/salome_adm/unix/make_common_starter.am

salomeinclude_HEADERS       = \
	SALOME_Basics.hxx \
	BasicsGenericDestructor.hxx \
	Basics_Utils.hxx \
	Basics_DirUtils.hxx \
	Basics_OCCTVersion.hxx

lib_LTLIBRARIES             = libSALOMEBasics.la
libSALOMEBasics_la_SOURCES  = \
	BasicsGenericDestructor.cxx \
	Basics_Utils.cxx \
	Basics_DirUtils.cxx \
	SALOME_Basics.hxx \
	BasicsGenericDestructor.hxx \
	Basics_Utils.hxx \
	Basics_DirUtils.hxx
libSALOMEBasics_la_LDFLAGS  = -no-undefined -version-info=0:0:0
libSALOMEBasics_la_CPPFLAGS = @CPPFLAGS@ 
