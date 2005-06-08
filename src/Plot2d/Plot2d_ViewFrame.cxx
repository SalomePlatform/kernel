//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_ViewFrame.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "Plot2d_ViewFrame.h"
#include "Plot2d_SetupViewDlg.h"
#include "Plot2d_Prs.h"

#include "QAD_Desktop.h"
#include "QAD_ResourceMgr.h"
#include "QAD_FileDlg.h"
#include "QAD_Tools.h"
#include "QAD_MessageBox.h"
#include "QAD_Config.h"
#include "SALOME_Selection.h"
#include "Plot2d_CurveContainer.h"
#include "Plot2d_Curve.h"
#include "Plot2d_FitDataDlg.h"
#include "utilities.h"
#include "qapplication.h"
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qcursor.h>
#include <qcolordialog.h>
#include <qwt_math.h>
#include <qwt_plot_canvas.h>
#include <stdlib.h>
#include "utilities.h"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
using namespace std;

#define DEFAULT_LINE_WIDTH     0     // (default) line width
#define DEFAULT_MARKER_SIZE    9     // default marker size
#define MIN_RECT_SIZE          11    // min sensibility area size

/* XPM */
const char* imageZoomCursor[] = { 
"32 32 3 1",
". c None",
"a c #000000",
"# c #ffffff",
"................................",
"................................",
".#######........................",
"..aaaaaaa.......................",
"................................",
".............#####..............",
"...........##.aaaa##............",
"..........#.aa.....a#...........",
".........#.a.........#..........",
".........#a..........#a.........",
"........#.a...........#.........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
"........#a............#a........",
".........#...........#.a........",
".........#a..........#a.........",
".........##.........#.a.........",
"........#####.....##.a..........",
".......###aaa#####.aa...........",
"......###aa...aaaaa.......#.....",
".....###aa................#a....",
"....###aa.................#a....",
"...###aa...............#######..",
"....#aa.................aa#aaaa.",
".....a....................#a....",
"..........................#a....",
"...........................a....",
"................................",
"................................",
"................................",
"................................"};


//=================================================================================
// Plot2d_ViewFrame implementation
//=================================================================================

/*!
  Constructor
*/
Plot2d_ViewFrame::Plot2d_ViewFrame( QWidget* parent, const QString& title )
     : QAD_ViewFrame( parent, title ),
       myOperation( NoOpId ), 
       myCurveType( 1 ), 
       myShowLegend( true ), myLegendPos( 1 ),
       myMarkerSize( DEFAULT_MARKER_SIZE ),
       myTitle( "" ), myXTitle( "" ), myYTitle( "" ),
       myBackground( white ),
       myTitleEnabled( true ), myXTitleEnabled( true ), myYTitleEnabled( true ),
       myXGridMajorEnabled( true ), myYGridMajorEnabled( true ), 
       myXGridMinorEnabled( false ), myYGridMinorEnabled( false ),
       myXGridMaxMajor( 8 ), myYGridMaxMajor( 8 ), myXGridMaxMinor( 5 ), myYGridMaxMinor( 5 ),
       myXMode( 0 ), myYMode( 0 )
       
