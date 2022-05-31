// Copyright (C) 2007-2022  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  File      : SALOMEDSImpl_Tool.cxx
//  Created   : Mon Oct 21 16:24:34 2002
//  Author    : Sergey RUIN
//  Project   : SALOME
//  Module    : SALOMEDSImpl
//
#include <stdio.h>
#include <iostream> 
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <iterator>
#include <sstream>

#include "Basics_DirUtils.hxx"
#include "Basics_Utils.hxx"

#include "SALOMEDSImpl_Tool.hxx"

#ifndef WIN32
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h> 
#include <unistd.h>
#else
#include <time.h>
#include <lmcons.h>
//#include <winbase.h>
#include <windows.h>
#endif


bool SALOMEDS_Exists(const std::string thePath)
{
	return Kernel_Utils::IsExists( thePath );
}

//============================================================================
// function : GetTempDir
// purpose  : Return a temp directory to store created files like "/tmp/sub_dir/"
//============================================================================
std::string SALOMEDSImpl_Tool::GetTmpDir()
{
  return Kernel_Utils::GetTmpDirByEnv("SALOME_TMP_DIR");
}

//============================================================================
// function : RemoveTemporaryFiles
// purpose  : Removes files listed in theFileList
//============================================================================
void SALOMEDSImpl_Tool::RemoveTemporaryFiles(const std::string& theDirectory,
                                             const std::vector<std::string>& theFiles,
                                             const bool IsDirDeleted)
{
  std::string aDirName = theDirectory;

  size_t i, aLength = theFiles.size(); 
  for(i=1; i<=aLength; i++) {
    std::string aFile(aDirName);
    aFile += theFiles[i-1];
    if(!SALOMEDS_Exists(aFile)) continue;

#ifdef WIN32
#if defined(UNICODE)
	std::wstring aFileToDelete = Kernel_Utils::utf8_decode_s(aFile);
#else
	std::string aFileToDelete = aFile;
#endif
    DeleteFile( aFileToDelete.c_str() );
#else 
    unlink(aFile.c_str());
#endif
  }

  if(IsDirDeleted) {
    if(SALOMEDS_Exists(aDirName)) {
#ifdef WIN32
#if defined(UNICODE)
		std::wstring aDirToDelete = Kernel_Utils::utf8_decode_s(aDirName);
#else
		std::string aDirToDelete = aDirName;
#endif
      RemoveDirectory(aDirToDelete.c_str());
#else
      rmdir(aDirName.c_str());
#endif
    }
  }

}

//============================================================================
// function : GetNameFromPath
// purpose  : Returns the name by the path
//============================================================================
std::string SALOMEDSImpl_Tool::GetNameFromPath(const std::string& thePath) {
  if (thePath.empty()) return "";
  int pos = (int)thePath.rfind('/'); //TODO: conversion from size_t to int
  if(pos >= 0) return thePath.substr(pos+1, thePath.size());
  pos = thePath.rfind('\\'); 
  if(pos >= 0) return thePath.substr(pos+1, thePath.size()); 
  pos = thePath.rfind('|');
  if(pos >= 0) return thePath.substr(pos+1, thePath.size()); 
  return thePath;
}

//============================================================================
// function : GetDirFromPath
// purpose  : Returns the dir by the path
//============================================================================
std::string SALOMEDSImpl_Tool::GetDirFromPath(const std::string& thePath) {
#ifdef WIN32
  std::string separator = "\\";
#else
  std::string separator = "/";
#endif

  std::string path;
  if (!thePath.empty()) {
    int pos = (int)thePath.rfind('/'); //TODO: conversion from size_t to int
    if (pos < 0) pos = thePath.rfind('\\');
    if (pos < 0) pos = thePath.rfind('|');
    
    if (pos >= 0)
      path = thePath.substr(0, pos+1);
    else
      path = std::string(".") + separator;

#ifdef WIN32  //Check if the only disk letter is given as path
    if (path.size() == 2 && path[1] == ':') path += separator;
#endif
    
    while ( (pos=path.find('|')) >= 0 )
      path.replace(pos, 1, separator);
  }
  
  return path;
}

//============================================================================
// function : 
// purpose  : The functions returns a list of substring of initial string 
//            divided by given separator
//============================================================================
std::vector<std::string> SALOMEDSImpl_Tool::splitString(const std::string& theValue, char separator)
{
  std::vector<std::string> vs;
  if(theValue[0] == separator && theValue.size() == 1) return vs;
  int pos = (int)theValue.find(separator); //TODO: conversion from size_t to int
  if(pos < 0) {
    vs.push_back(theValue);
    return vs;
  }
 
  std::string s = theValue;
  if(s[0] == separator) s = s.substr(1, s.size());
  while((pos = s.find(separator)) >= 0) {
    vs.push_back(s.substr(0, pos));
    s = s.substr(pos+1, s.size());
  }
               
  if(!s.empty() && s[0] != separator) vs.push_back(s);
  return vs;
}

