//  File      : QAD_Help.cxx
//  Created   : UI team, 03.11.00
//  Descrip   : Shows help in QAD-based applications

//  Modified  : Mon Dec 03 13:40:28 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2001
//  $Header$


#ifndef QAD_HELP_H
#define QAD_HELP_H

#include <qcstring.h>

class QAD_Help
{
public:
  QAD_Help();
  ~QAD_Help();
  
  static void index();
  static void search(); 
  static void contents ();       
  static void context( const QCString& context, const QCString& topic );     
};

#endif