{
  myCurves.setAutoDelete( true );
  /* Plot 2d View */
  myPlot = new Plot2d_Plot2d( this );
  setCentralWidget( myPlot );

  createActions();

  connect( myPlot, SIGNAL( plotMouseMoved( const QMouseEvent& ) ),
	   this,   SLOT( plotMouseMoved( const QMouseEvent& ) ) );
  connect( myPlot, SIGNAL( plotMousePressed( const QMouseEvent& ) ),
	   this,   SLOT( plotMousePressed( const QMouseEvent& ) ) );
  connect( myPlot, SIGNAL( plotMouseReleased( const QMouseEvent& ) ),
	   this,   SLOT( plotMouseReleased( const QMouseEvent& ) ) );
  connect( myPlot, SIGNAL( legendClicked( long ) ),
	   this,   SLOT( onLegendClicked( long ) ) );

  /* Initial Setup - get from the preferences */
  readPreferences();

  myPlot->setMargin( 5 );
  setCurveType( myCurveType, false );
  setXGrid( myXGridMajorEnabled, myXGridMaxMajor, myXGridMinorEnabled, myXGridMaxMinor, false );
  setYGrid( myYGridMajorEnabled, myYGridMaxMajor, myYGridMinorEnabled, myYGridMaxMinor, false );
  setMainTitle( myTitleEnabled, myTitle, false );
  setXTitle( myXTitleEnabled, myXTitle, false );
  setYTitle( myYTitleEnabled, myYTitle, false );
  setHorScaleMode( myXMode, false );
  setVerScaleMode( myYMode, false );
  setBackgroundColor( myBackground );
  setLegendPos( myLegendPos );
  showLegend( myShowLegend, false );
  myPlot->replot();

  if ( parent ) {
    resize( (int)(0.8 * parent->width()), (int)(0.8 * parent->height()) );
  }
}
/*!
  Destructor
*/
Plot2d_ViewFrame::~Plot2d_ViewFrame()
{
  myActions.clear();
  qApp->removeEventFilter( this );
}
/*!
  Creates popup menu actions
*/
void Plot2d_ViewFrame::createActions()
{
  QAD_ResourceMgr* rmgr = QAD_Desktop::getResourceManager();
  /* Linear/logarithmic mode */
  // Horizontal axis
  QActionPGroup* modeHorGrp = new QActionPGroup( this );
  modeHorGrp->setExclusive( TRUE );
  QActionP* linearXAction = new QActionP ( tr( "TOT_PLOT2D_MODE_LINEAR_HOR"),
				         rmgr->loadPixmap( "SALOMEGUI", tr("ICON_PLOT2D_MODE_LINEAR_HOR") ) ,
				         tr( "MEN_PLOT2D_MODE_LINEAR_HOR" ), 0, modeHorGrp );
  linearXAction->setStatusTip ( tr( "PRP_PLOT2D_MODE_LINEAR_HOR" ) );
  linearXAction->setToggleAction( true );
  myActions.insert( ModeXLinearId, linearXAction );
  QActionP* logXAction = new QActionP ( tr( "TOT_PLOT2D_MODE_LOGARITHMIC_HOR"),
				      rmgr->loadPixmap( "SALOMEGUI", tr("ICON_PLOT2D_MODE_LOGARITHMIC_HOR") ) ,
				      tr( "MEN_PLOT2D_MODE_LOGARITHMIC_HOR" ), 0, modeHorGrp );
  logXAction->setStatusTip ( tr( "PRP_PLOT2D_MODE_LOGARITHMIC_HOR" ) );
  logXAction->setToggleAction( true );
  myActions.insert( ModeXLogarithmicId, logXAction );
#if (QT_VERSION >= 0x030303) // mpv: do not use patches for qt version >= 3.3.3
  connect( modeHorGrp, SIGNAL( selected( QAction * ) ), this, SLOT( onHorMode() ) );
#else
  connect( modeHorGrp, SIGNAL( selected( QActionP * ) ), this, SLOT( onHorMode() ) );
#endif
  // Vertical axis
  QActionPGroup* modeVerGrp = new QActionPGroup( this );
  modeVerGrp->setExclusive( TRUE );
  QActionP* linearYAction = new QActionP ( tr( "TOT_PLOT2D_MODE_LINEAR_VER"),
				         rmgr->loadPixmap( "SALOMEGUI", tr("ICON_PLOT2D_MODE_LINEAR_VER") ) ,
				         tr( "MEN_PLOT2D_MODE_LINEAR_VER" ), 0, modeVerGrp );
  linearYAction->setStatusTip ( tr( "PRP_PLOT2D_MODE_LINEAR_VER" ) );
  linearYAction->setToggleAction( true );
  myActions.insert( ModeYLinearId, linearYAction );
  QActionP* logYAction = new QActionP ( tr( "TOT_PLOT2D_MODE_LOGARITHMIC_VER"),
				      rmgr->loadPixmap( "SALOMEGUI", tr("ICON_PLOT2D_MODE_LOGARITHMIC_VER") ) ,
				      tr( "MEN_PLOT2D_MODE_LOGARITHMIC_VER" ), 0, modeVerGrp );
  logYAction->setStatusTip ( tr( "PRP_PLOT2D_MODE_LOGARITHMIC_VER" ) );
  logYAction->setToggleAction( true );
  myActions.insert( ModeYLogarithmicId, logYAction );
#if (QT_VERSION >= 0x030303) // mpv: do not use patches for qt version >= 3.3.3
  connect( modeVerGrp, SIGNAL( selected( QAction* ) ), this, SLOT( onVerMode() ) );
#else
  connect( modeVerGrp, SIGNAL( selected( QActionP* ) ), this, SLOT( onVerMode() ) );
#endif
  /* Legend */
  QActionP* legendAction = new QActionP ( tr( "TOT_PLOT2D_SHOW_LEGEND"),
				        rmgr->loadPixmap( "SALOMEGUI", tr("ICON_PLOT2D_SHOW_LEGEND") ) ,
				        tr( "MEN_PLOT2D_SHOW_LEGEND" ), 0, this );
  legendAction->setStatusTip ( tr( "PRP_PLOT2D_SHOW_LEGEND" ) );
  legendAction->setToggleAction( true );
  myActions.insert( LegendId, legendAction );
  connect( legendAction, SIGNAL( activated() ), this, SLOT( onLegend() ) );

  /* Curve type */
  QActionPGroup* curveGrp = new QActionPGroup( this );
  curveGrp->setExclusive( TRUE );
  QActionP* pointsAction = new QActionP ( tr( "TOT_PLOT2D_CURVES_POINTS"),
				        rmgr->loadPixmap( "SALOMEGUI", tr("ICON_PLOT2D_CURVES_POINTS") ) ,
				        tr( "MEN_PLOT2D_CURVES_POINTS" ), 0, curveGrp );
  pointsAction->setStatusTip ( tr( "PRP_PLOT2D_CURVES_POINTS" ) );
  pointsAction->setToggleAction( true );
  myActions.insert( CurvePointsId, pointsAction );
  QActionP* linesAction = new QActionP ( tr( "TOT_PLOT2D_CURVES_LINES"),
				       rmgr->loadPixmap( "SALOMEGUI", tr("ICON_PLOT2D_CURVES_LINES") ) ,
				       tr( "MEN_PLOT2D_CURVES_LINES" ), 0, curveGrp );
  linesAction->setStatusTip ( tr( "PRP_PLOT2D_CURVES_LINES" ) );
  linesAction->setToggleAction( true );
  myActions.insert( CurveLinesId, linesAction );
  QActionP* splinesAction = new QActionP ( tr( "TOT_PLOT2D_CURVES_SPLINES"),
				         rmgr->loadPixmap( "SALOMEGUI", tr("ICON_PLOT2D_CURVES_SPLINES") ) ,
				         tr( "MEN_PLOT2D_CURVES_SPLINES" ), 0, curveGrp );
  splinesAction->setStatusTip ( tr( "PRP_PLOT2D_CURVES_SPLINES" ) );
  splinesAction->setToggleAction( true );
  myActions.insert( CurveSplinesId, splinesAction );
#if (QT_VERSION >= 0x030303) // mpv: do not use patches for qt version >= 3.3.3
  connect( curveGrp, SIGNAL( selected( QAction* ) ), this, SLOT( onCurves() ) );
#else
  connect( curveGrp, SIGNAL( selected( QActionP* ) ), this, SLOT( onCurves() ) );
#endif
  // Settings
  QActionP* settingsAction = new QActionP ( tr( "TOT_PLOT2D_SETTINGS"),
					  rmgr->loadPixmap( "SALOMEGUI", tr("ICON_PLOT2D_SETTINGS") ) ,
					  tr( "MEN_PLOT2D_SETTINGS" ), 0, this );
  settingsAction->setStatusTip ( tr( "PRP_PLOT2D_SETTINGS" ) );
  myActions.insert( SettingsId, settingsAction );
  connect( settingsAction, SIGNAL( activated() ), this, SLOT( onSettings() ) );

  // Fit Data
  QActionP* fitDataAction = new QActionP ( tr( "TOT_PLOT2D_FITDATA"),
					 rmgr->loadPixmap( "SALOMEGUI", tr("ICON_PLOT2D_FITDATA") ) ,
					 tr( "MEN_PLOT2D_FITDATA" ), 0, this );
  fitDataAction->setStatusTip ( tr( "PRP_PLOT2D_FITDATA" ) );
  myActions.insert( FitDataId, fitDataAction );
  connect( fitDataAction, SIGNAL( activated() ), this, SLOT( onFitData() ) );

  // Change background
  QActionP* changeBGAction = new QActionP ( tr( "TOT_PLOT2D_CHANGE_BACKGROUND"),
					    tr( "MEN_PLOT2D_CHANGE_BACKGROUND" ), 0, this );
  fitDataAction->setStatusTip ( tr( "PRP_PLOT2D_CHANGE_BACKGROUND" ) );
  myActions.insert( ChangeBackgroundId, changeBGAction );
  connect( changeBGAction, SIGNAL( activated() ), this, SLOT( onChangeBackground() ) );

  // Dump View
  QActionP* dumpViewAction = new QActionP ( tr( "MEN_VP3D_DUMPVIEW"),
					    tr( "MEN_VP3D_DUMPVIEW" ), 0, this );
  dumpViewAction->setStatusTip ( tr( "MEN_VP3D_DUMPVIEW" ) );
  myActions.insert( DumpViewId, dumpViewAction );
  connect( dumpViewAction, SIGNAL( activated() ), this, SLOT( onProcessViewDump() ) );
}
/*!
  Gets window's central widget
*/
QWidget* Plot2d_ViewFrame::getViewWidget()
{
  return (QWidget*)myPlot;
}
/* Popup management : sets Popup server */
void Plot2d_ViewFrame::setPopupServer( QAD_Application* App )
{
//  QAD_PopupClientServer::setPopupServer( (QAD_PopupServer*)App );
}
/*!
  Popup menu creation
*/
void Plot2d_ViewFrame::onCreatePopup()
{
  if ( myPopup ) {
    myPopup->clear();
    // scaling
    QPopupMenu* scalingPopup = new QPopupMenu( myPopup );
    myActions[ ModeXLinearId ]->addTo( scalingPopup );
    myActions[ ModeXLogarithmicId ]->addTo( scalingPopup );
    scalingPopup->insertSeparator();
    myActions[ ModeYLinearId ]->addTo( scalingPopup );
    myActions[ ModeYLogarithmicId ]->addTo( scalingPopup );
    myActions[ FitDataId ]->addTo( myPopup );
    myPopup->insertItem( tr( "SCALING_POPUP" ), scalingPopup );
    // curve type
    QPopupMenu* curTypePopup = new QPopupMenu( myPopup );
    myActions[ CurvePointsId ]->addTo( curTypePopup );
    myActions[ CurveLinesId ]->addTo( curTypePopup );
    myActions[ CurveSplinesId ]->addTo( curTypePopup );
    myPopup->insertItem( tr( "CURVE_TYPE_POPUP" ), curTypePopup );
    // legend
    myPopup->insertSeparator();
    myActions[ LegendId ]->addTo( myPopup );
    // settings
    myPopup->insertSeparator();
    myActions[ SettingsId ]->addTo( myPopup );
    // Change background
    myPopup->insertSeparator();
    myActions[ ChangeBackgroundId ]->addTo( myPopup );
    myActions[ DumpViewId ]->addTo( myPopup );
  }
}
/*!
  Renames curve if it is found
*/
void Plot2d_ViewFrame::rename( const Handle(SALOME_InteractiveObject)& IObject, QString newName ) 
{
  Plot2d_Curve* curve = getCurveByIO( IObject );
  if ( curve ) {
    curve->setVerTitle( newName );
    int key = hasCurve( curve );
    if ( key ) {
      myPlot->setCurveTitle( key, newName );
//      int legendIndex = myPlot->getLegend()->findFirstKey( key );
//      if ( legendIndex != myPlot->getLegend()->itemCnt() )
//	myPlot->getLegend()->setText( legendIndex, aSymbol );
    }
  }
  updateTitles();
}
/*!
  Returns true if interactive object is presented in the viewer
*/
bool Plot2d_ViewFrame::isInViewer( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  if( getCurveByIO( IObject ) != NULL )
    return 1;
  else{
    if(!IObject.IsNull()){
      QIntDictIterator<Plot2d_Curve> it(myCurves);
      for(; it.current();++it){
	if(it.current()->hasIO() && it.current()->getTableIO()->isSame(IObject))
	  return 1;
      }}
  }
  return 0;
}
/*!
  Returns true if interactive object is presented in the viewer and displayed
*/
bool Plot2d_ViewFrame::isVisible( const Handle(SALOME_InteractiveObject)& IObject ) 
{
  Plot2d_Curve* curve = getCurveByIO( IObject );
  if ( curve ) {
    int key = hasCurve( curve );
    if ( key )
      return myPlot->curve( key )->enabled();
  }
  return false;
}
/*!
  Return interactive obeject if is presented in the viewer
*/
Handle(SALOME_InteractiveObject) Plot2d_ViewFrame::FindIObject( const char* Entry )
{
  Handle(SALOME_InteractiveObject) o;
  QIntDictIterator<Plot2d_Curve> it( myCurves );
  for ( ; it.current(); ++it ) {
    if ( it.current()->hasIO() && !strcmp( it.current()->getIO()->getEntry(), Entry ) ) {
      o = it.current()->getIO();
      break;
    }
  }
  return o;
}
/*!
  Actually this method just re-displays curves which refers to the <IObject>
*/
void Plot2d_ViewFrame::Display( const Handle(SALOME_InteractiveObject)& IObject, bool update )
{
  Plot2d_Curve* curve = getCurveByIO( IObject );
  if ( curve )
    updateCurve( curve, update );
}
/*!
  Actually this method just erases all curves which don't refer to <IOBject> 
  and re-displays curve which is of <IObject>
*/
void Plot2d_ViewFrame::DisplayOnly( const Handle(SALOME_InteractiveObject)& IObject )
{
  Plot2d_Curve* curve = getCurveByIO( IObject );
  QList<Plot2d_Curve> clist;
  getCurves( clist );
  for ( int i = 0; i < clist.count(); i++ ) {
    if ( clist.at( i ) != curve )
      eraseCurve( curve );
    else
      updateCurve( curve, false );
  }
  myPlot->replot();
}
/*!
  Removes from the viewer the curves which refer to <IObject>
*/
void Plot2d_ViewFrame::Erase( const Handle(SALOME_InteractiveObject)& IObject, bool update )
{
  Plot2d_Curve* curve = getCurveByIO( IObject );
  if ( curve )
    eraseCurve( curve, update );
  // it can be table or container object selected
  QAD_Study* activeStudy = QAD_Application::getDesktop()->getActiveStudy();
  SALOMEDS::SObject_var aSO = activeStudy->getStudyDocument()->FindObjectID(IObject->getEntry());
  if ( !aSO->_is_nil() ) {
    SALOMEDS::ChildIterator_var aIter = activeStudy->getStudyDocument()->NewChildIterator( aSO );
    for ( ; aIter->More(); aIter->Next() ) {
      SALOMEDS::SObject_var aChildSO = aIter->Value();
      SALOMEDS::SObject_var refSO;
      if ( aChildSO->ReferencedObject( refSO ) && !refSO->_is_nil() )
	aChildSO = refSO;
      curve = getCurveByIO( new SALOME_InteractiveObject( aChildSO->GetID(), "", "" ) );
      if ( curve )
	eraseCurve( curve, update );
    }
  }
}
/*!
  Actually this method just re-displays all curves which are presented in the viewer
*/
void Plot2d_ViewFrame::DisplayAll()
{
  QList<Plot2d_Curve> clist;
  getCurves( clist );
  for ( int i = 0; i < clist.count(); i++ ) {
    updateCurve( clist.at( i ), false );
   }
  myPlot->replot();
}
/*!
   Removes all curves from the view
*/
void Plot2d_ViewFrame::EraseAll() 
{
  myPlot->clear();
  myCurves.clear();
  myPlot->replot();
}
/*!
  Redraws viewframe contents
*/
void Plot2d_ViewFrame::Repaint()
{
  myPlot->replot();
}
/*!
  Event filter
*/
bool Plot2d_ViewFrame::eventFilter( QObject* o, QEvent* e )
{
  if ( ( e->type() == QEvent::MouseButtonPress || e->type() == QEvent::KeyPress ) && o != myPlot->canvas() ) {
    myOperation = NoOpId;
    qApp->removeEventFilter( this );
  }
  return QMainWindow::eventFilter( o, e );
}
/*!
  Sets title
*/
void Plot2d_ViewFrame::setTitle( const QString& title )
{
  setMainTitle( myTitleEnabled, title, true );
}
/*!
  Reads Plot2d view settings from the preferences
*/
void Plot2d_ViewFrame::readPreferences()
{
  if ( QAD_CONFIG->hasSetting( "Plot2d:CurveType" ) ) {                                          // curve type
    myCurveType = QAD_CONFIG->getSetting( "Plot2d:CurveType" ).toInt();                             
    if ( myCurveType < 0 || myCurveType > 2 ) myCurveType = 1;
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:ShowLegend" ) ) {                                          
    myShowLegend = QAD_CONFIG->getSetting( "Plot2d:ShowLegend" ) == QString( "true" );           // show legend
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:LegendPos" ) ) {                                          // legend position
    myLegendPos  = QAD_CONFIG->getSetting( "Plot2d:LegendPos" ).toInt();                            
    if ( myLegendPos < 0 || myLegendPos > 3 ) myLegendPos = 1;
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:MarkerSize" ) ) {                                         // marker size
    myMarkerSize = QAD_CONFIG->getSetting( "Plot2d:MarkerSize" ).toInt();                           
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:Background" ) ) {                                         // background color
    QString bgString = QAD_CONFIG->getSetting( "Plot2d:Background" );                               
    QStringList bgData = QStringList::split( ":", bgString, true );
    int bgRed = 0, bgGreen = 0, bgBlue = 0;
    if ( bgData.count() > 0 ) bgRed   = bgData[ 0 ].toInt();
    if ( bgData.count() > 1 ) bgGreen = bgData[ 1 ].toInt();
    if ( bgData.count() > 2 ) bgBlue  = bgData[ 2 ].toInt();
    myBackground = QColor( bgRed, bgGreen, bgBlue );
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:ShowTitle" ) ) {                                          // main title
    myTitleEnabled  = QAD_CONFIG->getSetting( "Plot2d:ShowTitle" ) == QString( "true" );               
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:ShowHorTitle" ) ) {                                       // hor.axis title
    myXTitleEnabled = QAD_CONFIG->getSetting( "Plot2d:ShowHorTitle" ) == QString( "true" );
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:ShowVerTitle" ) ) {                                       // ver.axisn title
    myYTitleEnabled = QAD_CONFIG->getSetting( "Plot2d:ShowVerTitle" ) == QString( "true" );
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:EnableHorMajorGrid" ) ) {                                 // grid
    myXGridMajorEnabled = QAD_CONFIG->getSetting( "Plot2d:EnableHorMajorGrid" ) == QString( "true" ); 
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:EnableVerMajorGrid" ) ) {      
    myYGridMajorEnabled = QAD_CONFIG->getSetting( "Plot2d:EnableVerMajorGrid" ) == QString( "true" );
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:EnableHorMinorGrid" ) ) {
    myXGridMinorEnabled = QAD_CONFIG->getSetting( "Plot2d:EnableHorMinorGrid" ) == QString( "true" );
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:EnableVerMinorGrid" ) ) {
    myYGridMinorEnabled = QAD_CONFIG->getSetting( "Plot2d:EnableVerMinorGrid" ) == QString( "true" );
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:HorMajorGridMax" ) ) {
    myXGridMaxMajor = QAD_CONFIG->getSetting( "Plot2d:HorMajorGridMax" ).toInt();
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:VerMajorGridMax" ) ) {
    myYGridMaxMajor = QAD_CONFIG->getSetting( "Plot2d:VerMajorGridMax" ).toInt();
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:HorMinorGridMax" ) ) {
    myXGridMaxMinor = QAD_CONFIG->getSetting( "Plot2d:HorMinorGridMax" ).toInt();
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:VerMinorGridMax" ) ) {
    myYGridMaxMinor = QAD_CONFIG->getSetting( "Plot2d:VerMinorGridMax" ).toInt();
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:HorScaleMode" ) ) {                                       // scale mode
    myXMode = QAD_CONFIG->getSetting( "Plot2d:HorScaleMode" ).toInt();                             
    if ( myXMode < 0 || myXMode > 1 ) myXMode = 0;
  }
  if ( QAD_CONFIG->hasSetting( "Plot2d:VerScaleMode" ) ) {                                       
    myYMode = QAD_CONFIG->getSetting( "Plot2d:VerScaleMode" ).toInt();
    if ( myYMode < 0 || myYMode > 1 ) myYMode = 0;
  }
}
/*!
  Writes Plot2d view settings to the preferences
*/
void Plot2d_ViewFrame::writePreferences()
{
  QAD_CONFIG->addSetting( "Plot2d:CurveType",  myCurveType );                                     // curve type
  QAD_CONFIG->addSetting( "Plot2d:ShowLegend", myShowLegend ? "true" : "false" );                 // show legend
  QAD_CONFIG->addSetting( "Plot2d:LegendPos",  myLegendPos );                                     // legend position
  QAD_CONFIG->addSetting( "Plot2d:MarkerSize", myMarkerSize );                                    // marker size
  QStringList bgData; 
  bgData.append( QString::number( myBackground.red() ) );
  bgData.append( QString::number( myBackground.green() ) );
  bgData.append( QString::number( myBackground.blue() ) );
  QAD_CONFIG->addSetting( "Plot2d:Background",   bgData.join( ":" ) );                            // background color
  QAD_CONFIG->addSetting( "Plot2d:ShowTitle",    myTitleEnabled ?  "true" : "false" );            // titles
  QAD_CONFIG->addSetting( "Plot2d:ShowHorTitle", myXTitleEnabled ? "true" : "false" ); 
  QAD_CONFIG->addSetting( "Plot2d:ShowVerTitle", myYTitleEnabled ? "true" : "false" ); 
  QAD_CONFIG->addSetting( "Plot2d:EnableHorMajorGrid", myXGridMajorEnabled ? "true" : "false" );  // grid
  QAD_CONFIG->addSetting( "Plot2d:EnableVerMajorGrid", myYGridMajorEnabled ? "true" : "false" );  
  QAD_CONFIG->addSetting( "Plot2d:EnableHorMinorGrid", myXGridMinorEnabled ? "true" : "false" );  
  QAD_CONFIG->addSetting( "Plot2d:EnableVerMinorGrid", myYGridMinorEnabled ? "true" : "false" );  
  QAD_CONFIG->addSetting( "Plot2d:HorMajorGridMax", myXGridMaxMajor );
  QAD_CONFIG->addSetting( "Plot2d:VerMajorGridMax", myYGridMaxMajor );
  QAD_CONFIG->addSetting( "Plot2d:HorMinorGridMax", myXGridMaxMinor );
  QAD_CONFIG->addSetting( "Plot2d:VerMinorGridMax", myYGridMaxMinor );
  QAD_CONFIG->addSetting( "Plot2d:HorScaleMode", myXMode );                                       // scale mode
  QAD_CONFIG->addSetting( "Plot2d:VerScaleMode", myYMode );
}
/*!
  Prints mouse cursor coordinates into string
*/
QString Plot2d_ViewFrame::getInfo( const QPoint& pnt ) 
{
  bool xFound = false, yFound = false;
  double xCoord, yCoord;
  const QwtScaleDiv* aXscale = myPlot->axisScale( QwtPlot::xBottom );
  for ( int i = 0; i < aXscale->majCnt(); i++ ) {
    double majXmark = aXscale->majMark( i );
    int xmark = myPlot->transform( QwtPlot::xBottom, majXmark );
    if ( xmark-2 == pnt.x() ) {
      xCoord = majXmark; 
      xFound = true;
      MESSAGE("Plot2d_ViewFrame::getInfo : close maj X mark("<<i<<") = "<<majXmark<<" "<<xmark<<" "<<pnt.x());
      break;
    }
  }
  if ( !xFound ) {
    for ( int i = 0; i < aXscale->minCnt(); i++ ) {
      double minXmark = aXscale->minMark( i );
      int xmark = myPlot->transform( QwtPlot::xBottom, minXmark );
      if ( xmark-2 == pnt.x() ) {
	xCoord = minXmark; 
	xFound = true;
	MESSAGE("Plot2d_ViewFrame::getInfo : close min X mark("<<i<<") = "<<minXmark<<" "<<xmark<<" "<<pnt.x());
	break;
      }
    }
  }  
  const QwtScaleDiv* aYscale = myPlot->axisScale( QwtPlot::yLeft );
  for ( int i = 0; i < aYscale->majCnt(); i++ ) {
    double majYmark = aYscale->majMark( i );
    int ymark = myPlot->transform( QwtPlot::yLeft, majYmark );
    if ( ymark-2 == pnt.y() ) {
      yCoord = majYmark; 
      yFound = true;
      break;
    }
  }
  if ( !yFound ) {
    for ( int i = 0; i < aYscale->minCnt(); i++ ) {
      double minYmark = aYscale->minMark( i );
      int ymark = myPlot->transform( QwtPlot::yLeft, minYmark );
      if ( ymark-2 == pnt.y() ) {
	yCoord = minYmark; 
	yFound = true;
	break;
      }
    }
  }  
  QString strX = QString::number( xFound ? xCoord : myPlot->invTransform( QwtPlot::xBottom, pnt.x() ) ).stripWhiteSpace();
  if ( strX == "-0" )
    strX = "0";
  QString strY = QString::number( yFound ? yCoord : myPlot->invTransform( QwtPlot::yLeft, pnt.y() ) ).stripWhiteSpace();
  if ( strY == "-0" )
    strY = "0";
  QString info = tr("INF_COORDINATES").arg( strX ).arg( strY );
  return info;
}
/*!
  Converts Plot2d_Curve's marker style to Qwt marker style [ static ]
*/
static QwtSymbol::Style plot2qwtMarker( Plot2d_Curve::MarkerType m )
{
  QwtSymbol::Style ms = QwtSymbol::None;  
  switch ( m ) {
  case Plot2d_Curve::Circle:
    ms = QwtSymbol::Ellipse;   break;
  case Plot2d_Curve::Rectangle:
    ms = QwtSymbol::Rect;      break;
  case Plot2d_Curve::Diamond:
    ms = QwtSymbol::Diamond;   break;
  case Plot2d_Curve::DTriangle:
    ms = QwtSymbol::DTriangle; break;
  case Plot2d_Curve::UTriangle:
    ms = QwtSymbol::UTriangle; break;
  case Plot2d_Curve::LTriangle: // Qwt confuses LTriangle and RTriangle :(((
    ms = QwtSymbol::RTriangle; break;
  case Plot2d_Curve::RTriangle: // Qwt confuses LTriangle and RTriangle :(((
    ms = QwtSymbol::LTriangle; break;
  case Plot2d_Curve::Cross:
    ms = QwtSymbol::Cross;     break;
  case Plot2d_Curve::XCross:
    ms = QwtSymbol::XCross;    break;
  case Plot2d_Curve::None:
  default:
    ms = QwtSymbol::None;      break;
  }
  return ms;
}
/*!
  Converts Qwt marker style to Plot2d_Curve's marker style [ static ]
*/
static Plot2d_Curve::MarkerType qwt2plotMarker( QwtSymbol::Style m )
{
  Plot2d_Curve::MarkerType ms = Plot2d_Curve::None;  
  switch ( m ) {
  case QwtSymbol::Ellipse:
    ms = Plot2d_Curve::Circle;    break;
  case QwtSymbol::Rect:
    ms = Plot2d_Curve::Rectangle; break;
  case QwtSymbol::Diamond:
    ms = Plot2d_Curve::Diamond;   break;
  case QwtSymbol::DTriangle:
    ms = Plot2d_Curve::DTriangle; break;
  case QwtSymbol::UTriangle:
    ms = Plot2d_Curve::UTriangle; break;
  case QwtSymbol::RTriangle: // Qwt confuses LTriangle and RTriangle :(((
    ms = Plot2d_Curve::LTriangle; break;
  case QwtSymbol::LTriangle: // Qwt confuses LTriangle and RTriangle :(((
    ms = Plot2d_Curve::RTriangle; break;
  case QwtSymbol::Cross:
    ms = Plot2d_Curve::Cross;     break;
  case QwtSymbol::XCross:
    ms = Plot2d_Curve::XCross;    break;
  case QwtSymbol::None:
  default:
    ms = Plot2d_Curve::None;      break;
  }
  return ms;
}
/*!
  Converts Plot2d_Curve's line style to Qwt line style [ static ]
*/
static Qt::PenStyle plot2qwtLine( Plot2d_Curve::LineType p )
{
  Qt::PenStyle ps = Qt::NoPen;
  switch ( p ) {
  case Plot2d_Curve::Solid:
    ps = Qt::SolidLine;      break;
  case Plot2d_Curve::Dash:
    ps = Qt::DashLine;       break;
  case Plot2d_Curve::Dot:
    ps = Qt::DotLine;        break;
  case Plot2d_Curve::DashDot:
    ps = Qt::DashDotLine;    break;
  case Plot2d_Curve::DashDotDot:
    ps = Qt::DashDotDotLine; break;
  case Plot2d_Curve::NoPen:
  default:
    ps = Qt::NoPen;          break;
  }
  return ps;
}
/*!
  Converts Qwt line style to Plot2d_Curve's line style [ static ]
*/
static Plot2d_Curve::LineType qwt2plotLine( Qt::PenStyle p )
{
  Plot2d_Curve::LineType ps = Plot2d_Curve::NoPen;
  switch ( p ) {
  case Qt::SolidLine:
    ps = Plot2d_Curve::Solid;      break;
  case Qt::DashLine:
    ps = Plot2d_Curve::Dash;       break;
  case Qt::DotLine:
    ps = Plot2d_Curve::Dot;        break;
  case Qt::DashDotLine:
    ps = Plot2d_Curve::DashDot;    break;
  case Qt::DashDotDotLine:
    ps = Plot2d_Curve::DashDotDot; break;
  case Qt::NoPen:
  default:
    ps = Plot2d_Curve::NoPen;      break;
  }
  return ps;
}
/*!
  Adds curve into view
*/
void Plot2d_ViewFrame::displayCurve( Plot2d_Curve* curve, bool update )
{
  if ( !curve )
    return;
  if ( hasCurve( curve ) ) {
    updateCurve( curve, update );
  }
  else {
    long curveKey = myPlot->insertCurve( curve->getVerTitle() );
    myCurves.insert( curveKey, curve );
    if ( curve->isAutoAssign() ) {
      QwtSymbol::Style typeMarker;
      QColor           color;
      Qt::PenStyle     typeLine;
      myPlot->getNextMarker( typeMarker, color, typeLine );
      myPlot->setCurvePen( curveKey, QPen( color, DEFAULT_LINE_WIDTH, typeLine ) );
      myPlot->setCurveSymbol( curveKey, QwtSymbol( typeMarker, 
						   QBrush( color ), 
						   QPen( color ), 
						   QSize( myMarkerSize, myMarkerSize ) ) );
      curve->setColor( color );
      curve->setLine( qwt2plotLine( typeLine ) );
      curve->setMarker( qwt2plotMarker( typeMarker ) );
    }
    else {
      Qt::PenStyle     ps = plot2qwtLine( curve->getLine() );
      QwtSymbol::Style ms = plot2qwtMarker( curve->getMarker() );
      myPlot->setCurvePen( curveKey, QPen( curve->getColor(), curve->getLineWidth(), ps ) );
      myPlot->setCurveSymbol( curveKey, QwtSymbol( ms, 
						   QBrush( curve->getColor() ), 
						   QPen( curve->getColor() ), 
						   QSize( myMarkerSize, myMarkerSize ) ) );
    }
    if ( myCurveType == 0 )
      myPlot->setCurveStyle( curveKey, QwtCurve::NoCurve );
    else if ( myCurveType == 1 )
      myPlot->setCurveStyle( curveKey, QwtCurve::Lines );
    else if ( myCurveType == 2 )
      myPlot->setCurveStyle( curveKey, QwtCurve::Spline );
    myPlot->setCurveData( curveKey, curve->horData(), curve->verData(), curve->nbPoints() );
  }
  updateTitles();
  if ( update )
    myPlot->replot();
}
/*!
  Adds curves into view
*/
void Plot2d_ViewFrame::displayCurves( Plot2d_CurveContainer& curves, bool update )
{
  myPlot->setUpdatesEnabled( false );
  for ( int i = 0; i < curves.count(); i++ ) {
    displayCurve( curves.curve( i ), false );
  }
//  fitAll();
  myPlot->setUpdatesEnabled( true );
  if ( update )
    myPlot->replot();
}
/*!
  Erases curve
*/
void Plot2d_ViewFrame::eraseCurve( Plot2d_Curve* curve, bool update )
{
  if ( !curve )
    return;
  int curveKey = hasCurve( curve );
  if ( curveKey ) {
    myPlot->removeCurve( curveKey );
    myCurves.remove( curveKey );
    updateTitles();
    if ( update )
      myPlot->replot();
  }
}
/*!
  Erases curves
*/
void Plot2d_ViewFrame::eraseCurves( Plot2d_CurveContainer& curves, bool update )
{
  for ( int i = 0; i < curves.count(); i++ ) {
    eraseCurve( curves.curve( i ), false );
  }
//  fitAll();
  if ( update )
    myPlot->replot();
}
/*!
  Updates curves attributes
*/
void Plot2d_ViewFrame::updateCurve( Plot2d_Curve* curve, bool update )
{
  if ( !curve )
    return;
  int curveKey = hasCurve( curve );
  if ( curveKey ) {
    if ( !curve->isAutoAssign() ) {
      Qt::PenStyle     ps = plot2qwtLine( curve->getLine() );
      QwtSymbol::Style ms = plot2qwtMarker( curve->getMarker() );
      myPlot->setCurvePen( curveKey, QPen( curve->getColor(), curve->getLineWidth(), ps ) );
      myPlot->setCurveSymbol( curveKey, QwtSymbol( ms, 
						   QBrush( curve->getColor() ), 
						   QPen( curve->getColor() ), 
						   QSize( myMarkerSize, myMarkerSize ) ) );
    }
    myPlot->setCurveTitle( curveKey, curve->getVerTitle() );
    myPlot->setCurveData( curveKey, curve->horData(), curve->verData(), curve->nbPoints() );
    myPlot->curve( curveKey )->setEnabled( true );
    updateTitles();
    if ( update )
      myPlot->replot();
  }
}
/*!
  Returns curve key if is is displayed in the viewer and 0 otherwise
*/
int Plot2d_ViewFrame::hasCurve( Plot2d_Curve* curve )
{
  QIntDictIterator<Plot2d_Curve> it( myCurves );
  for ( ; it.current(); ++it ) {
    if ( it.current() == curve )
      return it.currentKey();
  }
  return 0;
}
Plot2d_Curve* Plot2d_ViewFrame::getCurveByIO( const Handle(SALOME_InteractiveObject)& theIObject )
{
  if ( !theIObject.IsNull() ) {
    QIntDictIterator<Plot2d_Curve> it( myCurves );
    for ( ; it.current(); ++it ) {
      if ( it.current()->hasIO() && it.current()->getIO()->isSame( theIObject ) )
	return it.current();
    }
  }
  return 0;
}
/*!
  Gets lsit of displayed curves
*/
int Plot2d_ViewFrame::getCurves( QList<Plot2d_Curve>& clist )
{
  clist.clear();
  clist.setAutoDelete( false );
  QIntDictIterator<Plot2d_Curve> it( myCurves );
  for ( ; it.current(); ++it ) {
    clist.append( it.current() );
  }
  return clist.count();
}

