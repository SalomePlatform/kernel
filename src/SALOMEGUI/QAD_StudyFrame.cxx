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
//  File   : QAD_StudyFrame.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

/*!
  \class QAD_StudyFrame QAD_StudyFrame.h
  \brief Frame window which contains QAD_LeftFrame and QAD_RightFrame.
*/

#include "QAD_StudyFrame.h"
#include "QAD_StudyFrame.h"
#include "QAD_RightFrame.h"
#include "QAD_LeftFrame.h"
#include "QAD_Splitter.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
#include "QAD_ObjectBrowser.h"
#include "QAD_PyInterp.h"

#include <qvaluelist.h>

using namespace std;

/*!
    Constructor
*/
QAD_StudyFrame::QAD_StudyFrame(QAD_Study* theStudy, QWidget* theParent, 
			       const QString& theTitle, ViewType theTypeView,
			       QAD_PyInterp*& theInterp, QMutex* theMutex):
  QMainWindow( theParent , theTitle,  WStyle_NormalBorder | 
	       WStyle_MinMax | WStyle_SysMenu | WDestructiveClose),
  myTitle(theTitle),
  myEntry(""),
  myTypeView(theTypeView),
  myStudy(theStudy), 
  myInterp(theInterp)
{
  setCaption( myTitle );
  setPalette(QAD_Application::getPalette());

  mySplitter = new QAD_Splitter( Qt::Horizontal, this);
  mySplitter->setCompressEnabled( true );

  setCentralWidget(mySplitter);
  myLeftFrm = new QAD_LeftFrame(myStudy->getStudyDocument(), mySplitter, theTitle );
  myRightFrm = new QAD_RightFrame( mySplitter, theTitle, myTypeView, myInterp, theMutex);

  QValueList<int> sizes;
  sizes.append( (int)(0.30*QAD_Application::getDesktop()->getMainFrame()->width()) );
  sizes.append( (int)(0.50*QAD_Application::getDesktop()->getMainFrame()->width()) );
  mySplitter->setSizes( sizes );

  QAD_ASSERT_DEBUG_ONLY ( theParent->inherits("QWorkspaceP") );
  QAD_ASSERT ( QObject::connect( (QWorkspaceP*)theParent, SIGNAL(windowActivated(QWidget*)), 
				 this, SLOT(onStudyFrameActivated(QWidget*))) );
}


/*!
    Destructor
*/
QAD_StudyFrame::~QAD_StudyFrame()
{ 
}

/*!
    Returns the rightframe of this frame	
*/
QAD_RightFrame* QAD_StudyFrame::getRightFrame() const
{
  return myRightFrm;
}

/*!
    Returns the leftframe of this frame	
*/
QAD_LeftFrame* QAD_StudyFrame::getLeftFrame() const
{
  return myLeftFrm;
}

/*!
    Returns 'true' if studyframe is visible
*/
void QAD_StudyFrame::setVisible( bool visible )
{
  if ( visible == isVisible() )
    return;
  
  if ( visible ) show();
  else hide();
}

/*!
   Called when StudyFrame is about to close
*/
void QAD_StudyFrame::closeEvent(QCloseEvent* e)
{
  if ( IsPyLocked() ) {
    e->ignore();
    return;
  }

  emit sfStudyFrameClosing(this); 
}

/*!
  Call when  
 */
void QAD_StudyFrame::compressLeft()
{
  QSplitterPHandle* h = mySplitter->getHandleAfter(myLeftFrm);
  if( h ) 
    h->compressBefore();
}

void QAD_StudyFrame::compressRight()
{
  QSplitterPHandle* h = mySplitter->getHandleAfter(myLeftFrm);
  if( h )
    h->compressAfter();
}

void QAD_StudyFrame::unCompressLeft()
{
  QSplitterPHandle* h = mySplitter->getHandleAfter(myLeftFrm);
  if( h )
    h->unCompressBefore();
}

void QAD_StudyFrame::unCompressRight()
{
  QSplitterPHandle* h = mySplitter->getHandleAfter(myLeftFrm);
  if( h )
    h->unCompressAfter();
}

bool QAD_StudyFrame::isCompressedLeft() const
{
  return mySplitter->isCompressed(myLeftFrm);
}

bool QAD_StudyFrame::isCompressedRight() const
{
  return mySplitter->isCompressed(myRightFrm);
}

/*!
    Returns the title of studyframe
*/
const QString& QAD_StudyFrame::title() const
{
    return myTitle;
}

/*!
    Sets the new title of studyframe
*/
void QAD_StudyFrame::setTitle( const QString& title )
{
  myTitle = title;	
  setCaption( myTitle );
}

/*!
    Returns the entry of studyframe
*/
const QString& QAD_StudyFrame::entry() const
{
  return myEntry;
}

/*!
    Sets the new entru of studyframe
*/
void QAD_StudyFrame::setEntry( const QString& entry )
{
  myEntry = entry;	
}

ViewType QAD_StudyFrame::getTypeView() const
{
  return myTypeView;	
}

/*!
    The slot is called when a studyframe is activated
*/
void QAD_StudyFrame::onStudyFrameActivated ( QWidget* activeWindow )
{
  emit sfStudyFrameActivated( (QAD_StudyFrame*) activeWindow );
  getRightFrame()->getViewFrame()->Activate( getStudy() );
}

/*!
  Returns the Python interpreter that belongs to this study
*/
QAD_PyInterp* QAD_StudyFrame::get_PyInterp(void)
{
  return myInterp;
}
