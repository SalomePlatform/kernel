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
//  File   : QAD_NameBrowser.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_NameBrowser.h"

#include "QAD_Study.h"
#include "QAD_Desktop.h"
#include "QAD_Application.h"
#include "QAD_Selection.h"
#include "QAD_InteractiveObject.h"

#include "utilities.h"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
using namespace std;

/*! 
  Constructs a QAD_NameBrowser which is a child of 'parent', with the 
  name 'name'
*/
QAD_NameBrowser::QAD_NameBrowser( QWidget* parent,  const char* name)
  : QMainWindow( parent, name, WStyle_NormalBorder | 
		 WStyle_MinMax | WStyle_SysMenu | WDestructiveClose  )
{
    if ( !name )
	setName( "QAD_NameBrowser" );

    resize( 321, 280 ); 
    setCaption( name );

    GroupBox1 = new QGroupBox( this, "GroupBox1" );
    GroupBox1->setGeometry( QRect( 10, 10, 300, 50 ) ); 
    GroupBox1->setTitle( tr( "Filters"  ) );

    ComboBox1 = new QComboBox( TRUE, GroupBox1 );
    ComboBox1->setGeometry( QRect( 10, 20, 120, 21 ) ); 
    ComboBox1->insertItem( tr( "Type"  ) );

    connect( ComboBox1, SIGNAL( textChanged(const QString &) ), this, SLOT( updateFilters() ) );


    PushButton5 = new QPushButton( tr( " = "  ), GroupBox1);
    PushButton5->setGeometry( QRect( 140, 20, 30, 21 ) ); 

    ComboBox2 = new QComboBox( TRUE, GroupBox1);
    ComboBox2->setGeometry( QRect( 180, 20, 111, 21 ) ); 
    ComboBox2->insertItem( tr( "All"  ) );
    connect( ComboBox2, SIGNAL( textChanged(const QString &) ), this, SLOT( updateFilters() ) );

    GroupBox2 = new QGroupBox( this, "GroupBox2" );
    GroupBox2->setGeometry( QRect( 10, 60, 300, 200 ) ); 
    GroupBox2->setTitle( tr( "Objects"  ) );

    PushButton1 = new QPushButton( tr( "All"  ), GroupBox2 );
    PushButton1->setGeometry( QRect( 10, 170, 71, 21 ) ); 
    connect( PushButton1, SIGNAL( clicked() ), this, SLOT( selectAllFilteredObjects() ) );

    ListView1 = new QListView( GroupBox2 );
    ListView1->setGeometry( QRect( 10, 20, 280, 140 ) ); 
    ListView1->addColumn("");
    ListView1->addColumn("");
    ListView1->addColumn("");
    ListView1->setColumnText( 0, tr( "Filtered"  ) );
    ListView1->setColumnWidth(0, 130);
    ListView1->setRootIsDecorated(TRUE);
    ListView1->setMultiSelection (TRUE);
    ListView1->setSelectionMode ( QListView::Extended );

    connect (ListView1,
	     SIGNAL( selectionChanged() ),
	     this, SLOT(onFilteredObjects()) );

    initFilters();
}

/*!
  Returns reference to QListView
*/
QListView* QAD_NameBrowser::getListView() const
{
  return ListView1;
}

/*!  
  Destroys the object and frees any allocated resources
*/
QAD_NameBrowser::~QAD_NameBrowser()
{
  // no need to delete child widgets, Qt does it all for us
}

void QAD_NameBrowser::selectAllFilteredObjects()
{
  ListView1->selectAll( true );
}

void QAD_NameBrowser::updateFilters()
{
  initFilters();
}

