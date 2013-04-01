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

#  SALOME HDFPersist : implementation of HDF persitent ( save/ restore )
#  File   : Makefile.in
#  Author : Paul RASCLE, EDF
#  Module : SALOME
#  $Header$
#
include $(top_srcdir)/salome_adm/unix/make_common_starter.am

salomeinclude_HEADERS      = \
	HDFattribute.hxx \
	HDFcontainerObject.hxx \
	HDFdataset.hxx \
	HDFexplorer.hxx \
	HDFexception.hxx \
	HDFfile.hxx \
	HDFgroup.hxx \
	HDFinternalObject.hxx \
	HDFobject.hxx \
	HDFarray.hxx \
	HDFOI.hxx \
	HDFtypes.h \
	HDFconvert.hxx \
	hdfi.h \
	HDFascii.hxx \
	HDFexport.hxx

# Libraries targets
lib_LTLIBRARIES                 = libSalomeHDFPersist.la
libSalomeHDFPersist_la_SOURCES  =\
	HDFfileCreate.c \
	HDFfileClose.c \
	HDFfileOpen.c \
	HDFfileMount.c \
	HDFfileUmount.c \
	HDFgroupCreate.c \
	HDFgroupOpen.c \
	HDFgroupClose.c \
	HDFdatasetOpen.c \
	HDFdatasetClose.c \
	HDFdatasetCreate.c \
	HDFdatasetWrite.c \
	HDFdatasetRead.c \
	HDFdatasetGetDim.c \
	HDFdatasetGetSize.c \
	HDFdatasetGetType.c \
	HDFdatasetGetnDim.c \
	HDFdatasetGetOrder.c \
	HDFarrayGetType.c \
	HDFarrayGetTypeId.c \
	HDFarrayCreate.c \
	HDFarrayClose.c \
	HDFarrayGetnDim.c \
	HDFarrayGetDim.c \
	HDFattrOpen.c \
	HDFattrClose.c \
	HDFattrWrite.c \
	HDFattrCreate.c \
	HDFattrRead.c \
	HDFattrGetType.c \
	HDFattrGetSize.c \
	HDFerrorModeLock.c \
	HDFnObjects.c \
	HDFobjectIdentify.c \
	HDFobjectType.c \
	HDFobject.cc \
	HDFarray.cc \
	HDFinternalObject.cc \
	HDFattribute.cc \
	HDFcontainerObject.cc \
	HDFdataset.cc \
	HDFfile.cc \
	HDFgroup.cc \
	HDFexplorer.cc \
	HDFconvert.cc \
	HDFascii.cc

libSalomeHDFPersist_la_LDFLAGS  = -no-undefined -version-info=0:0:0

libSalomeHDFPersist_la_CPPFLAGS = @HDF5_INCLUDES@ -D@MACHINE@ 

libSalomeHDFPersist_la_LIBADD   = @HDF5_LIBS@

EXTRA_DIST += \
  test1.c \
  test2.c \
  test3.cxx \
  test4.cxx \
  test5.cxx \
  test6.cxx \
  test7.cxx \
  test8.cxx \
  test9.cxx
