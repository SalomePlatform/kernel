//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_SetupPlot2dDlg.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "Plot2d_SetupPlot2dDlg.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Tools.h"
#include "QAD_MessageBox.h"
#include "Plot2d_Curve.h"
#include "Plot2d_CurveContainer.h"

#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qscrollview.h>
#include <qlayout.h>
#include <qcolordialog.h>
#include <qspinbox.h>

#include "utilities.h"

using namespace std;

#define DLG_SIZE_WIDTH    500 
#define DLG_SIZE_HEIGHT   400
#define MAX_LINE_WIDTH    100
#define MARGIN_SIZE       11
#define SPACING_SIZE      6

/*!
  Constructor
*/
Plot2d_SetupPlot2dDlg::Plot2d_SetupPlot2dDlg( SALOMEDS::SObject_var object, QWidget* parent )
    : QDialog( parent ? parent : QAD_Application::getDesktop(), 
	       "Plot2d_SetupPlot2dDlg", 
	       true, 
	       WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  setCaption( tr("TLT_SETUP_PLOT2D") );
  setSizeGripEnabled( TRUE );
  QGridLayout* topLayout = new QGridLayout( this ); 
  topLayout->setSpacing( SPACING_SIZE );
  topLayout->setMargin( MARGIN_SIZE );

  myItems.setAutoDelete( false );

  myObject = SALOMEDS::SObject::_duplicate( object );

  /* Top scroll view */
  myView = new QScrollView( this );
  QFrame* frame  = new QFrame( myView );
  frame->setFrameStyle( QFrame::Plain | QFrame::NoFrame );
  QGridLayout* frameLayout = new QGridLayout( frame, 1, 12 );
  frameLayout->setMargin( MARGIN_SIZE ); frameLayout->setSpacing( SPACING_SIZE );
  
  QFrame* lin;

  QLabel* labAxis = new QLabel( tr( "AXIS_LBL" ),       frame );
  QLabel* labData = new QLabel( tr( "DATA_LBL" ),       frame );
  QLabel* labUnit = new QLabel( tr( "UNITS_LBL" ),      frame );
  QLabel* labAttr = new QLabel( tr( "ATTRIBUTES_LBL" ), frame );
  labAxis->setAlignment( AlignCenter );
  labData->setAlignment( AlignCenter );
  labUnit->setAlignment( AlignCenter );
  labAttr->setAlignment( AlignCenter );
  QFont font = labAxis->font(); font.setBold( true );
  labAxis->setFont( font );
  labData->setFont( font );
  labUnit->setFont( font );
  labAttr->setFont( font );

  frameLayout->addMultiCellWidget( labAxis, 0,  0, 0, 1 );
      lin = new QFrame( frame ); lin->setFrameStyle( QFrame::VLine | QFrame::Sunken );
      frameLayout->addWidget( lin,          0,     2 );
  frameLayout->addWidget( labData,          0,     3 );
      lin = new QFrame( frame ); lin->setFrameStyle( QFrame::VLine | QFrame::Sunken );
      frameLayout->addWidget( lin,          0,     4 );
  frameLayout->addWidget( labUnit,          0,     5 );
      lin = new QFrame( frame ); lin->setFrameStyle( QFrame::VLine | QFrame::Sunken );
      frameLayout->addWidget( lin,          0,     6 );
  frameLayout->addMultiCellWidget( labAttr, 0,  0, 7, 11 );
  frameLayout->setColStretch(               12, 5 );
  lin = new QFrame( frame ); lin->setFrameStyle( QFrame::HLine | QFrame::Sunken );
  frameLayout->addMultiCellWidget( lin, 1, 1, 0, 12 );

  int row = 2;
  SALOMEDS::GenericAttribute_var        anAttr;
  SALOMEDS::AttributeTableOfInteger_var tblIntAttr;
  SALOMEDS::AttributeTableOfReal_var    tblRealAttr;
  
  /* Try table of integer */
  if ( myObject->FindAttribute( anAttr, "AttributeTableOfInteger" ) ) {
    tblIntAttr = SALOMEDS::AttributeTableOfInteger::_narrow( anAttr );
    if ( !tblIntAttr->_is_nil() ) {
      try {
	int nbRows = tblIntAttr->GetNbRows() ; 
	SALOMEDS::StringSeq_var rowTitles = tblIntAttr->GetRowTitles();
	SALOMEDS::StringSeq_var rowUnits  = tblIntAttr->GetRowUnits();
	for ( int i = 0; i < nbRows; i++ ) {
	  Plot2d_ItemContainer* item = new Plot2d_ItemContainer( this );
	  item->createWidgets( frame );
	  frameLayout->addWidget( item->myHBtn,        row, 0 );
	  frameLayout->addWidget( item->myVBtn,        row, 1 );
	  frameLayout->addWidget( item->myTitleLab,    row, 3 );
	  if ( rowTitles->length() > 0 )
	    item->myTitleLab->setText( QString( rowTitles[ i ] ) );
	  frameLayout->addWidget( item->myUnitLab,     row, 5 );
	  if ( rowUnits->length() > 0 )
	    item->myUnitLab->setText( QString( rowUnits[ i ] ) );
	  frameLayout->addWidget( item->myAutoCheck,   row, 7 );
	  frameLayout->addWidget( item->myLineCombo,   row, 8 );
	  frameLayout->addWidget( item->myLineSpin,    row, 9 );
	  frameLayout->addWidget( item->myMarkerCombo, row, 10 );
	  frameLayout->addWidget( item->myColorBtn,    row, 11 );
	  connect( item, SIGNAL( horToggled( bool ) ), this, SLOT( onHBtnToggled( bool ) ) );
	  connect( item, SIGNAL( verToggled( bool ) ), this, SLOT( onVBtnToggled( bool ) ) );
	  myItems.append( item );
	  row++;
	}
      }
      catch( ... ) {
	MESSAGE("Plot2d_SetupPlot2dDlg::Plot2d_SetupPlot2dDlg : Exception has been caught (int)!!!");
      }
    }
  }
  /* Try table of real */
  else if ( myObject->FindAttribute( anAttr, "AttributeTableOfReal" ) ) {
    tblRealAttr = SALOMEDS::AttributeTableOfReal::_narrow( anAttr );
    if ( !tblRealAttr->_is_nil() ) {
      try {
	int nbRows = tblRealAttr->GetNbRows() ; 
	SALOMEDS::StringSeq_var rowTitles = tblRealAttr->GetRowTitles();
	SALOMEDS::StringSeq_var rowUnits  = tblRealAttr->GetRowUnits();
	for ( int i = 0; i < nbRows; i++ ) {
	  Plot2d_ItemContainer* item = new Plot2d_ItemContainer( this );
	  item->createWidgets( frame );
	  frameLayout->addWidget( item->myHBtn,        row, 0 );
	  frameLayout->addWidget( item->myVBtn,        row, 1 );
	  frameLayout->addWidget( item->myTitleLab,    row, 3 );
	  if ( rowTitles->length() > 0 )
	    item->myTitleLab->setText( QString( rowTitles[ i ] ) );
	  frameLayout->addWidget( item->myUnitLab,     row, 5 );
	  if ( rowUnits->length() > 0 )
	    item->myUnitLab->setText( QString( rowUnits[ i ] ) );
	  frameLayout->addWidget( item->myAutoCheck,   row, 7 );
	  frameLayout->addWidget( item->myLineCombo,   row, 8 );
	  frameLayout->addWidget( item->myLineSpin,    row, 9 );
	  frameLayout->addWidget( item->myMarkerCombo, row, 10 );
	  frameLayout->addWidget( item->myColorBtn,    row, 11 );
	  connect( item, SIGNAL( horToggled( bool ) ), this, SLOT( onHBtnToggled( bool ) ) );
	  connect( item, SIGNAL( verToggled( bool ) ), this, SLOT( onVBtnToggled( bool ) ) );
	  myItems.append( item );
	  row++;
	}
      }
      catch( ... ) {
	MESSAGE("Plot2d_SetupPlot2dDlg::Plot2d_SetupPlot2dDlg : Exception has been caught (real)!!!");
      }
    }
  }
  lin = new QFrame( frame ); lin->setFrameStyle( QFrame::VLine | QFrame::Sunken );
  frameLayout->addMultiCellWidget( lin, 2, row+1, 2, 2 );
  lin = new QFrame( frame ); lin->setFrameStyle( QFrame::VLine | QFrame::Sunken );
  frameLayout->addMultiCellWidget( lin, 2, row+1, 4, 4 );
  lin = new QFrame( frame ); lin->setFrameStyle( QFrame::VLine | QFrame::Sunken );
  frameLayout->addMultiCellWidget( lin, 2, row+1, 6, 6 );
  frameLayout->setRowStretch( row+1, 5 );

  myView->addChild( frame, 0, 0 );
  myView->setResizePolicy( QScrollView::AutoOneFit );
  
  myView->setMinimumWidth( frame->sizeHint().width() + MARGIN_SIZE * 2 );

  /* OK/Cancel buttons */
  myOkBtn = new QPushButton( tr( "BUT_OK" ), this, "buttonOk" );
  myOkBtn->setAutoDefault( TRUE );
  myOkBtn->setDefault( TRUE );
  myCancelBtn = new QPushButton( tr( "BUT_CANCEL" ), this, "buttonCancel" );
  myCancelBtn->setAutoDefault( TRUE );

  topLayout->addMultiCellWidget( myView, 0, 0, 0, 2 );
  topLayout->addWidget( myOkBtn, 1, 0 );
  topLayout->setColStretch( 1, 5 );
  topLayout->addWidget( myCancelBtn, 1, 2 );

  connect( myOkBtn,     SIGNAL( clicked() ), this, SLOT( accept() ) );
  connect( myCancelBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );

  enableControls();

  /* Center widget inside it's parent widget */
  resize( DLG_SIZE_WIDTH, DLG_SIZE_HEIGHT  );
  QAD_Tools::centerWidget( this, parentWidget() );
}
/*!
  Destructor
*/
Plot2d_SetupPlot2dDlg::~Plot2d_SetupPlot2dDlg()
{
}
/*!
  Gets curves info ( indexes of row data in the table for horizontal and verical axes )
*/
void Plot2d_SetupPlot2dDlg::getCurvesSource( int& horIndex, QValueList<int>& verIndexes )
{
  /* collecting horizontal and vertical axis items */
  horIndex = -1;
  int i, j;
  for ( i = 0; i < myItems.count(); i++ ) {
    if ( myItems.at( i )->isHorizontalOn() ) {
      horIndex = i;
    }
    else if ( myItems.at( i )->isVerticalOn() ) {
      verIndexes.append( i );
    }
  }
}
/*!
  Gets curve attributes
*/
bool Plot2d_SetupPlot2dDlg::getCurveAttributes( const int vIndex, 
						   bool&     isAuto, 
						   int&      marker, 
						   int&      line, 
						   int&      lineWidth, 
						   QColor&   color)
{
  if ( vIndex >= 0 && vIndex < myItems.count() ) {
    isAuto    = myItems.at( vIndex )->isAutoAssign();
    marker    = myItems.at( vIndex )->getMarker();
    line      = myItems.at( vIndex )->getLine();
    lineWidth = myItems.at( vIndex )->getLineWidth();
    color     = myItems.at( vIndex )->getColor();
    return true;
  }
  return false;
}
/*!
  Creates and returns curves presentations
*/
void Plot2d_SetupPlot2dDlg::getCurves( Plot2d_CurveContainer& container )
{
  SALOMEDS::GenericAttribute_var        anAttr;
  SALOMEDS::AttributeTableOfInteger_var tblIntAttr;
  SALOMEDS::AttributeTableOfReal_var    tblRealAttr;
  
  /* clearing container contents */
  container.clear();

  /* collecting horizontal and vertical axis items */
  int horIndex;
  int i, j;
  QValueList<int> verIndex;
  getCurvesSource( horIndex, verIndex );
  if ( horIndex < 0 || verIndex.isEmpty() ) /* no curves can be created */
    return;
    
  /* Try table of integer */
  if ( myObject->FindAttribute( anAttr, "AttributeTableOfInteger" ) ) {
    tblIntAttr = SALOMEDS::AttributeTableOfInteger::_narrow( anAttr );
    if ( !tblIntAttr->_is_nil() ) {
      try {
	int nbCols = tblIntAttr->GetNbColumns() ; 
	SALOMEDS::StringSeq_var rowTitles = tblIntAttr->GetRowTitles();
	SALOMEDS::StringSeq_var rowUnits  = tblIntAttr->GetRowUnits();

	for ( i = 0; i < verIndex.count(); i++ ) {
	  Plot2d_Curve* curve = new Plot2d_Curve();
	  // curve titles
	  if ( rowTitles->length() > 0 ) {
	    curve->setHorTitle( QString( rowTitles[ horIndex ] ) );
	    curve->setVerTitle( QString( rowTitles[ verIndex[i] ] ) );
	  }
	  // curve units
	  if ( rowUnits->length() > 0 ) {
	    curve->setHorUnits( QString( rowUnits[ horIndex ] ) );
	    curve->setVerUnits( QString( rowUnits[ verIndex[i] ] ) );
	  }
	  // curve data
	  int nbPoints = 0;
	  for ( j = 1; j <= nbCols; j++ ) {
	    if ( tblIntAttr->HasValue( horIndex+1, j ) && tblIntAttr->HasValue( verIndex[i]+1, j ) )
	      nbPoints++;
	  }
	  if ( nbPoints > 0 ) {
	    double* xList = new double[ nbPoints ];
	    double* yList = new double[ nbPoints ];
	    for ( j = 1; j <= nbCols; j++ ) {
	      if ( tblIntAttr->HasValue( horIndex+1, j ) && tblIntAttr->HasValue( verIndex[i]+1, j ) ) {
		xList[j-1] = tblIntAttr->GetValue( horIndex   +1, j );
		yList[j-1] = tblIntAttr->GetValue( verIndex[i]+1, j );
	      }
	    }
	    curve->setData( xList, yList, nbPoints );
	  }
	  // curve attributes
	  curve->setLine( (Plot2d_Curve::LineType)myItems.at( verIndex[i] )->getLine(), myItems.at( verIndex[i] )->getLineWidth() );
	  curve->setMarker( (Plot2d_Curve::MarkerType)myItems.at( verIndex[i] )->getMarker() );
	  curve->setColor( myItems.at( verIndex[i] )->getColor() );
	  curve->setAutoAssign( myItems.at( verIndex[i] )->isAutoAssign() );
	  // add curve into container
	  container.addCurve( curve );
	}
      }
      catch( ... ) {
	MESSAGE("Plot2d_SetupPlot2dDlg::getCurves : Exception has been caught (int)!!!");
      }
    }
  }
  /* Try table of real */
  else if ( myObject->FindAttribute( anAttr, "AttributeTableOfReal" ) ) {
    tblRealAttr = SALOMEDS::AttributeTableOfReal::_narrow( anAttr );
    if ( !tblRealAttr->_is_nil() ) {
      try {
	int nbCols = tblRealAttr->GetNbColumns() ; 
	SALOMEDS::StringSeq_var rowTitles = tblRealAttr->GetRowTitles();
	SALOMEDS::StringSeq_var rowUnits  = tblRealAttr->GetRowUnits();

	for ( i = 0; i < verIndex.count(); i++ ) {
	  Plot2d_Curve* curve = new Plot2d_Curve();
	  // curve titles
	  if ( rowTitles->length() > 0 ) {
	    curve->setHorTitle( QString( rowTitles[ horIndex ] ) );
	    curve->setVerTitle( QString( rowTitles[ verIndex[i] ] ) );
	  }
	  // curve units
	  if ( rowUnits->length() > 0 ) {
	    curve->setHorUnits( QString( rowUnits[ horIndex ] ) );
	    curve->setVerUnits( QString( rowUnits[ verIndex[i] ] ) );
	  }
	  // curve data
	  int nbPoints = 0;
	  for ( j = 1; j <= nbCols; j++ ) {
	    if ( tblRealAttr->HasValue( horIndex+1, j ) && tblRealAttr->HasValue( verIndex[i]+1, j ) )
	      nbPoints++;
	  }
	  if ( nbPoints > 0 ) {
	    double* xList = new double[ nbPoints ];
	    double* yList = new double[ nbPoints ];
	    for ( j = 1; j <= nbCols; j++ ) {
	      if ( tblRealAttr->HasValue( horIndex+1, j ) && tblRealAttr->HasValue( verIndex[i]+1, j ) ) {
		xList[j-1] = tblRealAttr->GetValue( horIndex   +1, j );
		yList[j-1] = tblRealAttr->GetValue( verIndex[i]+1, j );
	      }
	    }
	    curve->setData( xList, yList, nbPoints );
	  }
	  // curve attributes
	  curve->setLine( (Plot2d_Curve::LineType)myItems.at( verIndex[i] )->getLine(), myItems.at( verIndex[i] )->getLineWidth() );
	  curve->setMarker( (Plot2d_Curve::MarkerType)myItems.at( verIndex[i] )->getMarker() );
	  curve->setColor( myItems.at( verIndex[i] )->getColor() );
	  curve->setAutoAssign( myItems.at( verIndex[i] )->isAutoAssign() );
	  // add curve into container
	  container.addCurve( curve );
	}
      }
      catch( ... ) {
	MESSAGE("Plot2d_SetupPlot2dDlg::getCurves : Exception has been caught (real)!!!");
      }
    }
  }
}
/*!
  Slot, called when any <H> button is clicked
*/
void Plot2d_SetupPlot2dDlg::onHBtnToggled( bool on )
{
  Plot2d_ItemContainer* item = ( Plot2d_ItemContainer* )sender();
  if ( on ) {
    for ( int i = 0; i < myItems.count(); i++ ) {
      if ( myItems.at( i ) != item )
	myItems.at( i )->setHorizontalOn( false );
    }
  }
  enableControls();
}
/*!
  Slot, called when any <V> button is clicked
*/
void Plot2d_SetupPlot2dDlg::onVBtnToggled( bool on )
{
  Plot2d_ItemContainer* item = ( Plot2d_ItemContainer* )sender();
  QList<Plot2d_ItemContainer> itemList;
  itemList.setAutoDelete( false );
  int i;
  if ( on ) {
    int totalOn = 0;
    for ( i = 0; i < myItems.count(); i++ ) {
      if ( myItems.at( i ) != item && !myItems.at( i )->isHorizontalOn() ) {
	if ( myItems.at( i )->myUnitLab->text() == item->myUnitLab->text() ) {
	  if ( myItems.at( i )->isVerticalOn() )
	    totalOn++;
	  else
	    itemList.append( myItems.at( i ) );
	}
	else {
	  myItems.at( i )->setVerticalOn( false );
	}
      }
    }
    if ( totalOn == 0 && !itemList.isEmpty() && 
	 QAD_MessageBox::info2( this, 
			        this->caption(), 
			        tr( "QUE_WANT_SAME_UNITS" ),
			        tr( "BUT_YES" ), 
			        tr( "BUT_NO" ), 
			        0, 1, 1 ) == 0 ) {
      for ( i = 0; i < itemList.count(); i++ ) {
	itemList.at( i )->blockSignals( true );
	itemList.at( i )->setVerticalOn( true );
	itemList.at( i )->blockSignals( false );
      }
    }
  }
  enableControls();
}
/*!
  Enables/disables buttons 
*/
void Plot2d_SetupPlot2dDlg::enableControls()
{
  bool bHSet = false;
  bool bVSet = false;
  for ( int i = 0; i < myItems.count(); i++ ) {
    if ( myItems.at( i )->isHorizontalOn() ) {
      bHSet = true;
      break;
    }
  }
  for ( int i = 0; i < myItems.count(); i++ ) {
    if ( myItems.at( i )->isVerticalOn() )
      bVSet = true;
    myItems.at( i )->enableWidgets( bHSet && myItems.at( i )->isVerticalOn() );
  }
  myOkBtn->setEnabled( bHSet && bVSet );
}

