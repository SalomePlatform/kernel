//  SALOME OCCViewer : build OCC Viewer into Salome desktop
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
//  File   : OCCViewer_ViewPort3d.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "OCCViewer_ViewPort3d.h"

#include "QAD.h"
#include "QAD_Settings.h"
#include "QAD_Config.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
#include "SALOME_Selection.h"

// QT Include
#include <qcolordialog.h>

// Open CASCADE Includes
#include <V3d_PerspectiveView.hxx>
#include <V3d_OrthographicView.hxx>

#if !defined WNT
#include <Xw.hxx>
#endif
using namespace std;

/*!
    Constructor
*/
OCCViewer_ViewPort3d::OCCViewer_ViewPort3d( QWidget* parent, const Handle( V3d_Viewer)& viewer,
					    V3d_TypeOfView  viewType, OCCViewer_ViewPort* prevView , 
					    const QRect* magnify ) : 
  OCCViewer_ViewPort( parent )
{
  if ( viewType == V3d_ORTHOGRAPHIC )
    myActiveView = new V3d_OrthographicView( viewer );        
  else
    myActiveView = new V3d_PerspectiveView( viewer );        
  
  myCurScale = 1.0;
  myDegenerated = true;    

  myActiveView->SetDegenerateModeOn();

  if ( prevView ) setOriginalView( prevView, *magnify );
}

/*!
    Destructor
*/
OCCViewer_ViewPort3d::~OCCViewer_ViewPort3d()
{
  myActiveView->Remove();    
}

/*!
    Creates the popup 
*/
void OCCViewer_ViewPort3d::onCreatePopup() 
{
  if ( myPopup ) {	
    QAD_Desktop*     Desktop = (QAD_Desktop*) QAD_Application::getDesktop();
    QAD_Study*   myActiveStudy = Desktop->getActiveStudy();
    SALOME_Selection*      Sel = SALOME_Selection::Selection( myActiveStudy->getSelection() );
    
    QString theContext;
    QString theParent("Viewer");
    QString theObject;
    
    Desktop->definePopup( theContext, theParent, theObject );
    Desktop->createPopup( myPopup, theContext, theParent, theObject);
    Desktop->customPopup( myPopup, theContext, theParent, theObject );

//    if (Sel->IObjectCount() == 0 && myPopup->count()<1) {
    if ( myPopup->count() > 0 )
      myIDs.append ( myPopup->insertSeparator() );	
    int id;
    myIDs.append ( id = myPopup->insertItem (tr ("MEN_VP3D_CHANGEBGR")) );	
    QAD_ASSERT ( myPopup->connectItem ( id, this, SLOT(onChangeBackgroundColor())) );
//    }
  }
}

/*!
    Sets new CASCADE view on viewport.
    Returns the previous view or null if the view hasn't been set.
*/
Handle (V3d_View) OCCViewer_ViewPort3d::setView( const Handle (V3d_View)& view )
{	
  Handle (V3d_View) oldView = myActiveView;
  myActiveView = view;
  if ( hasWindow() ) setWindow();
  return oldView;
}

/*!
    Returns CasCade 3D view
*/
Handle(V3d_View) OCCViewer_ViewPort3d::getView() const
{
  return myActiveView;
}

/*!
    Returns CasCade 3D viewer
*/
Handle (V3d_Viewer) OCCViewer_ViewPort3d::getViewer() const
{
  Handle (V3d_Viewer) viewer;
  if ( !myActiveView.IsNull() )
    viewer = myActiveView->Viewer();
  return viewer;
}

/*!
    Passed the handle of native window of the component to CASCADE view
    (sets window handle for V3d_View class instance).
*/
bool OCCViewer_ViewPort3d::setWindow()
{
  if ( !myActiveView.IsNull() )
    {
      int windowHandle = (int) winId();
      if ( windowHandle == 0 )
	return false;
      
      short hi, lo;
      lo = (short) windowHandle;
      hi = (short) (windowHandle >> 16);
      
      OCCViewer_ViewPort* preView = getOriginalView();
      if ( preView )
	{
	  /* Create new window ( 'magnify' operation ) 
	   */
	  Handle (V3d_View) view;
	  QRect rect = getMagnifyRect();			
	  if ( preView->inherits("OCCViewer_ViewPort3d") )	
	    {	
	      view = ((OCCViewer_ViewPort3d*)preView)->getView();
	    }			
	  if ( !view.IsNull() && !rect.isEmpty() )
	    {
	      OCCViewer_VService::SetMagnify(myActiveView, (int)hi, (int) lo, view,
					     rect.x(), rect.y(), 
					     rect.x() + rect.width(), rect.y() + rect.height(),
					     Xw_WQ_SAMEQUALITY);
	      return true;
	    }
	}
      
      /* CasCade will use our widget as the drawing window */
      OCCViewer_VService::SetWindow( myActiveView, (int) hi, (int) lo, Xw_WQ_SAMEQUALITY);
      return true;
    }
  return false;
}

/*!
  Forces the CASCADE view to resize window   
*/
void OCCViewer_ViewPort3d::windowResize()
{
  QApplication::syncX();
  if ( !myActiveView.IsNull() )
    myActiveView->MustBeResized();
}

