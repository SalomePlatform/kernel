//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_ActivateComponentDlg.cxx
//  Author : Michael Zorin (mzn)
//  Module : SALOME

#include "SALOMEGUI_ActivateComponentDlg.h"

#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qpixmap.h>
using namespace std;

static const char* const default_icon[] = { 
"48 48 17 1",
". c None",
"# c #161e4c",
"b c #1d3638",
"e c #2f585b",
"i c #345b5e",
"c c #386266",
"g c #3f7477",
"d c #4d8589",
"m c #519099",
"o c #6abbc1",
"a c #70c9d3",
"f c #79ddea",
"n c #7adff2",
"k c #7ce2f4",
"j c #993550",
"h c #d84b71",
"l c #ef537d",
"................................................",
"................................................",
"................................................",
"................................................",
"................................................",
"................########.########.########......",
"...............#aaaaaa###aaaaaa###aaaaaa##......",
"..............#aaaaaa#b#aaaaaa#b#aaaaaa#c#......",
".............########b########b########cc#......",
".............#dddddd#b#dddddd#b#dddddd#cc#......",
"...........########d########d########d#cc#......",
"..........#aaaaaa###aaaaaa###aaaaaa##d#cc#......",
".........#aaaaaa#b#aaaaaa#b#aaaaaa#c#d#cc#......",
"........########b########e########cc#d#c#.......",
"........#dddddd#b#dddddd#e#ffffff#cc#d####......",
"......########d########d########f#cc###g##......",
".....#aaaaaa###aaaaaa###hhhhhh##f#cc#gg#c#......",
"....#aaaaaa#b#aaaaaa#i#hhhhhh#j#f#cc###cc#......",
"...########b########i########jj#f#c#gg#cc#......",
"...#kkkkkk#b#kkkkkk#i#llllll#jj#f####g#cc#......",
"...#kkkkkk#b#kkkkkk#i#llllll#jj###m##g#cc#......",
"...#knnkkk#b#kkkkkk#i#llllll#jj#mm#c#g#cc#......",
"...#knnkkk#b#kkkkkk#i#llllll#jj###cc#g#c#.......",
"...#kkkkkk#b#kkkkkk#i#llllll#j#dd#cc#g####......",
"...#kkkkkk###kkkkkk###llllll####d#cc###g##......",
"...########g########g########o##d#cc#gg#c#......",
"....#gggggg#b#gggggg#b#oooooo#c#d#cc###cc#......",
"...########b########b########cc#d#c#gg#cc#......",
"...#kkkkkk#b#kkkkkk#b#kkkkkk#cc#d####g#cc#......",
"...#kkkkkk#b#kkkkkk#b#kkkkkk#cc###g##g#cc#......",
"...#kkkkkk#b#kkkkkk#b#kkkkkk#cc#gg#c#g#cc#......",
"...#kkkkkk#b#kkkkkk#b#kkkkkk#cc###cc#g#c#.......",
"...#kkkkkk#b#kkkkkk#b#kkkkkk#c#gg#cc#g##........",
"...#kkkkkk###kkkkkk###kkkkkk####g#cc###.........",
"...########g########g########g##g#cc#...........",
"....#gggggg#b#gggggg#b#gggggg#c#g#cc#...........",
"...########b########b########cc#g#c#............",
"...#kkkkkk#b#kkkkkk#b#kkkkkk#cc#g##.............",
"...#kkkkkk#b#kkkkkk#b#kkkkkk#cc###..............",
"...#kkkkkk#b#kkkkkk#b#kkkkkk#cc#................",
"...#kkkkkk#b#kkkkkk#b#kkkkkk#cc#................",
"...#kkkkkk#b#kkkkkk#b#kkkkkk#c#.................",
"...#kkkkkk###kkkkkk###kkkkkk##..................",
"...########.########.########...................",
"................................................",
"................................................",
"................................................",
"................................................"};

//==============================================================================================================================
/*!
 *  SALOMEGUI_ActivateComponentDlg::SALOMEGUI_ActivateComponentDlg
 *
 *  Constructor
 */
