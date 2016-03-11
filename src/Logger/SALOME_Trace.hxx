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

//  SALOME Logger : CORBA server managing trace output
//  File   : SALOME_Trace.cxx
//  Author : Vasily Rusyaev
//  Module : SALOME
//
#if !defined(AFX_LOGGER_H__96F2A3AB_F6F8_11D6_BFA6_000476A0958C__INCLUDED_)
#define AFX_LOGGER_H__96F2A3AB_F6F8_11D6_BFA6_000476A0958C__INCLUDED_

#include <sstream>
#include <SALOMEconfig.h>
#include "Logger.hh"

//these declarations for files don't using OCC includes (for example HDF)
# ifdef WIN32

#  ifndef Standard_EXPORT
#   define Standard_EXPORT __declspec( dllexport )
// For global variables :
#   define Standard_EXPORTEXTERN __declspec( dllexport ) extern
#   define Standard_EXPORTEXTERNC extern "C" __declspec( dllexport )
#  endif  /* Standard_EXPORT */

#  ifndef Standard_IMPORT
#   define Standard_IMPORT __declspec( dllimport ) extern
#   define Standard_IMPORTC extern "C" __declspec( dllimport )
#  endif  /* Standard_IMPORT */

# else  /* WIN32 */

#  ifndef Standard_EXPORT
#   define Standard_EXPORT
// For global variables :
#   define Standard_EXPORTEXTERN extern
#   define Standard_EXPORTEXTERNC extern "C"
#  endif  /* Standard_EXPORT */

#  ifndef Standard_IMPORT
#   define Standard_IMPORT extern
#   define Standard_IMPORTC extern "C"
#  endif  /* Standard_IMPORT */

# endif  /* WIN32 */

class SALOME_Trace : public std::ostringstream  
{
public:
        virtual ~SALOME_Trace();
        static Standard_EXPORT SALOME_Trace& Instance();
        // initializes Logger (if USE_LOGGER variable is set) and returns true, in case success
        int Initialize(CORBA::ORB_ptr theOrb);
        Standard_EXPORT void putMessage(std::ostream& msg);
protected:
        //disable creation of instances. It's necessary to use static SALOME_Logger& Instance()
        SALOME_Trace();
        SALOME_Logger::Logger_var m_pInterfaceLogger;// object reference on Logger server.
        int isInitialized;
};

#define GLogger SALOME_Trace::Instance()

#endif // !defined(AFX_LOGGER_H__96F2A3AB_F6F8_11D6_BFA6_000476A0958C__INCLUDED_)
