using namespace std;
//=============================================================================
// File      : HelpWindow.cxx
// Created   : Thu Dec 20 17:28:49 CET 2001
// Author    : Pascale NOYRET, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================
# include "HelpWindow.hxx"
# include "utilities.h"

# include <qtextview.h>
# include <qpushbutton.h>
# include <qtextstream.h> 
# include <qfile.h> 

HelpWindow::HelpWindow(QWidget* parent, const char* name ) 
     : QMainWindow( parent, name, WType_TopLevel | WDestructiveClose )
{
  BEGIN_OF("Constructeur HelpWindow");
  
  setCaption( tr( "Help" ) );

  myTextView = new QTextView( this, "myTextView" );
  QPalette pal = myTextView->palette();
  QColorGroup cg = pal.active();
  cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
  cg.setColor( QColorGroup::HighlightedText, Qt::white );
  cg.setColor( QColorGroup::Base, QColor( 255,255,220 )  ); 
  cg.setColor( QColorGroup::Text, Qt::black );
  pal.setActive  ( cg );
  cg = pal.inactive();
  cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
  cg.setColor( QColorGroup::HighlightedText, Qt::white );
  cg.setColor( QColorGroup::Base, QColor( 255,255,220 )  ); 
  cg.setColor( QColorGroup::Text, Qt::black );
  pal.setInactive( cg );
  cg = pal.disabled();
  cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
  cg.setColor( QColorGroup::HighlightedText, Qt::white );
  cg.setColor( QColorGroup::Base, QColor( 255,255,220 )  ); 
  cg.setColor( QColorGroup::Text, Qt::black );
  pal.setDisabled( cg );
  myTextView->setPalette( pal );
  
  setCentralWidget( myTextView );
  setMinimumSize( 450, 250 );

  QFile f ( "tmp.txt" );
  if ( f.open( IO_ReadOnly ) )   
    {
      QTextStream t( &f ); 
      while ( !t.eof() ) 
	{
	  myTextView->append(t.readLine());
	}
    }
  f.close();

  END_OF("Constructeur HelpWindow");
}

/*!
  Destructor
*/
HelpWindow::~HelpWindow()
{
  BEGIN_OF("Destructeur HelpWindow");
  END_OF("Destructeur HelpWindow");
};

/*!
  Sets text
*/
void HelpWindow::setText( const QString& text )
{
  myTextView->setText( text );
}


