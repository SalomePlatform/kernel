//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : QAD_Operation.cxx
//  Author : UI team
//  Module : SALOME
//  $Header$

/*!
  \class QAD_Operation QAD_Operation.h
  \brief Base class for OCAF operations in QAD-based application.
*/

#include "QAD.h"
#include "QAD_Operation.h"
#include "QAD_Study.h"
#include "QAD_Application.h"

// QT Include
#include <qapplication.h>
using namespace std;

/*!
    Constructor 
*/
QAD_Operation::QAD_Operation (QAD_Study* doc) :
myDoc ( doc )
{
  /* this class is useless without valid doc */
  QAD_ASSERT_DEBUG_ONLY( myDoc );
}

/*!
    Destructor 
*/
QAD_Operation::~QAD_Operation ()
{
}

/*!
    Returns reference to the document
*/
QAD_Study* QAD_Operation::getDoc() const
{
  return myDoc;
}

/*!
    Returns reference to the application
*/
QAD_Application* QAD_Operation::getApp() const
{
  return myDoc->getApp();
}

/*!
    Checks compatibility with the other launched operations.
    Returns \true if appointed operation can be performed.
    By default returns \false for all operations.
*/
bool QAD_Operation::compatibleWith( QAD_Operation* op ) const
{
  return false; 
}

/*!
    Returns true if operation can be aborted by pressing "Escape" key
    (default behaviour).
*/
bool QAD_Operation::supportCancelByEscape() const
{
  return true;
}

/*!
    Returns true if operation changes document data. In this case it won't be
    launched if document is opened as "read only". Value by default is true.
*/
bool QAD_Operation::changesData() const
{
  return true;
}

/*!
    Starts this operation.
*/
bool QAD_Operation::start()
{    
  /* open transaction */
  if ( !myDoc->onOperationStarted( this ) ) 
    return false;
  
  /* started OK */
  onStartOperation();                     
  return true;
}

/*!
    Called after operation started. Does nothing by default. 
    Override to set a desired behaviour.
*/
void QAD_Operation::onStartOperation()
{
}

/*!
    Ends the operation.
*/
void QAD_Operation::finish()
{    
  QApplication::setOverrideCursor( Qt::waitCursor );    
  
  onFinishOperation();                /* customization */    
  myDoc->onOperationFinished( this ); /* close transaction */
  
  QApplication::restoreOverrideCursor();
}

/*!
    Called after operation finished. Does nothing by default. 
    Override to set a desired behaviour.
*/
void QAD_Operation::onFinishOperation()
{
}

/*!
    Suspends operation process when the document is deactivated.
*/
void QAD_Operation::suspend()
{    
  onSuspendOperation();                /* customization */
  myDoc->onOperationSuspended( this ); /* notification */
}

/*!
    Called after operation suspended. Does nothing by default. 
    Override to set a desired behaviour.
*/
void QAD_Operation::onSuspendOperation()
{
}

/*!
    Resumes operation performing process when the document became active
    (if the operation wasn't aborted earlier).
*/
void QAD_Operation::resume()
{
  onResumeOperation();                /* customization */
  myDoc->onOperationResumed( this );  /* notification */
}

/*!
    Called after operation resumed. Does nothing by default. 
    Override to set a desired behaviour.
*/
void QAD_Operation::onResumeOperation()
{
}

/*!
    Cancels operation performing process without ending.
*/
void QAD_Operation::abort()
{
  QApplication::setOverrideCursor( Qt::waitCursor );    
  
  onAbortOperation();                 /* customization */    
  myDoc->onOperationAborted( this );  /* abort transaction */
  
  QApplication::restoreOverrideCursor();
}

/*!
    Called after operation aborted. Does nothing by default. 
    Override to set a desired behaviour.
*/
void QAD_Operation::onAbortOperation()
{
}
