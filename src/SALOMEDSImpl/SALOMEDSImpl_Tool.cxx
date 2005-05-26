using namespace std;
//  File      : SALOMEDSImpl_Tool.cxx
//  Created   : Mon Oct 21 16:24:34 2002
//  Author    : Sergey RUIN

//  Project   : SALOME
//  Module    : SALOMEDSImpl
//  Copyright : Open CASCADE

#include "SALOMEDSImpl_Tool.hxx"

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
TCollection_AsciiString SALOMEDSImpl_Tool::GetTmpDir()
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

  OSD_Protection aProtection(OSD_RW, OSD_RWX, OSD_RX, OSD_RX);
  aDir.Build(aProtection);

  return aTmpDir;
}

//============================================================================
// function : RemoveTemporaryFiles
// purpose  : Removes files listed in theFileList
//============================================================================
void SALOMEDSImpl_Tool::RemoveTemporaryFiles(const TCollection_AsciiString& theDirectory, 
					     const Handle(TColStd_HSequenceOfAsciiString)& theFiles,
					     const bool IsDirDeleted)
{
  TCollection_AsciiString aDirName = theDirectory;

  int i, aLength = theFiles->Length();
  for(i=1; i<=aLength; i++) {
    TCollection_AsciiString aFile(aDirName);
    aFile += theFiles->Value(i);
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
// function : GetNameFromPath
// purpose  : Returns the name by the path
//============================================================================
TCollection_AsciiString SALOMEDSImpl_Tool::GetNameFromPath(const TCollection_AsciiString& thePath) {
  if (thePath.IsEmpty()) return "";
  OSD_Path aPath = OSD_Path(thePath);
  TCollection_AsciiString aNameString(aPath.Name());
  return aNameString;
}

//============================================================================
// function : GetDirFromPath
// purpose  : Returns the dir by the path
//============================================================================
TCollection_AsciiString SALOMEDSImpl_Tool::GetDirFromPath(const TCollection_AsciiString& thePath) {
  if (thePath.IsEmpty()) return "";
  OSD_Path aPath = OSD_Path(thePath);
  TCollection_AsciiString aDirString(aPath.Trek());
  aDirString.ChangeAll('|','/');
  return aDirString;
}











