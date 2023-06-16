// Copyright (C) 2007-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  SALOME Registry : Registry server implementation
//  File   : RegistryConnexion.cxx
//  Author : Pascale NOYRET - Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$
//
# include "RegistryConnexion.hxx"
# include "SALOME_NamingService.hxx"
# include "Utils_Identity.hxx"
# include "Utils_CommException.hxx"
# include "OpUtil.hxx"
# include "utilities.h"

# include <cstdio>

Registry::Components_var Connexion(SALOME_NamingService_Abstract *naming)
{
        Registry::Components_var varComponents = 0 ;
        const char *registryName = "Registry" ;

        try
        {
                // Recuperation de la reference de l'objet
                CORBA::Object_var object = naming->Resolve( registryName ) ;
                if(CORBA::is_nil(object)) throw CommException( "unable to find the RegistryService" ) ;

                // Specialisation de l'objet generique
                varComponents = Registry::Components::_narrow( object ) ;
                ASSERT(! CORBA::is_nil(varComponents)) ;
        }
        catch( ... )
        {
                throw CommException ( "NamingService Connexion Error" ) ;
        }
        return varComponents ;
}


RegistryConnexion::RegistryConnexion(const char *ior , const char *ptrSessionName, const char *componentName, SALOME_NamingService_Abstract *ns ): \
        _Ior(duplicate(ior)), _VarComponents( Connexion(ns) ), _SessionName(ptrSessionName),_Name(""), _Id(0)
{
        this->add( componentName ) ;
}


RegistryConnexion::RegistryConnexion( void ): _Ior(0), _VarComponents( 0 ), _Name(""), _Id(0)
{
        INTERRUPTION(EXIT_FAILURE) ;
}


RegistryConnexion::~RegistryConnexion()
{
        BEGIN_OF("RegistryConnexion::~RegistryConnexion()" ) ;
        if( _Id != 0 )
        {
                ASSERT(_SessionName!="" ) ;
                ASSERT(_Name!="" ) ;
                _VarComponents->remove( _Id ) ;
        }
        _Id   = 0 ;
        delete [] (char*) _Ior;
        _Ior = 0;
        _SessionName = "";
        _Name = "" ;
        END_OF("RegistryConnexion::~RegistryConnexion()" ) ;
}


void RegistryConnexion::add( const char *aName )
{
        ASSERT(_SessionName!="" ) ;
        ASSERT(_Name=="" ) ;
        ASSERT(_Id==0 ) ;
        ASSERT(aName) ;
        SCRUTE(aName) ;
        ASSERT(strlen( aName )>0) ;

        const Identity lesInfos( aName ) ;
        Registry::Infos infos ;
                infos.name        = CORBA::string_dup( lesInfos.name() ) ;
                infos.pid         = lesInfos.pid() ;
                infos.machine     = CORBA::string_dup( lesInfos.host_char() ) ;
                infos.adip        = CORBA::string_dup( lesInfos.adip() ) ;
                infos.uid         = (CORBA::Long)lesInfos.uid() ; //!< TODO: pointer truncation from const PSID to long
                infos.pwname      = CORBA::string_dup( lesInfos.pwname() ) ;
                infos.tc_start    = (CORBA::Long)lesInfos.start() ; //!< TODO: conversation from const time_t to CORBA::Long
                infos.tc_hello    = 0 ;
                infos.tc_end      = 0 ;
                infos.difftime    = 0 ;
                infos.cdir        = CORBA::string_dup( lesInfos.rep() ) ;
                infos.status      = -1 ;
                infos.ior         = CORBA::string_dup(_Ior);

        ASSERT(!CORBA::is_nil(this->_VarComponents)) ;

        CORBA::ULong id = _VarComponents->add( infos ) ;

        SCRUTE(aName) ; SCRUTE(id) ;
        ASSERT(id) ;

        _Id   = id ;
        _Name = aName ;

        return ;
}


void RegistryConnexion::remove( void )
{
        ASSERT(_Id>0) ;
        ASSERT(!CORBA::is_nil(this->_VarComponents)) ;
        _VarComponents->remove( _Id ) ;

        _Name = "" ;
        _Id   = 0 ;

        return ;
}
