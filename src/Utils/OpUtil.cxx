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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : OpUtil.cxx
//  Module : SALOME

#include "utilities.h" 
#include "OpUtil.hxx"
#include <errno.h>
#include <string.h>

#ifndef WNT
#include <unistd.h>
#include <sys/stat.h>
#else
#include <winsock2.h>
#endif
using namespace std;

//int gethostname(char *name, size_t len);

std::string GetHostname()
{
  int ls = 100, r = 1;
  char *s;

  while (ls < 10000 && r) {
    ls *= 2;
    s = new char[ls];
    r = gethostname(s, ls-1);
    switch (r) 
      {
      case 0:
	  break;
      default:
#ifdef EINVAL
      case EINVAL:
#endif
#ifdef ENAMETOOLONG
      case ENAMETOOLONG:
#endif
        delete [] s;
	continue;
      }
  }

  if (r != 0) {
    s = new char[50];
    strcpy(s, "localhost");
  }

  // remove all after '.'
  char *aDot = (strchr(s,'.'));
  if (aDot) aDot[0] = '\0';

  string p = s;
  delete [] s;
  return p;
}


std::string OpUtil_Dir::GetTmpDirByEnv( const std::string& tmp_path_env )
{
  string dir;
  char* val = getenv( tmp_path_env.c_str() );
  val ? dir = string( val ) : "";
  return GetTmpDirByPath( dir );
}

std::string OpUtil_Dir::GetTmpDirByPath( const std::string& tmp_path )
{
  string aTmpDir = tmp_path;
  char sep = 
#ifdef WNT
            '\\';
#else
            '/';
#endif
  /*if ( tmp_path.length() > 0 )
  {
    char *Tmp_dir = getenv(tmp_path_env.c_str());
    if( Tmp_dir != NULL )
    {
      aTmpDir = string(Tmp_dir);
      if(aTmpDir[aTmpDir.size()-1] != sep)
        aTmpDir+=sep;
    }
  }*/
  if ( aTmpDir == "" )
  {
#ifdef WNT
    char *Tmp_dir = getenv("TEMP");
    if( Tmp_dir == NULL )
    {
      Tmp_dir = getenv("TMP");
      if (Tmp_dir == NULL)
        aTmpDir = string("C:\\");
      else 
        aTmpDir = string(Tmp_dir);
    }
    else
      aTmpDir = string(Tmp_dir);
#else
    aTmpDir = string("/tmp/");
#endif
  }

  if(aTmpDir[aTmpDir.size()-1] != sep)
    aTmpDir+=sep;

  srand((unsigned int)time(NULL));
  int aRND = 999 + (int)(100000.0*rand()/(RAND_MAX+1.0)); //Get a random number to present a name of a sub directory
  char buffer[127];
  sprintf(buffer, "%d", aRND);
  string aSubDir(buffer);
  if(aSubDir.size() <= 1) aSubDir = string("123409876");

  aTmpDir += aSubDir; //Get RND sub directory

  string aDir = aTmpDir;

  if(IsExists(aDir)) {
    for(aRND = 0; IsExists(aDir); aRND++) {
      sprintf(buffer, "%d", aRND);
      aDir = aTmpDir+buffer;  //Build a unique directory name
    }
  }

  if(aDir[aDir.size()-1] != sep) aDir+=sep;

#ifdef WNT
  CreateDirectory(aDir.c_str(), NULL);
#else
  mkdir(aDir.c_str(), 0x1ff); 
#endif

  return aDir;
}

//============================================================================
// function : GetTempDir
// purpose  : Return a temp directory to store created files like "/tmp/sub_dir/" 
//============================================================================ 
std::string OpUtil_Dir::GetTmpDir()
{
  return GetTmpDirByPath( "" );
  //Find a temporary directory to store a file
/*
  string aTmpDir = "";
  char sep = 
#ifdef WNT
            '\\';
#else
            '/';
#endif
  if ( tmp_path_env.length() > 0 )
  {
    char *Tmp_dir = getenv(tmp_path_env.c_str());
    if( Tmp_dir != NULL )
    {
      aTmpDir = string(Tmp_dir);
      if(aTmpDir[aTmpDir.size()-1] != sep)
        aTmpDir+=sep;
    }
  }
  if ( aTmpDir == "" )
  {
#ifdef WNT
    char *Tmp_dir = getenv("TEMP");
    if( Tmp_dir == NULL )
    {
      Tmp_dir = getenv("TMP");
      if (Tmp_dir == NULL)
        aTmpDir = string("C:\\");
      else 
        aTmpDir = string(Tmp_dir);
    }
    else
      aTmpDir = string(Tmp_dir);
#else
    aTmpDir = string("/tmp/");
#endif
  }

  srand((unsigned int)time(NULL));
  int aRND = 999 + (int)(100000.0*rand()/(RAND_MAX+1.0)); //Get a random number to present a name of a sub directory
  char buffer[127];
  sprintf(buffer, "%d", aRND);
  string aSubDir(buffer);
  if(aSubDir.size() <= 1) aSubDir = string("123409876");

  aTmpDir += aSubDir; //Get RND sub directory

  string aDir = aTmpDir;

  if(Exists(aDir)) {
    for(aRND = 0; Exists(aDir); aRND++) {
      sprintf(buffer, "%d", aRND);
      aDir = aTmpDir+buffer;  //Build a unique directory name
    }
  }

  if(aDir[aDir.size()-1] != sep) aDir+=sep;

#ifdef WNT
  CreateDirectory(aDir.c_str(), NULL);
#else
  mkdir(aDir.c_str(), 0x1ff); 
#endif

  return aDir;*/
}

bool OpUtil_Dir::IsExists(const string& thePath) 
{
#ifdef WNT 
  if (  GetFileAttributes (  thePath.c_str()  ) == 0xFFFFFFFF  ) { 
    if (  GetLastError () == ERROR_FILE_NOT_FOUND  ) {
      return false;
    }
  }
#else 
  int status = access ( thePath.c_str() , F_OK ); 
  if (status != 0) return false;
#endif
  return true;
}