void QAD_NameBrowser::initFilters()
{
  QAD_Desktop*     myDesktop = QAD_Application::getDesktop();
  QAD_Study*   myActiveStudy = myDesktop->getActiveStudy();
  SALOMEDS::Study_var aStudy = myActiveStudy->getStudyDocument();

  Update( aStudy );


//    QAD_Selection* Sel = QAD_Selection::Selection( myActiveStudy->getSelection() );
//    const QList<QAD_InteractiveObject>& ListIO = Sel->getInteractiveObjects();
//    int NbSelected = ListIO.count();
//    if ( NbSelected == 0 ) {
//      // setFilter "All"
//      Update( aStudy );

//    } else if ( NbSelected == 1 ) {
//      // setFilter "type of selected object"
//      QAD_InteractiveObject* IObject = Sel->getFisrtInteractiveObject();
//      SALOMEDS::SComponent_var SC = SALOMEDS::SComponent::_narrow ( myActiveStudy->FindObjectIOR( IObject->getFatherIOR() ));
//      if ( !SC->_is_nil() ) {
//        char* ObjectComp = "";
//        if ( SC->GetAttribute(SALOMEDS::Name, ObjectComp)) {
//  	Update( aStudy, SC );
//        }
//      }

//    } else {
//  //      const QList<QAD_InteractiveObject>& ListIO = Sel->getInteractiveObjects();
//  //      QListIterator<QAD_InteractiveObject> it( Sel->getInteractiveObjects() );
//  //      for ( ; it.current(); ++it ) {
//  //        QAD_InteractiveObject* IObject = it.current();
//  //        QString IOR = IObject->getFatherIOR() ));

//  //      }

//      // heterogeneous selection -> setFilter "All"
//      Update( aStudy );

//      // homogeneous selection -> setFilter "type of selected objects"
//    }
}

