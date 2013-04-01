# Copyright (C) 2013  CEA/DEN, EDF R&D, OPEN CASCADE
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

# LIBXML2 detection for Salome

set(LIBXML2_ROOT_DIR $ENV{LIBXML2_ROOT_DIR} CACHE PATH "Path to LibXml2 directory")
if(EXISTS ${LIBXML2_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${LIBXML2_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${LIBXML2_ROOT_DIR}/lib)
  set(CMAKE_PROGRAM_PATH ${LIBXML2_ROOT_DIR}/bin)
endif(EXISTS ${LIBXML2_ROOT_DIR})
find_package(LibXml2 REQUIRED)
#if(${LIBXML2_VERSION_STRING} VERSION_LESS "2.7.6")
#  message(FATAL_ERROR "LIbXml2 required version is 2.7.6, version is ${LIBXML2_VERSION_STRING} , please defined LIBXML2_ROOT_DIR as environment or cmake variable")
#endif(${LIBXML2_VERSION_STRING} VERSION_LESS "2.7.6")
