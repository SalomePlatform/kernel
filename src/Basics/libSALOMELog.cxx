// Copyright (C) 2007-2023  CEA, EDF, OPEN CASCADE
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

//  Author : Konstantin Leontev (OpenCascade)
//  Module : KERNEL
//  $Header$
//

#include "libSALOMELog.hxx"

#include <string>
#include <iostream>
#include <stdexcept>

enum class VerbosityMode { undefined, nolog, withlog };

static VerbosityMode isActivated = VerbosityMode::undefined;

namespace SALOME
{
  static constexpr char ERROR_LEVEL_VALUE = 0;
  static constexpr char ERROR_LEVEL_VALUE_STR[] = "ERROR";
  static constexpr char WARNING_LEVEL_VALUE = 1;
  static constexpr char WARNING_LEVEL_VALUE_STR[] = "WARNING";
  static constexpr char INFO_LEVEL_VALUE = 2;
  static constexpr char INFO_LEVEL_VALUE_STR[] = "INFO";
  static constexpr char DEBUG_LEVEL_VALUE = 7;
  static constexpr char DEBUG_LEVEL_VALUE_STR[] = "DEBUG";
  static constexpr char UNDEFINED_LEVEL_VALUE=99;

  enum class VerbosityLevelType { error_level=ERROR_LEVEL_VALUE, warning_level=WARNING_LEVEL_VALUE, info_level=INFO_LEVEL_VALUE, debug_level=DEBUG_LEVEL_VALUE, undefined_level=UNDEFINED_LEVEL_VALUE };
  static VerbosityLevelType verbosityLevel = VerbosityLevelType::undefined_level;

  static VerbosityLevelType FromIntToVerbosityLevel(char value)
  {
    switch(value)
    {
      case ERROR_LEVEL_VALUE:
        return VerbosityLevelType::error_level;
      case WARNING_LEVEL_VALUE:
        return VerbosityLevelType::warning_level;
      case INFO_LEVEL_VALUE:
        return VerbosityLevelType::info_level;
      case DEBUG_LEVEL_VALUE:
        return VerbosityLevelType::debug_level;
    }
    throw std::range_error("FromIntToVerbosityLevel : Invalid value for verbosity level !");
  }

  static VerbosityLevelType FromStrToVerbosityLevel(const std::string& val)
  {
    if(val == ERROR_LEVEL_VALUE_STR)
      return VerbosityLevelType::error_level;
    if(val == WARNING_LEVEL_VALUE_STR)
      return VerbosityLevelType::warning_level;
    if(val == INFO_LEVEL_VALUE_STR)
      return VerbosityLevelType::info_level;
    if(val == DEBUG_LEVEL_VALUE_STR)
      return VerbosityLevelType::debug_level;
    throw std::range_error("FromStrToVerbosityLevel : Invalid str value for verbosity level !");
  }

  static std::string FromVerbosityLevelToStr(VerbosityLevelType level)
  {
    switch(level)
    {
      case VerbosityLevelType::error_level:
        return std::string(ERROR_LEVEL_VALUE_STR);
      case VerbosityLevelType::warning_level:
        return std::string(WARNING_LEVEL_VALUE_STR);
      case VerbosityLevelType::info_level:
        return std::string(INFO_LEVEL_VALUE_STR);
      case VerbosityLevelType::debug_level:
        return std::string(DEBUG_LEVEL_VALUE_STR);
      default:
        throw std::range_error("FromVerbosityLevelToStr : not managed verbosity level !");
    }
  }

// ============================================================================
/*!
 *  Called by any log message macros to decide about log output in Release and
 *  Debug mode dynamically rely on SALOME_VERBOSE environment variable.
 *  Checks SALOME_VERBOSE only on the very first call and returns cached result
 *  for all followed calls.
 *  Returns true if SALOME_VERBOSE is positioned and not empty and if its
 *  numeric value greater than 0.
 */
// ============================================================================

  bool VerbosityActivated()
  {
    auto isEnvVarSet = []() -> VerbosityMode
    {
      const char *envVar = std::getenv("SALOME_VERBOSE");

      if (envVar && (envVar[0] != '\0'))
      {
        const int numValue = std::stoi(envVar);
        return numValue > 0?VerbosityMode::withlog:VerbosityMode::nolog;
      }

      return VerbosityMode::nolog;
    };

    if(isActivated == VerbosityMode::undefined)
      isActivated = isEnvVarSet();
    return isActivated == VerbosityMode::withlog;
  }

  void SetVerbosityActivated(bool flag)
  {
    isActivated = flag ? VerbosityMode::withlog:VerbosityMode::nolog;
  }

  VerbosityLevelType VerbosityLevel()
  {
    auto isEnvVarSet = []() -> VerbosityLevelType
    {
      const char *envVar = std::getenv("SALOME_VERBOSE_LEVEL");
      if (envVar && (envVar[0] != '\0'))
      {
        const int numValue = std::stoi(envVar);
        return FromIntToVerbosityLevel( static_cast<char>(numValue) );
      }
      return VerbosityLevelType::info_level;
    };
    if(verbosityLevel == VerbosityLevelType::undefined_level)
      verbosityLevel = isEnvVarSet();
    return verbosityLevel;
  }

  void BASICS_EXPORT SetVerbosityLevel(VerbosityLevelType level)
  {
    verbosityLevel = level;
  }

  void SetVerbosityLevelStr(const std::string& level)
  {
    verbosityLevel = FromStrToVerbosityLevel(level);
  }

  std::string VerbosityLevelStr()
  {
    return FromVerbosityLevelToStr( VerbosityLevel() );
  }

  bool IsDebugLevel()
  {
    return VerbosityLevel() >= VerbosityLevelType::debug_level;
  }

  bool IsInfoLevel()
  {
    return VerbosityLevel() >= VerbosityLevelType::info_level;
  }

  bool IsWarningLevel()
  {
    return VerbosityLevel() >= VerbosityLevelType::warning_level;
  }

  bool IsErrorLevel()
  {
    return VerbosityLevel() >= VerbosityLevelType::error_level;
  }
}
