//  File      : QAD_HelpWindow.h
//  Created   : Thu Jun 14 16:11:33 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$

#ifndef QAD_HELPWINDOW_H
#define QAD_HELPWINDOW_H

#include <qmainwindow.h>
#include <qtextbrowser.h>

class QComboBox;
class QPopupMenu;

class QAD_HelpWindow : public QMainWindow
{
  Q_OBJECT
public:
  QAD_HelpWindow();
  ~QAD_HelpWindow();
  
  void contents();
  void context( const QString& _source, const QString& _context = QString::null );

private slots:
  void setBackwardAvailable( bool );
  void setForwardAvailable( bool );
  void textChanged();
  void pathSelected( const QString & );
    
signals:
  void helpWindowClosed();

private:
  QTextBrowser* browser;
  QComboBox*    pathCombo;
  int           backwardId, forwardId;
  QString       selectedURL;
  QString       homeDir;
    
};

#endif

