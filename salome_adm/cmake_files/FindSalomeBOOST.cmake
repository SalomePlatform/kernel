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

# Boost detection dor Salome

set(Boost_ROOT_DIR $ENV{Boost_ROOT_DIR} CACHE PATH "Path to Boost directory")
if(EXISTS ${Boost_ROOT_DIR})
  set(CMAKE_INCLUDE_PATH ${Boost_ROOT_DIR}/include)
  set(CMAKE_LIBRARY_PATH ${Boost_ROOT_DIR}/lib)
endif(EXISTS ${Boost_ROOT_DIR})
set(Boost_USE_STATIC_LIBS       OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
#find_package( Boost 1.49 REQUIRED  )#COMPONENTS thread date_time signals filesystem system)
find_package( Boost REQUIRED  )#COMPONENTS thread date_time signals filesystem system)
if(WINDOWS)
  set(BOOST_DEFINITIONS -DBOOST_DISABLE_ASSERTS)
endif(WINDOWS)
