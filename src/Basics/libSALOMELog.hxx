// Copyright (C) 2007-2024  CEA, EDF, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  File   : libSALOMELog.hxx
//  Author : Konstantin Leontev (OpenCascade)
//  Module : KERNEL
//  $Header$
//
#pragma once

#include "SALOME_Basics.hxx"

#include <string>

namespace SALOME
{
  enum class VerbosityLevelType;

  bool BASICS_EXPORT VerbosityActivated();
  void BASICS_EXPORT SetVerbosityActivated(bool);

  void BASICS_EXPORT SetVerbosityLevel(VerbosityLevelType level);
  void BASICS_EXPORT SetVerbosityLevelStr(const std::string& level);
  bool BASICS_EXPORT IsDebugLevel();
  bool BASICS_EXPORT IsInfoLevel();
  bool BASICS_EXPORT IsWarningLevel();
  bool BASICS_EXPORT IsErrorLevel();
  VerbosityLevelType BASICS_EXPORT VerbosityLevel();
  std::string BASICS_EXPORT VerbosityLevelStr();
}
