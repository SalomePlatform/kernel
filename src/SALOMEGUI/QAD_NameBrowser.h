//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : QAD_NameBrowser.h
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#ifndef QAD_NAMEBROWSER_H
#define QAD_NAMEBROWSER_H

#include <qvariant.h>
#include <qmainwindow.h>
class QVBoxLayout; 
class QHBoxLayout; 
class QGridLayout; 
class QComboBox;
class QGroupBox;
class QLabel;
class QListView;
class QListViewItem;
class QPushButton;
class QToolButton;

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class QAD_NameBrowser : public QMainWindow
{ 
    Q_OBJECT

public:
    QAD_NameBrowser( QWidget* parent = 0, const char* name = 0);
    ~QAD_NameBrowser();

    QListView*		   getListView() const;

    void initFilters();

    QListViewItem* AddItem(const QString&,
			   const QString&,
			   const QString&);
    void Update(SALOMEDS::Study_ptr Study,
		SALOMEDS::SObject_ptr SO);
    void Update(SALOMEDS::Study_ptr Study, 
		SALOMEDS::SComponent_ptr SC = SALOMEDS::SComponent::_narrow( CORBA::Object::_nil() ));


    void closeEvent(QCloseEvent* e);

protected slots:
    void selectAllFilteredObjects();
    void updateFilters();

    void onFilteredObjects();

private:
    QGroupBox* GroupBox1;
    QComboBox* ComboBox1;
    QPushButton* PushButton5;
    QComboBox* ComboBox2;
    QGroupBox* GroupBox2;
    QPushButton* PushButton1;
    QListView* ListView1;

};

#endif // QAD_NAMEBROWSER_H
