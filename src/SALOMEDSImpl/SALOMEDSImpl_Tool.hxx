//  File      : SALOMEDSImpl_Tool.hxx
//  Created   : Mon Oct 21 16:24:50 2002
//  Author    : Sergey RUIN

//  Project   : SALOME
//  Module    : SALOMEDSImpl
//  Copyright : Open CASCADE


#ifndef __SALOMEDSIMPL_TOOL_H__
#define __SALOMEDSIMPL_TOOL_H__

#include <TCollection_AsciiString.hxx>
#include <TDF_Label.hxx>
#include <TColStd_HSequenceOfAsciiString.hxx>

class SALOMEDSImpl_Tool                                
{

public:
 
  // Returns the unique temporary directory, that is defined in SALOME_TMP_DIR if this variable is set
  // otherwise return /tmp/something/ for Unix or c:\something\ for WNT
  static TCollection_AsciiString GetTmpDir();

 
  // Removes files which are in <theDirectory>, the files for deletion are listed in <theFiles>
  // if <IsDirDeleted> is true <theDirectory> is also deleted if it is empty
  static void RemoveTemporaryFiles(const TCollection_AsciiString& theDirectory,
				   const Handle(TColStd_HSequenceOfAsciiString)& theFiles,
				   const bool IsDirDeleted);

  // Returns the name by the path
  // for an example: if thePath = "/tmp/aaa/doc1.hdf" the function returns "doc1"
  static TCollection_AsciiString GetNameFromPath(const TCollection_AsciiString& thePath);

  // Returns the directory by the path
  // for an example: if thePath = "/tmp/aaa/doc1.hdf" the function returns "/tmp/aaa"
  static TCollection_AsciiString GetDirFromPath(const TCollection_AsciiString& thePath);

};
#endif




