void QAD_NameBrowser::onFilteredObjects()
{
  QAD_Study*	  myActiveStudy	 = QAD_Application::getDesktop()->getActiveStudy();
  SALOMEDS::Study_var	 aStudy	 = myActiveStudy->getStudyDocument();
  QAD_Selection*	    Sel	 = QAD_Selection::Selection( myActiveStudy->getSelection() );
  QAD_StudyFrame* myActiveSFrame = myActiveStudy->getActiveStudyFrame();

  QString             ActiveComp = QAD_Application::getDesktop()->getActiveComponent();

  MESSAGE ( "QAD_NAMEBROWSER - NB SELECTED INTERACTIVE OBJECT : " << Sel->getInteractiveObjectCount() )

  QList<QAD_InteractiveObject> DeltaPos;
  DeltaPos.clear();
  QList<QAD_InteractiveObject> DeltaNeg;
  DeltaNeg.clear();

  QListViewItemIterator it( ListView1 );
  for ( ; it.current(); ++it ) {
    QListViewItem* item = it.current();
    QString theIOR = item->text(2);
    QString theEntry = item->text(1);

    if ( ActiveComp.isEmpty() ) {/* Neutral point = no component loaded */
      if ( !theEntry.isEmpty() && theIOR.isEmpty() ) { /* A component may be selected */
  	if ( item->isSelected() ) {
	  bool itemAlreadySelected = false;
	  
	  int nbSel = Sel->getInteractiveObjectCount();
	  for ( int i = 0; i < nbSel; i++ ) {
	    QAD_InteractiveObject* SO = Sel->getInteractiveObject(i);
	    if ( SO->getIOR().compare(item->text(1)) == 0 ) {
	      itemAlreadySelected = true;
	      break;
	    }
	  }
	  if (!itemAlreadySelected) {
	    QAD_InteractiveObject* SO = new QAD_InteractiveObject( item->text(1), "" ); 
	    DeltaPos.append( SO );
	  }
	} else {
	  int nbSel = Sel->getInteractiveObjectCount();
	  for ( int i = 0; i < nbSel; i++ ) {
	    QAD_InteractiveObject* SO = Sel->getInteractiveObject(i);
	    if ( SO->getIOR().compare(item->text(1)) == 0 ) {
	      DeltaNeg.append( SO );
	      break;
	    }
	  }
	}
	
      } else { /* An object is selected */
	if ( item->isSelected() ) {
	  item->setSelected( false );
	  item->repaint();
	}
      }
       
    } else {
      if ( !theIOR.isEmpty() ) {
	if ( item->isSelected() ) {
	  bool itemAllreadySelected = false;
	  
	  int nbSel = Sel->getInteractiveObjectCount();
	  for ( int i = 0; i < nbSel; i++ ) {
	    QAD_InteractiveObject* SO = Sel->getInteractiveObject(i);
	    if ( SO->getIOR().compare(theIOR) == 0 ) {
	      itemAllreadySelected = true;
	      break;
	    }
	  }
	  
	  if (!itemAllreadySelected) {
	    SALOMEDS::SObject_var obj = aStudy->FindObjectID(theEntry.latin1());
	    SALOMEDS::SComponent_var theComponent = obj->GetFatherComponent();
            SALOMEDS::GenericAttribute_var anAttr;
            SALOMEDS::AttributeIOR_var     anIOR;
	    Standard_CString ior  =" ";
	    Standard_CString iorFather  =" ";
	    if (obj->FindAttribute(anAttr, "AttributeIOR")) {
              anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
              ior = anIOR->Value();
            }
	    theComponent->ComponentIOR(iorFather);
	    QAD_InteractiveObject* SO = new QAD_InteractiveObject( QString(ior), 
								   QString(iorFather),
								   QString(theEntry) );
	    DeltaPos.append( SO );
	  }
	} else {
	  int nbSel = Sel->getInteractiveObjectCount();
	  for ( int i = 0; i < nbSel; i++ ) {
	    QAD_InteractiveObject* SO = Sel->getInteractiveObject(i);
	    if ( SO->getIOR().compare(theIOR) == 0 ) {
	      DeltaNeg.append( SO );
	      break;
	    }
	  }
	  
	}
      } else if ( !theEntry.isEmpty() && theIOR.isEmpty() ) {
	if ( item->isSelected() ) {
	  bool itemAllreadySelected = false;
	  
	  int nbSel = Sel->getInteractiveObjectCount();
	  for ( int i = 0; i < nbSel; i++ ) {
	    QAD_InteractiveObject* SO = Sel->getInteractiveObject(i);
	    if ( SO->getEntry().compare(theEntry) == 0 ) {
	      itemAllreadySelected = true;
	      break;
	    }
	  }
	  if (!itemAllreadySelected) {
	    SALOMEDS::SObject_var obj = aStudy->FindObjectID(theEntry.latin1());
	    SALOMEDS::SComponent_var theComponent = obj->GetFatherComponent();
            SALOMEDS::GenericAttribute_var anAttr;
            SALOMEDS::AttributeIOR_var     anIOR;
	    Standard_CString ior  =" ";
	    Standard_CString iorFather  =" ";
	    if (obj->FindAttribute(anAttr, "AttributeIOR")) {
              anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
              ior = anIOR->Value();
            }
	    theComponent->ComponentIOR(iorFather);
	    QAD_InteractiveObject* SO = new QAD_InteractiveObject( QString(ior), 
								   QString(iorFather),
								   QString(theEntry) );
	    DeltaPos.append( SO );
	  }
	} else {
	  int nbSel = Sel->getInteractiveObjectCount();
	  for ( int i = 0; i < nbSel; i++ ) {
	    QAD_InteractiveObject* SO = Sel->getInteractiveObject(i);
	    if ( SO->getEntry().compare(theEntry) == 0 ) {
	      DeltaNeg.append( SO );
	      break;
	    }
	  }
	  
	}
      } else {
	item->setSelected( false );
	item->repaint();
      }
    }
  }

  MESSAGE ( "NAMEBROWSER DeltaNeg.count() == " << DeltaNeg.count() )
  for ( QAD_InteractiveObject* SO = DeltaNeg.first(); SO;  SO = DeltaNeg.next() ) {
    MESSAGE ( "------" << SO->getIOR() )
    Sel->RemoveInteractiveObject(SO);
  }
  MESSAGE ( "NAMEBROWSER DeltaPos.count() == " << DeltaPos.count() )
  for ( QAD_InteractiveObject* SO = DeltaPos.first(); SO;  SO = DeltaPos.next() ) {
    MESSAGE ( "++++++" << SO->getIOR() )
    Sel->AddInteractiveObject(SO);
  }
}

