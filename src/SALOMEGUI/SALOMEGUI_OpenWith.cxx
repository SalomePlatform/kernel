using namespace std;
//  File      : SALOMEGUI_OpenWith.cxx
//  Created   : Wed Oct 24 12:19:34 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$


#include "SALOMEGUI_OpenWith.h"

#include <qlabel.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlistbox.h>

#define SPACING_SIZE             6
#define MARGIN_SIZE             11
#define MIN_LISTBOX_WIDTH      150
#define MIN_LISTBOX_HEIGHT     100

/* 
 *  Constructs modal SALOMEGUI_OpenWith which is a child of 'parent', with the 
 *  name 'name' 
 */
SALOMEGUI_OpenWith::SALOMEGUI_OpenWith(QWidget* parent,  const char* name)
    : QDialog( parent, name, true, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  if ( !name )
    setName( "SALOMEGUI_OpenWith" );
  setCaption( tr("OPEN_WITH_TLT") );
  setSizeGripEnabled( TRUE );

  QGridLayout* aTopLayout = new QGridLayout(this);
  aTopLayout->setMargin(MARGIN_SIZE);
  aTopLayout->setSpacing(SPACING_SIZE);
  
  AllwaysCheckBox = 0;
/*  
  AllwaysCheckBox = new QCheckBox( this, "AllwaysCheckBox" );
  AllwaysCheckBox->setText( tr( "MEN_COMPONENT_CHOICE1"  ) );
*/  
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
  
  aTopLayout->addWidget(ListComponent,   0, 0);
//  aTopLayout->addWidget(AllwaysCheckBox, 1, 0);
  aTopLayout->addLayout(aBtnLayout,      1, 0);
  
  // signals and slots connections
  connect( buttonOk,      SIGNAL( clicked() ),          this, SLOT( accept() ) );
  connect( buttonCancel,  SIGNAL( clicked() ),          this, SLOT( reject() ) );
  connect( ListComponent, SIGNAL( selectionChanged() ), this, SLOT( onSelectionChanged() ));

  updateButtonState();
}

/*  
 *  Destroys the object and frees any allocated resources
 */
SALOMEGUI_OpenWith::~SALOMEGUI_OpenWith()
{
}

/*
   Inserts new component into component list
*/
void SALOMEGUI_OpenWith::addComponent(const QString& aComp)
{
  ListComponent->insertItem(aComp);
}

/*
   Returns choosen component
*/
QString SALOMEGUI_OpenWith::getComponent()
{
  return ListComponent->currentText();
}

bool SALOMEGUI_OpenWith::getAlwaysFlag()
{
  if ( AllwaysCheckBox )
    return AllwaysCheckBox->isChecked();
  return false;
}
void SALOMEGUI_OpenWith::onSelectionChanged()
{
  updateButtonState();
}

void SALOMEGUI_OpenWith:: updateButtonState()
{
  buttonOk->setEnabled( ListComponent->currentItem() >=0 && 
		        ListComponent->currentItem() < ListComponent->count() ) ;
}
