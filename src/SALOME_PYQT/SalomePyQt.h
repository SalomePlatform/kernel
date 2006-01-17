//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SalomePyQt.h
//  Module : SALOME

#ifndef _SALOME_PYQT_H
#define _SALOME_PYQT_H

#include <qwidget.h>
#include <qworkspace.h>
#include <qstring.h>
#include <qmenubar.h>

class QAD_Study;
class SALOME_Selection;
class SalomePyQt_Selection : public QObject
{
  Q_OBJECT

public:
  ~SalomePyQt_Selection();
  static SalomePyQt_Selection* GetSelection( QAD_Study* );

  void Clear();
  void ClearIObjects();
  void ClearFilters();

signals:
  void currentSelectionChanged();

private slots:
  void onSelectionDestroyed();
  void onSelectionModified();

private:
  QAD_Study*        myStudy;
  SALOME_Selection* mySelection;
  
  SalomePyQt_Selection( QObject* );
};

enum MenuName {
  File        = 1,
  View        = 2,
  Edit        = 3,
  Preferences = 4,
  Tools       = 5,
  Window      = 6,
  Help        = 7  
};

class SalomePyQt
{
public:
  static QWidget*              getDesktop();
  static QWorkspace*           getMainFrame();
  static QMenuBar*             getMainMenuBar();
  static QPopupMenu*           getPopupMenu( const MenuName menu );
  static SalomePyQt_Selection* getSelection();
  static int                   getStudyId();
  static void                  putInfo( const QString& );
  static void                  putInfo( const QString&, int );

  static const QString&        getActiveComponent();

  static void                  updateObjBrowser( int studyId, bool updateSelection);

  static void                  addStringSetting(QString _name, QString _value, bool _autoValue);
  static void                  addIntSetting(QString _name, int _value, bool _autoValue);
  static void                  addDoubleSetting(QString _name, double _value, bool _autoValue);
  static bool                  removeSettings(QString name);
  static QString               getSetting(QString name);

  static QString               getFileName(QWidget*           parent, 
					   const QString&     initial, 
					   const QStringList& filters, 
					   const QString&     caption,
					   bool               open);
  static QStringList           getOpenFileNames(QWidget*           parent, 
						const QString&     initial, 
						const QStringList& filters, 
						const QString&     caption);
  static QString               getExistingDirectory(QWidget*       parent,
						    const QString& initial,
						    const QString& caption);
  static void                  helpContext(const QString& source, const QString& context);
  static bool                  dumpView(const QString& filename);
};

#endif
