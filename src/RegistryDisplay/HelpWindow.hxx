//=============================================================================
// File      : HelpWindow.hxx
// Created   : Thu Dec 20 17:28:41 CET 2001
// Author    : Pascale NOYRET, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================
# ifndef __HELPWINDOW_H__
# define __HELPWINDOW_H__

# include <qapplication.h>
# include <qmainwindow.h>

class QTextView;

class HelpWindow : public QMainWindow
{
public:
  HelpWindow( QWidget* parent = 0, const char* name = 0);
  ~HelpWindow();

  void setText( const QString& text );
  QTextView* textView() const { return myTextView; }

private:
  QTextView* myTextView;
};
# endif         /* __HELPWINDOW_H__ */


