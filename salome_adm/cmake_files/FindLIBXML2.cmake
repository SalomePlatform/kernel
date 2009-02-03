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
SET(LIBXML2_ROOT $ENV{LIBXML2_ROOT})
IF(LIBXML2_ROOT)
  FIND_PATH(LIBXML2_INCLUDE_DIR libxml/parser.h ${LIBXML2_ROOT}/include)
  FIND_LIBRARY(LIBXML2_LIBS libxml2 ${LIBXML2_ROOT}/lib)
ELSE(LIBXML2_ROOT)
  FIND_PATH(LIBXML2_INCLUDE_DIR libxml/parser.h /usr/include/libxml2)
  FIND_LIBRARY(LIBXML2_LIBS xml2 /usr/lib)
ENDIF(LIBXML2_ROOT)

SET(LIBXML2_INCLUDES -I${LIBXML2_INCLUDE_DIR})

SET(ICONV_ROOT $ENV{ICONV_ROOT})
IF(ICONV_ROOT)
  FIND_PATH(ICONV_INCLUDE_DIR iconv.h ${ICONV_ROOT}/include)
  FIND_LIBRARY(ICONV_LIBS iconv ${ICONV_ROOT}/lib)
  SET(LIBXML2_INCLUDES ${LIBXML2_INCLUDES} -I${ICONV_INCLUDE_DIR})
  SET(LIBXML2_LIBS ${LIBXML2_LIBS} ${ICONV_LIBS})
ENDIF(ICONV_ROOT)

SET(LIBXML_INCLUDES ${LIBXML2_INCLUDES})
SET(LIBXML_CPPFLAGS ${LIBXML2_INCLUDES})
SET(LIBXML_LIBS ${LIBXML2_LIBS})
