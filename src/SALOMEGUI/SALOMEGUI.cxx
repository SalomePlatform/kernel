using namespace std;
//  File      : SALOMEGUI.cxx
//  Created   : Thu Jun 14 12:00:40 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

#include "QAD.h"
#include "QAD_MessageBox.h"
#include "QAD_Application.h"
#include "SALOMEGUI_Application.h"

#include <qmainwindow.h>
#include <qapplication.h>

#include "SALOME_NamingService.hxx"

int main(int argc, char* argv[])
{    
    QApplication a( argc, argv );

    // Setting up the CORBA environment
    // Initializing omniORB
    SALOME_NamingService * name_service;
    CORBA::ORB_var orb;

    orb = CORBA::ORB_init(argc, argv, "omniORB3");
    
    // Get the reference the server.
    name_service = new SALOME_NamingService(orb);
    
    QAD_ASSERT ( QObject::connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) ) );
  
    /* Initialize the desktop and 'SALOMEGUI' application */    
    SALOMEGUI_Application* app = 
      new SALOMEGUI_Application ( "MDTV-Standard", "HDF","hdf" );
        
    if ( !SALOMEGUI_Application::addToDesktop ( app, name_service ) )
    {
      QAD_MessageBox::error1 ( 0,
			       QObject::tr("ERR_ERROR"), 
			       QObject::tr("ERR_APP_INITFAILED"),
			       QObject::tr("BUT_OK") ); 
      return -1;      
    } 

    QPalette pal;
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
    cg.setColor( QColorGroup::Background, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, Qt::black );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, Qt::white );
    cg.setColor( QColorGroup::Midlight, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, Qt::black );
    cg.setColor( QColorGroup::Base, Qt::white );
    cg.setColor( QColorGroup::Background, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Light, Qt::white );
    cg.setColor( QColorGroup::Midlight, QColor( 220, 220, 220) );
    cg.setColor( QColorGroup::Dark, QColor( 96, 96, 96) );
    cg.setColor( QColorGroup::Mid, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Text, Qt::black );
    cg.setColor( QColorGroup::BrightText, Qt::white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, Qt::white );
    cg.setColor( QColorGroup::Background, QColor( 192, 192, 192) );
    cg.setColor( QColorGroup::Shadow, Qt::black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, Qt::white );
    pal.setDisabled( cg );
    qApp->setPalette( pal );

    /* Run 'SALOMEGUI' application */
    QAD_Application::run();
    a.exec();

    delete name_service;
    orb->destroy();
    
    return 0;
}
