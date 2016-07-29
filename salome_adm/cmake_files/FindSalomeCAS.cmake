# Copyright (C) 2013-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
# Author: Adrien Bruneton
#

# OpenCascade detection for Salome
#
#  !! Please read the generic detection procedure in SalomeMacros.cmake !!
#

SALOME_FIND_PACKAGE_AND_DETECT_CONFLICTS(CAS CAS_INCLUDE_DIRS 1)
MARK_AS_ADVANCED(CAS_INCLUDE_DIRS
  CAS_BinLPlugin CAS_BinTObjPlugin   CAS_BinXCAFPlugin CAS_PTKernel
  CAS_StdLPlugin CAS_StdPlugin CAS_TKAdvTools  CAS_TKBin   CAS_TKBinL    CAS_TKBinTObj   
  CAS_TKBinXCAF   CAS_TKBO    CAS_TKBool    CAS_TKBRep    CAS_TKCAF   CAS_TKCDF   CAS_TKernel   
  CAS_TKFeat    CAS_TKFillet    CAS_TKG2d   CAS_TKG3d   CAS_TKGeomAlgo    CAS_TKGeomBase    
  CAS_TKHLR   CAS_TKIGES    CAS_TKLCAF    CAS_TKMath    CAS_TKMesh    CAS_TKMeshVS    CAS_TKNIS   
  CAS_TKOffset    CAS_TKOpenGl    CAS_TKPCAF    CAS_TKPLCAF   CAS_TKPrim    CAS_TKPShape    CAS_TKService   
  CAS_TKShapeSchema   CAS_TKShHealing   CAS_TKStdLSchema    CAS_TKStdSchema   CAS_TKSTEP    CAS_TKSTEP209   
  CAS_TKSTEPAttr    CAS_TKSTEPBase    CAS_TKSTL   CAS_TKTObj    CAS_TKTopAlgo   CAS_TKV2d   CAS_TKV3d   CAS_TKVRML    
  CAS_TKXCAF    CAS_TKXCAFSchema    CAS_TKXDEIGES   CAS_TKXDESTEP   CAS_TKXMesh   CAS_TKXml   CAS_TKXmlL    CAS_TKXmlTObj   
  CAS_TKXmlXCAF   CAS_TKXSBase    CAS_XCAFPlugin    CAS_XmlLPlugin    CAS_XmlPlugin   CAS_XmlTObjPlugin   
  CAS_XmlXCAFPlugin CAS_Xmu)

IF(CAS_VERSION_STR VERSION_LESS "7.0.1")
  MARK_AS_ADVANCED(CAS_FWOSPlugin)
ELSE()
  MARK_AS_ADVANCED(CAS_TKStd)
ENDIF()

IF(CAS_VERSION_STR VERSION_LESS "7.0")
  SET(CAS_STDPLUGIN TKStdSchema)
ELSE()
  SET(CAS_STDPLUGIN TKStd)
  SET(CAS_BINPLUGIN TKBin)
ENDIF()

IF(CAS_FOUND)
  SALOME_ACCUMULATE_HEADERS(CAS_INCLUDE_DIRS)
  SALOME_ACCUMULATE_ENVIRONMENT(LD_LIBRARY_PATH ${CAS_TKernel})
ENDIF()
