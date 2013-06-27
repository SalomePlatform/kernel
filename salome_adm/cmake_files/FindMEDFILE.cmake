# Copyright (C) 2007-2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

# ------

MESSAGE(STATUS "Check for medfile ...")

# ------

SET(MED_ENABLE_MED3 ON)

# ------

SET(MED3HOME $ENV{MED3HOME})
IF(NOT MED3HOME)
  FIND_PROGRAM(MDUMP mdump)
  IF(MDUMP)
    MESSAGE(STATUS "mdump found: ${MDUMP}")
    SET(MED3HOME ${MDUMP})
    GET_FILENAME_COMPONENT(MED3HOME ${MED3HOME} PATH)
    GET_FILENAME_COMPONENT(MED3HOME ${MED3HOME} PATH)
  ELSE(MDUMP)
    MESSAGE(STATUS "mdump not found, try to use MED3HOME environment variable or check your PATH")
    SET(MED_ENABLE_MED3 OFF)
  ENDIF(MDUMP)
ENDIF(NOT MED3HOME)

IF(MED_ENABLE_MED3)
  MESSAGE(STATUS "Searching medfile in ${MED3HOME}")
ENDIF(MED_ENABLE_MED3)

IF(MED_ENABLE_MED3)
  FIND_PATH(MED3_INCLUDE_DIR med.h ${MED3HOME}/include)
  IF(MED3_INCLUDE_DIR)
    SET(MED3_INCLUDES ${HDF5_INCLUDES} -I${MED3_INCLUDE_DIR} -D${MACHINE})#to be removed
    SET(MED3_DEFINITIONS "-D${MACHINE}")
    IF(${MED3HOME} STREQUAL /usr)
      SET(MED3_INCLUDE_DIRS)
    ELSE(${MED3HOME} STREQUAL /usr)
      SET(MED3_INCLUDE_DIRS ${MED3_INCLUDE_DIR})
    ENDIF(${MED3HOME} STREQUAL /usr)
    MESSAGE(STATUS "med.h found in ${MED3_INCLUDE_DIR}")
  ELSE(MED3_INCLUDE_DIR)
    MESSAGE(STATUS "med.h not found in ${MED3HOME}/include, check your installation.")
    SET(MED_ENABLE_MED3 OFF)
  ENDIF(MED3_INCLUDE_DIR)
ENDIF(MED_ENABLE_MED3)

SET(MED3_LIBS_C_ONLY)
FOREACH(name medC)
  IF(MED_ENABLE_MED3)
    FIND_LIBRARY(${name}_LIB ${name} ${MED3HOME}/lib)
    IF(${name}_LIB)
      MESSAGE(STATUS "${name} lib found: ${${name}_LIB}")
      SET(MED3_LIBS_C_ONLY ${MED3_LIBS} ${${name}_LIB})
    ELSE(${name}_LIB)
      MESSAGE(STATUS "${name} lib not found in ${MED3HOME}/lib, check your installation.")
      SET(MED_ENABLE_MED3 OFF)
    ENDIF(${name}_LIB)
  ENDIF(MED_ENABLE_MED3)
ENDFOREACH(name medC)

SET(MED3_HAS_FORTRAN_BINDINGS 0)
IF(MED_ENABLE_MED3)
  FIND_LIBRARY(med_LIB med ${MED3HOME}/lib)
  IF(med_LIB)
    MESSAGE(STATUS "med lib found: ${med_LIB}")
    SET(MED3_LIBS ${MED3_LIBS_C_ONLY} ${med_LIB})
    SET(MED3_HAS_FORTRAN_BINDINGS 1)
  ELSE(med_LIB)
    MESSAGE(STATUS "med lib not found in ${MED3HOME}/lib")
  ENDIF(med_LIB)

  FIND_LIBRARY(medimport_LIB medimport ${MED3HOME}/lib)
  IF(medimport_LIB)
    MESSAGE(STATUS "medimport lib found: ${medimport_LIB}")
    SET(MED3_LIBS ${MED3_LIBS} ${medimport_LIB})
  ELSE(medimport_LIB)
    MESSAGE(STATUS "medimport lib not found in ${MED3HOME}/lib")
    SET(MED3_HAS_FORTRAN_BINDINGS 0)
  ENDIF(medimport_LIB)
ENDIF(MED_ENABLE_MED3)

IF(MED_ENABLE_MED3)
ELSE(MED_ENABLE_MED3)
  MESSAGE(STATUS "medfile not found ... optional ... disabled")
ENDIF(MED_ENABLE_MED3)
