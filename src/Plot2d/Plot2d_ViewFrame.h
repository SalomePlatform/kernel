//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_ViewFrame.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#ifndef Plot2d_ViewFrame_H
#define Plot2d_ViewFrame_H

#include "QAD_ViewFrame.h"
#include "QAD_Popup.h"
#include "QAD_Action.h"
#include "Plot2d_Curve.h"
#include "Plot2d_CurveContainer.h"
#include "SALOME_Selection.h"
#include <qmainwindow.h>
#include <qmap.h>
#include <qintdict.h>
#include <qwt_plot.h>
#include <qwt_legend.h>

//=================================================================================
// class    : Plot2d_ViewFrame
// purpose  : Plot2d view frame
//=================================================================================
class Plot2d_Plot2d;
typedef QIntDict<Plot2d_Curve> CurveDict;

class QAD_EXPORT Plot2d_ViewFrame : public QAD_ViewFrame, public QAD_PopupClientServer 
{ 
  Q_OBJECT

  enum { NoOpId, FitAllId, FitAreaId, ZoomId, PanId, DumpId, 
	 ModeXLinearId, ModeXLogarithmicId, ModeYLinearId, ModeYLogarithmicId,
	 LegendId, CurvePointsId, CurveLinesId, CurveSplinesId, SettingsId, FitDataId, ChangeBackgroundId };
public:
  /* Construction/destruction */
  Plot2d_ViewFrame( QWidget* parent, const QString& title = "" );
  virtual ~Plot2d_ViewFrame();

public:
  /* inherited */
  ViewType          getTypeView() const { return VIEW_PLOT2D; }
  QWidget*          getViewWidget();
  void              SetSelectionMode( Selection_Mode mode ) {}
  void              onAdjustTrihedron( ) {}

  /* popup management */
  void              setPopupServer( QAD_Application* );
  void              onCreatePopup();

  /*  interactive object management */
  void              highlight( const Handle(SALOME_InteractiveObject)& IObject, 
			       bool highlight, bool update = true ) {}
  void              unHighlightAll() {}
  void              rename( const Handle(SALOME_InteractiveObject)& IObject,
			    QString newName );
  bool              isInViewer( const Handle(SALOME_InteractiveObject)& IObject );
  bool              isVisible( const Handle(SALOME_InteractiveObject)& IObject );

  /*  undo/redo management */
  void              undo( SALOMEDS::Study_var aStudy,
			  const char* StudyFrameEntry ) {}
  void              redo( SALOMEDS::Study_var aStudy,
			  const char* StudyFrameEntry ) {}

  /* interactive objects handling */
  Handle(SALOME_InteractiveObject) FindIObject( const char* Entry );

  /* display */		
  void           Display( const Handle(SALOME_InteractiveObject)& IObject, bool update = true );
  void           DisplayOnly( const Handle(SALOME_InteractiveObject)& IObject );
  void           Erase( const Handle(SALOME_InteractiveObject)& IObject, bool update = true );
  void           DisplayAll();
  void           EraseAll();
  void           Repaint();

  /* Reimplemented from SALOME_View */
  void          Display( const SALOME_Prs2d* );
  void          Erase( const SALOME_Prs2d*, const bool = false );
  SALOME_Prs*   CreatePrs( const char* entry = 0 );
  virtual void  BeforeDisplay( SALOME_Displayer* d );
  virtual void  AfterDisplay ( SALOME_Displayer* d );

  /* event filter */
  bool    eventFilter( QObject* o, QEvent* e );

  /* operations */
  void    setTitle( const QString& title );
  QString getTitle() const { return myTitle; }
  void    displayCurve( Plot2d_Curve* curve, bool update = false );
  void    displayCurves( Plot2d_CurveContainer& curves, bool update = false );
  void    eraseCurve( Plot2d_Curve* curve, bool update = false );
  void    eraseCurves( Plot2d_CurveContainer& curves, bool update = false );
  int     getCurves( QList<Plot2d_Curve>& clist );
  int     hasCurve( Plot2d_Curve* curve );
  Plot2d_Curve* getCurveByIO( const Handle(SALOME_InteractiveObject)& theIObject );
  void    updateCurve( Plot2d_Curve* curve, bool update = false );
  void    fitAll();
  void    fitArea( const QRect& area );

