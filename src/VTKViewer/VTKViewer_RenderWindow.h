//  File      : VTKViewer_RenderWindow.h
//  Created   : Wed Mar 20 11:33:30 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : VTKViewer
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef VTKViewer_RenderWindow_h
#define VTKViewer_RenderWindow_h

#include <stdlib.h>
#include <vtkRenderWindow.h>
#include <qgl.h>

#include "QAD_Popup.h"

// ------------------------------------------------------------
// :TRICKY: Fri Apr 21 22:19:27 2000 Pagey
// The Signal/Slot mechanism used by Qt requires that QGLWidget 
// appear as the first class when using multiple inheritance. 
// Hence the order of the two classes QGLWidget and vtkRenderWindow
// matters here. Be careful not to change it by accident. 
// ------------------------------------------------------------
class VTK_EXPORT VTKViewer_RenderWindow : 
public QGLWidget, virtual public vtkRenderWindow, 
  public QAD_PopupClientServer
{
  Q_OBJECT 
    public:
  /* popup management */
  void 	                  onCreatePopup();

  /* ------------------------------------------------------------
   * Methods from vtkRenderWindow class. 
   */
  static VTKViewer_RenderWindow *New(QWidget *parent=0, const char *name=0) {
    return new VTKViewer_RenderWindow(parent,name) ;
  };
  const char *GetClassName() {return "VTKViewer_RenderWindow";};
  void PrintSelf(ostream& os, vtkIndent indent);
  
  virtual void Start(void);
  virtual void Frame(void);
  void WindowInitialize(void);
  virtual void SetFullScreen(int);
  virtual void WindowRemap(void);
  void PrefFullScreen(void);
  void SetSize(int,int);

  virtual void *GetGenericDisplayId();
  virtual void *GetGenericWindowId();
  virtual void *GetGenericContext();

  // stereo rendering stuff
  virtual void StereoUpdate();

  // Description:
  // Set/Get the pixel data of an image, transmitted as RGBRGB... 
  virtual unsigned char *GetPixelData(int x,int y,int x2,int y2,int front);
  virtual void SetPixelData(int x,int y,int x2,int y2,unsigned char *,int front);

  // Description:
  // Set/Get the pixel data of an image, transmitted as RGBARGBA... 
  virtual float *GetRGBAPixelData(int x,int y,int x2,int y2,int front);
  virtual void SetRGBAPixelData(int x,int y,int x2,int y2,float *,int front,
                                int blend=0);

  // Description:
  // Set/Get the zbuffer data from an image
  virtual float *GetZbufferData( int x1, int y1, int x2, int y2 );
  virtual void SetZbufferData( int x1, int y1, int x2, int y2, float *buffer );
  virtual void MakeCurrent();

  /* ------------------------------------------------------------
   * Methods from QGLWidget class. 
   */
  virtual void setContext( QGLContext *context, 
			   const QGLContext *shareContext=0, 
			   bool deleteOldContex=TRUE ) ;

  protected slots:
    void onChangeBackgroundColor();

  //NRIprotected:
 public:
  bool mInitialized ;
  VTKViewer_RenderWindow(QWidget *parent, const char *name);
  ~VTKViewer_RenderWindow() ;
  VTKViewer_RenderWindow(const VTKViewer_RenderWindow&) { } 
  void operator=(const VTKViewer_RenderWindow&) { } 

  /* ------------------------------------------------------------
   * Methods from QGLWidget class. 
   */
  virtual void initializeGL() ;
  virtual void paintGL() ;
  virtual void resizeGL(int w, int h) ;
  virtual void mouseMoveEvent( QMouseEvent * ) ;
  virtual void mousePressEvent( QMouseEvent * ) ;
  virtual void mouseReleaseEvent( QMouseEvent * ) ;
  virtual void keyPressEvent( QKeyEvent * ) ;
  //
  // Every focus-in and focus-out event results in a repaint 
  // through the default implementations of focusInEvent and
  // focusOutEvent. This results in a flicker in the vtkQGLRenderWindow
  // ever time the cursor moves in or out of the widget. We can 
  // disble this by calling repaint(FALSE) in the implementation
  // of these methods. 
  //
/*    virtual void focusInEvent( QFocusEvent * ) { repaint(FALSE) ; } */
/*    virtual void focusOutEvent( QFocusEvent * ) { repaint(FALSE) ; } */
  //
  // If this widget is not the top level widget, it does not 
  // get focus until it receives its first mouse click. By 
  // overloading the enterEvent and leaveEvent methods, we 
  // give keyboard focus to the widget when the mouse enters
  // and remove the focus once the mouse leaves. 
  //
 /*   virtual void enterEvent( QEvent * ) { setFocus() ; } */
/*    virtual void leaveEvent( QEvent * ) { clearFocus() ; } */

  /* ------------------------------------------------------------
   * A few signals which will help us write an interactor for this
   * render window. 
   */
 signals:
  //
  // Mouse moved. 
  //
  void MouseMove(const QMouseEvent *event) ;
  //
  // Left button pressed/released at the specified location.
  //
  void LeftButtonPressed(const QMouseEvent *event) ;
  void LeftButtonReleased(const QMouseEvent *event) ;
  //
  // Middle button pressed/released at the specified location.
  //
  void MiddleButtonPressed(const QMouseEvent *event) ;
  void MiddleButtonReleased(const QMouseEvent *event) ;
  //
  // Right button pressed/released at the specified location.
  //
  void RightButtonPressed(const QMouseEvent *event) ;
  void RightButtonReleased(const QMouseEvent *event) ;
  //
  // Any mouse button pressed.
  //
  void ButtonPressed(const QMouseEvent *event) ;
  void ButtonReleased(const QMouseEvent *event) ;
  //
  // Any key pressed.
  //
  void KeyPressed(QKeyEvent *event) ;
};

#endif
