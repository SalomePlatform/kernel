# Copyright (C) 2012-2016  CEA/DEN, EDF R&D, OPEN CASCADE
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
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
  ${MPI_INCLUDE_DIRS}
)

FILE(GLOB SalomeHDFPersist_SOURCES_C "${CMAKE_CURRENT_SOURCE_DIR}/HDF*.c")
FILE(GLOB SalomeHDFPersist_SOURCES_CC "${CMAKE_CURRENT_SOURCE_DIR}/HDF*.cc")
SET(SalomeHDFPersist_SOURCES ${SalomeHDFPersist_SOURCES_C} ${SalomeHDFPersist_SOURCES_CC})
ADD_DEFINITIONS(${HDF5_DEFINITIONS})

ADD_LIBRARY(SalomeHDFPersist ${SalomeHDFPersist_SOURCES})
TARGET_LINK_LIBRARIES(SalomeHDFPersist ${HDF5_LIBRARIES} ${MPI_LIBRARIES} ${PLATFORM_LIBS})
INSTALL(TARGETS SalomeHDFPersist EXPORT ${PROJECT_NAME}TargetGroup DESTINATION ${SALOME_INSTALL_LIBS})

FILE(GLOB COMMON_HEADERS_HXX "${CMAKE_CURRENT_SOURCE_DIR}/*.hxx")
FILE(GLOB COMMON_HEADERS_H "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
SET(COMMON_HEADERS
  ${COMMON_HEADERS_HXX}
  ${COMMON_HEADERS_H}
)
INSTALL(FILES ${COMMON_HEADERS} DESTINATION ${SALOME_INSTALL_HEADERS})
