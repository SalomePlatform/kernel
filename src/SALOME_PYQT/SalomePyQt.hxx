//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SalomePyQt.hxx
//  Module : SALOME

#ifndef _SALOME_PYQT_H
#define _SALOME_PYQT_H

#include <SALOME_Selection.h>

#include <qwidget.h>
#include <qworkspace.h>
#include <qstring.h>
#include <qmenubar.h>

class SalomePyQt
{
public:
  static QWidget*          getDesktop();
  static QWorkspace*       getMainFrame();
  static QMenuBar*         getMainMenuBar();
  static SALOME_Selection* getSelection();
  static int               getStudyId();
  static void              putInfo( const QString& );
  static void              putInfo( const QString&, int );

  static const QString&    getActiveComponent();

  static void              updateObjBrowser( int studyId, bool updateSelection);

  static void addStringSetting(QString _name, QString _value, bool _autoValue);
  static void addIntSetting(QString _name, int _value, bool _autoValue);
  static void addDoubleSetting(QString _name, double _value, bool _autoValue);
  static bool removeSettings(QString name);
  static QString getSetting(QString name);

  static QString getFileName(QWidget*           parent, 
                             const QString&     initial, 
                             const QStringList& filters, 
                             const QString&     caption,
                             bool               open);
  static QString getExistingDirectory(QWidget*       parent,
                                      const QString& initial,
                                      const QString& caption);
  static void helpContext(const QString& source, const QString& context);
};

#endif
