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

//  SALOME Registry : Registry server implementation
//  File   : RegistryService.hxx
//  Author : Pascale NOYRET - Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$
//
# ifndef __RegistryService_h__
# define __RegistryService_h__

#include "SALOME_Registry.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Registry)

# include <map>

#ifdef WIN32
#pragma warning(disable:4275) // Disable warning interface non dll
#pragma warning(disable:4251) // Warning DLL Interface ...
#endif

class REGISTRY_EXPORT RegistryService : public POA_Registry::Components  //, public PortableServer::RefCountServantBase
{

public :
        enum Status { UNDEF=-1 , TERMINATED=0 , RUNNING=1 } ;

        class client_infos
        {
        protected :
                const char* const       _ior;
                const char* const       _name;
                const long              _pid ;
                const char* const       _machine;
                const char* const       _adip;
                const long              _uid;
                const char* const       _pwname;
                const long              _ts_start ;
                const long              _difftime;
                const char* const       _cdir ;
                      long              _ts_hello ;
                      long              _ts_end ;
                      Status            _status;
        public :
                friend class RegistryService ;

                client_infos( const Registry::Infos &infos );
                ~client_infos();
        }; 



public :
        RegistryService(void);
        virtual ~RegistryService(void);

        void ping();
        CORBA::Long getPID();
        virtual CORBA::ULong add (const Registry::Infos & infos);
        virtual CORBA::ULong size ( void );
#ifndef WIN32
        virtual void remove( const CORBA::ULong id );
        virtual void hello( const CORBA::ULong id );
#else
        virtual void remove( CORBA::ULong id );
        virtual void hello( CORBA::ULong id );
#endif
        virtual void end(void);

        virtual Registry::AllInfos *getall(void);
        virtual Registry::AllInfos *history(void);

        void SessionName( const char *sessionName ) ;

        void SetOrb( CORBA::ORB_ptr orb ) { _orb = CORBA::ORB::_duplicate(orb); return; }

        void Shutdown() { if(!CORBA::is_nil(_orb)) _orb->shutdown(0); }

protected :

        CORBA::ORB_var _orb;
        const char              *_SessionName ;
        int                      _Compteur ;
        std::map<int,client_infos *>     _reg ;
        std::map<int,client_infos *>     _fin ;

        Registry::AllInfos* makeseq(std::map<int,client_infos *> &mymap );

} ;

# endif