/*!
  Updates titles according to curves
*/
#define BRACKETIZE(x) QString( "[ " ) + x + QString( " ]" )
void Plot2d_ViewFrame::updateTitles() 
{
  QAD_Study* activeStudy = QAD_Application::getDesktop()->getActiveStudy();
  QIntDictIterator<Plot2d_Curve> it( myCurves );
  QStringList aXTitles;
  QStringList aYTitles;
  QStringList aXUnits;
  QStringList aYUnits;
  QStringList aTables;
  int i = 0;
  while ( it.current() ) {
    // collect titles and units from all curves...
    QString xTitle = it.current()->getHorTitle().stripWhiteSpace();
    QString yTitle = it.current()->getVerTitle().stripWhiteSpace();
    QString xUnits = it.current()->getHorUnits().stripWhiteSpace();
    QString yUnits = it.current()->getVerUnits().stripWhiteSpace();
    
    aYTitles.append( yTitle );
    if ( aXTitles.find( xTitle ) == aXTitles.end() )
      aXTitles.append( xTitle );
    if ( aXUnits.find( xUnits ) == aXUnits.end() )
      aXUnits.append( xUnits );
    if ( aYUnits.find( yUnits ) == aYUnits.end() )
      aYUnits.append( yUnits );

    if ( activeStudy && it.current()->hasTableIO() ) { 
      SALOMEDS::SObject_var SO = activeStudy->getStudyDocument()->FindObjectID( it.current()->getTableIO()->getEntry() );
      if ( !SO->_is_nil() ) {
	SALOMEDS::GenericAttribute_var anAttr;
	if ( SO->FindAttribute( anAttr, "AttributeName" ) ) {
	  SALOMEDS::AttributeName_var aNameAttr = SALOMEDS::AttributeName::_narrow( anAttr );
	  QString aName = aNameAttr->Value();
	  if ( !aName.isEmpty() && aTables.find( aName ) == aTables.end() )
	    aTables.append( aName );
	}
      }
    }

    ++it;
    ++i;
  }
  // ... and update plot 2d view
  QString xUnits, yUnits;
  if ( aXUnits.count() == 1 && !aXUnits[0].isEmpty() )
    xUnits = BRACKETIZE( aXUnits[0] );
  if ( aYUnits.count() == 1 && !aYUnits[0].isEmpty())
    yUnits = BRACKETIZE( aYUnits[0] );
  QString xTitle, yTitle;
  if ( aXTitles.count() == 1 && aXUnits.count() == 1 )
    xTitle = aXTitles[0];
  if ( aYTitles.count() == 1 )
    yTitle = aYTitles[0];

  if ( !xTitle.isEmpty() && !xUnits.isEmpty() )
    xTitle += " ";
  if ( !yTitle.isEmpty() && !yUnits.isEmpty() )
    yTitle += " ";

  setXTitle( myXTitleEnabled, xTitle + xUnits );
  setYTitle( myYTitleEnabled, yTitle + yUnits );
  setTitle( aTables.join("; ") );
}
/*!
  Fits the view to see all data
*/
void Plot2d_ViewFrame::fitAll()
{
  myPlot->setAxisAutoScale( QwtPlot::yLeft );
  myPlot->setAxisAutoScale( QwtPlot::xBottom );
  myPlot->replot();
}
/*!
  Fits the view to rectangle area (pixels)
*/
void Plot2d_ViewFrame::fitArea( const QRect& area )
{
  QRect rect = area.normalize();
  if ( rect.width() < MIN_RECT_SIZE ) {
    rect.setWidth( MIN_RECT_SIZE );
    rect.setLeft( rect.left() - MIN_RECT_SIZE/2 );
  }
  if ( rect.height() < MIN_RECT_SIZE ) {
    rect.setHeight( MIN_RECT_SIZE );
    rect.setTop( rect.top() - MIN_RECT_SIZE/2 );
  }
  myPlot->setAxisScale( QwtPlot::yLeft, 
		        myPlot->invTransform( QwtPlot::yLeft, rect.top() ), 
 		        myPlot->invTransform( QwtPlot::yLeft, rect.bottom() ) );
  myPlot->setAxisScale( QwtPlot::xBottom, 
		        myPlot->invTransform( QwtPlot::xBottom, rect.left() ), 
		        myPlot->invTransform( QwtPlot::xBottom, rect.right() ) );
  myPlot->replot();
}
/*!
  Tests if it is necessary to start operation on mouse action
*/
int Plot2d_ViewFrame::testOperation( const QMouseEvent& me )
{
  int btn = me.button() | me.state();
  int zoomBtn = ControlButton | LeftButton;
  int panBtn  = ControlButton | MidButton;
  int fitBtn  = ControlButton | RightButton;

  if ( btn == zoomBtn )
    return ZoomId;
  if ( btn == panBtn ) 
    return PanId;
  if ( btn == fitBtn )
    return FitAreaId;
  return NoOpId;
}
/*!
  Mode toolbar buttons slot - horizontal axis (<Linear>/<Logarithmic>)
*/
void Plot2d_ViewFrame::onHorMode()
{
  if ( myActions[ ModeXLinearId ]->isOn() )
    setHorScaleMode( 0 );
  else if ( myActions[ ModeXLogarithmicId ]->isOn() )
    setHorScaleMode( 1 );
}
/*!
  Mode toolbar buttons slot - vertical axis (<Linear>/<Logarithmic>)
*/
void Plot2d_ViewFrame::onVerMode()
{
  if ( myActions[ ModeYLinearId ]->isOn() )
    setVerScaleMode( 0 );
  else if ( myActions[ ModeYLogarithmicId ]->isOn() )
    setVerScaleMode( 1 );
}
/*!
  "Show/hide legend" toolbar action slot
*/
void Plot2d_ViewFrame::onLegend()
{
  showLegend( myActions[ LegendId ]->isOn() );
}
/*!
  "Curve type" toolbar action slot
*/
void Plot2d_ViewFrame::onCurves()
{
  if ( myActions[ CurvePointsId ]->isOn() )
    setCurveType( 0 );
  else if ( myActions[ CurveLinesId ]->isOn() )
    setCurveType( 1 );
  else if ( myActions[ CurveSplinesId ]->isOn() )
    setCurveType( 2 );
}
/*!
  "Settings" toolbar action slot
*/
void Plot2d_ViewFrame::onSettings()
{
#ifdef TEST_AUTOASSIGN
  typedef QMap<int,int> IList;
  typedef QMap<QString,int> SList;
  IList mars, lins;
  SList cols;
  cols[ "red-min" ]   = 1000;
  cols[ "red-max" ]   = -1;
  cols[ "green-min" ] = 1000;
  cols[ "green-max" ] = -1;
  cols[ "blue-min" ]  = 1000;
  cols[ "blue-max" ]  = -1;
  for ( unsigned i = 0; i < 10000; i++ ) {
    QwtSymbol::Style typeMarker;
    QColor           color;
    Qt::PenStyle     typeLine;
    myPlot->getNextMarker( typeMarker, color, typeLine );
    if ( mars.contains(typeMarker) )
      mars[ typeMarker ] = mars[ typeMarker ]+1;
    else
      mars[ typeMarker ] = 0;
    if ( lins.contains(typeLine) )
      lins[ typeLine ] = lins[ typeLine ]+1;
    else
      lins[ typeLine ] = 0;
    if ( cols[ "red-max" ] < color.red() )
      cols[ "red-max" ] = color.red();
    if ( cols[ "red-min" ] > color.red() )
      cols[ "red-min" ] = color.red();
    if ( cols[ "green-max" ] < color.green() )
      cols[ "green-max" ] = color.green();
    if ( cols[ "green-min" ] > color.green() )
      cols[ "green-min" ] = color.green();
    if ( cols[ "blue-max" ] < color.blue() )
      cols[ "blue-max" ] = color.blue();
    if ( cols[ "blue-min" ] > color.blue() )
      cols[ "blue-min" ] = color.blue();
  }
  for (IList::Iterator it = mars.begin();  it != mars.end(); ++it)
    MESSAGE("markers( " << it.key() << ") = " << it.data() );
  for (IList::Iterator it = lins.begin();  it != lins.end(); ++it)
    MESSAGE("lines( " << it.key() << ") = " << it.data() );
  for (SList::Iterator it = cols.begin();  it != cols.end(); ++it)
    MESSAGE("colors( " << it.key() << ") = " << it.data() );
#endif
  
  Plot2d_SetupViewDlg* dlg = new Plot2d_SetupViewDlg( this, true );
  dlg->setMainTitle( myTitleEnabled, myTitle );
  dlg->setXTitle( myXTitleEnabled, myXTitle );
  dlg->setYTitle( myYTitleEnabled, myYTitle );
  dlg->setCurveType( myCurveType );
  dlg->setLegend( myShowLegend, myLegendPos );
  dlg->setMarkerSize( myMarkerSize );
  dlg->setBackgroundColor( myBackground );
  dlg->setMajorGrid( myXGridMajorEnabled, myPlot->axisMaxMajor( QwtPlot::xBottom ),
		     myYGridMajorEnabled, myPlot->axisMaxMajor( QwtPlot::yLeft ) );
  dlg->setMinorGrid( myXGridMinorEnabled, myPlot->axisMaxMinor( QwtPlot::xBottom ),
		     myYGridMinorEnabled, myPlot->axisMaxMinor( QwtPlot::yLeft ) );
  dlg->setScaleMode( myXMode, myYMode );

  if ( dlg->exec() == QDialog::Accepted ) {
    // horizontal axis title
    setXTitle( dlg->isXTitleEnabled(), dlg->getXTitle(), false );
    // vertical axis title
    setYTitle( dlg->isYTitleEnabled(), dlg->getYTitle(), false );
    // main title
    setMainTitle( dlg->isMainTitleEnabled(), dlg->getMainTitle(), true );
    // curve type
    if ( myCurveType != dlg->getCurveType() ) {
      setCurveType( dlg->getCurveType(), false );
    }
    // legend
    if ( myShowLegend != dlg->isLegendEnabled() ) {
      showLegend( dlg->isLegendEnabled(), false );
    }
    if ( myLegendPos != dlg->getLegendPos() ) {
      setLegendPos( dlg->getLegendPos() );
    }
    // marker size
    if ( myMarkerSize != dlg->getMarkerSize() ) {
      setMarkerSize( dlg->getMarkerSize(), false );
    }
    // background color
    if ( myBackground != dlg->getBackgroundColor() ) {
      setBackgroundColor( dlg->getBackgroundColor() );
    }
    // grid
    bool aXGridMajorEnabled, aXGridMinorEnabled, aYGridMajorEnabled, aYGridMinorEnabled;
    int  aXGridMaxMajor,     aXGridMaxMinor,     aYGridMaxMajor,     aYGridMaxMinor;
    dlg->getMajorGrid( aXGridMajorEnabled, aXGridMaxMajor, aYGridMajorEnabled, aYGridMaxMajor );
    dlg->getMinorGrid( aXGridMinorEnabled, aXGridMaxMinor, aYGridMinorEnabled, aYGridMaxMinor );
    setXGrid( aXGridMajorEnabled, aXGridMaxMajor, aXGridMinorEnabled, aXGridMaxMinor, false );
    setYGrid( aYGridMajorEnabled, aYGridMaxMajor, aYGridMinorEnabled, aYGridMaxMinor, false );
    // scale mode
    if ( myXMode != dlg->getXScaleMode() ) {
      setHorScaleMode( dlg->getXScaleMode() );
    }
    if ( myYMode != dlg->getYScaleMode() ) {
      setVerScaleMode( dlg->getYScaleMode() );
    }
    // update view
    myPlot->replot();
    // update preferences
    if ( dlg->isSetAsDefault() ) 
      writePreferences();
  }
  delete dlg;
}
/*!
  "Fit Data" command slot
*/
void Plot2d_ViewFrame::onFitData()
{
  Plot2d_FitDataDlg* dlg = new Plot2d_FitDataDlg( this );
  int ixMin = myPlot->canvasMap( QwtPlot::xBottom ).i1();
  int ixMax = myPlot->canvasMap( QwtPlot::xBottom ).i2();
  int iyMin = myPlot->canvasMap( QwtPlot::yLeft ).i1();
  int iyMax = myPlot->canvasMap( QwtPlot::yLeft ).i2();
  double xMin = myPlot->invTransform(QwtPlot::xBottom, ixMin);
  double xMax = myPlot->invTransform(QwtPlot::xBottom, ixMax);
  double yMin = myPlot->invTransform(QwtPlot::yLeft, iyMin);
  double yMax = myPlot->invTransform(QwtPlot::yLeft, iyMax);
  
  dlg->setRange( xMin, xMax, yMin, yMax );
  if ( dlg->exec() == QDialog::Accepted ) {
    int mode = dlg->getRange( xMin, xMax, yMin, yMax );
    if ( mode == 0 || mode == 2 ) 
      myPlot->setAxisScale( QwtPlot::yLeft, yMax, yMin );
    if ( mode == 0 || mode == 1 ) 
      myPlot->setAxisScale( QwtPlot::xBottom, xMin, xMax ); 
    myPlot->replot();
  }
  delete dlg;
}
/*!
  Change background color
*/
void Plot2d_ViewFrame::onChangeBackground()
{
  QColor selColor = QColorDialog::getColor ( backgroundColor(), this );	
  if ( selColor.isValid() ) {
    setBackgroundColor( selColor );
  }
}
/*!
  Sets curve type
*/
void Plot2d_ViewFrame::setCurveType( int curveType, bool update )
{
  myCurveType = curveType;
  if ( curveType == 0 )
    myActions[ CurvePointsId ]->setOn( true );
  else if ( curveType == 1 )
    myActions[ CurveLinesId ]->setOn( true );
  else if ( curveType == 2 )
    myActions[ CurveSplinesId ]->setOn( true );
  
  QArray<long> keys = myPlot->curveKeys();
  for ( int i = 0; i < keys.count(); i++ ) {
    if ( myCurveType == 0 )
      myPlot->setCurveStyle( keys[i], QwtCurve::NoCurve );
    else if ( myCurveType == 1 )
      myPlot->setCurveStyle( keys[i], QwtCurve::Lines );
    else if ( myCurveType == 2 )
      myPlot->setCurveStyle( keys[i], QwtCurve::Spline );
  }
  if ( update )
    myPlot->replot();
}
/*!
  Shows/hides legend
*/
void Plot2d_ViewFrame::showLegend( bool show, bool update )
{
  myShowLegend = show;
  myActions[ LegendId ]->setOn( myShowLegend );
  {
    myPlot->setAutoLegend( myShowLegend );
    myPlot->enableLegend( myShowLegend );
  }
  if ( update )
    myPlot->replot();
}
/*!
  Sets legend position : 0 - left, 1 - right, 2 - top, 3 - bottom
*/
void Plot2d_ViewFrame::setLegendPos( int pos )
{
  myLegendPos = pos;
  switch( pos ) {
  case 0:
    myPlot->setLegendPos( Qwt::Left );
    break;
  case 1:
    myPlot->setLegendPos( Qwt::Right );
    break;
  case 2:
    myPlot->setLegendPos( Qwt::Top );
    break;
  case 3:
    myPlot->setLegendPos( Qwt::Bottom );
    break;
  }
}
/*!
  Sets new marker size
*/
void Plot2d_ViewFrame::setMarkerSize( const int size, bool update )
{
  if ( myMarkerSize != size ) {
    myMarkerSize = size;
    QArray<long> keys = myPlot->curveKeys();
    for ( int i = 0; i < keys.count(); i++ ) {
      QwtPlotCurve* crv = myPlot->curve( keys[i] );
      if ( crv ) {
	QwtSymbol aSymbol = crv->symbol();
	aSymbol.setSize( myMarkerSize, myMarkerSize );
	crv->setSymbol( aSymbol );
#if QWT_VERSION < 0x040200 // mpv: porting to the QWT 4.2.0
	int legendIndex = myPlot->getLegend()->findFirstKey( keys[i] );
	if ( legendIndex != myPlot->getLegend()->itemCnt() )
	  myPlot->getLegend()->setSymbol( legendIndex, aSymbol );
#endif
      }
    }
    if ( update )
      myPlot->replot();
  }
}
/*!
  Sets background color
*/
void Plot2d_ViewFrame::setBackgroundColor( const QColor& color )
{
  myBackground = color;
  //myPlot->setCanvasBackground( myBackground );
  myPlot->canvas()->setPalette( myBackground );
  myPlot->setPalette( myBackground );
  QPalette aPal = myPlot->getLegend()->palette();
  for ( int i = 0; i < QPalette::NColorGroups; i++ ) {
    QPalette::ColorGroup cg = (QPalette::ColorGroup)i;
    aPal.setColor( cg, QColorGroup::Base, myBackground );
    aPal.setColor( cg, QColorGroup::Background, myBackground );
  }
  myPlot->getLegend()->setPalette( aPal );
}
/*!
  Gets background color
*/
QColor Plot2d_ViewFrame::backgroundColor() const
{
  return myBackground;
}
/*!
  Sets hor.axis grid parameters
*/
void Plot2d_ViewFrame::setXGrid( bool xMajorEnabled, const int xMajorMax, 
				 bool xMinorEnabled, const int xMinorMax, 
				 bool update )
{
  myXGridMajorEnabled = xMajorEnabled;
  myXGridMinorEnabled = xMinorEnabled;
  myXGridMaxMajor = xMajorMax;
  myXGridMaxMinor = xMinorMax;
  myPlot->setAxisMaxMajor( QwtPlot::xBottom, myXGridMaxMajor );
  myPlot->setAxisMaxMinor( QwtPlot::xBottom, myXGridMaxMinor );
  myPlot->enableGridX( myXGridMajorEnabled );
  myPlot->enableGridXMin( myXGridMinorEnabled );
  if ( update )
    myPlot->replot();
}
/*!
  Sets ver.axis grid parameters
*/
void Plot2d_ViewFrame::setYGrid( bool yMajorEnabled, const int yMajorMax, 
				 bool yMinorEnabled, const int yMinorMax, 
				 bool update )
{
  myYGridMajorEnabled = yMajorEnabled;
  myYGridMinorEnabled = yMinorEnabled;
  myYGridMaxMajor = yMajorMax;
  myYGridMaxMinor = yMinorMax;
  myPlot->setAxisMaxMajor( QwtPlot::yLeft, myYGridMaxMajor );
  myPlot->setAxisMaxMinor( QwtPlot::yLeft, myYGridMaxMinor );
  myPlot->enableGridY( myYGridMajorEnabled );
  myPlot->enableGridYMin( myYGridMinorEnabled );
  if ( update )
    myPlot->replot();
}
/*!
  Sets main title
*/
void Plot2d_ViewFrame::setMainTitle( bool enabled, const QString& title, bool update )
{
  myTitleEnabled = enabled;
  myTitle = title;
  myPlot->setTitle( myTitleEnabled ? myTitle : QString::null );
  if ( update )
    myPlot->replot();
}
/*!
  Sets hor.axis title
*/
void Plot2d_ViewFrame::setXTitle( bool enabled, const QString& title, bool update )
{
  myXTitleEnabled = enabled;
  myXTitle = title;
  myPlot->setAxisTitle( QwtPlot::xBottom, myXTitleEnabled ? myXTitle : QString::null );
  if ( update )
    myPlot->replot();
}
/*!
  Sets ver.axis title
*/
void Plot2d_ViewFrame::setYTitle( bool enabled, const QString& title, bool update )
{
  myYTitleEnabled = enabled;
  myYTitle = title;
  myPlot->setAxisTitle( QwtPlot::yLeft, myYTitleEnabled ? myYTitle : QString::null );
  if ( update )
    myPlot->replot();
}
/*!
  Sets scale mode for horizontal axis: 0 - linear, 1 - logarithmic
*/
void Plot2d_ViewFrame::setHorScaleMode( const int mode, bool update )
{
  myXMode = mode;
  if ( myXMode == 0 ) { // linear
    myActions[ ModeXLogarithmicId ]->setOn( false );
    myActions[ ModeXLinearId ]->setOn( true );
    myPlot->changeAxisOptions( QwtPlot::xBottom, QwtAutoScale::Logarithmic, false );
  }
  else {               // logarithmic
    myActions[ ModeXLinearId ]->setOn( false );
    myActions[ ModeXLogarithmicId ]->setOn( true );
    myPlot->changeAxisOptions( QwtPlot::xBottom, QwtAutoScale::Logarithmic, true );
  }
  if ( update )
//    myPlot->replot();
    fitAll();
}
/*!
  Sets scale mode for vertical axis: 0 - linear, 1 - logarithmic
*/
void Plot2d_ViewFrame::setVerScaleMode( const int mode, bool update )
{
  myYMode = mode;
  if ( myYMode == 0 ) { // linear
    myActions[ ModeYLogarithmicId ]->setOn( false );
    myActions[ ModeYLinearId ]->setOn( true );
    myPlot->changeAxisOptions( QwtPlot::yLeft, QwtAutoScale::Logarithmic, false );
  }
  else {               // logarithmic
    myActions[ ModeYLinearId ]->setOn( false );
    myActions[ ModeYLogarithmicId ]->setOn( true );
    myPlot->changeAxisOptions( QwtPlot::yLeft, QwtAutoScale::Logarithmic, true );
  }
  if ( update )
//    myPlot->replot();
    fitAll();
}
/*!
  Slot, called when Legend item is clicked
*/
void Plot2d_ViewFrame::onLegendClicked( long key )
{
  Plot2d_Curve* curve = myCurves[ key ];
  if ( curve && curve->hasIO() ) {
    SALOME_Selection* Sel = SALOME_Selection::Selection( QAD_Application::getDesktop()->getActiveStudy()->getSelection() );
    Sel->ClearIObjects();
    Sel->AddIObject( curve->getIO(), true );
  }
}

