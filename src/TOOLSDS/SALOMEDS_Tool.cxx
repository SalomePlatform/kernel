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

//  File      : SALOMEDS_Tool.cxx
//  Created   : Mon Oct 21 16:24:34 2002
//  Author    : Sergey RUIN
//  Project   : SALOME
//  Module    : SALOMEDS
//
#include "SALOMEDS_Tool.hxx"

#include "utilities.h"
#include "Basics_DirUtils.hxx"

#ifndef WIN32
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h> 
#include <unistd.h>
#else
#include <time.h>
#include <lmcons.h>
#endif

#include <iostream> 
#include <fstream>
#include <stdlib.h>

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

bool Exists(const std::string thePath) 
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
std::string SALOMEDS_Tool::GetTmpDir()
{
  return Kernel_Utils::GetTmpDirByEnv("SALOME_TMP_DIR");
  //Find a temporary directory to store a file

  /*string aTmpDir = "";

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
  if(aDir[aDir.size()-1] != '\\') aDir+='\\';
#else
  if(aDir[aTmpDir.size()-1] != '/') aDir+='/';
#endif


#ifdef WIN32
  CreateDirectory(aDir.c_str(), NULL);
#else
  mkdir(aDir.c_str(), 0x1ff); 
#endif

  return aDir;*/
}

