// Copyright (C) 2007-2020  CEA/DEN, EDF R&D, OPEN CASCADE
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
//  Author  : Alexander A. BORODIN
//  Module : SALOME
//
#include "Basics_DirUtils.hxx"
#include "Basics_Utils.hxx"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <algorithm>

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

#define _extension_ ".hdf"

namespace Kernel_Utils
{
  std::string GetBaseName( const std::string& file_path, const bool with_extension )
  {
    std::string tmp_str = file_path;
    auto pos = file_path.rfind( _separator_ );
    if ( pos >= 0 )
      tmp_str = pos < (int)file_path.size()-1 ? file_path.substr( pos+1 ) : "";

    pos = tmp_str.rfind( _extension_ );
    if( !with_extension && pos >= 0 )
      tmp_str = pos < (int)tmp_str.size()-1 ? tmp_str.substr( 0, pos ) : "";

    return tmp_str;
  }

  std::string GetDirName( const std::string& file_path )
  {
    auto pos = file_path.rfind( _separator_ );
    if ( pos >= 0 )
      return pos < (int)file_path.size()-1 ? file_path.substr(0, pos ) : "";
    return std::string(".");
  }

  std::string GetTmpDirByEnv( const std::string& tmp_path_env )
  {
#if defined WIN32 && defined UNICODE
   std::wstring w_tmp_path_env = utf8_decode_s( tmp_path_env );
   wchar_t* val = _wgetenv( w_tmp_path_env.c_str() );
   std::string dir = val ? utf8_encode_s(val) : "";
#else 
    char* val = getenv( tmp_path_env.c_str() );
	std::string dir = val ? val : "";
#endif
    return GetTmpDirByPath( dir );
  }

  std::string GetTmpDirByPath( const std::string& tmp_path )
  {
    std::string aTmpDir = tmp_path;
    if ( aTmpDir == "" || !IsExists( aTmpDir ))
    {
#ifdef WIN32
#ifdef UNICODE
	  wchar_t *wTmp_dir = _wgetenv( L"TEMP" );
	  if ( wTmp_dir == NULL ) {
			wTmp_dir = _wgetenv(L"TMP");
			if (wTmp_dir == NULL)
				wTmp_dir = L"C:\\";
	  }
	  aTmpDir = utf8_encode_s( wTmp_dir );
#else
      char *Tmp_dir = getenv("TEMP");
      if ( Tmp_dir == NULL )
      {
        Tmp_dir = getenv("TMP");
        if ( Tmp_dir == NULL )
          aTmpDir = "C:\\";
        else
          aTmpDir = Tmp_dir;
      }
      else
        aTmpDir = Tmp_dir;
#endif
#else
      aTmpDir = "/tmp/";
#endif
    }

    if ( aTmpDir.back() != _separator_ )
      aTmpDir += _separator_;

    srand( (unsigned int)time( NULL ));
    int aRND = 999 + (int)(100000.0*rand()/(RAND_MAX+1.0)); //Get a random number to present a name of a sub directory
    char buffer[127];
    sprintf( buffer, "%d", aRND );
    std::string aSubDir( buffer );
    if ( aSubDir.size() <= 1 ) aSubDir = "123409876";

    aTmpDir += aSubDir; //Get RND sub directory

    std::string aDir = aTmpDir;

    for ( aRND = 0; IsExists( aDir ); aRND++ )
    {
      sprintf( buffer, "%d", aRND );
      aDir = aTmpDir + buffer;  //Build a unique directory name
    }

    if ( aDir.back() != _separator_ ) aDir += _separator_;

#ifdef WIN32	
#ifdef UNICODE
	std::wstring aDirToCreate = utf8_decode_s(aDir);
#else
	std::string aDirToCreate = aDir;
#endif
    CreateDirectory( aDirToCreate.c_str(), NULL );
#else
    mkdir( aDir.c_str(), 0x1ff );
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
  
  std::string AddExtension( const std::string& name )
  {
    std::string tmp_str = name;
    auto pos = tmp_str.rfind( _extension_ );
    if( pos < 0 )
      return tmp_str.append( _extension_ );
    return tmp_str;
  }

  //============================================================================
  // function : IsExists
  // purpose  : Returns True(False) if the path (not)exists
  //============================================================================ 
  bool IsExists(const std::string& thePath) 
  {
#if defined WIN32 && defined UNICODE
	int status = _waccess( utf8_decode_s( thePath).c_str(), F_OK );
#else
    int status = access ( thePath.c_str() , F_OK ); 
#endif
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
#ifdef UNICODE
	  std::wstring aPathToCheck = utf8_decode_s( thePath );
#else
	  std::string aPathToCheck = thePath;
#endif
    if (  GetFileAttributes ( aPathToCheck.c_str()  ) == 0xFFFFFFFF  ) {
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
#ifdef UNICODE
	std::wstring aPathToCheck = utf8_decode_s(thePath);
#else
	std::string aPathToCheck = thePath;
#endif

    HANDLE hFile = FindFirstFile( aPathToCheck.c_str(), &aFileData );
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

  //============================================================================
  // function : BackSlashToSlash  
  // purpose  : Convert back slash to slash
  //============================================================================ 
  std::string BackSlashToSlash(const std::string& path) {
	  std::string res = path;
	  std::replace(res.begin(), res.end(), '\\', '/');
	  return res;
  }


  //============================================================================
  // function : BackSlashToSlash  
  // purpose  : Convert back slash to slash
  //============================================================================ 
  std::string HomePath() {
#ifdef WIN32
    std::string homedir = getenv("USERPROFILE");
#else
    std::string homedir = getenv("HOME");       
#endif
    return homedir;
  }
}
