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
#include "QAD_RightFrame.h"
#include "QAD_LeftFrame.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
#include "QAD_ObjectBrowser.h"
#include <qvaluelist.h>
using namespace std;

/*!
    Constructor
*/
QAD_StudyFrame::QAD_StudyFrame(QAD_Study* study, QWidget* parent, const QString& title, 
			       QAD_PyInterp* interp, ViewType typeView) :
  QMainWindow( parent , title,  WStyle_NormalBorder | 
	       WStyle_MinMax | WStyle_SysMenu | WDestructiveClose),
  myStudy(study)
{
  myTypeView = typeView;
  myTitle = title;
  setCaption( myTitle );
  setPalette(QAD_Application::getPalette());

  myEntry = "";
  _interp = interp;

  s1 = new QAD_Splitter( Qt::Horizontal, this);
  s1->setCompressEnabled( true );

  setCentralWidget( s1 );
  myLeftFrm = new QAD_LeftFrame(study->getStudyDocument(), s1 , title );
  myRightFrm = new QAD_RightFrame( s1, title, _interp, myTypeView);

  QValueList<int> sizes;
  sizes.append( (int)(0.30*QAD_Application::getDesktop()->getMainFrame()->width()) );
  sizes.append( (int)(0.50*QAD_Application::getDesktop()->getMainFrame()->width()) );
  s1->setSizes( sizes );

  QAD_ASSERT_DEBUG_ONLY ( parent->inherits("QWorkspaceP") );
  QAD_ASSERT ( QObject::connect( (QWorkspaceP*)parent, SIGNAL(windowActivated(QWidget*)), 
				 this, SLOT(onStudyFrameActivated(QWidget*))) );
}

/*!
    Constructor
*/
QAD_StudyFrame::QAD_StudyFrame(QAD_Study* study, QWidget* parent  ) :
  QMainWindow ( parent ),
  myStudy(study)
{
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
  emit sfStudyFrameClosing(this); 
}

/*!
  Call when  
 */
void QAD_StudyFrame::compressLeft()
{
  s1->compress(myLeftFrm);
}

void QAD_StudyFrame::compressRight()
{
  s1->compress(myRightFrm);  
}

void QAD_StudyFrame::unCompressLeft()
{
  s1->unCompress(myLeftFrm);
}

void QAD_StudyFrame::unCompressRight()
{
  s1->unCompress(myRightFrm);  
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
}

/*!
  Returns the Python interpreter that belongs to this study
*/
QAD_PyInterp* QAD_StudyFrame::get_PyInterp(void)
{
  return _interp;
}
