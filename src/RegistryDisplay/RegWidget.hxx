//=============================================================================
// File      : RegWidget.hxx
// Created   : Mon Nov  5 17:26:23 CET 2001
// Author    : Pascale NOYRET, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

# ifndef __REGWIDGET_H__
# define __REGWIDGET_H__
# include <qapplication.h>
# include <qmainwindow.h>

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Registry)

#include <IntervalWindow.hxx>

class QTabWidget;
class QListView;
class QListViewItem;
class QWidget;
class QTimer;
class QCloseEvent;
class QTextView;
#if QT_VERSION > 300
class QPushButton;
#endif
class HelpWindow;
class InfoWindow;

class RegWidget : public QMainWindow
{
  
  Q_OBJECT

public:
   RegWidget( CORBA::ORB_var &orb , QWidget *parent = 0, const char *name = 0 );
  ~RegWidget();
  void       SetListe();
  void       SetListeHistory();
  void       InfoReg();
  void       InfoHistory();

  bool       eventFilter( QObject* object, QEvent* event );

  static RegWidget* GetRegWidget( CORBA::ORB_var &orb , QWidget *parent = 0, const char *name = 0 );

public slots:
  void       slotHelp();
  void       slotListeSelect();
  void       slotClientChanged( QListViewItem* );
  void       slotHistoryChanged( QListViewItem* );
  void       slotSelectRefresh();
  void       slotIntervalOk();
protected:  
  static QString setlongText( const Registry::Infos &c_info );
  int            numitem( const QString& name, const QString& pid, const QString& machine, const Registry::AllInfos* c_info );
  void           closeEvent( QCloseEvent *e);
   
protected :
  QListView*          _clients;
  QListView*          _history;
  QWidget*            _parent;
  QTabWidget*         _tabWidget;
  QPushButton*        _refresh;
  QPushButton*        _interval;
  QTimer*             _counter;
  Registry::AllInfos* _serverhistory;
  Registry::AllInfos* _serverclients;
  InfoWindow*         myInfoWindow;
  HelpWindow*         myHelpWindow;
  IntervalWindow*     myIntervalWindow;
  int                 myRefreshInterval;

private:
  const      Registry::Components_var _VarComponents;
  static     RegWidget* myRegWidgetPtr;
};

class InfoWindow : public QMainWindow {
public:
  InfoWindow( QWidget* parent = 0, const char* name = 0);

  void setText( const QString& text );
  QTextView* textView() const { return myTextView; }

private:
  QTextView* myTextView;

};

# endif         /* # ifndef __REGWIDGET_H__ */









