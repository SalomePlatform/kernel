using namespace std;
//  File      : VTKViewer_RenderWindow.cxx
//  Created   : Wed Mar 20 11:34:28 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : VTKViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#include "VTKViewer_RenderWindow.h"
#include "utilities.h"
#include "QAD_Settings.h"
#include "QAD_Config.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
#include "QAD_Tools.h"
#include "SALOME_Selection.h"

#include <qcolordialog.h>

#include <stdlib.h>
#include <math.h>
#include <iostream.h>

#include <vtkRenderWindowInteractor.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <qgl.h>

#if QT_VERSION > 300
#include <qcursor.h>
#endif

VTKViewer_RenderWindow::VTKViewer_RenderWindow(QWidget *parent, const char *name) :
  QGLWidget(parent, name)
{
  mInitialized = false ;
  //NRI - 22/02/2002  setFocusPolicy(QWidget::StrongFocus) ;
  //NRI - comment rev 1.6 - setFocus();
}

VTKViewer_RenderWindow::~VTKViewer_RenderWindow() {
  this->ReferenceCount-- ;
}

void VTKViewer_RenderWindow::PrintSelf(ostream& os, vtkIndent indent) {
  this->vtkRenderWindow::PrintSelf(os, indent);

  QGLFormat myFormat = this->format() ;
  os << indent << "qGLVersion: " << qGLVersion() << endl ;
  os << indent << "doubleBuffer: " << myFormat.doubleBuffer() << endl ;
  os << indent << "depth: " << myFormat.depth() << endl ;
  os << indent << "rgba: " << myFormat.rgba() << endl ;
  os << indent << "alpha: " << myFormat.alpha() << endl ;
  os << indent << "accum: " << myFormat.accum() << endl ;
  os << indent << "stencil: " << myFormat.stencil() << endl ;
  os << indent << "stereo: " << myFormat.stereo() << endl ;
  os << indent << "directRendering: " << myFormat.directRendering() << endl ;
}

void VTKViewer_RenderWindow::Start(void) {
  // 
  // Initialize the QGLWidget part of the widget if it has not
  // been initialized so far. 
  //
  if( ! this->mInitialized ) {
    this->WindowInitialize() ;
  }
  this->MakeCurrent() ;
}

// End the rendering process and display the image. 
void VTKViewer_RenderWindow::Frame(void) {
  glFlush() ;
  if( (! this->AbortRender) && // the render is not being aborted
      (! autoBufferSwap() ) && // buffers are not switched automatically
      doubleBuffer() &&        // double buffering is enabled on QGLWidget side
      this->vtkRenderWindow::DoubleBuffer &&    // double buffering is enabled on VTK side
      this->SwapBuffers ) {    // VTK wants us to swap buffers
    QGLWidget::swapBuffers() ;
  }
}
void* VTKViewer_RenderWindow::GetGenericDisplayId() {
  return ((void*)x11Display());
}

void* VTKViewer_RenderWindow::GetGenericWindowId() {
  return ((void*)winId());
}

void* VTKViewer_RenderWindow::GetGenericContext() {
  return ((void*)(this->context())->currentContext());
}

// Initialize the window for rendering.
void VTKViewer_RenderWindow::WindowInitialize(void) {
  if( ! this->mInitialized ) {
    this->initializeGL() ;
    this->MakeCurrent() ;

    vtkDebugMacro(<< " glMatrixMode ModelView\n");
    glMatrixMode( GL_MODELVIEW );

    vtkDebugMacro(<< " zbuffer enabled\n");
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );

    vtkDebugMacro(" texture stuff\n");
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    // initialize blending for transparency
    vtkDebugMacro(<< " blend func stuff\n");
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable(GL_BLEND);
  
    if (this->PointSmoothing)
      {
	glEnable(GL_POINT_SMOOTH);
      }
    else
      {
	glDisable(GL_POINT_SMOOTH);
      }

    if (this->LineSmoothing)
      {
	glEnable(GL_LINE_SMOOTH);
      }
    else
      {
	glDisable(GL_LINE_SMOOTH);
      }

    if (this->PolygonSmoothing)
      {
	glEnable(GL_POLYGON_SMOOTH);
      }
    else
      {
	glDisable(GL_POLYGON_SMOOTH);
      }

    glEnable( GL_NORMALIZE );
    glAlphaFunc(GL_GREATER,0);
  
    this->Mapped = 1;

  }
}

