//=============================================================================
// File      : Utils_Identity.hxx
// Created   : Mon Nov  5 17:02:37 CET 2001
// Author    : Pascale NOYRET, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

# if !defined(  __IDENTITE_H__ )
# define __IDENTITE_H__

extern "C"
{
# include <stdlib.h>
# include <unistd.h>
# include <time.h>
# include <sys/utsname.h>
}

class Identity
{

protected :
	const char* const	_name ;
	const struct utsname	_hostid;
	const char* const	_adip; // Internet address
	const uid_t		_uid ;
	const char* const	_pwname ;
	const char* const	_dir ;
	const pid_t		_pid ;
	const time_t		_start;
	const char* const	_cstart ;


private :
	Identity( void );
	Identity( const Identity &monid );


public :
	Identity(const char *name);
	~Identity();
	friend ostream & operator<< ( ostream& os , const Identity& monid );

	const char* const	name( void ) const;
	const pid_t&		pid(void) const;
	const struct utsname&	hostid(void) const;
	const char* const	adip(void) const;
	const uid_t&		uid(void) const;
	const char* const	pwname(void) const;
	const time_t&		start(void) const;
	const char* const 	rep (void) const;

	const char*		host_char(void ) const;
	const char*		start_char(void) const;

} ;
# endif		/* # if !defined(  __IDENTITE_H__ ) */
