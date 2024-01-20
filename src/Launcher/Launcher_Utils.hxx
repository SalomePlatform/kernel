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

// Author: André RIBES - EDF R&D
//
#ifndef __LAUNCHER_UTILS_HXX__
#define __LAUNCHER_UTILS_HXX__

#include <iostream>

#ifdef WIN32
# if defined LAUNCHER_EXPORTS || defined Launcher_EXPORTS
#  define LAUNCHER_EXPORT __declspec(dllexport)
# else
#  define LAUNCHER_EXPORT __declspec(dllimport)
# endif

#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif

#else
# define LAUNCHER_EXPORT
#endif

#if defined(_DEBUG_) || defined(_DEBUG)
//#define LAUNCHER_DEGBUG
#endif

// MESSAGES
#define LAUNCHER_MESS_INIT(deb) std::cerr << deb
#define LAUNCHER_MESS_BEGIN(deb) LAUNCHER_MESS_INIT(deb)<<__FILE__ <<" ["<<__LINE__<<"] : "
#define LAUNCHER_MESS_END std::endl;
#define LAUNCHER_INFOS(msg) {LAUNCHER_MESS_BEGIN("- Trace ") << msg << LAUNCHER_MESS_END}

#if defined(LAUNCHER_DEGBUG)
#define LAUNCHER_MESSAGE(msg) {LAUNCHER_MESS_BEGIN("- Trace ") << msg << LAUNCHER_MESS_END}
#else /* ifdef LAUNCHER_DEGBUG*/
#define LAUNCHER_MESSAGE(msg) {}
#endif /* ifdef LAUNCHER_DEGBUG*/

class LAUNCHER_EXPORT LauncherException
{
public:
  const std::string msg;

  LauncherException(const std::string m) : msg(m) {}
};


#endif
