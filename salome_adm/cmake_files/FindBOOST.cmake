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
SET(BOOST_ROOT $ENV{BOOST_ROOT})
IF(BOOST_ROOT)
  FIND_PATH(BOOST_INCLUDES boost/shared_ptr.hpp ${BOOST_ROOT})
  FIND_LIBRARY(BOOST_LIB_THREAD libboost_thread-vc90-mt-gd-1_35 ${BOOST_ROOT}/lib)
  FIND_LIBRARY(BOOST_LIB_DATE_TIME libboost_date_time-vc90-mt-gd-1_35 ${BOOST_ROOT}/lib)
  SET(BOOST_LIBS ${BOOST_LIB_THREAD} ${BOOST_LIB_DATE_TIME})
  SET(BOOST_CPPFLAGS -I${BOOST_INCLUDES} -DBOOST_DISABLE_ASSERTS)
ELSE(BOOST_ROOT)
  SET(boost_thread -lboost_thread)
ENDIF(BOOST_ROOT)
