using namespace std;
//  File      : QAD_Splitter.cxx
//  Created   : Thu Jun 14 16:36:01 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#include "QAD_Splitter.h"

/*!
  Constructs an horizontal splitter.
*/

QAD_Splitter::QAD_Splitter( QWidget *parent, const char *name )
  :QSplitterP(parent,name)
{
}

/*!
  Constructs splitter with orientation \a o.
*/
QAD_Splitter::QAD_Splitter( Orientation o, QWidget *parent, const char *name )
  :QSplitterP(o, parent, name)
{
}

/*!
  Destructs the splitter.
*/
QAD_Splitter::~QAD_Splitter()
{
}
