//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_TableDlg.cxx
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_TableDlg.h"
#include "QAD_Tools.h"
#include "QAD_MessageBox.h"
#include <qlayout.h>
#include <qvalidator.h>
#include <qtable.h>
#include <qtabwidget.h>
#include <qvaluelist.h>
#include <qmemarray.h>
#include <qinputdialog.h>
#include <qlabel.h>
#include "utilities.h"
using namespace std;

#define MARGIN_SIZE       11
#define SPACING_SIZE      6
#define SPACER_SIZE       5
#define MIN_TABLE_WIDTH   200
#define MIN_TABLE_HEIGHT  200


class SALOMEGUI_Table : public QTable {
public:
  SALOMEGUI_Table( Orientation orient, QWidget* parent = 0, const char* name = 0 ) 
    : QTable( parent, name ), myValidator( 0 ), myOrientation( orient ) {}
  SALOMEGUI_Table( Orientation orient, int numRows, int numCols, QWidget* parent = 0, const char* name = 0 )
    : QTable( numRows, numCols, parent, name ), myValidator( 0 ), myOrientation( orient ) {}
  
  void setValidator( QValidator* v = 0 ) { myValidator = v;  }
  bool isEditing() const { return QTable::isEditing(); }
  
protected:
  QWidget* createEditor ( int row, int col, bool initFromCell ) const
    {
      bool testUnits = ( myOrientation == Horizontal && col == 0 ) || ( myOrientation == Vertical && row == 0 );
      QWidget* wg = QTable::createEditor( row, col, initFromCell );
      if ( wg && wg->inherits("QLineEdit") && myValidator && !testUnits ) 
	(( QLineEdit*)wg)->setValidator( myValidator );
      return wg;
    }

protected:
  QValidator* myValidator;
  Orientation myOrientation;
};