void VTKViewer_RenderWindow::SetFullScreen(int arg) {
  //
  // We do not need to do anything if the FullScreen mode
  // is already set to the specified value. 
  //
  if( this->FullScreen == arg ) return ;

  //
  // :TODO: Fri Apr 21 16:41:06 2000 Pagey
  // This is not implemented in QGLWidget yet. Hence, we
  // will ignore it.
  //
  vtkDebugMacro(<< " QGLWidget::SetFullScreen() not supported by QGLWidget yet.\n") ;
}

void VTKViewer_RenderWindow::WindowRemap(void) {
  //
  // :TODO: Fri Apr 21 16:44:35 2000 Pagey
  // I am not sure why we would ever need to do this under Qt. 
  // Hence, I have not done anything here yet. 
  //
  vtkDebugMacro(<< " QGLWidget::WindowRemap() not supported by QGLWidget yet.\n") ;
}

void VTKViewer_RenderWindow::PrefFullScreen(void) {
  //
  // :TODO: Fri Apr 21 16:46:30 2000 Pagey
  // Since, SetFullScreen() is not supported yet, this is useless.
  //
  vtkDebugMacro(<< " QGLWidget::PrefFullScreen() not supported by QGLWidget yet.\n") ;
}

void VTKViewer_RenderWindow::SetSize(int w, int h) {
  if ((this->Size[0] != w)||(this->Size[1] != h)) {
    this->Modified();
    this->Size[0] = w;
    this->Size[1] = h;
  }
  
  if( this->Interactor ) {
    this->Interactor->SetSize(w, h) ;
  }

  // if we arent mappen then just set the ivars 
  if (!this->Mapped) {
    return;
  }

  glViewport( 0, 0, (GLint)w, (GLint)h ) ;
}

void VTKViewer_RenderWindow::StereoUpdate() {
  //
  // :NOTE: Fri Apr 21 16:55:32 2000 Pagey
  // This routine is taken directly from vtkOpenGLRenderWindow.cxx. 
  // I am not sure what it does. Hope it works. 
  //
  if (this->StereoRender && (!this->StereoStatus))
    {
      switch (this->StereoType) 
	{
	case VTK_STEREO_CRYSTAL_EYES:
	  {
	  }
	  break;
	case VTK_STEREO_RED_BLUE:
	  {
	    this->StereoStatus = 1;
	  }
	}
    }
  else if ((!this->StereoRender) && this->StereoStatus)
    {
      switch (this->StereoType) 
	{
	case VTK_STEREO_CRYSTAL_EYES:
	  {
	    this->StereoStatus = 0;
	  }
	  break;
	case VTK_STEREO_RED_BLUE:
	  {
	    this->StereoStatus = 0;
	  }
	}
    }
}

unsigned char *VTKViewer_RenderWindow::GetPixelData(int x1, int y1, int x2, int y2, int front) {
  //
  // :NOTE: Fri Apr 21 16:58:53 2000 Pagey
  // This routine is taken directly from vtkOpenGLRenderWindow.cxx. 
  // I am not sure what it does. Hope it works. 
  //
  int     y_low, y_hi;
  int     x_low, x_hi;
  unsigned char   *data = NULL;

  // set the current window 
  this->MakeCurrent();

  if (y1 < y2)
    {
      y_low = y1; 
      y_hi  = y2;
    }
  else
    {
      y_low = y2; 
      y_hi  = y1;
    }

  if (x1 < x2)
    {
      x_low = x1; 
      x_hi  = x2;
    }
  else
    {
      x_low = x2; 
      x_hi  = x1;
    }

  if (front)
    {
      glReadBuffer(GL_FRONT);
    }
  else
    {
      glReadBuffer(GL_BACK);
    }

  data = new unsigned char[(x_hi - x_low + 1)*(y_hi - y_low + 1)*3];

#ifdef sparc
  // We need to read the image data one row at a time and convert it
  // from RGBA to RGB to get around a bug in Sun OpenGL 1.1
  long    xloop, yloop;
  unsigned char *buffer;
  unsigned char *p_data = NULL;
  
  buffer = new unsigned char [4*(x_hi - x_low + 1)];
  p_data = data;
  for (yloop = y_low; yloop <= y_hi; yloop++)
    {
      // read in a row of pixels
      glReadPixels(x_low,yloop,(x_hi-x_low+1),1,
		   GL_RGBA, GL_UNSIGNED_BYTE, buffer);
      for (xloop = 0; xloop <= x_hi-x_low; xloop++)
	{
	  *p_data = buffer[xloop*4]; p_data++;
	  *p_data = buffer[xloop*4+1]; p_data++;
	  *p_data = buffer[xloop*4+2]; p_data++;
	}
    }
  
  delete [] buffer;  
#else
  // If the Sun bug is ever fixed, then we could use the following
  // technique which provides a vast speed improvement on the SGI
  
  // Calling pack alignment ensures that we can grab the any size window
  glPixelStorei( GL_PACK_ALIGNMENT, 1 );
  glReadPixels(x_low, y_low, x_hi-x_low+1, y_hi-y_low+1, GL_RGB,
               GL_UNSIGNED_BYTE, data);
#endif
  
  return data;
}

