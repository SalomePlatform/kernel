//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_TableDlg.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#ifndef SALOMEGUI_TABLE_DLG_H
#define SALOMEGUI_TABLE_DLG_H

#include <qdialog.h>
#include <qlineedit.h>
#include <qpushbutton.h>

class SALOMEGUI_Table;
class SALOMEGUI_TableWidget;

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEGUI_TableDlg : public QDialog
{ 
  Q_OBJECT

public:

  enum { ttNone, ttInt, ttReal, ttBoth, ttAuto };

  SALOMEGUI_TableDlg( QWidget* parent, 
		      SALOMEDS::SObject_var obj, 
		      bool edit = false,
		      int which = ttAuto, 
		      Orientation orient = Horizontal,
		      bool showColumnTitles = true );
  ~SALOMEGUI_TableDlg();

public slots:
  void onOK();

private:
  void initDlg();

private:
  SALOMEGUI_TableWidget* myIntTable;
  SALOMEGUI_TableWidget* myRealTable;
  QPushButton*           myOKBtn;
  QPushButton*           myCancelBtn;

  SALOMEDS::SObject_var myObject;
};
 
class SALOMEGUI_TableWidget : public QWidget
{
  Q_OBJECT
public:
  SALOMEGUI_TableWidget( QWidget* parent = 0, 
			 const char* name = 0, 
			 bool edit = false, 
			 Orientation orient = Horizontal,
			 bool showColumnTitles = true );
  ~SALOMEGUI_TableWidget();

  void    setTableTitle( const QString& title );
  QString getTableTitle();
  void    setNumRows( const int num );
  int     getNumRows();
  void    setNumCols( const int num );
  int     getNumCols();
  void    setRowTitles( QStringList& tlts );
  void    getRowTitles( QStringList& tlts );
  void    setColTitles( QStringList& tlts );
  void    getColTitles( QStringList& tlts );
  void    setUnitsTitle( const QString& tlt );
  void    setUnits( QStringList& units );
  void    getUnits( QStringList& units );
  void    setRowData( int row, QStringList& data );
  void    getRowData( int row, QStringList& data );

  SALOMEGUI_Table* getTable()     { return myTable; } 
  QLineEdit*       getTitleEdit() { return myTitleEdit; }

  bool    eventFilter( QObject* o, QEvent* e);

public slots:
  void updateButtonsState();
  void addRow();
  void addCol();
  void delRow();
  void delCol();
  void adjustTable();
  void selectAll();
  void clearTable();

private:
  QLineEdit*       myTitleEdit;
  SALOMEGUI_Table* myTable;
  QPushButton*     myAddRowBtn;
  QPushButton*     myAddColBtn;
  QPushButton*     myDelRowBtn;
  QPushButton*     myDelColBtn;
  QPushButton*     myAdjustBtn;
  QPushButton*     mySelectAllBtn;
  QPushButton*     myClearBtn;
  Orientation      myOrientation;
};

#endif // SALOMEGUI_TABLE_DLG_H

