using namespace std;
//  File      : QAD_LeftFrame.cxx
//  Created   : Thu Jun 14 14:53:30 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2001
//  $Header$

#include "QAD_LeftFrame.h"
#include "QAD_ObjectBrowser.h"
#include <qlayout.h>

/*!
  \class QAD_LeftFrame QAD_LeftFrame.h
  \brief Frame window which contains QAD_ObjectBrowser.
*/

/*!
  Constructor
*/
QAD_LeftFrame::QAD_LeftFrame( SALOMEDS::Study_var study, QWidget *parent, const char *name)
     : QWidget( parent, name )
{
  QVBoxLayout* layout = new QVBoxLayout( this );
  layout->setMargin( 0 ); layout->setSpacing( 6 );
  myObjectBrowser = new QAD_ObjectBrowser( study, this );
  layout->addWidget( myObjectBrowser );
}

/*!
  Destructor
*/
QAD_LeftFrame::~QAD_LeftFrame()
{
}

/*!
  Returns reference to Object Browser [ static ]
*/
QAD_ObjectBrowser* QAD_LeftFrame::getObjectBrowser() const
{
  return myObjectBrowser;
}
