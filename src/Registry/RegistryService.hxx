//=============================================================================
// File      : RegistryService.hxx
// Created   : Mon Nov  5 17:26:23 CET 2001
// Author    : Pascale NOYRET - Antoine YESSAYAN, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

# ifndef __RegistryService_h__
# define __RegistryService_h__

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Registry)

# include <map>


class RegistryService : public POA_Registry::Components  //, public PortableServer::RefCountServantBase
{

public :
	enum Status { UNDEF=-1 , TERMINATED=0 , RUNNING=1 } ;

	class client_infos
	{
	protected :
		const char* const	_ior;
		const char* const	_name;
		const long		_pid ;
		const char* const	_machine;
		const char* const	_adip;
		const long		_uid;
		const char* const	_pwname;
		const long		_ts_start ;
		const long		_difftime;
		const char* const	_cdir ;
		      long 		_ts_hello ;
		      long 		_ts_end ;
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
	virtual CORBA::ULong add (const Registry::Infos & infos);
	virtual CORBA::ULong size ( void );
	virtual void remove( const CORBA::ULong id );
	virtual void hello( const CORBA::ULong id );
	virtual void end(void);

	virtual Registry::AllInfos *getall(void);
	virtual Registry::AllInfos *history(void);

	void SessionName( const char *sessionName ) ;


protected :

	const char		*_SessionName ;
	int			 _Compteur ;
	map<int,client_infos *>	 _reg ;
	map<int,client_infos *>	 _fin ;

	Registry::AllInfos* makeseq(map<int,client_infos *> &mymap );

} ;

# endif