/*!
  Slot, called when user presses mouse button
*/
void Plot2d_ViewFrame::plotMousePressed( const QMouseEvent& me )
{
  if ( myOperation == NoOpId )
    myOperation = testOperation( me );
  if ( myOperation != NoOpId ) {
    myPnt = me.pos();
    if ( myOperation == ZoomId ) {
      myPlot->canvas()->setCursor( QCursor( QPixmap( imageZoomCursor ) ) );
    }
    else if ( myOperation == PanId ) {
      myPlot->canvas()->setCursor( QCursor( Qt::SizeAllCursor ) );
    }
    else if ( myOperation == FitAreaId ) {
      myPlot->canvas()->setCursor( QCursor( Qt::PointingHandCursor ) );
      myPlot->setOutlineStyle( Qwt::Rect );
    }
  }
  else {
    int btn = me.button() | me.state();
    if ( btn == RightButton ) {
      createPopup();
      if ( myPopup ) {
	QAD_Tools::checkPopup( myPopup );
	if ( myPopup->count()>0 ) {
	  myPopup->exec( QCursor::pos() );
	}
	destroyPopup();
      }
    }
    else {
      myPlot->setOutlineStyle( Qwt::Cross );
      QAD_Application::getDesktop()->putInfo( getInfo( me.pos() ) );
    }
  }
}
/*!
  Slot, called when user moves mouse
*/
void Plot2d_ViewFrame::plotMouseMoved( const QMouseEvent& me )
{
  int    dx = me.pos().x() - myPnt.x();
  int    dy = me.pos().y() - myPnt.y();

  if ( myOperation != NoOpId) {
    if ( myOperation == ZoomId ) {
      this->incrementalZoom( dx, dy ); 
      myPnt = me.pos();
    }
    else if ( myOperation == PanId ) {
      this->incrementalPan( dx, dy );
      myPnt = me.pos();
    }
  }
  else {
    QAD_Application::getDesktop()->putInfo( getInfo( me.pos() ) );
  }
}
/*!
  Slot, called when user releases mouse
*/
void Plot2d_ViewFrame::plotMouseReleased( const QMouseEvent& me )
{
  if ( myOperation == FitAreaId ) {
    QRect rect( myPnt, me.pos() );
    fitArea( rect );
  }
  myPlot->canvas()->setCursor( QCursor( Qt::CrossCursor ) );
  myPlot->setOutlineStyle( Qwt::Triangle );
  QAD_Application::getDesktop()->putInfo( tr( "INF_READY" ) );
  myOperation = NoOpId;
}
/*!
  View operations : Pan view
*/
void Plot2d_ViewFrame::onViewPan() 
{ 
  myOperation = PanId;
  qApp->installEventFilter( this );
}
/*!
  View operations : Zoom view
*/
void Plot2d_ViewFrame::onViewZoom() 
{
  myOperation = ZoomId;
  qApp->installEventFilter( this );
}
/*!
  View operations : Fot All
*/
void Plot2d_ViewFrame::onViewFitAll() 
{ 
  fitAll();
}
/*!
  View operations : Fit Area
*/
void Plot2d_ViewFrame::onViewFitArea() 
{
  myOperation = FitAreaId;
  qApp->installEventFilter( this );
}
/*!
  View operations : Global panning
*/
void Plot2d_ViewFrame::onViewGlobalPan() 
{ MESSAGE( "Plot2d_ViewFrame::onViewGlobalPan : NOT SUPPORTED" ); }
/*!
  View operations : Rotate view
*/
void Plot2d_ViewFrame::onViewRotate() 
{ MESSAGE( "Plot2d_ViewFrame::onViewRotate : NOT SUPPORTED" ); }
/*!
  View operations : Reset view
*/
void Plot2d_ViewFrame::onViewReset() 
{ MESSAGE( "Plot2d_ViewFrame::onViewReset : NOT SUPPORTED" ); }
/*!
  View operations : View front
*/
void Plot2d_ViewFrame::onViewFront() 
{ MESSAGE( "Plot2d_ViewFrame::onViewFront : NOT SUPPORTED" ); }
/*!
  View operations : View back
*/
void Plot2d_ViewFrame::onViewBack() 
{ MESSAGE( "Plot2d_ViewFrame::onViewBack : NOT SUPPORTED" ); }
/*!
  View operations : View right
*/
void Plot2d_ViewFrame::onViewRight() 
{ MESSAGE( "Plot2d_ViewFrame::onViewRight : NOT SUPPORTED" ); }
/*!
  View operations : View left
*/
void Plot2d_ViewFrame::onViewLeft() 
{ MESSAGE( "Plot2d_ViewFrame::onViewLeft : NOT SUPPORTED" ); }
/*!
  View operations : View bottom
*/
void Plot2d_ViewFrame::onViewBottom() 
{ MESSAGE( "Plot2d_ViewFrame::onViewBottom : NOT SUPPORTED" ); }
/*!
  View operations : View top
*/
void Plot2d_ViewFrame::onViewTop() 
{ MESSAGE( "Plot2d_ViewFrame::onViewTop : NOT SUPPORTED" ); }
/*!
  View operations : Show/hide trihedron
*/
void Plot2d_ViewFrame::onViewTrihedron() 
{ MESSAGE( "Plot2d_ViewFrame::onViewTrihedron : NOT SUPPORTED" ); }