//============================================================================
// function : RemoveTemporaryFiles
// purpose  : Removes files listed in theFileList
//============================================================================
void SALOMEDS_Tool::RemoveTemporaryFiles(const std::string& theDirectory, 
                                         const SALOMEDS::ListOfFileNames& theFiles,
                                         const bool IsDirDeleted)
{
  std::string aDirName = theDirectory;

  int i, aLength = theFiles.length();
  for(i=1; i<=aLength; i++) {
    std::string aFile(aDirName);
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
// function : PutFilesToStream
// purpose  : converts the files from a list 'theFiles' to the stream
//============================================================================
namespace
{
  SALOMEDS::TMPFile* 
  PutFilesToStream(const std::string& theFromDirectory,
                   const SALOMEDS::ListOfFileNames& theFiles,
                   const SALOMEDS::ListOfFileNames& theFileNames,
                   const int theNamesOnly)
  {
    int i, aLength = theFiles.length();
    if(aLength == 0)
      return (new SALOMEDS::TMPFile);
    
    //Get a temporary directory for saved a file
    std::string aTmpDir = theFromDirectory;
    
    long aBufferSize = 0;
    long aCurrentPos;
    
    int aNbFiles = 0;
    int* aFileNameSize= new int[aLength];
    long* aFileSize= new long[aLength];
    
    //Determine the required size of the buffer
    
    for(i=0; i<aLength; i++) {
      
      //Check if the file exists
      
      if (!theNamesOnly) { // mpv 15.01.2003: if only file names must be stroed, then size of files is zero
        std::string aFullPath = aTmpDir + const_cast<char*>(theFiles[i].in());   
        if(!Exists(aFullPath)) continue;
#ifdef WIN32
        std::ifstream aFile(aFullPath.c_str(), std::ios::binary);
#else
        std::ifstream aFile(aFullPath.c_str());
#endif
        aFile.seekg(0, std::ios::end);
        aFileSize[i] = aFile.tellg();
        aBufferSize += aFileSize[i];              //Add a space to store the file
      }
      aFileNameSize[i] = strlen(theFileNames[i])+1;
      aBufferSize += aFileNameSize[i];          //Add a space to store the file name
      aBufferSize += (theNamesOnly)?4:12;       //Add 4 bytes: a length of the file name,
      //    8 bytes: length of the file itself
      aNbFiles++;
    } 
    
    if ( aNbFiles == 0 ) return (new SALOMEDS::TMPFile);
    aBufferSize += 4;      //4 bytes for a number of the files that will be written to the stream;
    unsigned char* aBuffer = new unsigned char[aBufferSize];  
    if(aBuffer == NULL)
      return (new SALOMEDS::TMPFile);
    
    //Initialize 4 bytes of the buffer by 0
    memset(aBuffer, 0, 4); 
    //Copy the number of files that will be written to the stream
    memcpy(aBuffer, &aNbFiles, ((sizeof(int) > 4) ? 4 : sizeof(int))); 
    
    
    aCurrentPos = 4;
    
    for(i=0; i<aLength; i++) {
      std::ifstream *aFile;
      if (!theNamesOnly) { // mpv 15.01.2003: we don't open any file if theNamesOnly = true
        std::string aFullPath = aTmpDir + const_cast<char*>(theFiles[i].in());
        if(!Exists(aFullPath)) continue;
#ifdef WIN32
        aFile = new std::ifstream(aFullPath.c_str(), std::ios::binary);
#else
        aFile = new std::ifstream(aFullPath.c_str());
#endif  
      }
      //Initialize 4 bytes of the buffer by 0
      memset((aBuffer + aCurrentPos), 0, 4); 
      //Copy the length of the file name to the buffer
      memcpy((aBuffer + aCurrentPos), (aFileNameSize + i), ((sizeof(int) > 4) ? 4 : sizeof(int))); 
      aCurrentPos += 4;
      
      //Copy the file name to the buffer
      memcpy((aBuffer + aCurrentPos), theFileNames[i], aFileNameSize[i]);
      aCurrentPos += aFileNameSize[i];
      
      if (!theNamesOnly) { // mpv 15.01.2003: we don't copy file content to the buffer if !theNamesOnly
        //Initialize 8 bytes of the buffer by 0
        memset((aBuffer + aCurrentPos), 0, 8); 
        //Copy the length of the file to the buffer
        memcpy((aBuffer + aCurrentPos), (aFileSize + i), ((sizeof(long) > 8) ? 8 : sizeof(long)));
        aCurrentPos += 8;
        
        aFile->seekg(0, std::ios::beg);
        aFile->read((char *)(aBuffer + aCurrentPos), aFileSize[i]);
        aFile->close();
        delete(aFile);
        aCurrentPos += aFileSize[i];
      }
    }
    
    delete[] aFileNameSize;
    delete[] aFileSize;
    
    
    CORBA::Octet* anOctetBuf =  (CORBA::Octet*)aBuffer;
    
    return (new SALOMEDS::TMPFile(aBufferSize, aBufferSize, anOctetBuf, 1));
  }
  
}


SALOMEDS::TMPFile* 
SALOMEDS_Tool::PutFilesToStream(const std::string& theFromDirectory,
                                const SALOMEDS::ListOfFileNames& theFiles,
                                const int theNamesOnly)
{
  SALOMEDS::ListOfFileNames aFileNames(theFiles);
  return ::PutFilesToStream(theFromDirectory,theFiles,aFileNames,theNamesOnly);
}


SALOMEDS::TMPFile* 
SALOMEDS_Tool::PutFilesToStream(const SALOMEDS::ListOfFileNames& theFiles,
                                const SALOMEDS::ListOfFileNames& theFileNames)
{
  return ::PutFilesToStream("",theFiles,theFileNames,0);
}

//============================================================================
// function : PutStreamToFile
// purpose  : converts the stream "theStream" to the files
//============================================================================
SALOMEDS::ListOfFileNames_var 
SALOMEDS_Tool::PutStreamToFiles(const SALOMEDS::TMPFile& theStream,
                                const std::string& theToDirectory,
                                const int theNamesOnly)
{
  SALOMEDS::ListOfFileNames_var aFiles = new SALOMEDS::ListOfFileNames;

  if(theStream.length() == 0)
    return aFiles;

  //Get a temporary directory for saving a file
  std::string aTmpDir = theToDirectory;

  unsigned char *aBuffer = (unsigned char*)theStream.NP_data();

  if(aBuffer == NULL)
    return aFiles;

  long aFileSize, aCurrentPos = 4;
  int i, aFileNameSize, aNbFiles = 0;

  //Copy the number of files in the stream
  memcpy(&aNbFiles, aBuffer, sizeof(int)); 

  aFiles->length(aNbFiles);

  for(i=0; i<aNbFiles; i++) {

    //Put a length of the file name to aFileNameSize
    memcpy(&aFileNameSize, (aBuffer + aCurrentPos), ((sizeof(int) > 4) ? 4 : sizeof(int))); 
    aCurrentPos += 4;

    char *aFileName = new char[aFileNameSize];
    //Put a file name to aFileName
    memcpy(aFileName, (aBuffer + aCurrentPos), aFileNameSize); 
#ifdef WIN32
    for (int i = 0; i < strlen(aFileName); i++)
    {
      if (aFileName[i] == ':')
	aFileName[i] = '_';
    }
#endif
    aCurrentPos += aFileNameSize;
 
    //Put a length of the file to aFileSize
    if (!theNamesOnly) {
      memcpy(&aFileSize, (aBuffer + aCurrentPos), ((sizeof(long) > 8) ? 8 : sizeof(long)));
      aCurrentPos += 8;    
      
      std::string aFullPath = aTmpDir + aFileName;
#ifdef WIN32
      std::ofstream aFile(aFullPath.c_str(), std::ios::binary);
#else
      std::ofstream aFile(aFullPath.c_str());
#endif
      aFile.write((char *)(aBuffer+aCurrentPos), aFileSize); 
      aFile.close();  
      aCurrentPos += aFileSize;
    }
    aFiles[i] = CORBA::string_dup(aFileName);
    delete[] aFileName;
  }

  return aFiles;
}

//============================================================================
// function : GetNameFromPath
// purpose  : Returns the name by the path
//============================================================================
std::string SALOMEDS_Tool::GetNameFromPath(const std::string& thePath) {
  if (thePath.empty()) return "";
  std::string aPath = thePath;
  bool isFound = false;
  int pos = aPath.rfind('/');
  if(pos > 0) {
    aPath = aPath.substr(pos+1, aPath.size());
    isFound = true;
  }    
  if(!isFound) {
    pos = aPath.rfind('\\'); 
    if(pos > 0) {
      aPath = aPath.substr(pos+1, aPath.size()); 
      isFound = true;
    }  
  }  
  if(!isFound) {  
    pos = aPath.rfind('|');
    if(pos > 0) aPath =  aPath.substr(pos+1, aPath.size()); 
  }

  pos = aPath.rfind('.'); 
  if(pos > 0)  aPath = aPath.substr(0, pos); //Remove extension
    
  return aPath;
}

//============================================================================
// function : GetDirFromPath
// purpose  : Returns the dir by the path
//============================================================================
std::string SALOMEDS_Tool::GetDirFromPath(const std::string& thePath) {
  if (thePath.empty()) return "";

  int pos = thePath.rfind('/');
  std::string path;
  if(pos > 0) {
    path = thePath.substr(0, pos+1);
  }
  if(path.empty()) {
    pos = thePath.rfind('\\');
    if(pos > 0) path = thePath.substr(0, pos+1); 
  }
  if(path.empty()) {
    pos = thePath.rfind('|');
    if(pos > 0) path = thePath.substr(0, pos+1); 
  }
  if(path.empty()) {
    path = thePath+"/";
  }
  
#ifdef WIN32  //Check if the only disk letter is given as path
  if(path.size() == 2 && path[1] == ':') path +='\\';
#endif

  for(int i = 0, len = path.size(); i<len; i++) 
    if(path[i] == '|') path[i] = '/';
  return path;
}

//=======================================================================
// name    : GetFlag
// Purpose : Retrieve specified flaf from "AttributeFlags" attribute
//=======================================================================
bool SALOMEDS_Tool::GetFlag( const int             theFlag,
                             SALOMEDS::Study_var   theStudy,
                             SALOMEDS::SObject_var theObj )
{
  SALOMEDS::GenericAttribute_var anAttr;
  if ( !theObj->_is_nil() && theObj->FindAttribute( anAttr, "AttributeFlags" ) )
  {
    SALOMEDS::AttributeFlags_var aFlags = SALOMEDS::AttributeFlags::_narrow( anAttr );
    return aFlags->Get( theFlag );
  }

  return false;
}

//=======================================================================
// name    : SetFlag
// Purpose : Set/Unset specified flaf from "AttributeFlags" attribute
//=======================================================================
bool SALOMEDS_Tool::SetFlag( const int           theFlag,
                             SALOMEDS::Study_var theStudy,
                             const std::string&  theEntry,
                             const bool          theValue )
{
  SALOMEDS::SObject_var anObj = theStudy->FindObjectID(theEntry.c_str());

  if ( !anObj->_is_nil() )
  {
    SALOMEDS::GenericAttribute_var aGAttr;
    if ( anObj->FindAttribute( aGAttr, "AttributeFlags" ) )
    {
      SALOMEDS::AttributeFlags_var anAttr = SALOMEDS::AttributeFlags::_narrow( aGAttr );
      anAttr->Set( theFlag, theValue );
    }
    else if ( theValue )
    {
      SALOMEDS::StudyBuilder_var aBuilder = theStudy->NewBuilder();
      SALOMEDS::AttributeFlags_var anAttr = SALOMEDS::AttributeFlags::_narrow(
        aBuilder->FindOrCreateAttribute( anObj, "AttributeFlags" ) );
      anAttr->Set( theFlag, theValue );
    }
    return true;
  }

  return false;
}

//=======================================================================
// name    : getAllChildren
// Purpose : Get all children of object.
//           If theObj is null all objects of study are returned
//=======================================================================
void SALOMEDS_Tool::GetAllChildren( SALOMEDS::Study_var               theStudy,
                                    SALOMEDS::SObject_var             theObj,
                                    std::list<SALOMEDS::SObject_var>& theList )
{
  if ( theObj->_is_nil() )
  {
    SALOMEDS::SComponentIterator_var anIter = theStudy->NewComponentIterator();
    for ( ; anIter->More(); anIter->Next() )
    {
      SALOMEDS::SObject_var anObj = SALOMEDS::SObject::_narrow( anIter->Value() );
      if ( !anObj->_is_nil() )
      {
        theList.push_back( anObj );
        GetAllChildren( theStudy, anObj, theList );
      }
    }
  }
  else
  {
    SALOMEDS::ChildIterator_var anIter = theStudy->NewChildIterator( theObj );
    for ( ; anIter->More(); anIter->Next() )
    {
      SALOMEDS::SObject_var anObj = anIter->Value();
      SALOMEDS::SObject_var aRef;
      if ( !anObj->ReferencedObject( aRef ) )
      {
        theList.push_back( anObj );
        GetAllChildren( theStudy, anObj, theList );
      }
    }
  }
}