/*!
  Constructor
*/
SALOMEGUI_TableDlg::SALOMEGUI_TableDlg( QWidget* parent, 
				        SALOMEDS::SObject_var obj, 
				        bool edit,
				        int which,
				        Orientation orient,
				        bool showColumnTitles )
     : QDialog( parent, "", false, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu | WDestructiveClose),
       myIntTable( 0 ), myRealTable( 0 )
{
  setCaption( edit ? tr( "EDIT_TABLE_TLT" ) : tr( "VIEW_TABLE_TLT" ) );
  setSizeGripEnabled( true );

  myObject = SALOMEDS::SObject::_duplicate( obj );
  bool bHasIntTable = false;
  bool bHasRealTable = false;
  if ( !myObject->_is_nil() ) {
    SALOMEDS::GenericAttribute_var anAttr;
    bHasIntTable  = myObject->FindAttribute( anAttr, "AttributeTableOfInteger");
    bHasRealTable = myObject->FindAttribute( anAttr, "AttributeTableOfReal");
  }
  
  QVBoxLayout* mainLayout = new QVBoxLayout( this );
  mainLayout->setMargin( MARGIN_SIZE );
  mainLayout->setSpacing( SPACING_SIZE );

  bool bDoInt  = which == ttInt  || which == ttBoth || which == ttAuto && bHasIntTable;
  bool bDoReal = which == ttReal || which == ttBoth || which == ttAuto && bHasRealTable;

  QWidget* top;
  QVBoxLayout* tl;
  if ( bDoInt && bDoReal ) {
    top = new QTabWidget( this, "TabWidget" );
    ( ( QTabWidget* ) top) ->setMargin( MARGIN_SIZE );
  }
  else {
    top = new QWidget( this, "DumbWidget" );
    tl  = new QVBoxLayout( top ); tl->setMargin( 0 ); tl->setSpacing( SPACING_SIZE );
  }

  if ( bDoInt ) {
    myIntTable = new SALOMEGUI_TableWidget( top, "myIntTable", edit, orient, showColumnTitles );
    myIntTable->getTable()->setValidator( new QIntValidator( this ) );
    if ( bDoInt && bDoReal )
      ( ( QTabWidget* )top )->addTab( myIntTable, tr( "TABLE_OF_INTEGER_TLT" ) );
    else
      tl->addWidget( myIntTable );
  }
  if ( bDoReal ) {
    myRealTable = new SALOMEGUI_TableWidget( top, "myRealTable", edit, orient, showColumnTitles );
    myRealTable->getTable()->setValidator( new QDoubleValidator( this ) );
    if ( bDoInt && bDoReal )
      ( ( QTabWidget* )top )->addTab( myRealTable, tr( "TABLE_OF_REAL_TLT" ) );
    else
      tl->addWidget( myRealTable );
  }
  if ( !bDoInt && !bDoReal ) {
    QLabel *dumbLabel = new QLabel( tr( "ERR_TABLE_NOT_AVAILABLE" ), top, "DumbLabel" );
    dumbLabel->setAlignment( AlignCenter );
    tl->addWidget( dumbLabel );
  }

  QHBoxLayout* btnLayout = new QHBoxLayout; 
  btnLayout->setMargin( 0 ); btnLayout->setSpacing( SPACING_SIZE );
  
  myOKBtn = new QPushButton( tr( "BUT_OK" ), this );
  if ( edit ) {
    myCancelBtn = new QPushButton( tr( "BUT_CANCEL" ), this );
    btnLayout->addWidget( myOKBtn );
    btnLayout->addItem( new QSpacerItem( SPACER_SIZE, SPACER_SIZE, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
    btnLayout->addWidget( myCancelBtn );
    connect( myOKBtn,     SIGNAL( clicked() ), this, SLOT( onOK() ) );
    connect( myCancelBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );
  }
  else {
    btnLayout->addItem( new QSpacerItem( SPACER_SIZE, SPACER_SIZE, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
    btnLayout->addWidget( myOKBtn );
    btnLayout->addItem( new QSpacerItem( SPACER_SIZE, SPACER_SIZE, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
    connect( myOKBtn,     SIGNAL( clicked() ), this, SLOT( accept() ) );
  }

  mainLayout->addWidget( top );
  mainLayout->addLayout( btnLayout );

  initDlg();
  resize( 500, 400 );
  QAD_Tools::centerWidget( this, parent );
}

/*!
  Destructor
*/
SALOMEGUI_TableDlg::~SALOMEGUI_TableDlg()
{
}

/*!
  <OK> button slot, saves table(s)
  Called only in create/edit mode ( <edit> parameter for constructor is true )
*/
void SALOMEGUI_TableDlg::onOK()
{
  myOKBtn->setFocus(); // accept possible changes
  bool done = true;

  if ( !myObject->_is_nil() ) {
    SALOMEDS::Study_var study = myObject->GetStudy();
    SALOMEDS::GenericAttribute_var        anAttr;
    SALOMEDS::AttributeTableOfInteger_var tblIntAttr;
    SALOMEDS::AttributeTableOfReal_var    tblRealAttr;

    if ( !study->_is_nil() ) {
      SALOMEDS::StudyBuilder_var builder = study->NewBuilder();
      builder->NewCommand(); // start transaction !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      try {
	if ( myIntTable ) {
	  builder->RemoveAttribute( myObject, "AttributeTableOfInteger" );
	  tblIntAttr = SALOMEDS::AttributeTableOfInteger::_narrow( 
			 builder->FindOrCreateAttribute( myObject, "AttributeTableOfInteger" ) );

	  int i;
	  int nbRows  = myIntTable->getNumRows();
	  int nbCols  = myIntTable->getNumCols();
	  QString tlt = myIntTable->getTableTitle();
	  QStringList rowTitles, colTitles, units;
	  myIntTable->getRowTitles( rowTitles );
	  myIntTable->getColTitles( colTitles );
	  myIntTable->getUnits( units );
	  
	  if ( nbRows > 0) {
	    // data
	    int nRow = 0;
	    tblIntAttr->SetNbColumns( nbCols );
	    for ( i = 0; i < nbRows; i++ ) {
	      QStringList data;
	      myIntTable->getRowData( i, data );
	      bool bEmptyRow = true;
	      for ( int j = 0; j < data.count(); j++ ) {
		if ( !data[ j ].isNull() ) {
		  tblIntAttr->PutValue( data[ j ].toInt(), nRow+1, j+1 );
		  bEmptyRow = false;
		}
	      }
	      if ( !bEmptyRow ) {  // Skip rows with no data !!!
		// set row title
		tblIntAttr->SetRowTitle( nRow+1, rowTitles[ i ].isNull() ? strdup( "" ) : strdup( rowTitles[ i ] ) ); 
		// set row unit
		tblIntAttr->SetRowUnit( nRow+1, units[ i ].isNull() ? strdup( "" ) : strdup( units[ i ] ) ); 
		nRow++;
	      }
	    }
	    if ( nRow > 0 ) { // Set columns only if table is not empty, otherwise exception is raised !!!
	      // column titles
	      for ( i = 0; i < colTitles.count(); i++ )
		tblIntAttr->SetColumnTitle( i+1, colTitles[ i ].isNull() ? strdup( "" ) : strdup( colTitles[ i ] ) );
	    }
	  }
	  // title
	  tblIntAttr->SetTitle( myIntTable->getTableTitle().latin1() );
	}
	if ( myRealTable ) {
	  builder->RemoveAttribute( myObject, "AttributeTableOfReal" );
	  tblRealAttr = SALOMEDS::AttributeTableOfReal::_narrow( 
			 builder->FindOrCreateAttribute( myObject, "AttributeTableOfReal" ) );

	  int i;
	  int nbRows  = myRealTable->getNumRows();
	  int nbCols  = myRealTable->getNumCols();
	  QString tlt = myRealTable->getTableTitle();
	  QStringList rowTitles, colTitles, units;
	  myRealTable->getRowTitles( rowTitles );
	  myRealTable->getColTitles( colTitles );
	  myRealTable->getUnits( units );
	  
	  if ( nbRows > 0) {
	    // data
	    int nRow = 0;
	    tblRealAttr->SetNbColumns( nbCols );
	    for ( i = 0; i < nbRows; i++ ) {
	      QStringList data;
	      myRealTable->getRowData( i, data );
	      bool bEmptyRow = true;
	      for ( int j = 0; j < data.count(); j++ ) {
		if ( !data[ j ].isNull() ) {
		  tblRealAttr->PutValue( data[ j ].toDouble(), nRow+1, j+1 );
		  bEmptyRow = false;
		}
	      }
	      if ( !bEmptyRow ) {  // Skip rows with no data !!!
		// set row title
		tblRealAttr->SetRowTitle( nRow+1, rowTitles[ i ].isNull() ? strdup( "" ) : strdup( rowTitles[ i ] ) ); 
		// set row unit
		tblRealAttr->SetRowUnit( nRow+1, units[ i ].isNull() ? strdup( "" ) : strdup( units[ i ] ) ); 
		nRow++;
	      }
	    }
	    if ( nRow > 0 ) { // Set columns only if table is not empty, otherwise exception is raised !!!
	      // column titles
	      for ( i = 0; i < colTitles.count(); i++ )
		tblRealAttr->SetColumnTitle( i+1, colTitles[ i ].isNull() ? strdup( "" ) : strdup( colTitles[ i ] ) );
	    }
	  }
	  // title
	  tblRealAttr->SetTitle( myRealTable->getTableTitle().latin1() );
	}
	if ( myIntTable || myRealTable)
	  builder->CommitCommand(); // commit transaction !!!!!!!!!!!!!!!!!!!!!!!!!!!
	else
	  builder->AbortCommand();  // abort transaction  !!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
      catch( ... ) {
	MESSAGE("SALOMEGUI_TableDlg::onOK : Exception has been caught !!!");
	builder->AbortCommand();  // abort transaction  !!!!!!!!!!!!!!!!!!!!!!!!!!!
	done = false;
	QAD_MessageBox::error1 ( this, tr("ERR_ERROR"), tr("ERR_APP_EXCEPTION"), tr ("BUT_OK") );
      }
    }
  }
  if ( done ) 
    accept();
}

/*!
   Populates table with data
*/
void SALOMEGUI_TableDlg::initDlg()
{
  int i, j;
  if ( !myObject->_is_nil() ) {
    SALOMEDS::GenericAttribute_var anAttr;
    SALOMEDS::AttributeTableOfInteger_var tblIntAttr;
    SALOMEDS::AttributeTableOfReal_var    tblRealAttr;
    if ( myObject->FindAttribute( anAttr, "AttributeTableOfInteger") ) {
      tblIntAttr = SALOMEDS::AttributeTableOfInteger::_narrow( anAttr );
    }
    if ( myObject->FindAttribute( anAttr, "AttributeTableOfReal") ) {
      tblRealAttr = SALOMEDS::AttributeTableOfReal::_narrow( anAttr );
    }
    // Table of integer
    if ( !tblIntAttr->_is_nil() && myIntTable ) {
      try {
	SALOMEGUI_Table* tbl = myIntTable->getTable();
	// title
	myIntTable->setTableTitle( strdup( tblIntAttr->GetTitle() ) );
	// nb of rows & cols
	int nbRows = tblIntAttr->GetNbRows() ; 
	int nbCols = tblIntAttr->GetNbColumns();
	myIntTable->setNumRows( nbRows );
	myIntTable->setNumCols( nbCols );
	// rows titles
	QStringList strlist;
	SALOMEDS::StringSeq_var rowTitles = tblIntAttr->GetRowTitles();
	for ( i = 0; i < nbRows; i++ ) {
	  if ( rowTitles->length() > 0 )
	    strlist.append( strdup( rowTitles[i] ) );
	  else
	    strlist.append( "" );
	}
	myIntTable->setRowTitles( strlist );
	// columns titles
	strlist.clear();
	SALOMEDS::StringSeq_var colTitles = tblIntAttr->GetColumnTitles();
	for ( i = 0; i < nbCols; i++ ) {
	  if ( colTitles->length() > 0 )
	    strlist.append( strdup( colTitles[i] ) );
	  else
	    strlist.append( "" );
	}
	myIntTable->setColTitles( strlist );
	// units
	strlist.clear();
	SALOMEDS::StringSeq_var rowUnits = tblIntAttr->GetRowUnits();
	if ( rowUnits->length() > 0 ) {
	  for ( i = 0; i < nbRows; i++ )
	    strlist.append( strdup( rowUnits[i] ) );
	  myIntTable->setUnits( strlist );
	}
	// data
	for ( i = 1; i <= nbRows; i++ ) {
	  strlist.clear();
	  for ( j = 1; j <= nbCols; j++ ) {
	    if ( tblIntAttr->HasValue( i, j ) )
	      strlist.append( QString::number( tblIntAttr->GetValue( i, j ) ) );
	    else
	      strlist.append( QString::null );
	  }
	  myIntTable->setRowData( i-1, strlist );
	}
	myIntTable->adjustTable();
      }
      catch( ... ) {
	MESSAGE("SALOMEGUI_TableDlg::initDlg : Exception has been caught !!!");
      }
    } 
    // Table of real
    if ( !tblRealAttr->_is_nil() && myRealTable ) {
      try {
	SALOMEGUI_Table* tbl = myRealTable->getTable();
	// title
	myRealTable->setTableTitle( strdup( tblRealAttr->GetTitle() ) );
	// nb of rows & cols
	int nbRows = tblRealAttr->GetNbRows() ; 
	int nbCols = tblRealAttr->GetNbColumns();
	myRealTable->setNumRows( nbRows );
	myRealTable->setNumCols( nbCols );
	// rows titles
	QStringList strlist;
	SALOMEDS::StringSeq_var rowTitles = tblRealAttr->GetRowTitles();
	for ( i = 0; i < nbRows; i++ ) {
	  if ( rowTitles->length() > 0 )
	    strlist.append( strdup( rowTitles[i] ) );
	  else
	    strlist.append( "" );
	}
	myRealTable->setRowTitles( strlist );
	// columns titles
	strlist.clear();
	SALOMEDS::StringSeq_var colTitles = tblRealAttr->GetColumnTitles();
	for ( i = 0; i < nbCols; i++ ) {
	  if ( colTitles->length() > 0 )
	    strlist.append( strdup( colTitles[i] ) );
	  else
	    strlist.append( "" );
	}
	myRealTable->setColTitles( strlist );
	// units
	strlist.clear();
	SALOMEDS::StringSeq_var rowUnits = tblRealAttr->GetRowUnits();
	if ( rowUnits->length() > 0 ) {
	  for ( i = 0; i < nbRows; i++ )
	    strlist.append( strdup( rowUnits[i] ) );
	  myRealTable->setUnits( strlist );
	}
        // data
	for ( i = 1; i <= nbRows; i++ ) {
	  strlist.clear();
	  for ( j = 1; j <= nbCols; j++ ) {
	    if ( tblRealAttr->HasValue( i, j ) )
	      strlist.append( QString::number( tblRealAttr->GetValue( i, j ) ) );
	    else
	      strlist.append( QString::null );
	  }
	  myRealTable->setRowData( i-1, strlist );
	}
	myRealTable->adjustTable();
      }
      catch( ... ) {
	MESSAGE("SALOMEGUI_TableDlg::initDlg : Exception has been caught !!!");
      }
    } 
  }
}

/*!
  Constructor
*/
SALOMEGUI_TableWidget::SALOMEGUI_TableWidget( QWidget* parent, 
					      const char* name, 
					      bool edit, 
					      Orientation orient,
					      bool showColumnTitles )
     : QWidget( parent, name ), myOrientation( orient )
{
  QGridLayout* mainLayout = new QGridLayout( this );
  mainLayout->setMargin( 0 );
  mainLayout->setSpacing( SPACING_SIZE );

  myTitleEdit = new QLineEdit( this, "TitleEdit" );
  myTitleEdit->setAlignment( AlignCenter );
  myTitleEdit->setReadOnly( !edit );
  QFont fnt = myTitleEdit->font();
  fnt.setBold( true ); 
  myTitleEdit->setFont( fnt );

  myTable = new SALOMEGUI_Table( orient, this, "Table" );
  myTable->setNumRows( 5 );
  myTable->setNumCols( 5 );
  myTable->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
  myTable->setMinimumSize( MIN_TABLE_WIDTH, MIN_TABLE_HEIGHT );
  myTable->setSelectionMode( QTable::Single );
  myTable->setShowGrid( true );
  myTable->setColumnMovingEnabled( false );
  myTable->setRowMovingEnabled( false );
  myTable->setReadOnly( !edit );
  myTable->setDragEnabled( false );
  setUnitsTitle( tr( "UNITS_TLT" ) );

  if ( !showColumnTitles ) {
    if ( myOrientation == Horizontal ) {
      myTable->horizontalHeader()->hide();
      myTable->setTopMargin( 0 );
    }
    else {
      myTable->verticalHeader()->hide();
      myTable->setLeftMargin( 0 );
    }
  }

  mainLayout->addWidget( myTitleEdit, 0, 0 );
  mainLayout->addWidget( myTable, 1, 0 );

  if ( edit ) {
    myAddRowBtn    = new QPushButton( tr( "ADD_ROW_BTN" ), this, "AddRowBtn" );
    myDelRowBtn    = new QPushButton( tr( "REMOVE_ROW_BTN" ), this, "DelRowBtn" );
    myAddColBtn    = new QPushButton( tr( "ADD_COLUMN_BTN" ), this, "AddColBtn" );
    myDelColBtn    = new QPushButton( tr( "REMOVE_COLUMN_BTN" ), this, "DelColBtn" );
    myAdjustBtn    = new QPushButton( tr( "ADJUST_CELLS_BTN" ), this, "AdjustBtn" );
    mySelectAllBtn = new QPushButton( tr( "SELECT_ALL_BTN" ), this, "SelectAllBtn" );
    myClearBtn     = new QPushButton( tr( "CLEAR_BTN"), this, "ClearBtn" );
    QVBoxLayout* btnLayout = new QVBoxLayout; btnLayout->setMargin( 0 ); btnLayout->setSpacing( SPACING_SIZE );
    btnLayout->addWidget( myAddRowBtn );
    btnLayout->addWidget( myDelRowBtn );
    btnLayout->addWidget( myAddColBtn );
    btnLayout->addWidget( myDelColBtn );
    btnLayout->addStretch();
    btnLayout->addWidget( myAdjustBtn );
    btnLayout->addStretch();
    btnLayout->addWidget( mySelectAllBtn );
    btnLayout->addWidget( myClearBtn );
    mainLayout->addLayout( btnLayout, 1, 1 );
    connect( myTable, SIGNAL( selectionChanged() ),        this, SLOT( updateButtonsState() ) );
    connect( myTable, SIGNAL( currentChanged( int, int) ), this, SLOT( updateButtonsState() ) );
    connect( myAddRowBtn,    SIGNAL( clicked() ),   this, SLOT( addRow() ) );
    connect( myAddColBtn,    SIGNAL( clicked() ),   this, SLOT( addCol() ) );
    connect( myDelRowBtn,    SIGNAL( clicked() ),   this, SLOT( delRow() ) );
    connect( myDelColBtn,    SIGNAL( clicked() ),   this, SLOT( delCol() ) );
    connect( myAdjustBtn,    SIGNAL( clicked() ),   this, SLOT( adjustTable() ) );
    connect( mySelectAllBtn, SIGNAL( clicked() ),   this, SLOT( selectAll() ) );
    connect( myClearBtn,     SIGNAL( clicked() ),   this, SLOT( clearTable() ) );
    myTable->horizontalHeader()->installEventFilter( this );
    myTable->verticalHeader()->installEventFilter( this );
    myTable->installEventFilter( this );
  }
  updateButtonsState();
}
/*!
  Destructor
*/
SALOMEGUI_TableWidget::~SALOMEGUI_TableWidget()
{
}
/*!
  Sets table title
*/
void SALOMEGUI_TableWidget::setTableTitle( const QString& title )
{
  myTitleEdit->setText( title );
}
/*!
  Gets table title
*/
QString SALOMEGUI_TableWidget::getTableTitle()
{
  return myTitleEdit->text();
}
/*!
  Sets total number of rows
*/
void SALOMEGUI_TableWidget::setNumRows( const int num )
{
  myOrientation == Horizontal ? myTable->setNumRows( num ) : myTable->setNumCols( num );
}
/*!
  Gets total number of rows
*/
int SALOMEGUI_TableWidget::getNumRows()
{
  return myOrientation == Horizontal ? myTable->numRows() : myTable->numCols();
}
/*!
  Sets total number of columns
*/
void SALOMEGUI_TableWidget::setNumCols( const int num )
{
  // !!! first column contains units !!!
  myOrientation == Horizontal ? myTable->setNumCols( num+1 ) : myTable->setNumRows( num+1 );
//  myOrientation == Horizontal ? myTable->setColumnReadOnly( 0, true ) : myTable->setRowReadOnly( 0, true );
}
/*!
  Gets total number of columns
*/
int SALOMEGUI_TableWidget::getNumCols()
{
  // !!! first column contains units !!!
  return myOrientation == Horizontal ? myTable->numCols()-1 : myTable->numRows()-1;
}
/*!
  Sets rows titles
*/
void SALOMEGUI_TableWidget::setRowTitles( QStringList& tlts )
{
  for ( int i = 0; i < tlts.count(); i++ ) {
    myOrientation == Horizontal ? 
      myTable->verticalHeader()->setLabel( i, tlts[i] ) : 
      myTable->horizontalHeader()->setLabel( i, tlts[i] );
  }
}
/*!
  Gets rows titles
*/
void SALOMEGUI_TableWidget::getRowTitles( QStringList& tlts )
{
  tlts.clear();
  if ( myOrientation == Horizontal ) {
    for ( int i = 0; i < myTable->numRows(); i++ ) {
      tlts.append( myTable->verticalHeader()->label( i ) );
    }
  }
  else {
    for ( int i = 0; i < myTable->numCols(); i++ ) {
      tlts.append( myTable->horizontalHeader()->label( i ) );
    }
  }
}
/*!
  Sets columns titles
*/
void SALOMEGUI_TableWidget::setColTitles( QStringList& tlts )
{
  // !!! first column contains units !!!
  for ( int i = 0; i < tlts.count(); i++ ) {
    myOrientation == Horizontal ? 
      myTable->horizontalHeader()->setLabel( i+1, tlts[i] ) :
      myTable->verticalHeader()->setLabel( i+1, tlts[i] );
  }
}
/*!
  Sets columns titles
*/
void SALOMEGUI_TableWidget::getColTitles( QStringList& tlts )
{
  // !!! first column contains units !!!
  tlts.clear();
  if ( myOrientation == Horizontal ) {
    for ( int i = 1; i < myTable->numCols(); i++ ) {
      tlts.append( myTable->horizontalHeader()->label( i ) );
    }
  }
  else {
    for ( int i = 1; i < myTable->numRows(); i++ ) {
      tlts.append( myTable->verticalHeader()->label( i ) );
    }
  }
}
/*!
  Sets units title
*/
void SALOMEGUI_TableWidget::setUnitsTitle( const QString& tlt ) {
  // !!! first column contains units !!!
  myOrientation == Horizontal ? myTable->horizontalHeader()->setLabel( 0, tlt ) : myTable->verticalHeader()->setLabel( 0, tlt );
}
/*!
  Sets units
*/
void SALOMEGUI_TableWidget::setUnits( QStringList& units )
{
  for ( int i = 0; i < units.count(); i++ ) {
    myOrientation == Horizontal ? myTable->setText( i, 0, units[i] ) : myTable->setText( 0, i, units[i] );
  }
}
/*!
  Gets units
*/
void SALOMEGUI_TableWidget::getUnits( QStringList& units )
{
  units.clear();
  if ( myOrientation == Horizontal ) {
    for ( int i = 0; i < myTable->numRows(); i++ )
      units.append( myTable->text( i, 0 ).isNull() ? QString("") : myTable->text( i, 0 ) );
  }
  else {
    for ( int i = 0; i < myTable->numCols(); i++ )
      units.append( myTable->text( 0, i ).isNull() ? QString("") : myTable->text( 0, i ) );
  }
}
/*!
  Sets row data
*/
void SALOMEGUI_TableWidget::setRowData( int row, QStringList& data )
{
  if ( row >= 0 && row < getNumRows() ) {
    for ( int i = 0; i < data.count(); i++ ) {
      if ( data[i].isNull() ) {
	myOrientation == Horizontal ? myTable->clearCell( row, i+1 ) :
	                              myTable->clearCell( i+1, row );
      }
      else {
	myOrientation == Horizontal ? myTable->setText( row, i+1, data[i] ) :
	                              myTable->setText( i+1, row, data[i] );
      }
    }
  }
}
/*!
  Gets row data
*/
void SALOMEGUI_TableWidget::getRowData( int row, QStringList& data )
{
  data.clear();
  if ( row >= 0 && row < getNumRows() ) {
    if ( myOrientation == Horizontal ) {
      for ( int i = 1; i < myTable->numCols(); i++ )
	data.append( myTable->text( row, i ) );
    }
    else {
      for ( int i = 1; i < myTable->numRows(); i++ )
	data.append( myTable->text( i, row ) );
    }
  }
}
/*!
  Adjusts table cell to see contents, <Adjust Cells> button slot
*/
void SALOMEGUI_TableWidget::adjustTable()
{
  int i;
  for ( i = 0; i < myTable->numRows(); i++ )
    myTable->adjustRow( i );
  for ( i = 0; i < myTable->numCols(); i++ )
    myTable->adjustColumn( i );
}
/*!
  Called when selection changed in table
*/
void SALOMEGUI_TableWidget::updateButtonsState()
{
  if ( myTable->isReadOnly() )
    return;
  bool bDR = false; // <Delete Row(s)>
  bool bDC = false; // <Delete Column(s)>
  bool bSA = false; // <Select All>
  bool bCT = false; // <Clear>
  int i;
  int c = myOrientation == Horizontal ? 0 : 1;
  for ( i = c; i < myTable->numRows(); i++ ) {
    if ( myTable->isRowSelected( i, true ) )
      bDR = true;
    else 
      bSA = true;
  }
  c = myOrientation == Horizontal ? 1 : 0;
  for ( i = c; i < myTable->numCols(); i++ ) {
    if ( myTable->isColumnSelected( i, true ) )
      bDC = true;
    else 
      bSA = true;
  }
  int nbSel = myTable->numSelections();
  for ( i = 0; i < nbSel; i++ ) {
    QTableSelection ts = myTable->selection( i );
    for ( int j = ts.topRow(); j < ts.bottomRow()+1; j++) {
      for ( int k = ts.leftCol(); k < ts.rightCol()+1; k++) {
	if ( myTable->item( j, k ) )
	  bCT = true;
      }
    }
  }
  if ( myTable->item( myTable->currentRow(), myTable->currentColumn() ) )
    bCT = true;
  myDelRowBtn->setEnabled( bDR );
  myDelColBtn->setEnabled( bDC );
  mySelectAllBtn->setEnabled( bSA );
  myClearBtn->setEnabled( bCT );
}
/*!
  <Add row> button slot
*/
void SALOMEGUI_TableWidget::addRow()
{
  myTable->insertRows( myTable->numRows(), 1 );
  updateButtonsState();
}
/*!
  <Add column> button slot
*/
void SALOMEGUI_TableWidget::addCol()
{
  myTable->insertColumns( myTable->numCols(), 1 );
  updateButtonsState();
}
/*!
  <Delete row(s)> button slot
*/
void SALOMEGUI_TableWidget::delRow()
{
  int c = myOrientation == Horizontal ? 0 : 1;
  QValueList<int> il;
  int i;
  for ( i = c; i < myTable->numRows(); i++ )
    if ( myTable->isRowSelected( i, true ) )
      il.append( i );
  if ( il.count() > 0 ) {
    QMemArray<int> ildel( il.count() );
    for ( i = 0; i < il.count(); i++ )
      ildel[ i ] = il[ i ];
    myTable->removeRows( ildel );
  }
  updateButtonsState();
}
/*!
  <Delete column(s)> button slot
*/
void SALOMEGUI_TableWidget::delCol()
{
  int c = myOrientation == Horizontal ? 1 : 0;
  QValueList<int> il;
  int i;
  for ( i = c; i < myTable->numCols(); i++ )
    if ( myTable->isColumnSelected( i, true ) )
      il.append( i );
  if ( il.count() > 0 ) {
    QMemArray<int> ildel( il.count() );
    for ( i = 0; i < il.count(); i++ )
      ildel[ i ] = il[ i ];
    myTable->removeColumns( ildel );
  }
  updateButtonsState();
}
/*!
  <Select All> button slot
*/
void SALOMEGUI_TableWidget::selectAll()
{
  myTable->clearSelection();
  QTableSelection ts;
  ts.init( 0, 0 ); ts.expandTo( myTable->numRows()-1, myTable->numCols()-1 );
  myTable->addSelection( ts );
  updateButtonsState();
}
/*!
  <Clear> button slot
*/
void SALOMEGUI_TableWidget::clearTable()
{
  int nbSel = myTable->numSelections();
  for ( int i = 0; i < nbSel; i++ ) {
    QTableSelection ts = myTable->selection( i );
    for ( int j = ts.topRow(); j < ts.bottomRow()+1; j++) {
      if ( myOrientation == Vertical && j == 0 ) {
//	continue;      // UNITS
      }
      for ( int k = ts.leftCol(); k < ts.rightCol()+1; k++) {
	if ( myOrientation == Horizontal && k == 0 ) {
//	  continue;   // UNITS
	}
	myTable->clearCell( j, k );
      }
    }
  }
  if ( nbSel == 0 )
    myTable->clearCell( myTable->currentRow(), myTable->currentColumn() );
  myTable->clearSelection();
  updateButtonsState();
}
/*!
  Event filter - handles titles editing
*/
bool SALOMEGUI_TableWidget::eventFilter( QObject* o, QEvent* e )
{
  if ( e->type() == QEvent::MouseButtonDblClick) {
    QMouseEvent* me = ( QMouseEvent* )e;
    if ( me->button() == LeftButton && !myTable->isReadOnly() ) {
      if ( o == myTable->horizontalHeader() ) {
	for ( int i = 0; i < myTable->horizontalHeader()->count(); i++ ) {
	  QRect rect = myTable->horizontalHeader()->sectionRect( i );
	  rect.addCoords( 1, 1, -1, -1 );
	  if ( rect.contains( myTable->horizontalHeader()->mapFromGlobal( me->globalPos() ) ) ) {
	    if ( myOrientation == Vertical || i != 0 ) {
	      bool bOk;
	      QString tlt = QInputDialog::getText( tr( "SET_TITLE_TLT" ), 
						   tr( "TITLE_LBL" ),
						   QLineEdit::Normal,
						   myTable->horizontalHeader()->label( i ),
						   &bOk,
						   this );
	      if ( bOk && !tlt.isNull() )
		myTable->horizontalHeader()->setLabel( i, tlt );
	      break;
	    }
	  }
	}
      }
      if ( o == myTable->verticalHeader() ) {
	for ( int i = 0; i < myTable->verticalHeader()->count(); i++ ) {
	  QRect rect = myTable->verticalHeader()->sectionRect( i );
	  rect.addCoords( 1, 1, -1, -1 );
	  if ( rect.contains( myTable->verticalHeader()->mapFromGlobal( me->globalPos() ) ) ) {
	    if ( myOrientation == Horizontal || i != 0 ) {
	      bool bOk;
	      QString tlt = QInputDialog::getText( tr( "SET_TITLE_TLT" ), 
						   tr( "TITLE_LBL" ),
						   QLineEdit::Normal,
						   myTable->verticalHeader()->label( i ),
						   &bOk,
						   this );
	      if ( bOk && !tlt.isNull() )
		myTable->verticalHeader()->setLabel( i, tlt );
	      break;
	    }
	  }
	}
      }
    }    
  }
  else if ( e->type() == QEvent::KeyRelease && o == myTable ) {
    QKeyEvent* ke = (QKeyEvent*)e;
    if ( ke->key() == Key_Delete && !myTable->isEditing() ) {
      clearTable();
    }
    else if ( ke->key() == Key_Backspace && !myTable->isEditing() ) {
      clearTable();
      int i = myTable->currentRow();
      int j = myTable->currentColumn() - 1;
      if ( j < 0 ) { j = myTable->numCols()-1; i--; }
      if ( i >= 0 && j >= 0 )
	myTable->setCurrentCell( i, j );
    }
  }
  return QWidget::eventFilter( o, e );
}