// ====================================================================================
/*!
  Constructor
*/
Plot2d_ItemContainer::Plot2d_ItemContainer( QObject* parent, const char* name )
     : QObject( parent, name ), 
       myEnabled( true )
{
}
/*!
  Creates widgets
*/
void Plot2d_ItemContainer::createWidgets( QWidget* parentWidget )
{
  myHBtn = new QToolButton( parentWidget );
  myHBtn->setText( tr( " H " ) );
  myHBtn->setToggleButton( true );
  myHBtn->setOn( false );

  myVBtn = new QToolButton( parentWidget );
  myVBtn->setText( tr( " V " ) );
  myVBtn->setToggleButton( true );
  myVBtn->setOn( false );
  
  myTitleLab = new QLabel( parentWidget );
  myUnitLab  = new QLabel( parentWidget );
  myUnitLab->setAlignment( AlignCenter);
  
  myAutoCheck = new QCheckBox( tr( "AUTO_CHECK_LBL" ), parentWidget );
  myAutoCheck->setChecked( true );

  myLineCombo = new QComboBox( false, parentWidget );
  myLineCombo->insertItem( tr( "NONE_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "SOLID_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DASH_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DOT_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DASHDOT_LINE_LBL" ) );
  myLineCombo->insertItem( tr( "DAHSDOTDOT_LINE_LBL" ) );
  myLineCombo->setCurrentItem( 1 ); // SOLID by default

  myLineSpin = new QSpinBox( 0, MAX_LINE_WIDTH, 1, parentWidget );
  myLineSpin->setValue( 0 );        // width = 0 by default

  myMarkerCombo = new QComboBox( false, parentWidget );
  myMarkerCombo->insertItem( tr( "NONE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "CIRCLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "RECTANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "DIAMOND_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "DTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "UTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "LTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "RTRIANGLE_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "CROSS_MARKER_LBL" ) );
  myMarkerCombo->insertItem( tr( "XCROSS_MARKER_LBL" ) );
  myMarkerCombo->setCurrentItem( 1 ); // CIRCLE by default

  myColorBtn = new QToolButton( parentWidget );

  connect( myAutoCheck, SIGNAL( clicked() ),       this, SLOT( onAutoChanged() ) );
  connect( myColorBtn,  SIGNAL( clicked() ),       this, SLOT( onColorChanged() ) );
  connect( myHBtn,      SIGNAL( toggled( bool ) ), this, SLOT( onHVToggled( bool ) ) );
  connect( myVBtn,      SIGNAL( toggled( bool ) ), this, SLOT( onHVToggled( bool ) ) );
  setColor( QColor( 0, 0, 0 ) );
  updateState();
}
/*!
  Enables attributes widgets
*/
void Plot2d_ItemContainer::enableWidgets( bool enable )
{
  myEnabled = enable;
  updateState();
}
/*!
  Sets horizontal button's state on
*/
void Plot2d_ItemContainer::setHorizontalOn( bool on )
{
  myHBtn->setOn( on );
}
/*!
  Gets horizontal button's state
*/
bool Plot2d_ItemContainer::isHorizontalOn() const
{
  return myHBtn->isOn();
}
/*!
  Sets vertical button's state on
*/
void Plot2d_ItemContainer::setVerticalOn( bool on )
{
  myVBtn->setOn( on );
}
/*!
  Gets vertical button's state
*/
bool Plot2d_ItemContainer::isVerticalOn() const
{
  return myVBtn->isOn();
}
/*!
  Sets item AutoAssign flag state
*/
void Plot2d_ItemContainer::setAutoAssign( bool on )
{
  myAutoCheck->setChecked( on );
  updateState();
}
/*!
  Gets item AutoAssign flag state
*/
bool Plot2d_ItemContainer::isAutoAssign() const
{
  return myAutoCheck->isChecked();
}
/*!
  Sets item line type and width
*/
void Plot2d_ItemContainer::setLine( const int line, const int width )
{
  myLineCombo->setCurrentItem( line );
}
/*!
  Gets item line type
*/
int Plot2d_ItemContainer::getLine() const
{
  return myLineCombo->currentItem();
}
/*!
  Gets item line width
*/
int Plot2d_ItemContainer::getLineWidth() const
{
  return myLineSpin->value();
}
/*!
  Sets item marker type
*/
void Plot2d_ItemContainer::setMarker( const int marker )
{
  myMarkerCombo->setCurrentItem( marker );
}
/*!
  Gets item marker type
*/
int Plot2d_ItemContainer::getMarker() const
{
  return myMarkerCombo->currentItem();
}
/*!
  Sets item color
*/
void Plot2d_ItemContainer::setColor( const QColor& color )
{
  QPalette pal = myColorBtn->palette();
  QColorGroup ca = pal.active();
  ca.setColor( QColorGroup::Button, color );
  QColorGroup ci = pal.inactive();
  ci.setColor( QColorGroup::Button, color );
  pal.setActive( ca );
  pal.setInactive( ci );
  myColorBtn->setPalette( pal );
}
/*!
  Gets item color
*/
QColor Plot2d_ItemContainer::getColor() const
{
  return myColorBtn->palette().active().button();
}
/*!
  Enables/disables widgets
*/
void Plot2d_ItemContainer::updateState()
{
  myAutoCheck->setEnabled( myEnabled );
  myLineCombo->setEnabled( myEnabled && !myAutoCheck->isChecked() ); 
  myLineSpin->setEnabled( myEnabled && !myAutoCheck->isChecked() ); 
  myMarkerCombo->setEnabled( myEnabled && !myAutoCheck->isChecked() ); 
  myColorBtn->setEnabled( myEnabled && !myAutoCheck->isChecked() ); 
}
/*!
  Slot, called when user clickes <Auto assign> check box
*/
void Plot2d_ItemContainer::onAutoChanged()
{
  updateState();
  emit( autoClicked() );
}
/*!
  <Color> button slot, invokes color selection dialog box
*/
void Plot2d_ItemContainer::onColorChanged()
{
  QColor color = QColorDialog::getColor( getColor() );
  if ( color.isValid() ) {
    setColor( color );
  }
}
/*!
  <H> and <V> buttons slot
*/
void Plot2d_ItemContainer::onHVToggled( bool on )
{
  const QObject* snd = sender();
  if ( snd == myHBtn ) {
    if ( on ) {
      if ( myVBtn->isOn() ) {
//	blockSignals( true );
	myVBtn->setOn( false );
//	blockSignals( false );
      }
    }
    emit horToggled( on );
  }
  else {
    if ( on ) {
      if ( myHBtn->isOn() ) {
//	blockSignals( true );
	myHBtn->setOn( false );
//	blockSignals( false );
      }
    }
    emit verToggled( on );
  }
}





