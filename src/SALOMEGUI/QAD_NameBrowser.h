//  File      : QAD_NameBrowser.h
//  Created   : Mon Nov 26 10:15:34 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
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