//=================================================================================
// Plot2d_Plot2d implementation
//=================================================================================

/*!
  Constructor
*/
Plot2d_Plot2d::Plot2d_Plot2d( QWidget* parent )
     : QwtPlot( parent )
{
  // outline
  enableOutline( true );
  setOutlineStyle( Qwt::Triangle );
  setOutlinePen( green );
  // legend
  setAutoLegend( false );
  setLegendFrameStyle( QFrame::Box | QFrame::Sunken );
  enableLegend( false );
  // grid
  enableGridX( false );
  enableGridXMin( false );
  enableGridY( false );
  enableGridYMin( false );
  // auto scaling by default
  setAxisAutoScale( QwtPlot::yLeft );
  setAxisAutoScale( QwtPlot::xBottom );
}
/*!
  Recalculates and redraws Plot 2d view 
*/
void Plot2d_Plot2d::replot()
{
  updateLayout();  // to fix bug(?) of Qwt - view is not updated when title is changed
  QwtPlot::replot(); 
}
/*!
  Checks if two colors are close to each other [ static ]
  uses COLOR_DISTANCE variable as max tolerance for comparing of colors
*/
const long COLOR_DISTANCE = 100;
const int  MAX_ATTEMPTS   = 10;
static bool closeColors( const QColor& color1, const QColor& color2 )
{
  long tol = abs( color2.red()   - color1.red() ) + 
             abs( color2.green() - color1.green() ) +
	     abs( color2.blue()  - color1.blue() );

  return ( tol <= COLOR_DISTANCE );
}
/*!
  Gets new unique marker for item if possible
*/
void Plot2d_Plot2d::getNextMarker( QwtSymbol::Style& typeMarker, QColor& color, Qt::PenStyle& typeLine ) 
{
  bool bOk = false;
  int cnt = 1;
  while ( !bOk ) {
    int aRed    = (int)( 256.0 * random() / RAND_MAX);    // generate random color
    int aGreen  = (int)( 256.0 * random() / RAND_MAX);    // ...
    int aBlue   = (int)( 256.0 * random() / RAND_MAX);    // ...
    int aMarker = (int)( 9.0 * random() / RAND_MAX) + 1;  // 9 markers types ( not including empty )
    int aLine   = (int)( 5.0 * random() / RAND_MAX) + 1;  // 5 line types ( not including empty )

    typeMarker = ( QwtSymbol::Style )aMarker;
    color      = QColor( aRed, aGreen, aBlue );
    typeLine   = ( Qt::PenStyle )aLine;

    cnt++;
    if ( cnt == MAX_ATTEMPTS )
      bOk = true;
    else
      bOk = !existMarker( typeMarker, color, typeLine );
  }
/*
  static int aMarker = -1;
  static int aColor  = -1;
  static int aLine   = -1;

  if ( myColors.isEmpty() ) {
    // creating colors list
    myColors.append( Qt::white );
    myColors.append( Qt::blue );
    myColors.append( Qt::gray );
    myColors.append( Qt::darkGreen );
    myColors.append( Qt::magenta );
    myColors.append( Qt::darkGray );
    myColors.append( Qt::red );
    myColors.append( Qt::darkBlue );
    myColors.append( Qt::darkYellow );
    myColors.append( Qt::cyan );
    myColors.append( Qt::darkRed );
    myColors.append( Qt::darkCyan );
    myColors.append( Qt::yellow );
    myColors.append( Qt::darkMagenta );
    myColors.append( Qt::green );
    myColors.append( Qt::black );
  }

  int nbMarkers = 11;                   // QwtSymbol supports 11 marker types
  int nbLines   = 6;                    // Qt supports 6 line types
  int nbColors  = myColors.count();     // number of default colors supported

  aMarker = ( aMarker + 1 ) % nbMarkers;  
  if ( aMarker == QwtSymbol::None || aMarker == QwtSymbol::Triangle ) aMarker++;
  aColor  = ( aColor  + 1 ) % nbColors;
  aLine   = ( aLine   + 1 ) % nbLines;    
  if ( aLine == Qt::NoPen ) aLine++;             

  typeMarker = ( QwtSymbol::Style )aMarker;
  color      = myColors[ aColor ];
  typeLine   = ( Qt::PenStyle )aLine;
  if ( !existMarker( typeMarker, color, typeLine ) )
    return;

  int i, j, k;
  for ( i = 0; i < nbMarkers; i++ ) {
    aMarker = ( aMarker + 1 ) % nbMarkers;
    if ( aMarker == QwtSymbol::None || aMarker == QwtSymbol::Triangle ) aMarker++;
    for ( j = 0; j < nbColors; j++ ) {
      aColor  = ( aColor  + 1 ) % nbColors;
      for ( k = 0; k < nbLines; k++ ) {
        aLine = ( aLine + 1 ) % nbLines;
	if ( aLine == Qt::NoPen ) aLine++;             
        if ( !existMarker( ( QwtSymbol::Style )aMarker, aColor, ( Qt::PenStyle )aLine ) ) {
          typeMarker = ( QwtSymbol::Style )aMarker;
          color      = myColors[ aColor ];
          typeLine   = ( Qt::PenStyle )aLine;
          return;
        }
      }
    }
  }
*/
}
/*!
  Checks if marker belongs to any enitity
*/
bool Plot2d_Plot2d::existMarker( const QwtSymbol::Style typeMarker, const QColor& color, const Qt::PenStyle typeLine ) 
{
  // getting all curves
  QArray<long> keys = curveKeys();
  QColor aRgbColor;

  if ( closeColors( color, backgroundColor() ) )
      return true;
  for ( int i = 0; i < keys.count(); i++ ) {
    QwtPlotCurve* crv = curve( keys[i] );
    if ( crv ) {
      QwtSymbol::Style aStyle = crv->symbol().style();
      QColor           aColor = crv->pen().color();
      Qt::PenStyle     aLine  = crv->pen().style();
//      if ( aStyle == typeMarker && aColor == color && aLine == typeLine )
      if ( aStyle == typeMarker && closeColors( aColor,color ) && aLine == typeLine )
	return true;
    }
  }
  return false;
}

