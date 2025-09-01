# Copyright (C) 2012-2025  CEA, EDF, OPEN CASCADE
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


# Define the destination directory
set(INSTALL_APPLI_DIR ${SALOME_INSTALL_SCRIPT_DATA}/appli)

ADD_SUBDIRECTORY(salome_tester)

# This variable will go into environment script
set(APPLI ${INSTALL_APPLI_DIR}/salome)
# get_filename_component(PYTHON_SITE_PACKAGES_PATH_ABS "${CMAKE_INSTALL_PREFIX}/${_pydir}" ABSOLUTE)
get_filename_component(SALOME_INSTALL_PYTHON_ABS "${CMAKE_INSTALL_PREFIX}/${SALOME_KERNEL_INSTALL_PYTHON}" ABSOLUTE)
get_filename_component(SALOME_INSTALL_BINS_ABS "${CMAKE_INSTALL_PREFIX}/${SALOME_INSTALL_BINS}" ABSOLUTE)
get_filename_component(APPLI_ABS "${CMAKE_INSTALL_PREFIX}/${APPLI}" ABSOLUTE)

# Install the salome file with execution permissions
SALOME_INSTALL_SCRIPTS(${CMAKE_CURRENT_SOURCE_DIR}/salome ${INSTALL_APPLI_DIR})
SALOME_INSTALL_SCRIPTS(${CMAKE_CURRENT_SOURCE_DIR}/.bashrc ${INSTALL_APPLI_DIR})

# Create the extra.env.d directory in the build directory
set(EXTRA_ENV_DIR ${CMAKE_CURRENT_BINARY_DIR}/extra.env.d)
file(MAKE_DIRECTORY ${EXTRA_ENV_DIR})

# Get a list of all .py.in files in the current directory
file(GLOB TEMPLATE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.py.in")

# Generate and install each template file
foreach(TEMPLATE_FILE ${TEMPLATE_FILES})
    # Get the file name without the extension
    get_filename_component(FILE_NAME ${TEMPLATE_FILE} NAME_WE)

    # Generate and install the script with default permissions
    SALOME_CONFIGURE_FILE(${TEMPLATE_FILE} ${FILE_NAME}.py INSTALL ${INSTALL_APPLI_DIR}/extra.env.d)
endforeach()
