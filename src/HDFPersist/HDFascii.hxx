//  File      : HDFascii.hxx
//  Created   : Mon Jun 10 16:24:50 2003
//  Author    : Sergey RUIN

//  Project   : SALOME
//  Module    : HDFPersist
//  Copyright : Open CASCADE


#ifndef __HDFascii_H__
#define __HDFascii_H__

#include <Standard_Macro.hxx>


class Standard_EXPORT HDFascii                                
{
public:
 
  static char* ConvertFromHDFToASCII(const char* thePath, 
				     bool isReplaced = true, 
				     const char* theExtension = NULL);
				     
  static char* ConvertFromASCIIToHDF(const char* thePath);
  
  static bool isASCII(const char* thePath);
};
#endif