  /* view parameters */
  void    setCurveType( int curveType, bool update = true );
  int     getCurveType() const { return myCurveType; }
  void    showLegend( bool show, bool update = true );
  void    setLegendPos( int pos );
  int     getLegendPos() const { return myLegendPos; }
  void    setMarkerSize( const int size, bool update = true  );
  int     getMarkerSize() const { return myMarkerSize; }
  void    setBackgroundColor( const QColor& color );
  QColor  backgroundColor() const;
  void    setXGrid( bool xMajorEnabled, const int xMajorMax, bool xMinorEnabled, const int xMinorMax, bool update = true );
  void    setYGrid( bool yMajorEnabled, const int yMajorMax, bool yMinorEnabled, const int yMinorMax, bool update = true );
  void    setMainTitle( bool enabled, const QString& title,  bool update = true);
  void    setXTitle( bool enabled, const QString& title, bool update = true );
  QString getXTitle() const { return myXTitle; }
  void    setYTitle( bool enabled, const QString& title, bool update = true );
  QString getYTitle() const { return myYTitle; }
  void    setHorScaleMode( const int mode, bool update = true );
  int     getHorScaleMode() const { return myXMode; }
  void    setVerScaleMode( const int mode, bool update = true );
  int     getVerScaleMode() const { return myYMode; }

  void    incrementalPan ( const int incrX, const int incrY );
  void    incrementalZoom( const int incrX, const int incrY );

protected:
  void    createActions();
  int     testOperation( const QMouseEvent& );
  void    readPreferences();
  void    writePreferences();
  QString getInfo( const QPoint& pnt );
  void    updateTitles();

public slots:
  void    onViewPan(); 
  void    onViewZoom();
  void    onViewFitAll();
  void    onViewFitArea();
  void    onViewGlobalPan(); 
  void    onViewRotate();
  void    onViewReset();     
  void    onViewFront(); 
  void    onViewBack(); 
  void    onViewRight(); 
  void    onViewLeft();     
  void    onViewBottom();
  void    onViewTop();
  void    onViewTrihedron(); 
  void    onHorMode();
  void    onVerMode();
  void    onLegend();
  void    onCurves();
  void    onSettings();
  void    onFitData();
  void    onChangeBackground();

  void    onPanLeft();
  void    onPanRight();
  void    onPanUp();
  void    onPanDown();
  void    onZoomIn();
  void    onZoomOut();

protected slots:
  void    onLegendClicked( long key );
  void    plotMousePressed( const QMouseEvent& );
  void    plotMouseMoved( const QMouseEvent& );
  void    plotMouseReleased( const QMouseEvent& );

private:
  Plot2d_Plot2d*               myPlot;
  ActionMap                    myActions;
  int                          myOperation;
  QPoint                       myPnt;
  CurveDict                    myCurves;

  int                          myCurveType;
  bool                         myShowLegend;
  int                          myLegendPos;
  int                          myMarkerSize;
  QColor                       myBackground;
  QString                      myTitle, myXTitle, myYTitle;
  bool                         myTitleEnabled, myXTitleEnabled, myYTitleEnabled;
  bool                         myXGridMajorEnabled, myYGridMajorEnabled;
  bool                         myXGridMinorEnabled, myYGridMinorEnabled;
  int                          myXGridMaxMajor, myYGridMaxMajor;
  int                          myXGridMaxMinor, myYGridMaxMinor;
  int                          myXMode, myYMode;
};

//=================================================================================
// class    : Plot2d_Plot2d
// purpose  : Plot2d View
//=================================================================================
class Plot2d_Plot2d : public QwtPlot 
{
public:
  Plot2d_Plot2d( QWidget* parent );

  void       replot();
  void       getNextMarker( QwtSymbol::Style& typeMarker, QColor& color, Qt::PenStyle& typeLine );
  QwtLegend* getLegend() { return d_legend; }

protected:
  bool       existMarker( const QwtSymbol::Style typeMarker, const QColor& color, const Qt::PenStyle typeLine );

protected:
  QValueList<QColor> myColors;
};

#endif // Plot2d_ViewFrame_H




