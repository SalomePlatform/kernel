using namespace std;
//  File      : SALOMEGUI_LoadStudiesDlg.cxx
//  Created   : Wed Apr 03 13:37:13 2002
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SALOMEGUI_LoadStudiesDlg.h"

#include <qlabel.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qpushbutton.h>

#define SPACING_SIZE             6
#define MARGIN_SIZE             11
#define MIN_LISTBOX_WIDTH      150
#define MIN_LISTBOX_HEIGHT     100

SALOMEGUI_LoadStudiesDlg::SALOMEGUI_LoadStudiesDlg( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
    if ( !name )
	setName( "SALOMEGUI_LoadStudiesDlg" );
    resize( 321, 181 ); 
    setCaption( name );
    setSizeGripEnabled( TRUE );

    QGridLayout* aTopLayout = new QGridLayout(this);
    aTopLayout->setMargin(MARGIN_SIZE);
    aTopLayout->setSpacing(SPACING_SIZE);

    TextLabel1 = new QLabel( this, "TextLabel1" );
    TextLabel1->setGeometry( QRect( 11, 12, 297, 16 ) ); 
    TextLabel1->setText( tr( "MEN_STUDIES_CHOICE"  ) );

    QHBoxLayout* aBtnLayout = new QHBoxLayout;
    aBtnLayout->setSpacing( SPACING_SIZE );
    aBtnLayout->setMargin( 0 );
    
    buttonOk = new QPushButton( this, "buttonOk" );
    buttonOk->setText( tr( "BUT_OK"  ) );
    buttonOk->setAutoDefault( true );
    buttonOk->setDefault( true );
    
    buttonCancel = new QPushButton( this, "buttonCancel" );
    buttonCancel->setText( tr( "BUT_CANCEL"  ) );
    buttonCancel->setAutoDefault( true ); 
  
    aBtnLayout->addWidget( buttonOk );
    aBtnLayout->addItem( new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum ) );
    aBtnLayout->addWidget( buttonCancel );

    ListComponent = new QListBox( this, "ListComponent" );
    ListComponent->setVScrollBarMode(QListBox::AlwaysOn);
    ListComponent->setMinimumSize(MIN_LISTBOX_WIDTH, MIN_LISTBOX_HEIGHT);
    ListComponent->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    ListComponent->setSelectionMode(QListBox::Single);

    aTopLayout->addWidget(TextLabel1,    0, 0);
    aTopLayout->addWidget(ListComponent, 1, 0);
    aTopLayout->addLayout(aBtnLayout,    2, 0);

    // signals and slots connections
    connect( buttonOk, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( buttonCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

/*  
 *  Destroys the object and frees any allocated resources
 */
SALOMEGUI_LoadStudiesDlg::~SALOMEGUI_LoadStudiesDlg()
{
  // no need to delete child widgets, Qt does it all for us
}
