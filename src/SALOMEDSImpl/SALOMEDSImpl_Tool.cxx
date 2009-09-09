//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
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

using namespace std;



bool Exists(const string thePath) 
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




//============================================================================
// function : GetTempDir
// purpose  : Return a temp directory to store created files like "/tmp/sub_dir/" 
//============================================================================ 
string SALOMEDSImpl_Tool::GetTmpDir()
{
  //Find a temporary directory to store a file

  string aTmpDir;

  char *Tmp_dir = getenv("SALOME_TMP_DIR");
  if(Tmp_dir != NULL) {
    aTmpDir = string(Tmp_dir);
#ifdef WIN32
    if(aTmpDir[aTmpDir.size()-1] != '\\') aTmpDir+='\\';
#else
    if(aTmpDir[aTmpDir.size()-1] != '/') aTmpDir+='/';
#endif      
  }
  else {
#ifdef WIN32
    aTmpDir = string("C:\\");
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

#ifdef WIN32
  if(aDir[aTmpDir.size()-1] != '\\') aDir+='\\';
#else
  if(aDir[aTmpDir.size()-1] != '/') aDir+='/';
#endif


#ifdef WIN32
  CreateDirectory(aDir.c_str(), NULL);
#else
  mkdir(aDir.c_str(), 0x1ff); 
#endif

  return aDir;
}

//============================================================================
// function : RemoveTemporaryFiles
// purpose  : Removes files listed in theFileList
//============================================================================
void SALOMEDSImpl_Tool::RemoveTemporaryFiles(const string& theDirectory, 
					     const vector<string>& theFiles,
					     const bool IsDirDeleted)
{
  string aDirName = theDirectory;

  int i, aLength = theFiles.size();
  for(i=1; i<=aLength; i++) {
    string aFile(aDirName);
    aFile += theFiles[i-1];
    if(!Exists(aFile)) continue;

#ifdef WIN32
    DeleteFile(aFile.c_str());
#else 
    unlink(aFile.c_str());
#endif
  }

  if(IsDirDeleted) {
    if(Exists(aDirName)) {
#ifdef WIN32
      RemoveDirectory(aDirName.c_str());
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
string SALOMEDSImpl_Tool::GetNameFromPath(const string& thePath) {
  if (thePath.empty()) return "";
  int pos = thePath.rfind('/');
  if(pos > 0) return thePath.substr(pos+1, thePath.size());
  pos = thePath.rfind('\\'); 
  if(pos > 0) return thePath.substr(pos+1, thePath.size()); 
  pos = thePath.rfind('|');
  if(pos > 0) return thePath.substr(pos+1, thePath.size()); 
  return thePath;
}

//============================================================================
// function : GetDirFromPath
// purpose  : Returns the dir by the path
//============================================================================
string SALOMEDSImpl_Tool::GetDirFromPath(const string& thePath) {
#ifdef WIN32
  string separator = "\\";
#else
  string separator = "/";
#endif

  string path;
  if (!thePath.empty()) {
    int pos = thePath.rfind('/');
    if (pos < 0) pos = thePath.rfind('\\');
    if (pos < 0) pos = thePath.rfind('|');
    
    if (pos > 0)
      path = thePath.substr(0, pos+1);
    else
      path = string(".") + separator;

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
vector<string> SALOMEDSImpl_Tool::splitString(const string& theValue, char separator)
{
  vector<string> vs;
  if(theValue[0] == separator && theValue.size() == 1) return vs;
  int pos = theValue.find(separator);
  if(pos < 0) {
    vs.push_back(theValue);
    return vs;
  }
 
  string s = theValue;
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
vector<string> SALOMEDSImpl_Tool::splitStringWithEmpty(const string& theValue, char sep)
{
  vector<string> aResult;
  if(theValue[0] == sep ) aResult.push_back(string());
  int pos = theValue.find(sep);
  if(pos < 0 ) {
    aResult.push_back(theValue);
    return aResult;
  }

  string s = theValue;
  if(s[0] == sep) s = s.substr(1, s.size());
  while((pos = s.find(sep)) >= 0) {
    aResult.push_back(s.substr(0, pos));
    s = s.substr(pos+1, s.size());
  }

  if(!s.empty() && s[0] != sep) aResult.push_back(s);
  if(theValue[theValue.size()-1] == sep) aResult.push_back(string());

  return aResult;
}

//============================================================================
// function : 
// purpose  : The functions returns a list of lists of substrings of initial string 
//            divided by two given separators include empty strings
//============================================================================
vector< vector<string> > SALOMEDSImpl_Tool::splitStringWithEmpty(const string& theValue, char sep1, char sep2)
{
  vector< vector<string> > aResult;
  if(theValue.size() > 0) {
    vector<string> aSections = splitStringWithEmpty( theValue, sep1 );
    for( int i = 0, n = aSections.size(); i < n; i++ )
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
  int status;

 status = gettimeofday( &tval, &tzone );
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
string SALOMEDSImpl_Tool::GetUserName()
{
#ifdef WIN32
  char*  pBuff = new char[UNLEN + 1];
  DWORD  dwSize = UNLEN + 1;
  string retVal;
  ::GetUserNameA( pBuff, &dwSize );
  string theTmpUserName(pBuff,(int)dwSize -1 );
  retVal = theTmpUserName;
  delete [] pBuff;
  return retVal;
#else
 struct passwd *infos;
 infos = getpwuid(getuid()); 
 return string(infos->pw_name);
#endif
}






