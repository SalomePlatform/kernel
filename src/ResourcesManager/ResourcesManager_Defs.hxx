// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

#ifndef __RESOURCESMANAGER_DEFS_HXX__
#define __RESOURCESMANAGER_DEFS_HXX__

#ifdef WIN32
# if defined RESOURCESMANAGER_EXPORTS || defined ResourcesManager_EXPORTS
#  define RESOURCESMANAGER_EXPORT __declspec( dllexport )
# else
#  define RESOURCESMANAGER_EXPORT __declspec( dllimport )
# endif
#else
# define RESOURCESMANAGER_EXPORT
#endif

// MESSAGES
#define RES_MESS_INIT(deb) std::cerr << deb
#define RES_MESS_BEGIN(deb) RES_MESS_INIT(deb)<<__FILE__ <<" ["<<__LINE__<<"] : "
#define RES_MESS_END std::endl;
#define RES_INFOS(msg) {RES_MESS_BEGIN("- Trace ") << msg << RES_MESS_END}

#if defined(_DEBUG_) || defined(_DEBUG)
#define RES_MESSAGE(msg) {RES_MESS_BEGIN("- Trace ") << msg << RES_MESS_END}
#else /* ifdef _DEBUG_*/
#define RES_MESSAGE(msg) {}
#endif /* ifdef _DEBUG_*/

#endif // __RESOURCESMANAGER_DEFS_HXX__
