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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : Utils_ORB_INIT.hxx
//  Author : Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$

# if ! defined( __ORB_INIT_HXX__ )
# define __ORB_INIT_HXX__

# include "CORBA.h" 

#include "Utils_CommException.hxx"


/*!
 * Ce composant prend en charge la connexion et la deconnexion a l'orb
 * Il est souhaitable de l'utiliser dans un SINGLETON.
 */

class ORB_INIT
{

private :
	CORBA::ORB_var _orb ;

public :
	ORB_INIT( void );
	virtual ~ORB_INIT();
	CORBA::ORB_var & operator() ( int argc , char **argv ) throw( CommException ) ;

	inline CORBA::ORB_var &orb( void );
} ;

inline CORBA::ORB_var &ORB_INIT::orb( void )
{
	return _orb ;
}

# endif
