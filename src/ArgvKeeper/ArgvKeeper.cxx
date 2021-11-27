// Copyright (C) 2007-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "ArgvKeeper.hxx"
#include <mutex>

namespace
{
  std::recursive_mutex mutex; //!< mutex to protect access to static data
  std::vector<std::string> argv_list; //!< cmd line arguments
  bool isInitialized = false; //!< \c true if cmd line arguments were initialized
}

//! \brief Store cmd line arguments
void SetArgcArgv(int argc, char* argv[])
{
  std::vector<std::string> tmp_list;
  for (int i=0; i < argc; ++i)
    tmp_list.push_back(std::string(argv[i]));
  SetArgcArgv(tmp_list);
}

//! \overload void SetArgcArgv(int argc, char* argv[])
void SetArgcArgv(const std::vector<std::string>& argv)
{
  std::lock_guard<std::recursive_mutex> g(mutex);
  if (!isInitialized && !argv.empty())
  {
    isInitialized = true;
    for (const std::string& value: argv) {
      argv_list.push_back(value);
    }
  }
}

//! \brief Get cmd line arguments
std::vector<std::string> GetArgcArgv()
{
  std::lock_guard<std::recursive_mutex> g(mutex);
  return argv_list;
}

//! \brief Check if cmd line arguments were initialized
//!
//! In debug mode returns \c true if SetArgcArgv() was called or \c false otherwise.
//! In release mode always returns \c true.
bool ArgcArgvInitialized()
{
#ifdef DEBUG
  return isInitialized;
#else
  return true;
#endif
}
