//  SALOME Registry : Registry server implementation
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : RegistryService.cxx
//  Author : Pascale NOYRET - Antoine YESSAYAN, EDF
//  Module : SALOME
//  $Header$

# include "RegistryService.hxx"
# include "OpUtil.hxx"
# include "utilities.h"

extern "C"
{
# include <time.h>
}

#ifndef WNT
#include <unistd.h>
#else
#include <process.h>
#endif
using namespace std;

/* ------------------------------*/
/* Constructeurs et Destructeurs */
/* ------------------------------*/

RegistryService::RegistryService( void ) : _SessionName(0), _Compteur(0)
{
	MESSAGE("Passage dans RegistryService::RegistryService()") ;
	_orb = CORBA::ORB::_nil();
}


RegistryService::~RegistryService()
{
	BEGIN_OF("RegistryService::~RegistryService()") ;
	map<int,client_infos *>::iterator im;
	for (im=_reg.begin();im!=_reg.end(); im++)
        {
		const client_infos &lesInfos = *(*im).second ;
		MESSAGE("Deletion") ; SCRUTE( lesInfos._name ) ;
		_reg.erase ( im ) ;
	}
	ASSERT(_reg.size()==0) ;
	for (im=_fin.begin();im!=_fin.end(); im++)
        {
		const client_infos &lesInfos = *(*im).second ;
		MESSAGE("Deletion") ; SCRUTE( lesInfos._name ) ;
		_fin.erase ( im ) ;
	}
	ASSERT(_fin.size()==0) ;
	_Compteur = -1 ;
	if ( _SessionName )
	{
#ifndef WNT
		delete [] _SessionName ;
#else
		delete [] (char*)_SessionName ;
#endif
		_SessionName = 0 ;
	}
	END_OF("RegistryService::~RegistryService()") ;
}

/* ------------------------------*/
/* Contrats IDL                  */
/* ------------------------------*/

CORBA::ULong RegistryService::size ( void )
{
	ASSERT(_SessionName) ;
	ASSERT(strlen(_SessionName)>0) ;
	return _reg.size() ;
}


CORBA::ULong RegistryService::add( const Registry::Infos & infos )
{
	BEGIN_OF("RegistryService::add") ;
	ASSERT(_SessionName) ;
	ASSERT(strlen(_SessionName)>0) ;
	client_infos *ptr_Component = new client_infos( infos ) ;
	ASSERT(ptr_Component) ;
	SCRUTE(ptr_Component->_name) ;

	_Compteur++;
	_reg[_Compteur]=ptr_Component;

	END_OF("RegistryService::add") ;
	return (CORBA::ULong)_Compteur ;
}

#ifndef WNT
void RegistryService::remove( const CORBA::ULong id)
#else
void RegistryService::remove( CORBA::ULong id)
#endif
{
	BEGIN_OF("RegistryService::remove") ;
	SCRUTE(id) ;
	ASSERT(_SessionName) ;
	ASSERT(strlen(_SessionName)>0) ;
	
	ASSERT(_reg.find(id)!=_reg.end()) 
	_reg[id]->_status=TERMINATED;
	_reg[id]->_ts_end = time(NULL) ;

	_fin[id]=_reg[id];
	SCRUTE(_fin.size()) ;

	map<int,client_infos *>::iterator pos = _reg.find ( id ) ;
	 _reg.erase ( pos ) ;
	SCRUTE(_reg.size()) ;
	SCRUTE(_fin.size()) ;

	END_OF("RegistryService::remove") ;
	return ;
}


#ifndef WNT
void RegistryService::hello( const CORBA::ULong id )
#else
void RegistryService::hello( CORBA::ULong id )
#endif
{
	BEGIN_OF("RegistryService::hello") ;
	SCRUTE(id) ;
	ASSERT(_SessionName) ;
	ASSERT(strlen(_SessionName)>0) ;

	ASSERT(_reg.find(id)!=_reg.end()) 
	_reg[id]->_ts_hello = time(NULL) ;
		
	END_OF("RegistryService::hello") ;
	return ;
}


