//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : Plot2d_SetupViewDlg.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#ifndef Plot2d_SetupViewDlg_H
#define Plot2d_SetupViewDlg_H

#include <qdialog.h>

//=================================================================================
// class    : Plot2d_SetupViewDlg
// purpose  : Settings dialog for Plot2d view
//=================================================================================

class QCheckBox;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QToolButton;
class QPushButton;

class Plot2d_SetupViewDlg : public QDialog
{ 
  Q_OBJECT

public:
  Plot2d_SetupViewDlg( QWidget* parent = 0, bool showDefCheck = false );
  ~Plot2d_SetupViewDlg();

  void    setMainTitle( bool enable, const QString& title = QString::null );
  bool    isMainTitleEnabled();
  QString getMainTitle();
  void    setXTitle( bool enable, const QString& title = QString::null );
  bool    isXTitleEnabled();
  QString getXTitle();
  void    setYTitle( bool enable, const QString& title = QString::null );
  bool    isYTitleEnabled();
  QString getYTitle();
  void    setCurveType( const int type );
  int     getCurveType();
  void    setLegend( bool enable, int pos );
  bool    isLegendEnabled();
  int     getLegendPos();
  void    setMarkerSize( const int size );
  int     getMarkerSize();
  void    setBackgroundColor( const QColor& color );
  QColor  getBackgroundColor();
  void    setMajorGrid( bool enableX, const int xdiv, bool enableY, const int divY );
  void    getMajorGrid( bool& enableX, int& xdiv, bool& enableY, int& divY );
  void    setMinorGrid( bool enableX, const int xdiv, bool enableY, const int divY );
  void    getMinorGrid( bool& enableX, int& xdiv, bool& enableY, int& divY );
  void    setScaleMode( const int xMode, const int yMode );
  int     getXScaleMode();
  int     getYScaleMode();
  bool    isSetAsDefault();

protected slots:
  void   onMainTitleChecked();
  void   onXTitleChecked();
  void   onYTitleChecked();
  void   onBackgroundClicked();
  void   onLegendChecked();
  void   onXGridMajorChecked();
  void   onYGridMajorChecked();
  void   onXGridMinorChecked();
  void   onYGridMinorChecked();

private:
  QCheckBox*     myTitleCheck;
  QLineEdit*     myTitleEdit;
  QCheckBox*     myTitleXCheck;
  QLineEdit*     myTitleXEdit;
  QCheckBox*     myTitleYCheck;
  QLineEdit*     myTitleYEdit;
  QToolButton*   myBackgroundBtn;
  QCheckBox*     myXGridCheck;
  QSpinBox*      myXGridSpin;
  QCheckBox*     myYGridCheck;
  QSpinBox*      myYGridSpin;
  QCheckBox*     myXMinGridCheck;
  QSpinBox*      myXMinGridSpin;
  QCheckBox*     myYMinGridCheck;
  QSpinBox*      myYMinGridSpin;
  QComboBox*     myCurveCombo;
  QCheckBox*     myLegendCheck;
  QComboBox*     myLegendCombo;
  QSpinBox*      myMarkerSpin;
  QComboBox*     myXModeCombo;
  QComboBox*     myYModeCombo;
  QCheckBox*     myDefCheck;

  QPushButton*   myOkBtn;
  QPushButton*   myCancelBtn;
};

#endif // Plot2d_SetupViewDlg_H

