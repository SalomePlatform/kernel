//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
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
//  File   : SALOMEGUI_AboutDlg.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_AboutDlg.h"
#include "QAD_Desktop.h"
#include "QAD_Tools.h"
#include <qlayout.h>
#include <qpixmap.h>
#include <qmessagebox.h>
using namespace std;

/* 
 * Constructor
 */
SALOMEGUI_AboutDlg::SALOMEGUI_AboutDlg(QWidget* parent, const char* name )
    : QDialog( parent, name, true, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu )
{
  QPalette pal = palette();
  QColorGroup cg;
  cg.setColor( QColorGroup::Foreground, Qt::black );
  cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
  cg.setColor( QColorGroup::Light, Qt::white );
  cg.setColor( QColorGroup::Midlight, QColor( 223, 223, 223) );
  cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
  cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
  cg.setColor( QColorGroup::Text, Qt::black );
  cg.setColor( QColorGroup::BrightText, Qt::white );
  cg.setColor( QColorGroup::ButtonText, Qt::black );
  cg.setColor( QColorGroup::Base, Qt::white ); 
  cg.setColor( QColorGroup::Background, Qt::white );
  cg.setColor( QColorGroup::Shadow, Qt::black );
  cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
  cg.setColor( QColorGroup::HighlightedText, Qt::white );
  pal.setActive( cg );
  pal.setInactive( cg );
  pal.setDisabled( cg );
  setPalette(pal);

  setCaption( tr("ABOUT_TLT") );
  setSizeGripEnabled( false );
  QVBoxLayout* SALOMEGUI_AboutDlgLayout = new QVBoxLayout( this ); 
  SALOMEGUI_AboutDlgLayout->setSpacing( 6 );
  SALOMEGUI_AboutDlgLayout->setMargin( 11 );
  
  AboutPicLab = new QLabel( this, "AboutPicLab" );
  AboutPicLab->setFrameStyle( QLabel::Plain | QLabel::Box );
  QPixmap icon1 ( QAD_Desktop::getResourceManager()->loadPixmap( "CLIENT", 
		 						tr("ICON_ABOUT") ));

  QString msg;
  if(!QAD_Desktop::getResourceManager()->loadResources( "CLIENT", msg ))
    {
      QMessageBox::warning( this, tr("WRN_WARNING"), msg, tr ("BUT_OK") );
    }

  AboutPicLab->setPixmap( icon1 );
  AboutPicLab->setScaledContents( false );
  AboutPicLab->setAlignment( AlignCenter );
  AboutPicLab->installEventFilter(this);
  SALOMEGUI_AboutDlgLayout->addWidget( AboutPicLab );
  
  VersionLab = new QLabel( this, "VersionLab" );
  VersionLab->setText( tr( "INF_VERSION"  ) );
  VersionLab->setAlignment( AlignCenter );
  VersionLab->installEventFilter(this);
  SALOMEGUI_AboutDlgLayout->addWidget( VersionLab );
  
  CopyrightLab = new QLabel( this, "CopyrightLab" );
  CopyrightLab->setText( tr( "INF_COPYRIGHT"  ) );
  CopyrightLab->setAlignment( AlignCenter );
  CopyrightLab->installEventFilter(this);
  SALOMEGUI_AboutDlgLayout->addWidget( CopyrightLab );
  
  LicenseLab = new QLabel( this, "LicenseLab" );
  LicenseLab->setText( tr( "INF_LICENSE"  ) );
  LicenseLab->setAlignment( int( QLabel::WordBreak | QLabel::AlignCenter ) );
  QFont LicenseLabFont(  LicenseLab->font() );
  LicenseLabFont.setPointSize( 10 );
  LicenseLab->setFont( LicenseLabFont ); 
  LicenseLab->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
  LicenseLab->installEventFilter(this);
  SALOMEGUI_AboutDlgLayout->addWidget( LicenseLab );
  LicenseLab->polish();

  setMaximumSize(minimumSize());  

  QAD_Tools::alignWidget(this, parent, AlignCenter);
}

/*  
 * Destroys the object and frees any allocated resources
 */
SALOMEGUI_AboutDlg::~SALOMEGUI_AboutDlg()
{
  // no need to delete child widgets, Qt does it all for us
}

/*  
 *  Main event handler. Reimplemented to handle application
 *  font changes
 */
bool SALOMEGUI_AboutDlg::event( QEvent* ev )
{
  bool ret = QDialog::event( ev ); 
  if ( ev->type() == QEvent::ApplicationFontChange ) {
    QFont LicenseLabFont(  LicenseLab->font() );
    LicenseLabFont.setPointSize( 10 );
    LicenseLab->setFont( LicenseLabFont ); 
  } 
  else if ( ev->type() == QEvent::MouseButtonRelease ) {
    accept();
  }
  return ret;
}

bool SALOMEGUI_AboutDlg::eventFilter( QObject* o,  QEvent* e)
{
  if (e->type() == QEvent::MouseButtonRelease) {
    accept();
  }
  return QDialog::eventFilter(o, e);
}