void RegistryService::end( void )
{
	ASSERT(_SessionName) ;
	ASSERT(strlen(_SessionName)>0) ;
	BEGIN_OF( "RegistryService::end( void )" ) ;
	exit( EXIT_SUCCESS ) ;
}


Registry::AllInfos* RegistryService::getall( void )
{
	ASSERT(_SessionName) ;
	ASSERT(strlen(_SessionName)>0) ;
	return RegistryService::makeseq(_reg) ;
}

Registry::AllInfos* RegistryService::history( void )
{
	ASSERT(_SessionName) ;
	ASSERT(strlen(_SessionName)>0) ;
	return RegistryService::makeseq(_fin) ;
}

Registry::AllInfos* RegistryService::makeseq(map<int,client_infos *> &mymap )
{
	int i=0 ;

	Registry::AllInfos *all = new Registry::AllInfos ;
	ASSERT(all) ;
	const int RegLength = mymap.size();
	all->length(RegLength);

	map<int,client_infos *>::iterator im;
	for (im=mymap.begin();im!=mymap.end(); im++)
        {

		Registry::Infos &infos = (*all)[i] ;
		const client_infos &lesInfos = *(*im).second ;
		infos.name	= CORBA::string_dup( lesInfos._name ) ;
		infos.pid	= lesInfos._pid ;
		infos.pwname	= lesInfos._pwname ;
		infos.machine	= CORBA::string_dup( lesInfos._machine ) ;
		infos.adip	= CORBA::string_dup( lesInfos._adip ) ;
		infos.uid	= lesInfos._uid ;
		infos.tc_start	= lesInfos._ts_start + lesInfos._difftime ;
		infos.tc_hello	= lesInfos._ts_hello + lesInfos._difftime ;
		infos.tc_end	= lesInfos._ts_end + lesInfos._difftime ;
		infos.difftime	= lesInfos._difftime ;
		infos.cdir	= CORBA::string_dup( lesInfos._cdir ) ;
		infos.status	= lesInfos._status ;

		i++;
	}

	return all ;
}


/* ------------------------------*/
/* Autres                        */
/* ------------------------------*/

RegistryService::client_infos::client_infos( const Registry::Infos &infos ):\
									_ior(duplicate(infos.ior)),\
									_name(duplicate(infos.name)),\
									_pid(infos.pid),\
									_machine(duplicate(infos.machine)),\
									_adip(duplicate(infos.adip)),\
        								_uid(infos.uid),\
									_pwname(duplicate(infos.pwname)),\
									_ts_start(time(NULL)),\
									_difftime(infos.tc_start - _ts_start),\
									_cdir(duplicate(infos.cdir)),\
									_ts_hello(_ts_start),\
									_ts_end(0),\
        								_status(RUNNING)
{
  //	SCRUTE(_ior) ;
	;
}

RegistryService::client_infos::~client_infos()
{
	delete [] (char*)_ior ; (char*&)_ior = NULL  ;
	delete [] (char*)_name ; (char*&)_name = NULL  ;
	delete [] (char*)_machine ; (char*&)_machine = NULL ;
	delete [] (char*)_pwname ; (char*&)_pwname = NULL ;
	delete [] (char*)_adip ; (char*&)_adip = NULL ;
	delete [] (char*)_cdir ; (char*&)_cdir = NULL ;
}

void RegistryService::SessionName( const char *sessionName )
{
	ASSERT(sessionName) ;
	ASSERT(strlen(sessionName)>0) ;
	_SessionName = duplicate(sessionName) ;
	return ;
}
void RegistryService::ping()
{
#ifndef WNT
  MESSAGE(" RegistryService::ping() pid "<< getpid());
#else
  MESSAGE(" RegistryService::ping() pid "<< _getpid());
#endif
}