/*!
  Update
*/
void QAD_NameBrowser::Update(SALOMEDS::Study_ptr Study,
			     SALOMEDS::SObject_ptr SO)
{
  SALOMEDS::SObject_var RefSO;
  SALOMEDS::ChildIterator_var it = Study->NewChildIterator(SO);

  for (; it->More();it->Next()) {
    SALOMEDS::SObject_var CSO= it->Value();
    SALOMEDS::GenericAttribute_var anAttr;
    SALOMEDS::AttributeName_var    aName;
    SALOMEDS::AttributeIOR_var     anIOR;
    QListViewItem* Item;

    if (CSO->FindAttribute(anAttr, "AttributeName")) {
      aName = SALOMEDS::AttributeName::_narrow(anAttr);
      if (CSO->FindAttribute(anAttr, "AttributeIOR")) {
        anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
        Item = AddItem ( aName->Value(), CSO->GetID(), anIOR->Value());
      }
    }

    Update(Study,CSO);
  }
}

/*!
  Update
*/
void QAD_NameBrowser::Update(SALOMEDS::Study_ptr Study,
			     SALOMEDS::SComponent_ptr SC)
{
  ListView1->clear();

  if (Study->_is_nil()) {
    return;
  }

//  Standard_CString name;
  Standard_CString dataType;
  Standard_CString ior="";

  QListViewItem* Item;

  SALOMEDS::SComponentIterator_var itcomp;
  SALOMEDS::GenericAttribute_var anAttr;
  SALOMEDS::AttributeName_var    aName;
  SALOMEDS::AttributeIOR_var     anIOR;

  if ( !SC->_is_nil() ) {
    if (SC->FindAttribute(anAttr, "AttributeName")) {
      aName = SALOMEDS::AttributeName::_narrow(anAttr);
      Item = AddItem (aName->Value(),SC->GetID(),ior);
    } else {
      Item = AddItem (dataType,SC->GetID(),ior);
    }
    Update ( Study, SALOMEDS::SObject::_narrow(SC) );
    
  } else {
    itcomp = Study->NewComponentIterator();
    for (; itcomp->More(); itcomp->Next()) {
      SALOMEDS::SComponent_var SC1 = itcomp->Value();
      dataType = SC1->ComponentDataType();
      
      if (SC1->FindAttribute(anAttr, "AttributeIOR")) {
        anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
        ior = anIOR->Value();
      }
      
      if ( QString(dataType).compare("Interface Applicative") != 0 ) {
	if (SC1->FindAttribute(anAttr, "AttributeName")) {
          aName = SALOMEDS::AttributeName::_narrow(anAttr);
	  Item = AddItem (aName->Value(), SC1->GetID(), ior);
	} else {
	  Item = AddItem (dataType, SC1->GetID(), ior);
	}
	
	Update ( Study, SALOMEDS::SObject::_narrow(SC1) );
      }
    }
  }
}

QListViewItem* QAD_NameBrowser::AddItem(const QString& theText,
					const QString& theEntry,
					const QString& theIOR)

{
  QListViewItem* anItem = new QListViewItem( ListView1 );
  anItem->setText(0, theText);
  anItem->setText(1, theEntry);
  anItem->setText(2, theIOR);

  return anItem;
}

/*!
  Called when NameBrowser is about to close
*/
void QAD_NameBrowser::closeEvent(QCloseEvent* e)
{
  QAD_Application::getDesktop()->closeNameBrowser();
  e->accept();
}