//==========================================================
/*!
 *  Plot2d_ViewFrame::Display
 *  Display presentation
 */
//==========================================================
void Plot2d_ViewFrame::Display( const SALOME_Prs2d* prs )
{
  // try do downcast object
  const Plot2d_Prs* aPlot2dPrs = dynamic_cast<const Plot2d_Prs*>( prs );
  if ( !aPlot2dPrs || aPlot2dPrs->IsNull() )
    return;

  // display all curves from presentation
  Plot2d_CurveContainer aCurves = aPlot2dPrs->GetObjects();
  displayCurves( aCurves );
}

//==========================================================
/*!
 *  Plot2d_ViewFrame::Erase
 *  Erase presentation
 */
//==========================================================
void Plot2d_ViewFrame::Erase( const SALOME_Prs2d* prs, const bool )
{
  // try do downcast object
  const Plot2d_Prs* aPlot2dPrs = dynamic_cast<const Plot2d_Prs*>( prs );
  if ( !aPlot2dPrs || aPlot2dPrs->IsNull() )
    return;

  // erase all curves from presentation
  Plot2d_CurveContainer aCurves = aPlot2dPrs->GetObjects();
  eraseCurves( aCurves );
}
  
//==========================================================
/*!
 *  Plot2d_ViewFrame::CreatePrs
 *  Create presentation by entry
 */
