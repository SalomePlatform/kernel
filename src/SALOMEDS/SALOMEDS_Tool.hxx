//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_Tool.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_Tool_H__
#define __SALOMEDS_Tool_H__



// IDL headers
#include <SALOMEDS.hh> 

class SALOMEDS_Tool                                
{
public:
 
  // Returns the unique temporary directory, that is defined in SALOME_TMP_DIR if this variable is set
  // otherwise return /tmp/something/ for Unix or c:\something\ for WNT
  static char* GetTmpDir();

 
  // Removes files which are in <theDirectory>, the files for deletion are listed in <theFiles>
  // if <IsDirDeleted> is true <theDirectory> is also deleted if it is empty
  static void RemoveTemporaryFiles(const char* theDirectory,
				   const SALOMEDS::ListOfFileNames& theFiles,
				   const bool IsDirDeleted);

  // Converts files listed in <theFiles> which are in <theFromDirectory> into a byte sequence TMPFile
  static SALOMEDS::TMPFile* PutFilesToStream(const char* theFromDirectory, 
					     const SALOMEDS::ListOfFileNames& theFiles,
					     const int theNamesOnly = 0);

  // Converts a byte sequence <theStream> to files and places them in <theToDirectory>
  static SALOMEDS::ListOfFileNames* PutStreamToFiles(const SALOMEDS::TMPFile& theStream,
                                                     const char* theToDirectory,
						     const int theNamesOnly = 0);

  // Returns the name by the path
  // for an example: if thePath = "/tmp/aaa/doc1.hdf" the function returns "doc1"
  static char* GetNameFromPath(const char* thePath);

  // Returns the directory by the path
  // for an example: if thePath = "/tmp/aaa/doc1.hdf" the function returns "/tmp/aaa"
  static char* GetDirFromPath(const char* thePath);

};
#endif
