//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_SetupPlot2dDlg.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#ifndef Plot2d_SetupPlot2dDlg_H
#define Plot2d_SetupPlot2dDlg_H

#include <qdialog.h>
#include <qlist.h>

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

//=================================================================================
// class    : Plot2d_SetupPlot2dDlg
// purpose  : Dialog box for setup Plot2d view
//=================================================================================

class QScrollView;
class QPushButton;
class QLabel;
class QCheckBox;
class QComboBox;
class QSpinBox;
class QToolButton;
class Plot2d_ItemContainer;
class Plot2d_CurveContainer;

class Plot2d_SetupPlot2dDlg : public QDialog
{ 
  Q_OBJECT

public:
  Plot2d_SetupPlot2dDlg( SALOMEDS::SObject_var object, QWidget* parent = 0 );
  ~Plot2d_SetupPlot2dDlg();

  void getCurves( Plot2d_CurveContainer& container );
  void getCurvesSource( int& horIndex, QValueList<int>& verIndexes );
  bool getCurveAttributes( const int vIndex, bool& isAuto, int& marker, int& line, int& lineWidth, QColor& color);

private slots:
  void onHBtnToggled( bool );
  void onVBtnToggled( bool );
  void enableControls();

private:
  QScrollView*                myView;
  QPushButton*                myOkBtn;
  QPushButton*                myCancelBtn;
  QList<Plot2d_ItemContainer> myItems;

  SALOMEDS::SObject_var       myObject;
};

class Plot2d_ItemContainer : public QObject
{
  Q_OBJECT

public:
  Plot2d_ItemContainer( QObject* parent = 0, const char* name = 0 );
  
  void   createWidgets( QWidget* parentWidget );
  void   enableWidgets( bool enable );

  void   setHorizontalOn( bool on );
  bool   isHorizontalOn() const;
  void   setVerticalOn( bool on );
  bool   isVerticalOn() const;
  bool   isAutoAssign() const;
  void   setAutoAssign( bool on );
  void   setLine( const int line, const int width );
  int    getLine() const;
  int    getLineWidth() const;
  void   setMarker( const int marker );
  int    getMarker() const;
  void   setColor( const QColor& color );
  QColor getColor() const;

protected:
  void   updateState();

signals:
  void   autoClicked();
  void   horToggled( bool );
  void   verToggled( bool );

public slots:
  void   onAutoChanged();
  void   onColorChanged();
  void   onHVToggled( bool );

public:
  bool                  myEnabled;
  QToolButton*          myHBtn;
  QToolButton*          myVBtn;
  QLabel*               myTitleLab;
  QLabel*               myUnitLab;
  QCheckBox*            myAutoCheck;
  QComboBox*            myLineCombo;
  QSpinBox*             myLineSpin;
  QComboBox*            myMarkerCombo;
  QToolButton*          myColorBtn;
};

#endif // Plot2d_SetupPlot2dDlg_H

