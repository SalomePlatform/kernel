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
//  File   : Utils_Identity.cxx
//  Author : Pascale NOYRET, EDF
//  Module : SALOME
//  $Header$
//
# include <iostream>
# include "utilities.h"
# include "Utils_Identity.hxx"

extern "C"
{
# include <string.h>

#ifndef WIN32 /* unix functionality */
# include <pwd.h>
#endif
}

#ifndef WIN32 /* unix functionality */

# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <netdb.h>

const char* duplicate( const char *const str ) ;

const struct utsname get_uname( void )
{
        struct utsname          hostid;
#if defined(_DEBUG_) || defined(_DEBUG)
        const int retour=uname(&hostid);
        ASSERT(retour>=0);
#else
        uname(&hostid);
#endif
        return hostid ;
}

const char* get_adip( void )
{
        struct utsname  hostid;
#if defined(_DEBUG_) || defined(_DEBUG)
        const int retour=uname(&hostid);
        ASSERT(retour>=0);
#else
        uname(&hostid);
#endif

        const hostent* pour_adip=gethostbyname(hostid.nodename);
        ASSERT(pour_adip!=NULL);
        const in_addr ip_addr=*(struct in_addr*)(pour_adip->h_addr) ;
        return duplicate(inet_ntoa(ip_addr));
}
const char* const get_pwname( void )
{
        struct passwd *papa = getpwuid(getuid());
        return papa->pw_name ;
}

#else /* Windows functionality */

#include <windows.h>
#include <direct.h>
#include <process.h>

const char* duplicate( const char *const str ) ;

const char* get_uname( void )
{
        static std::string hostName(4096, 0);
        static DWORD nSize = hostName.length();
        static int res = ::GetComputerNameEx(ComputerNameDnsFullyQualified, &hostName[0], &nSize);
        ASSERT( res );
        return hostName.c_str();
}

const char* get_adip( void )
{
        //#include <Nspapi.h>
        //#include <Svcguid.h>
        //static GUID sType = SVCID_HOSTNAME;
        //static CSADDR_INFO* ips = new CSADDR_INFO[8]; // in case multiple IP addresses are returned
        //static DWORD nSize = 1024;
        //static std::string uname = get_uname();
        //static int res = ::GetAddressByName( NS_DEFAULT, &sType, &uname[0], 0, 0, 0, ips, &nSize, 0, 0 );
        //if ( res )
        //  return ips[0].LocalAddr.lpSockaddr->sa_data;

        static hostent* he = ::gethostbyname( get_uname() );
        if ( he && he->h_addr_list && he->h_length >0 ) {
          static char str[16];
      unsigned i1 = (unsigned char)he->h_addr_list[0][0];
      unsigned i2 = (unsigned char)he->h_addr_list[0][1];
      unsigned i3 = (unsigned char)he->h_addr_list[0][2];
      unsigned i4 = (unsigned char)he->h_addr_list[0][3];
      sprintf ( str, "%03u.%03u.%03u.%03u", i1, i2, i3, i4 );
                return str;
        }
        return "<unknown>";
}

const char* const get_pwname( void )
{
  static std::string retVal(4096, 0);
  static DWORD  dwSize = retVal.length() + 1;
  static int res = GetUserName( &retVal[0], &dwSize );
  ASSERT( res );
  return retVal.c_str();
}

PSID getuid() {
        PSID         retVal        = NULL;
        HANDLE       hProcessToken = INVALID_HANDLE_VALUE;
        PTOKEN_OWNER pTKowner      = NULL;
        LPVOID buffer = NULL;
        DWORD dwsize = 0;

        if (  !OpenProcessToken ( GetCurrentProcess (), TOKEN_QUERY, &hProcessToken )) return 0;
        if (!GetTokenInformation(hProcessToken, TokenOwner, buffer, dwsize, &dwsize)) return 0;
        pTKowner = (PTOKEN_OWNER)buffer;
        if ( pTKowner != NULL ) {
                retVal = pTKowner->Owner;
        }
        if ( hProcessToken != INVALID_HANDLE_VALUE ) CloseHandle ( hProcessToken );

        return retVal;
}

#define getcwd _getcwd
#define getpid _getpid

#endif /* WIN32 */


Identity::Identity( const char *name ): _name(duplicate(name)),\
                                                        _hostid(get_uname()),\
                                                        _adip(get_adip()),\
                                                        _uid(getuid()) ,\
                                                        _pwname(get_pwname()) ,\
                                                        _dir(getcwd(NULL,4096)),\
                                                        _pid(getpid()) ,\
                                                        _start(time(NULL)),\
                                                        _cstart(ctime(&_start))
//CCRT
{
        ASSERT(strlen(_dir)<4096);
}


Identity::~Identity(void)
{
        delete [] (char*)_name ;
        (char*&)_name = NULL ;

        //delete [] (char*)_dir ;
        //(char*&)_dir = NULL ;
        free((char*)_dir);
#ifndef WIN32
  // free the memory only on Unix
  // becasue at Windows it is the same static variable
  // (function get_adip() returns the same char* as get_uname() )
        delete [] (char*)_adip ;
#endif
        (char*&)_adip = NULL ;

}

/*------------*/
/* Accessors  */
/*------------*/

const char* const Identity::name (void) const
{
        return  _name ;
}
#ifndef WIN32
        const pid_t& Identity::pid(void) const
#else
        const DWORD& Identity::pid(void) const
#endif
{
        return _pid ;
}

#ifndef WIN32
        const struct utsname &Identity::hostid(void) const
#else
        const char* const Identity::hostid(void) const
#endif
{
    return _hostid ;
}

#ifndef WIN32
        const uid_t& Identity::uid(void) const
#else
        const PSID& Identity::uid(void) const
#endif
{
        return _uid ;
}
const time_t &Identity::start(void) const
{
        return _start ;
}
const char* const Identity::rep (void) const
{
        return  _dir ;
}
const char* const Identity::pwname (void) const
{
        return  _pwname ;
}
const char* const Identity::adip (void) const
{
        return _adip ;
}

/*------------------*/
/* Other methods    */
/*------------------*/

const char* Identity::host_char( void ) const
{
#ifndef WIN32
        return _hostid.nodename;
#else
        return _hostid;
#endif
}

const char* Identity::start_char(void) const
{
        return ctime(&_start) ;
}

std::ostream & operator<< ( std::ostream& os , const Identity& monid )
{
        ASSERT(monid._name!=NULL) ;
        os << "Identity :" << std::endl ;
        os << '\t' << "Component name : " << monid._name << std::endl ;
        os << '\t' << "Numero de PID :  " << monid._pid << std::endl;
        os << '\t' << "Uid utilisateur  : "   << monid._uid << std::endl;
        os << '\t' << "nom utilisateur  : "   << monid._pwname << std::endl;
#ifndef WIN32
        os << '\t' << "Nom de machine : " << monid._hostid.nodename << std::endl;
#else
        os << '\t' << "Nom de machine : " << monid._hostid << std::endl;
#endif
        os << '\t' << "Adresse IP : " << monid._adip << std::endl;
        os << '\t' << "Heure de lancement : " << monid._cstart ; //ctime(&monid._start) ;
        os << '\t' << "Dans le repertoire : " << monid._dir << std::endl;

        return os ;
}
