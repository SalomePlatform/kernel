using namespace std;
//  File      : SALOMEDS_Tool.cxx
//  Created   : Mon Oct 21 16:24:34 2002
//  Author    : Sergey RUIN

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE

#include "SALOMEDS_Tool.hxx"

#include "utilities.h"

#include <TCollection_AsciiString.hxx> 
#include <stdio.h>
#include <iostream.h> 
#include <fstream.h>
#include <OSD_Path.hxx>
#include <OSD_File.hxx>
#include <OSD_Directory.hxx>
#include <OSD_Process.hxx>
#include <OSD_Directory.hxx>
#include <OSD_Protection.hxx>
#include <OSD_SingleProtection.hxx>
#include <OSD_FileIterator.hxx>

#include <sys/time.h>
#include <stdlib.h>

//============================================================================
// function : GetTempDir
// purpose  : Return a temp directory to store created files like "/tmp/sub_dir/" 
//============================================================================ 
std::string SALOMEDS_Tool::GetTmpDir()
{
  //Find a temporary directory to store a file

  TCollection_AsciiString aTmpDir;

  char *Tmp_dir = getenv("SALOME_TMP_DIR");
  if(Tmp_dir != NULL) {
    aTmpDir = TCollection_AsciiString(Tmp_dir);
#ifdef WIN32
    if(aTmpDir.Value(aTmpDir.Length()) != '\\') aTmpDir+='\\';
#else
    if(aTmpDir.Value(aTmpDir.Length()) != '/') aTmpDir+='/';
#endif      
  }
  else {
#ifdef WIN32
    aTmpDir = TCollection_AsciiString("C:\\");
#else
    aTmpDir = TCollection_AsciiString("/tmp/");
#endif
  }

  srand((unsigned int)time(NULL));
  int aRND = 999 + (int)(100000.0*rand()/(RAND_MAX+1.0)); //Get a random number to present a name of a sub directory
  TCollection_AsciiString aSubDir(aRND);
  if(aSubDir.Length() <= 1) aSubDir = TCollection_AsciiString("123409876");

  MESSAGE("#### RND "  << aRND);

  aTmpDir += aSubDir; //Get RND sub directory

#ifdef WIN32
  if(aTmpDir.Value(aTmpDir.Length()) != '\\') aTmpDir+='\\';
#else
  if(aTmpDir.Value(aTmpDir.Length()) != '/') aTmpDir+='/';
#endif

  OSD_Path aPath(aTmpDir);
  OSD_Directory aDir(aPath);

  for(aRND = 0; aDir.Exists(); aRND++) {
    aTmpDir.Insert((aTmpDir.Length() - 1), TCollection_AsciiString(aRND));  //Build a unique directory name
    aPath = OSD_Path(aTmpDir);
    aDir = OSD_Directory(aPath);
  }

  MESSAGE("#### TMP" << aTmpDir.ToCString());

  OSD_Protection aProtection(OSD_RW, OSD_RWX, OSD_RX, OSD_RX);
  aDir.Build(aProtection);

  return aTmpDir.ToCString();
}

//============================================================================
// function : RemoveTemporaryFiles
// purpose  : Removes files listed in theFileList
//============================================================================
void SALOMEDS_Tool::RemoveTemporaryFiles(const char* theDirectory, 
					 const SALOMEDS::ListOfFileNames& theFiles,
					 const bool IsDirDeleted)
{
  TCollection_AsciiString aDirName(const_cast<char*>(theDirectory));

  int i, aLength = theFiles.length();
  for(i=0; i<aLength; i++) {
    TCollection_AsciiString aFile(aDirName);
//     aFile += (char*)theFiles[i];
    aFile += (char*)theFiles[i].in();
    OSD_Path anOSDPath(aFile);
    OSD_File anOSDFile(anOSDPath);
    if(!anOSDFile.Exists()) continue;

    OSD_Protection aProtection = anOSDFile.Protection();
    aProtection.SetUser(OSD_RW);
    anOSDFile.SetProtection(aProtection);

    anOSDFile.Remove();
  }

  if(IsDirDeleted) {
    OSD_Path aPath(aDirName);
    OSD_Directory aDir(aPath);
    OSD_FileIterator anIterator(aPath, '*');

    if(aDir.Exists() && !anIterator.More()) aDir.Remove();
  }

}