//==========================================================
SALOME_Prs* Plot2d_ViewFrame::CreatePrs( const char* entry )
{
  Plot2d_Prs* prs = new Plot2d_Prs();
  if ( entry ) {
    QIntDictIterator<Plot2d_Curve> it( myCurves );
    for ( ; it.current(); ++it ) {
      if ( it.current()->hasIO() && !strcmp( it.current()->getIO()->getEntry(), entry ) ) {
	prs->AddObject( it.current() );
      }
    }
  }
  return prs;
}

//==========================================================
/*!
 *  Plot2d_ViewFrame::BeforeDisplay
 *  Axiluary method called before displaying of objects
 */
//==========================================================
void  Plot2d_ViewFrame::BeforeDisplay( SALOME_Displayer* d )
{
  d->BeforeDisplay( this, SALOME_Plot2dViewType() );
}

//==========================================================
/*!
 *  Plot2d_ViewFrame::AfterDisplay
 *  Axiluary method called after displaying of objects
 */
//==========================================================
void  Plot2d_ViewFrame::AfterDisplay( SALOME_Displayer* d )
{
  d->AfterDisplay( this, SALOME_Plot2dViewType() );
}

#define INCREMENT_FOR_OP 10

//=======================================================================
// Plot2d_ViewFrame::onPanLeft
// Performs incremental panning to the left
//=======================================================================
void Plot2d_ViewFrame::onPanLeft()
{
  this->incrementalPan( -INCREMENT_FOR_OP, 0 );
}