//==============================================================================================================================
SALOMEGUI_ActivateComponentDlg::SALOMEGUI_ActivateComponentDlg ( QWidget * parent, const QString& component, const QPixmap icon )
     : QDialog ( parent, "ActivateModuleDlg", true,  WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  QPixmap defaultIcon( ( const char** ) default_icon );
  setCaption( tr( "CAPTION" ) );
  setSizeGripEnabled( TRUE );
  
  QGridLayout* ActivateModuleDlgLayout = new QGridLayout( this ); 
  ActivateModuleDlgLayout->setMargin( 11 ); ActivateModuleDlgLayout->setSpacing( 6 );

  // Module's name and icon
  myComponentFrame = new QFrame( this, "myComponentFrame" );
  myComponentFrame->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Expanding ) );
  myComponentFrame->setMinimumHeight( 100 );
  myComponentFrame->setFrameStyle( QFrame::Box | QFrame::Sunken );
  
  QGridLayout* myComponentFrameLayout = new QGridLayout( myComponentFrame ); 
  myComponentFrameLayout->setMargin( 11 ); myComponentFrameLayout->setSpacing( 6 );

  // --> icon
  myComponentIcon = new QLabel( myComponentFrame, "myComponentIcon" );
  myComponentIcon->setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
  myComponentIcon->setPixmap( !icon.isNull() ? icon : defaultIcon );
  myComponentIcon->setScaledContents( false );
  myComponentIcon->setAlignment( AlignCenter );
  // --> name
  myComponentLab = new QLabel( component, myComponentFrame, "myComponentLab" );
  QFont fnt = myComponentLab->font(); fnt.setBold( TRUE ); myComponentLab->setFont( fnt ); 
  myComponentLab->setAlignment( AlignCenter );

  myComponentFrameLayout->addWidget( myComponentIcon, 0, 0 );
  myComponentFrameLayout->addWidget( myComponentLab,  0, 1 );

  // Info
  QVBoxLayout* infoLayout = new QVBoxLayout();
  infoLayout->setMargin( 0 ); infoLayout->setSpacing( 6 );
  
  // --> top line
  QFrame* myLine1 = new QFrame( this, "myLine1" );
  myLine1->setFrameStyle( QFrame::HLine | QFrame::Plain );
  // --> info label  
  myInfoLabel = new QLabel( tr ("ActivateComponent_DESCRIPTION"), this, "myInfoLabel" );
  myInfoLabel->setAlignment( AlignCenter );
  // --> bottom line
  QFrame*  myLine2 = new QFrame( this, "myLine2" );
  myLine2->setFrameStyle( QFrame::HLine | QFrame::Plain );
  
  infoLayout->addStretch();
  infoLayout->addWidget( myLine1 );
  infoLayout->addWidget( myInfoLabel );
  infoLayout->addWidget( myLine2 );
  infoLayout->addStretch();
  
  // Buttons
  QHBoxLayout* btnLayout = new QHBoxLayout(); 
  btnLayout->setMargin( 0 ); btnLayout->setSpacing( 6 );
  
  // --> New
  myNewBtn = new QPushButton( tr( "NEW" ), this, "myNewBtn" );
  myNewBtn->setDefault( true ); myNewBtn->setAutoDefault( true );
  // --> Open
  myOpenBtn = new QPushButton( tr( "OPEN" ), this, "myOpenBtn" );
  myOpenBtn->setAutoDefault( true );
  // --> Load
  myLoadBtn = new QPushButton( tr( "LOAD" ), this, "myLoadBtn" );
  myLoadBtn->setAutoDefault( true );
  // --> Cancel
  myCancelBtn = new QPushButton( tr( "CANCEL" ), this, "myCancelBtn" );
  myCancelBtn->setAutoDefault( true );
  
  btnLayout->addWidget( myNewBtn );
  btnLayout->addWidget( myOpenBtn );
  btnLayout->addWidget( myLoadBtn );
  btnLayout->addStretch();
  btnLayout->addSpacing( 70 );
  btnLayout->addStretch();
  btnLayout->addWidget( myCancelBtn );

  ActivateModuleDlgLayout->addWidget(          myComponentFrame, 0,    0    );
  ActivateModuleDlgLayout->addLayout(          infoLayout,       0,    1    );
  ActivateModuleDlgLayout->addMultiCellLayout( btnLayout,        1, 1, 0, 1 );

  // signals and slots connections
  connect( myNewBtn,    SIGNAL( clicked() ), this, SLOT( onButtonClicked() ) );
  connect( myOpenBtn,   SIGNAL( clicked() ), this, SLOT( onButtonClicked() ) );
  connect( myLoadBtn,   SIGNAL( clicked() ), this, SLOT( onButtonClicked() ) );
  connect( myCancelBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );
}

//==============================================================================================================================
/*!
 *  SALOMEGUI_ActivateComponentDlg::onButtonClicked
 *
 *  Buttons slot
 */
//==============================================================================================================================
void SALOMEGUI_ActivateComponentDlg::onButtonClicked()
{
  QPushButton* btn = ( QPushButton* )sender();
  if ( btn == myNewBtn )
    done( 1 );
  if ( btn == myOpenBtn )
    done( 2 );
  if ( btn == myLoadBtn )
    done( 3 );
}