/*!
  Called at 'window fit' transformation
*/
void OCCViewer_ViewPort3d::fitWindow(const QRect& rect)
{
  if ( !myActiveView.IsNull() )
    {
      myActiveView->WindowFit( rect.x(), rect.y(), rect.x() + rect.width(), 
			       rect.y() + rect.height());
    }
}

/*!
    Called at 'zoom' transformation
*/
void OCCViewer_ViewPort3d::zoom(int x0, int y0, int x, int y)
{
  if ( !myActiveView.IsNull() )
    myActiveView->Zoom(x0, y0, x, y);
}

/*!
  Centers the viewport 
*/
void OCCViewer_ViewPort3d::setCenter(int x, int y)
{
  if ( !myActiveView.IsNull() )
    {
      myActiveView->Place(x, y, myCurScale);
    }
}

/*!
  Called at 'pan' transformation
*/
void OCCViewer_ViewPort3d::pan(int dx, int dy)
{
  if ( !myActiveView.IsNull() )
    {
      myActiveView->Pan(dx, dy, 1.0);
    }
}

/*!
    Inits 'rotation' transformation
*/
void OCCViewer_ViewPort3d::startRotation(int x0, int y0)
{
  if ( !myActiveView.IsNull() )
    {
      myDegenerated = myActiveView->DegenerateModeIsOn();
      myActiveView->SetDegenerateModeOn();
      myActiveView->StartRotation(x0, y0);
    }
}

/*!
    Rotates the viewport
*/
void OCCViewer_ViewPort3d::rotate(int x, int y)
{
  if ( !myActiveView.IsNull() )
    {
      myActiveView->Rotation(x, y);
    }
}

/*!
    Resets the viewport after 'rotation' 
*/
void OCCViewer_ViewPort3d::endRotation()
{
  if ( !myActiveView.IsNull() ) 
    {
      if ( !myDegenerated )
	myActiveView->SetDegenerateModeOff();
    }
}

/*!
    Called to update the background color
*/
QColor OCCViewer_ViewPort3d::backgroundColor() const
{
  if ( !myActiveView.IsNull() ) 
    {	
      Standard_Real aRed, aGreen, aBlue;
      myActiveView->BackgroundColor(Quantity_TOC_RGB, aRed, aGreen, aBlue);
      int red = (int) (aRed * 255);
      int green = (int) (aGreen * 255);
      int blue = (int) (aBlue * 255);
      return QColor(red, green, blue);
    }
  return OCCViewer_ViewPort::backgroundColor();
}

/*!
    Sets the background color    
*/
void OCCViewer_ViewPort3d::setBackgroundColor( const QColor& color)
{
  if ( !myActiveView.IsNull() ) 
    {		
      myActiveView->SetBackgroundColor( Quantity_TOC_RGB, color.red()/255., 
					color.green()/255., color.blue()/255.);

      /* VSR : PAL5420 ---------------------------------------------------
      QAD_CONFIG->addSetting( "OCCViewer:BackgroundColorRed",   color.red() );
      QAD_CONFIG->addSetting( "OCCViewer:BackgroundColorGreen", color.green() );
      QAD_CONFIG->addSetting( "OCCViewer:BackgroundColorBlue",  color.blue() );
      VSR : PAL5420 --------------------------------------------------- */
      myActiveView->Update();
    }
}

/*!
    Sets the background color with color selection dialog
*/
void OCCViewer_ViewPort3d::onChangeBackgroundColor()
{
  if ( !myActiveView.IsNull() ) 
    {	
      QColor selColor = QColorDialog::getColor ( backgroundColor(), this );	
      if ( selColor.isValid() )
	{	
	  setBackgroundColor( selColor );
	}
    }		
}

/*!
    Repaints the viewport
*/
void OCCViewer_ViewPort3d::paintEvent (QPaintEvent* e )
{
  if ( myActiveView.IsNull() )
    {
      QPainter thePainter (this);
      thePainter.drawText(50, 50, QObject::tr("ERR_VP_NOVIEW"), 7);
    }
  else
    {
      QApplication::syncX();	
      if ( !myHasWindow )
	{
	  myHasWindow = setWindow();
	  if ( myHasWindow ) fitAll();
	}	
      if ( myHasWindow ) 
	{	
	  QRect visibleRect = e->rect();
	  if ( (!myDrawRect) && (!myPaintersRedrawing) )
	    {
	      myActiveView->Redraw ( visibleRect.x(), visibleRect.y(), visibleRect.width(), 
				     visibleRect.height());
	    }
	} 
    } 
  OCCViewer_ViewPort::paintEvent( e );
}

/*!
    Called at 'global panning' transformation    
*/
void OCCViewer_ViewPort3d::activateGlobalPanning()
{
  if ( !myActiveView.IsNull() )
    {
      myCurScale = myActiveView->Scale();
      myActiveView->FitAll(0.01, false);
    }
  OCCViewer_ViewPort::activateGlobalPanning();
}

/*!
    Fits all objects in view
*/
void OCCViewer_ViewPort3d::fitAll( bool withZ )
{
  if ( !myActiveView.IsNull() )
    {
      myActiveView->FitAll(0.01, true);
      if ( withZ ) myActiveView->ZFitAll(0.0);
    }
}

/*!
    Resets the view 
*/
void OCCViewer_ViewPort3d::reset() 
{
  if ( !myActiveView.IsNull() )
    {
      myActiveView->Reset();
    }
}
