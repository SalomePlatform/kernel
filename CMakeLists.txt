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

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.7 FATAL_ERROR)

IF(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

# Temporary step instead Find Calcium.cmake
SET(CALCIUM_IDL_INT_F77 long)
SET(CALCIUM_CORBA_INT_F77 CORBA::Long)
SET(LONG_OR_INT int)

# Temporary definitions
SET(DOXYGEN_IS_OK 0)
IF(WINDOWS)
 SET(CPPUNIT_IS_OK 0)
ENDIF(WINDOWS)
SET(WITH_LOCAL 1)
SET(WITH_BATCH 1)

ENABLE_TESTING()
SET(VERSION "7.0.0")
SET(VERSION_DEV "1")

INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindPLATFORM.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindPYTHON.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindMPI.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindOMNIORB.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindPTHREADS.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindHDF5.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindBOOST.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindLIBXML2.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindSWIG.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindCPPUNIT.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindDOXYGEN.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindLIBBATCH.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/FindSPHINX.cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/salome_adm/cmake_files/SalomeMacros.cmake)

EXECUTE_PROCESS(COMMAND ${PYTHON_EXECUTABLE} -c "import sys; t=sys.argv[-1].split(\".\") ; t[:]=(int(elt) for elt in t) ; sys.stdout.write(\"0x%02x%02x%02x\"%tuple(t))" ${VERSION}
  OUTPUT_VARIABLE XVERSION)

SET(KERNEL_salomebin_BINS bin/salome)

SET(KERNEL_salomelib_LIBS lib/salome)

SET(KERNEL_salomeidl_IDLS idl/salome)

SET(KERNEL_salomeinclude_HEADERS include/salome)

SET(KERNEL_salomeres_DATA share/salome/resources/kernel)

SET(KERNEL_salomeres_SCRIPTS share/salome/resources/kernel)

SET(KERNEL_salomescript_SCRIPTS bin/salome)

SET(KERNEL_salomescript_DATA bin/salome)

SET(KERNEL_salomescript_PYTHON bin/salome)

SET(KERNEL_appliskel_SCRIPTS bin/salome/appliskel)

SET(KERNEL_appliskel_PYTHON bin/salome/appliskel)

SET(KERNEL_pythondir lib/python${PYTHON_VERSION}/site-packages)

SET(KERNEL_salomepythondir ${KERNEL_pythondir}/salome)

SET(KERNEL_sharedpkgpython_PYTHON ${KERNEL_salomepythondir}/shared_modules)

SET(KERNEL_salomeadmcmake salome_adm/cmake_files)

IF(CORBA_GEN)
  ADD_SUBDIRECTORY(idl)
ENDIF(CORBA_GEN)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(resources)

IF(NOT WITHONLYLAUNCHER)
  ADD_SUBDIRECTORY(bin)
  ADD_SUBDIRECTORY(salome_adm)
  ADD_SUBDIRECTORY(doc)
ENDIF(NOT WITHONLYLAUNCHER)

SET(input ${CMAKE_CURRENT_SOURCE_DIR}/KERNEL_version.h.in)
SET(output ${CMAKE_CURRENT_BINARY_DIR}/KERNEL_version.h)
MESSAGE(STATUS "Creation of ${output}")
CONFIGURE_FILE(${input} ${output} @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/KERNEL_version.h DESTINATION ${KERNEL_salomeinclude_HEADERS})