//============================================================================
// function : 
// purpose  : The functions returns a list of substring of initial string 
//            divided by given separator include empty strings
//============================================================================

std::vector<std::string> treatRepetation(const std::string& theValue);

std::vector<std::string> treatRepetation(const std::string& theValue)
{
  std::vector<std::string> aResult;
  int pos = (int)theValue.find(";*="); //TODO: conversion from size_t to int
  if(pos < 0 )
    {
      aResult.push_back(theValue);
      return aResult;
    }
  std::string val(theValue.substr(0, pos));
  std::string suffix(theValue.substr(pos+3));
  int nb;
  std::istringstream tmp(suffix);
  tmp >> nb;
  for(int i=0; i<nb; i++)
    aResult.push_back(val);
  return aResult;
}

std::vector<std::string> SALOMEDSImpl_Tool::splitStringWithEmpty(const std::string& theValue, char sep)
{
  std::vector<std::string> aResult;
  if(theValue[0] == sep ) aResult.push_back(std::string());
  int pos = (int)theValue.find(sep); //TODO: conversion from size_t to int
  if(pos < 0 ) {
    if(sep == '|')
      {
        std::vector<std::string> tmp = treatRepetation(theValue);
        std::copy(tmp.begin(), tmp.end(), std::back_insert_iterator< std::vector<std::string> >(aResult));
      }
    else
      aResult.push_back(theValue);
    return aResult;
  }

  std::string s = theValue;
  if(s[0] == sep) s = s.substr(1, s.size());
  while((pos = s.find(sep)) >= 0) {
    if(sep == '|')
      {
        std::vector<std::string> tmp = treatRepetation(s.substr(0, pos));
        std::copy(tmp.begin(), tmp.end(), std::back_insert_iterator< std::vector<std::string> >(aResult));
      }
    else
      aResult.push_back(s.substr(0, pos));
    s = s.substr(pos+1, s.size());
  }

  if(!s.empty() && s[0] != sep) {
    if(sep == '|')
      {
        std::vector<std::string> tmp = treatRepetation(s);
        std::copy(tmp.begin(), tmp.end(), std::back_insert_iterator< std::vector<std::string> >(aResult));
      }
    else
      aResult.push_back(s);
  }
  if(theValue[theValue.size()-1] == sep) aResult.push_back(std::string());

  return aResult;
}

//============================================================================
// function : 
// purpose  : The functions returns a list of lists of substrings of initial string 
//            divided by two given separators include empty strings
//============================================================================
std::vector< std::vector<std::string> > SALOMEDSImpl_Tool::splitStringWithEmpty(const std::string& theValue, char sep1, char sep2)
{
  std::vector< std::vector<std::string> > aResult;
  if(theValue.size() > 0) {
    std::vector<std::string> aSections = splitStringWithEmpty( theValue, sep1 );
    for( size_t i = 0, n = aSections.size(); i < n; i++ )
      aResult.push_back( splitStringWithEmpty( aSections[i], sep2 ) );
  }
  return aResult;
}


void SALOMEDSImpl_Tool::GetSystemDate(int& year, int& month, int& day, int& hours, int& minutes, int& seconds)
{
#ifdef WIN32
  SYSTEMTIME    st;

  GetLocalTime ( &st );

  month = st.wMonth;
  day = st.wDay;
  year = st.wYear;
  hours = st.wHour;
  minutes = st.wMinute;
  seconds = st.wSecond;
#else
  struct tm transfert;
  struct timeval tval;
  struct timezone tzone;
  //int status;

  /*status = */ gettimeofday( &tval, &tzone );
  memcpy(&transfert, localtime((time_t *)&tval.tv_sec), sizeof(tm));

  month    = transfert.tm_mon + 1;
  day      = transfert.tm_mday;
  year     = transfert.tm_year + 1900;
  hours    = transfert.tm_hour;
  minutes  = transfert.tm_min ;
  seconds  = transfert.tm_sec ;
#endif
}

//Warning undef of Ascii Winwows define
#ifdef WIN32
# undef GetUserName
#endif
std::string SALOMEDSImpl_Tool::GetUserName()
{
#ifdef WIN32
  char*  pBuff = new char[UNLEN + 1];
  DWORD  dwSize = UNLEN + 1;
  std::string retVal;
  ::GetUserNameA( pBuff, &dwSize );
  std::string theTmpUserName(pBuff,(int)dwSize -1 );
  retVal = theTmpUserName;
  delete [] pBuff;
  return retVal;
#else
 struct passwd *infos;
 infos = getpwuid(getuid()); 
 return std::string(infos->pw_name);
#endif
}