//============================================================================
// function : PutFilesToStream
// purpose  : converts the files from a list 'theFiles' to the stream
//============================================================================
SALOMEDS::TMPFile* 
SALOMEDS_Tool::PutFilesToStream(const char* theFromDirectory,
				const SALOMEDS::ListOfFileNames& theFiles,
				const int theNamesOnly)
{
  int i, aLength = theFiles.length();
  if(aLength == 0)
//    return NULL;
    return (new SALOMEDS::TMPFile);

  TCollection_AsciiString aTmpDir(const_cast<char*>(theFromDirectory)); //Get a temporary directory for saved a file

  long aBufferSize = 0;
  long aCurrentPos;

  int aNbFiles = 0;
  int* aFileNameSize= new int[aLength];
  long* aFileSize= new long[aLength];

  //Determine the required size of the buffer

  for(i=0; i<aLength; i++) {

    //Check if the file exists
    
    if (!theNamesOnly) { // mpv 15.01.2003: if only file names must be stroed, then size of files is zero
      TCollection_AsciiString aFullPath = aTmpDir + CORBA::string_dup(theFiles[i]);   
      OSD_Path anOSDPath(aFullPath);
      OSD_File anOSDFile(anOSDPath);
      if(!anOSDFile.Exists()) continue;
#ifdef WNT
      ifstream aFile(aFullPath.ToCString(), ios::binary);
#else
      ifstream aFile(aFullPath.ToCString());
#endif
      aFile.seekg(0, ios::end);
      aFileSize[i] = aFile.tellg();
      aBufferSize += aFileSize[i];              //Add a space to store the file
    }
    aFileNameSize[i] = strlen(theFiles[i])+1;
    aBufferSize += aFileNameSize[i];          //Add a space to store the file name
    aBufferSize += (theNamesOnly)?4:12;       //Add 4 bytes: a length of the file name,
                                              //    8 bytes: length of the file itself
    aNbFiles++;
  } 

  aBufferSize += 4;      //4 bytes for a number of the files that will be written to the stream;
  unsigned char* aBuffer = new unsigned char[aBufferSize];  
  if(aBuffer == NULL)
//    return NULL; 
    return (new SALOMEDS::TMPFile);

  //Initialize 4 bytes of the buffer by 0
  memset(aBuffer, 0, 4); 
  //Copy the number of files that will be written to the stream
  memcpy(aBuffer, &aNbFiles, ((sizeof(int) > 4) ? 4 : sizeof(int))); 


  aCurrentPos = 4;

  for(i=0; i<aLength; i++) {
    ifstream *aFile;
    if (!theNamesOnly) { // mpv 15.01.2003: we don't open any file if theNamesOnly = true
      TCollection_AsciiString aFullPath = aTmpDir + CORBA::string_dup(theFiles[i]);
      OSD_Path anOSDPath(aFullPath);
      OSD_File anOSDFile(anOSDPath);
      if(!anOSDFile.Exists()) continue;
#ifdef WNT
      aFile = new ifstream(aFullPath.ToCString(), ios::binary);
#else
      aFile = new ifstream(aFullPath.ToCString());
#endif  
    }
    //Initialize 4 bytes of the buffer by 0
    memset((aBuffer + aCurrentPos), 0, 4); 
    //Copy the length of the file name to the buffer
    memcpy((aBuffer + aCurrentPos), (aFileNameSize + i), ((sizeof(int) > 4) ? 4 : sizeof(int))); 
    aCurrentPos += 4;

    //Copy the file name to the buffer
    memcpy((aBuffer + aCurrentPos), theFiles[i], aFileNameSize[i]);
    aCurrentPos += aFileNameSize[i];
    
    if (!theNamesOnly) { // mpv 15.01.2003: we don't copy file content to the buffer if !theNamesOnly
      //Initialize 8 bytes of the buffer by 0
      memset((aBuffer + aCurrentPos), 0, 8); 
      //Copy the length of the file to the buffer
      memcpy((aBuffer + aCurrentPos), (aFileSize + i), ((sizeof(long) > 8) ? 8 : sizeof(long)));
      aCurrentPos += 8;
      
      aFile->seekg(0, ios::beg);
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

//============================================================================
// function : PutStreamToFile
// purpose  : converts the stream "theStream" to the files
//============================================================================
SALOMEDS::ListOfFileNames_var 
SALOMEDS_Tool::PutStreamToFiles(const SALOMEDS::TMPFile& theStream,
				const char* theToDirectory,
				const int theNamesOnly)
{
  if(theStream.length() == 0) return NULL;
  TCollection_AsciiString aTmpDir(const_cast<char*>(theToDirectory)); //Get a temporary directory for saving a file

  unsigned char *aBuffer = (unsigned char*)theStream.NP_data();

  if(aBuffer == NULL) return NULL;

  long aBufferSize = theStream.length();
  long aFileSize, aCurrentPos = 4;
  int i, aFileNameSize, aNbFiles = 0;

  //Copy the number of files in the stream
  memcpy(&aNbFiles, aBuffer, sizeof(int)); 

  SALOMEDS::ListOfFileNames_var aFiles = new SALOMEDS::ListOfFileNames;
  aFiles->length(aNbFiles);

  for(i=0; i<aNbFiles; i++) {

    //Put a length of the file name to aFileNameSize
    memcpy(&aFileNameSize, (aBuffer + aCurrentPos), ((sizeof(int) > 4) ? 4 : sizeof(int))); 
    aCurrentPos += 4;

    char *aFileName = new char[aFileNameSize];
    //Put a file name to aFileName
    memcpy(aFileName, (aBuffer + aCurrentPos), aFileNameSize); 
    aCurrentPos += aFileNameSize;
 
    //Put a length of the file to aFileSize
    if (!theNamesOnly) {
      memcpy(&aFileSize, (aBuffer + aCurrentPos), ((sizeof(long) > 8) ? 8 : sizeof(long)));
      aCurrentPos += 8;    
      
      TCollection_AsciiString aFullPath = aTmpDir + aFileName;
      ofstream aFile(aFullPath.ToCString());
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
std::string SALOMEDS_Tool::GetNameFromPath(const char* thePath) {
  if (thePath == NULL) return string("");
  OSD_Path aPath = OSD_Path(TCollection_AsciiString((char*)thePath));
  TCollection_AsciiString aNameString(aPath.Name());
  return aNameString.ToCString();
}

//============================================================================
// function : GetDirFromPath
// purpose  : Returns the dir by the path
//============================================================================
std::string SALOMEDS_Tool::GetDirFromPath(const char* thePath) {
  if (thePath == NULL) return string("");
  OSD_Path aPath = OSD_Path(TCollection_AsciiString((char*)thePath));
  TCollection_AsciiString aDirString(aPath.Trek());
  aDirString.ChangeAll('|','/');
  return aDirString.ToCString();
}
