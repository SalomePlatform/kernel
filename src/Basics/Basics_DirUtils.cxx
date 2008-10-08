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
//  File   : Basics_DirUtils.cxx
//  Autor  : Alexander A. BORODIN
//  Module : SALOME

#include "Basics_DirUtils.hxx"

#include <errno.h>

#ifndef WIN32
# include <sys/stat.h>
#else
# include <windows.h>
# include <time.h>
#endif

using namespace std;

#ifdef WIN32
# define _separator_ '\\'
#else
# define _separator_ '/'
#endif

namespace Kernel_Utils
{
  string GetTmpDirByEnv( const std::string& tmp_path_env )
  {
    string dir;
    char* val = getenv( tmp_path_env.c_str() );
    val ? dir = string( val ) : "";
    return GetTmpDirByPath( dir );
  }

  string GetTmpDirByPath( const std::string& tmp_path )
  {
    string aTmpDir = tmp_path;
    if ( aTmpDir == "" )
      {
#ifdef WIN32
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
    
    if(aTmpDir[aTmpDir.size()-1] != _separator_)
      aTmpDir+=_separator_;
    
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
    
    if(aDir[aDir.size()-1] != _separator_) aDir += _separator_;
    
#ifdef WIN32
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
  string GetTmpDir()
  {
    return GetTmpDirByPath( "" );
  }
  
  string GetTmpFileName()
  {
    string tmpDir = GetTmpDir();
    string aFilePath = "";
    if(IsExists(tmpDir)) {
      srand((unsigned int)time(NULL));
      int aRND = 999 + (int)(100000.0*rand()/(RAND_MAX+1.0)); //Get a random number to present a name of a sub directory
      char buffer[127];
      sprintf(buffer, "%d", aRND);
      string aSubDir(buffer);
      if(aSubDir.size() <= 1) aSubDir = string("123409876");
      
      aFilePath = tmpDir;
      for(aRND = 0; IsExists(aFilePath); aRND++) {
        sprintf(buffer, "%d", aRND);
        aFilePath = tmpDir+buffer;  //Build a unique file name
      }
    }
    return aFilePath;
  }
  
  
  bool IsExists(const string& thePath) 
  {
#ifdef WIN32 
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
  
}
