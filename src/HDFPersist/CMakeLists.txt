# Copyright (C) 2012  CEA/DEN, EDF R&D, OPEN CASCADE
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

INCLUDE_DIRECTORIES(
  ${HDF5_INCLUDE_DIRS}
)

SET(SalomeHDFPersist_SOURCES
  HDFfileCreate.c
  HDFfileClose.c
  HDFfileOpen.c
  HDFfileMount.c
  HDFfileUmount.c
  HDFgroupCreate.c
  HDFgroupOpen.c
  HDFgroupClose.c
  HDFdatasetOpen.c
  HDFdatasetClose.c
  HDFdatasetCreate.c
  HDFdatasetWrite.c
  HDFdatasetRead.c
  HDFdatasetGetDim.c
  HDFdatasetGetSize.c
  HDFdatasetGetType.c
  HDFdatasetGetnDim.c
  HDFdatasetGetOrder.c
  HDFarrayGetType.c
  HDFarrayGetTypeId.c
  HDFarrayCreate.c
  HDFarrayClose.c
  HDFarrayGetnDim.c
  HDFarrayGetDim.c
  HDFattrOpen.c
  HDFattrClose.c
  HDFattrWrite.c
  HDFattrCreate.c
  HDFattrRead.c
  HDFattrGetType.c
  HDFattrGetSize.c
  HDFerrorModeLock.c
  HDFnObjects.c
  HDFobjectIdentify.c
  HDFobjectType.c
  HDFobject.cc
  HDFarray.cc
  HDFinternalObject.cc
  HDFattribute.cc
  HDFcontainerObject.cc
  HDFdataset.cc
  HDFfile.cc
  HDFgroup.cc
  HDFexplorer.cc
  HDFconvert.cc
  HDFascii.cc
)

ADD_LIBRARY(SalomeHDFPersist SHARED ${SalomeHDFPersist_SOURCES})
SET_TARGET_PROPERTIES(SalomeHDFPersist PROPERTIES COMPILE_FLAGS "-D${MACHINE} ${HDF5_DEFINITIONS} ${PLATFORM_DEFINITIONS}")
TARGET_LINK_LIBRARIES(SalomeHDFPersist ${HDF5_LIBS} ${PLATFORM_LIBS})
INSTALL(TARGETS SalomeHDFPersist DESTINATION ${KERNEL_salomelib_LIBS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
FILE(GLOB COMMON_HEADERS_H "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
SET(COMMON_HEADERS
  ${COMMON_HEADERS_HXX}
  ${COMMON_HEADERS_H}
)
INSTALL(FILES ${COMMON_HEADERS} DESTINATION ${KERNEL_salomeinclude_HEADERS})