//=======================================================================
// Plot2d_ViewFrame::onPanRight
// Performs incremental panning to the right
//=======================================================================
void Plot2d_ViewFrame::onPanRight()
{
  this->incrementalPan( INCREMENT_FOR_OP, 0 );
}

//=======================================================================
// Plot2d_ViewFrame::onPanUp
// Performs incremental panning to the top
//=======================================================================
void Plot2d_ViewFrame::onPanUp()
{
  this->incrementalPan( 0, -INCREMENT_FOR_OP );
}

//=======================================================================
// Plot2d_ViewFrame::onPanDown
// Performs incremental panning to the bottom
//=======================================================================
void Plot2d_ViewFrame::onPanDown()
{
  this->incrementalPan( 0, INCREMENT_FOR_OP );
}

//=======================================================================
// Plot2d_ViewFrame::onZoomIn
// Performs incremental zooming in
//=======================================================================
void Plot2d_ViewFrame::onZoomIn()
{
  this->incrementalZoom( INCREMENT_FOR_OP, INCREMENT_FOR_OP );
}

//=======================================================================
// Plot2d_ViewFrame::onZoomOut
// Performs incremental zooming out
//=======================================================================
void Plot2d_ViewFrame::onZoomOut()
{
  this->incrementalZoom( -INCREMENT_FOR_OP, -INCREMENT_FOR_OP );
}

//=======================================================================
// Plot2d_ViewFrame::incrementalPan
// Incremental zooming operation
//=======================================================================
void Plot2d_ViewFrame::incrementalPan( const int incrX, const int incrY ) {
  QwtDiMap xMap = myPlot->canvasMap( QwtPlot::xBottom );
  QwtDiMap yMap = myPlot->canvasMap( QwtPlot::yLeft );
  
  myPlot->setAxisScale( QwtPlot::yLeft,
		        myPlot->invTransform( QwtPlot::yLeft, yMap.i1()-incrY ),
		        myPlot->invTransform( QwtPlot::yLeft, yMap.i2()-incrY ) );
  myPlot->setAxisScale( QwtPlot::xBottom,
		        myPlot->invTransform( QwtPlot::xBottom, xMap.i1()-incrX ),
		        myPlot->invTransform( QwtPlot::xBottom, xMap.i2()-incrX ) );
  myPlot->replot();
}

//=======================================================================
// Plot2d_ViewFrame::incrementalZoom
// Incremental panning operation
//=======================================================================
void Plot2d_ViewFrame::incrementalZoom( const int incrX, const int incrY ) {
  QwtDiMap xMap = myPlot->canvasMap( QwtPlot::xBottom );
  QwtDiMap yMap = myPlot->canvasMap( QwtPlot::yLeft );
  
  myPlot->setAxisScale( QwtPlot::yLeft,
		       myPlot->invTransform( QwtPlot::yLeft, yMap.i1() ),
		       myPlot->invTransform( QwtPlot::yLeft, yMap.i2() + incrY ) );
  myPlot->setAxisScale( QwtPlot::xBottom,
		       myPlot->invTransform( QwtPlot::xBottom, xMap.i1() ),
		       myPlot->invTransform( QwtPlot::xBottom, xMap.i2() - incrX ) );
  myPlot->replot();
}

