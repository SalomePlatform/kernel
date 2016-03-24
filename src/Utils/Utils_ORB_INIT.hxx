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

//  SALOME Utils : general SALOME's definitions and tools
//  File   : Utils_ORB_INIT.hxx
//  Author : Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$
//
# if ! defined( __ORB_INIT_HXX__ )
# define __ORB_INIT_HXX__

#include <SALOMEconfig.h>

#include "SALOME_Utils.hxx"

#include "omniORB4/CORBA.h" 

#include "Utils_CommException.hxx"

#ifdef WIN32
#pragma warning(disable:4251) // Warning DLL Interface ...
#pragma warning(disable:4290) // Warning Exception ...
#endif

/*!
 * Ce composant prend en charge la connexion et la deconnexion a l'orb
 * Il est souhaitable de l'utiliser dans un SINGLETON.
 */

class UTILS_EXPORT ORB_INIT
{

private :
        CORBA::ORB_var _orb ;

public :
        ORB_INIT( void );
        virtual ~ORB_INIT();
        void explicit_destroy();
        CORBA::ORB_var & operator() ( int argc , char **argv ) throw( CommException ) ;

        inline CORBA::ORB_var &orb( void );
} ;

inline CORBA::ORB_var &ORB_INIT::orb( void )
{
        return _orb ;
}

# endif
