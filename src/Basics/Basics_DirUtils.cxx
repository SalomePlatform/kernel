// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  File   : Basics_DirUtils.cxx
//  Autor  : Alexander A. BORODIN
//  Module : SALOME
//
#include "Basics_DirUtils.hxx"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#ifndef WIN32
# include <sys/stat.h>
# include <dirent.h>
# include <unistd.h>
#else
#include <io.h>
#define F_OK 0
#define access _access
# include <windows.h>
# include <time.h>
#endif

#ifdef WIN32
# define _separator_ '\\'
#else
# define _separator_ '/'
#endif

namespace Kernel_Utils
{
  std::string GetBaseName( const std::string& file_path )
  {
    int pos = file_path.rfind( _separator_ );
    if ( pos >= 0 )
      return pos < (int)file_path.size()-1 ? file_path.substr( pos+1 ) : "";
    return file_path;
  }

  std::string GetDirName( const std::string& file_path )
  {
    int pos = file_path.rfind( _separator_ );
    if ( pos >= 0 )
      return pos < (int)file_path.size()-1 ? file_path.substr(0, pos ) : "";
    return std::string(".");
  }

  std::string GetTmpDirByEnv( const std::string& tmp_path_env )
  {
    char* val = getenv( tmp_path_env.c_str() );
    std::string dir = val ? val : "";
    return GetTmpDirByPath( dir );
  }

  std::string GetTmpDirByPath( const std::string& tmp_path )
  {
    std::string aTmpDir = tmp_path;
    if ( aTmpDir == "" )
      {
#ifdef WIN32
        char *Tmp_dir = getenv("TEMP");
        if( Tmp_dir == NULL )
          {
            Tmp_dir = getenv("TMP");
            if (Tmp_dir == NULL)
              aTmpDir = std::string("C:\\");
            else 
              aTmpDir = std::string(Tmp_dir);
          }
        else
          aTmpDir = std::string(Tmp_dir);
#else
        aTmpDir = std::string("/tmp/");
#endif
      }
    
    if(aTmpDir[aTmpDir.size()-1] != _separator_)
      aTmpDir+=_separator_;
    
    srand((unsigned int)time(NULL));
    int aRND = 999 + (int)(100000.0*rand()/(RAND_MAX+1.0)); //Get a random number to present a name of a sub directory
    char buffer[127];
    sprintf(buffer, "%d", aRND);
    std::string aSubDir(buffer);
    if(aSubDir.size() <= 1) aSubDir = std::string("123409876");
    
    aTmpDir += aSubDir; //Get RND sub directory
    
    std::string aDir = aTmpDir;
    
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
  // purpose  : Returns a temp directory to store created files like "/tmp/sub_dir/" 
  //============================================================================ 
  std::string GetTmpDir()
  {
    return GetTmpDirByPath( "" );
  }

  //============================================================================
  // function : GetTempFileName
  // purpose  : Returns the unique temporary file name without any extension /tmp/something/file for Unix or c:\something\file for WIN32
  //============================================================================ 
  std::string GetTmpFileName()
  {
    std::string tmpDir = GetTmpDir();
    std::string aFilePath = "";
    if(IsExists(tmpDir)) {
      srand((unsigned int)time(NULL));
      int aRND = 999 + (int)(100000.0*rand()/(RAND_MAX+1.0)); //Get a random number to present a name of a sub directory
      char buffer[127];
      sprintf(buffer, "%d", aRND);
      std::string aSubDir(buffer);
      if(aSubDir.size() <= 1) aSubDir = std::string("123409876");
      
      aFilePath = tmpDir;
      for(aRND = 0; IsExists(aFilePath); aRND++) {
        sprintf(buffer, "%d", aRND);
        aFilePath = tmpDir+buffer;  //Build a unique file name
      }
    }
    return aFilePath;
  }
  
  //============================================================================
  // function : IsExists
  // purpose  : Returns True(False) if the path (not)exists
  //============================================================================ 
  bool IsExists(const std::string& thePath) 
  {
    int status = access ( thePath.c_str() , F_OK ); 
    if (status != 0) return false;
    return true;
  }

  //============================================================================
  // function : IsWritable
  // purpose  : Returns True(False) if the path is (not) writable
  //============================================================================ 
  bool IsWritable(const std::string& thePath) 
  {
#ifdef WIN32 
    if (  GetFileAttributes (  thePath.c_str()  ) == 0xFFFFFFFF  ) { 
      if (  GetLastError () == FILE_ATTRIBUTE_READONLY ) {
        return false;
      }
    }
#else 
    int status = access(thePath.c_str(),W_OK); 
    if (status != 0) return false;
#endif
    return true;
  }


  //============================================================================
  // function : GetDirByPath
  // purpose  : Returns directory by path and converts it to native system format
  //============================================================================ 
  std::string GetDirByPath(const std::string& thePath)
  {
    if (thePath.empty())
      return "";
    std::string path = thePath;
    std::string::size_type length = path.length();

    //detect all separators in Unix format
    for ( unsigned int i = 0; i < length; i++ )
    {
      if( path[i] == '/' )
        path[i] = '|';
    }

    //detect all separators in Windows format
    for ( unsigned int i = 0; i < length; i++ )
    {
      if( path[i] == '\\' )
        path[i] = '|';
    }


    std::string::size_type pos = path.rfind('|');
    if ( pos == std::string::npos )
    {
#ifdef WIN32
      //check for disk letter ( C: )
      if ( path.length() == 2 && path[1] == ':' )
        path += _separator_;
#else
      //not valid path
      return "";
#endif
    }
    else
    {
      //remove right subdirectory or filename from path
      path = path.substr( 0, pos );
    }

    length = path.length();
    for ( unsigned int i = 0; i < length; i++ )
    {
      if( path[i] == '|' )
        path[i] = _separator_;
    }
    return path;
  }

  //============================================================================
  // function : IsEmptyDir
  // purpose  : Returns True(False) if the path (not) empty
  //            Also returns False if the path is not valid
  //============================================================================ 
  bool IsEmptyDir(const std::string& thePath) 
  {
    if ( thePath.empty() || !IsExists(thePath))
      return false;

    bool result = false;

#ifdef WIN32
    WIN32_FIND_DATA aFileData;
    HANDLE hFile = FindFirstFile( thePath.c_str(), &aFileData );
    if ( hFile == INVALID_HANDLE_VALUE )
    {
      //empty dir
      result = true;
    }
    else
    {
      //close serching. path is not empty
      FindClose( hFile );
    }
#else
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(thePath.c_str())) == NULL)
    {
      //Could not open directory
      return false;
    }
    else
    {
      result = true; //empty if no file found
      while ((dirp = readdir(dp)) != NULL && result )
        {
          std::string file_name(dirp->d_name);
          result = file_name.empty() || file_name == "." || file_name == ".."; //if any file - break and return false
        }
        closedir(dp);
    }
#endif
    return result;
  }
}