void VTKViewer_RenderWindow::SetPixelData(int x1, int y1, int x2, int y2,
					  unsigned char *data, int front) {

  //
  // :NOTE: Fri Apr 21 17:00:16 2000 Pagey
  // This routine is taken directly from vtkOpenGLRenderWindow.cxx. 
  // I am not sure what it does. Hope it works. 
  //
  int     y_low, y_hi;
  int     x_low, x_hi;

  // set the current window 
  this->MakeCurrent();

  if (front)
    {
      glDrawBuffer(GL_FRONT);
    }
  else
    {
      glDrawBuffer(GL_BACK);
    }

  if (y1 < y2)
    {

      y_low = y1; 
      y_hi  = y2;
    }
  else
    {
      y_low = y2; 
      y_hi  = y1;
    }
  
  if (x1 < x2)
    {
      x_low = x1; 
      x_hi  = x2;
    }
  else
    {
      x_low = x2; 
      x_hi  = x1;
    }

#ifdef sparc
  // We need to read the image data one row at a time and convert it
  // from RGBA to RGB to get around a bug in Sun OpenGL 1.1
  long    xloop, yloop;
  unsigned char *buffer;
  unsigned char *p_data = NULL;
  
  buffer = new unsigned char [4*(x_hi - x_low + 1)];

  // now write the binary info one row at a time
  glDisable(GL_BLEND);
  p_data = data;
  for (yloop = y_low; yloop <= y_hi; yloop++)
    {
      for (xloop = 0; xloop <= x_hi - x_low; xloop++)
	{
	  buffer[xloop*4] = *p_data; p_data++;
	  buffer[xloop*4+1] = *p_data; p_data++;
	  buffer[xloop*4+2] = *p_data; p_data++;
	  buffer[xloop*4+3] = 0xff;
	}
      /* write out a row of pixels */
      glMatrixMode( GL_MODELVIEW );
      glPushMatrix();
      glLoadIdentity();
      glMatrixMode( GL_PROJECTION );
      glPushMatrix();
      glLoadIdentity();
      glRasterPos3f( (2.0 * (GLfloat)(x_low) / this->Size[0] - 1),
		     (2.0 * (GLfloat)(yloop) / this->Size[1] - 1),
		     -1.0 );
      glMatrixMode( GL_PROJECTION );
      glPopMatrix();
      glMatrixMode( GL_MODELVIEW );
      glPopMatrix();

      glDrawPixels((x_hi-x_low+1),1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    }
  glEnable(GL_BLEND);
#else
  // If the Sun bug is ever fixed, then we could use the following
  // technique which provides a vast speed improvement on the SGI
  
  // now write the binary info
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  glRasterPos3f( (2.0 * (GLfloat)(x_low) / this->Size[0] - 1), 
                 (2.0 * (GLfloat)(y_low) / this->Size[1] - 1),
                 -1.0 );
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();

  glPixelStorei( GL_UNPACK_ALIGNMENT, 1);
  glDisable(GL_BLEND);
  glDrawPixels((x_hi-x_low+1), (y_hi - y_low + 1),
               GL_RGB, GL_UNSIGNED_BYTE, data);
  glEnable(GL_BLEND);
#endif
}

float *VTKViewer_RenderWindow::GetRGBAPixelData(int x1, int y1, int x2, int y2, int front)
{
  //
  // :NOTE: Fri Apr 21 17:00:16 2000 Pagey
  // This routine is taken directly from VTKViewer_RenderWindow.cxx. 
  // I am not sure what it does. Hope it works. 
  //
  int     y_low, y_hi;
  int     x_low, x_hi;
  int     width, height;
  float   *data = NULL;

  // set the current window 
  this->MakeCurrent();

  if (y1 < y2)
    {
      y_low = y1; 
      y_hi  = y2;
    }
  else
    {
      y_low = y2; 
      y_hi  = y1;
    }

  if (x1 < x2)
    {
      x_low = x1; 
      x_hi  = x2;
    }
  else
    {
      x_low = x2; 
      x_hi  = x1;
    }

  if (front)
    {
      glReadBuffer(GL_FRONT);
    }
  else
    {
      glReadBuffer(GL_BACK);
    }

  width  = abs(x_hi - x_low) + 1;
  height = abs(y_hi - y_low) + 1;

  data = new float[ (width*height*4) ];

  glReadPixels( x_low, y_low, width, height, GL_RGBA, GL_FLOAT, data);

  return data;
}

void VTKViewer_RenderWindow::SetRGBAPixelData(int x1, int y1, int x2, int y2,
					      float *data, int front, int blend)
{
  //
  // :NOTE: Fri Apr 21 17:00:16 2000 Pagey
  // This routine is taken directly from VTKViewer_RenderWindow.cxx. 
  // I am not sure what it does. Hope it works. 
  //
  int     y_low, y_hi;
  int     x_low, x_hi;
  int     width, height;

  // set the current window 
  this->MakeCurrent();

  if (front)
    {
      glDrawBuffer(GL_FRONT);
    }
  else
    {
      glDrawBuffer(GL_BACK);
    }

  if (y1 < y2)
    {
      y_low = y1; 
      y_hi  = y2;
    }
  else
    {
      y_low = y2; 
      y_hi  = y1;
    }
  
  if (x1 < x2)
    {
      x_low = x1; 
      x_hi  = x2;
    }
  else
    {
      x_low = x2; 
      x_hi  = x1;
    }
  
  width  = abs(x_hi-x_low) + 1;
  height = abs(y_hi-y_low) + 1;

  /* write out a row of pixels */
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  glRasterPos3f( (2.0 * (GLfloat)(x_low) / this->Size[0] - 1), 
                 (2.0 * (GLfloat)(y_low) / this->Size[1] - 1),
		 -1.0 );
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();

  if (!blend)
    {
      glDisable(GL_BLEND);
      glDrawPixels( width, height, GL_RGBA, GL_FLOAT, data);
      glEnable(GL_BLEND);
    }
  else
    {
      glDrawPixels( width, height, GL_RGBA, GL_FLOAT, data);
    }
}

float *VTKViewer_RenderWindow::GetZbufferData( int x1, int y1, int x2, int y2  )
{
  int             y_low, y_hi;
  int             x_low, x_hi;
  int             width, height;
  float           *z_data = NULL;

  // set the current window 
  this->MakeCurrent();

  if (y1 < y2)
    {
      y_low = y1; 
      y_hi  = y2;
    }
  else
    {
      y_low = y2; 
      y_hi  = y1;
    }

  if (x1 < x2)
    {
      x_low = x1; 
      x_hi  = x2;
    }
  else
    {
      x_low = x2; 
      x_hi  = x1;
    }

  width =  abs(x2 - x1)+1;
  height = abs(y2 - y1)+1;

  z_data = new float[width*height];

  glReadPixels( x_low, y_low, 
		width, height,
		GL_DEPTH_COMPONENT, GL_FLOAT,
		z_data );

  return z_data;
}

void VTKViewer_RenderWindow::SetZbufferData( int x1, int y1, int x2, int y2,
					     float *buffer )
{
  int             y_low, y_hi;
  int             x_low, x_hi;
  int             width, height;

  // set the current window 
  this->MakeCurrent();

  if (y1 < y2)
    {
      y_low = y1; 
      y_hi  = y2;
    }
  else
    {
      y_low = y2; 
      y_hi  = y1;
    }

  if (x1 < x2)
    {
      x_low = x1; 
      x_hi  = x2;
    }
  else
    {
      x_low = x2; 
      x_hi  = x1;
    }

  width =  abs(x2 - x1)+1;
  height = abs(y2 - y1)+1;

  glMatrixMode( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  glRasterPos2f( 2.0 * (GLfloat)(x_low) / this->Size[0] - 1, 
                 2.0 * (GLfloat)(y_low) / this->Size[1] - 1);
  glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();

  glDrawPixels( width, height, GL_DEPTH_COMPONENT, GL_FLOAT, buffer);

}

void VTKViewer_RenderWindow::MakeCurrent() {
  if( this->mInitialized ) {
    //const QGLContext* current = this->context()->currentContext();
    //if(!current)
    this->makeCurrent();
  }
}

void VTKViewer_RenderWindow::initializeGL() {
  if ( ! this->mInitialized ) {
    //
    // Construct a format which is similar to the
    // format set by vtkOpenGLRenderWindow.cxx
    //
    QGLFormat myFormat ;
    if( this->vtkRenderWindow::DoubleBuffer ) {
      myFormat.setDoubleBuffer(true) ;
    } else {
      myFormat.setDoubleBuffer(false) ;
    }
    if( this->StereoCapableWindow ) {
      myFormat.setStereo(true) ;
    } else {
      myFormat.setStereo(false) ;
    }
    myFormat.setRgba(true) ;
    myFormat.setDepth(true) ;
    myFormat.setDirectRendering(true) ;

    setFormat(myFormat) ;

    glClearColor(0.0, 0.0, 0.0, 0.0) ;
    this->mInitialized = true ;

    setMouseTracking(true);
  }
}

void VTKViewer_RenderWindow::paintGL() {
  Render() ;
}

void VTKViewer_RenderWindow::resizeGL(int w, int h) {
  SetSize(w, h) ;
}

void VTKViewer_RenderWindow::setContext( QGLContext *context, 
					 const QGLContext *shareContext, 
					 bool deleteOldContex ) {
  mInitialized = false ;
  QGLWidget::setContext(context, shareContext, deleteOldContex) ;
}

void VTKViewer_RenderWindow::mouseMoveEvent( QMouseEvent *event ) {
  emit MouseMove(event) ;
}

void VTKViewer_RenderWindow::mousePressEvent( QMouseEvent *event ) {
  //NRI - comment rev 1.6 - setFocus() ;
  //
  // Emit a ButtonPressed signal for all mouse presses.
  //
  emit ButtonPressed(event) ;
  //
  // Emit a signal for the button which was pressed.
  //
  switch(event->button()) {
  case LeftButton:
    emit LeftButtonPressed(event) ;
    break ;
  case MidButton:
    emit MiddleButtonPressed(event) ;
    break ;
  case RightButton:
    if ( event->state() == Qt::ControlButton ) {
      emit RightButtonPressed(event) ;
    } else {
      QPopupMenu* popup = createPopup();
      if ( popup ) {
	QAD_Tools::checkPopup( popup );
	if ( popup->count()>0 ) {
	  popup->exec( QCursor::pos() );
	}
	destroyPopup();
      }
    }
    break;
  default:
    break ;
  }
  return ;
}


void VTKViewer_RenderWindow::mouseReleaseEvent( QMouseEvent *event ) {
  //
  // Emit a ButtonPressed signal for all mouse releases.
  //
  emit ButtonReleased(event) ;
  //
  // Emit a signal for the the mouse button which was
  // released.
  //
  switch(event->button()) {
  case LeftButton:
    emit LeftButtonReleased(event) ;
    break ;
  case MidButton:
    emit MiddleButtonReleased(event) ;
    break ;
  case RightButton:
    emit RightButtonReleased(event) ;
    break;
  default:
    break ;
  }
  return ;
}

void VTKViewer_RenderWindow::keyPressEvent (QKeyEvent * event) {
  //
  // Let the interactor handle this.
  //
  emit KeyPressed(event) ;
}

/*!
    Creates the popup 
*/
void VTKViewer_RenderWindow::onCreatePopup() 
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


void VTKViewer_RenderWindow::onChangeBackgroundColor()
{
  float red, green, blue;
  float backint[3];

  vtkRendererCollection * theRenderers = GetRenderers();
  theRenderers->InitTraversal();
  vtkRenderer * theRenderer = theRenderers->GetNextItem();
  theRenderer->GetBackground(backint);

  QColor selColor = QColorDialog::getColor ( QColor(int(backint[0]*255), int(backint[1]*255), int(backint[2]*255)), NULL );	
  if ( selColor.isValid() ) {
    theRenderer->SetBackground( selColor.red()/255., selColor.green()/255., selColor.blue()/255. ); 
    QAD_CONFIG->addSetting( "VTKViewer:BackgroundColorRed",   selColor.red() );
    QAD_CONFIG->addSetting( "VTKViewer:BackgroundColorGreen", selColor.green() );
    QAD_CONFIG->addSetting( "VTKViewer:BackgroundColorBlue",  selColor.blue() );
  }
}
