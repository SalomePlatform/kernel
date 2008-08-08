//  SALOME Utils : general SALOME's definitions and tools
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : OpUtil.hxx
//  Module : SALOME

#ifndef _OPUTIL_HXX
#define _OPUTIL_HXX

#include <SALOME_Utils.hxx>

#include <string>

UTILS_EXPORT std::string GetHostname();
UTILS_EXPORT const char *duplicate(const char * const);

class UTILS_EXPORT OpUtil_Dir
{
public:
  // Returns the unique temporary directory, that is defined in tmp_path_env if this variable is set
  // otherwise return /tmp/something/ for Unix or c:\something\ for WNT
  static std::string GetTmpDirByEnv( const std::string& tmp_path_env );

  // Returns the unique temporary directory, that is defined in tmp_path if this variable is set
  // otherwise return /tmp/something/ for Unix or c:\something\ for WNT
  static std::string GetTmpDirByPath( const std::string& tmp_path );
  
  // Returns the unique temporary directory in 
  // /tmp/something/ for Unix or c:\something\ for WNT
  static std::string GetTmpDir();

  // Returns True(False) if the path (not)exists
  static bool IsExists( const std::string& path );

};

#endif
