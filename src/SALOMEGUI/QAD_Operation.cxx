using namespace std;
//  File      : QAD_Operation.cxx
//  Created   : UI team, 22.09.00
//  Descrip   : Base class for operations in QAD-based application
//  Author    : UI team
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2001
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
