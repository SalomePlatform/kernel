//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEGUI_StudyPropertiesDlg.cxx
//  Author : Sergey ANIKIN
//  Module : SALOME
//  $Header$

#include "SALOMEGUI_StudyPropertiesDlg.h"

#include "QAD_Desktop.h"
#include "QAD_ListView.h"

#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include <qpushbutton.h>
#include <qlayout.h>
using namespace std;

#define  DEFAULT_MARGIN 11
#define DEFAULT_SPACING 6
#define     SPACER_SIZE 5
#define MIN_LIST_WIDTH  300
#define MIN_LIST_HEIGHT 150

class SALOMEGUI_PropItem : public QAD_ListViewItem
{
public:
// constructor  
  SALOMEGUI_PropItem(QAD_ListView*     parent,
		     const QString     theName,
		     const bool        theEditable,
		     const int         theUserType) :
  QAD_ListViewItem(parent, theName, theEditable)
  {
    setUserType(theUserType);
  }
// constructor
  SALOMEGUI_PropItem(QAD_ListView*     parent,
		     QAD_ListViewItem* after,
		     const QString     theName,
		     const bool        theEditable,
		     const int         theUserType) :
  QAD_ListViewItem(parent, after, theName, theEditable)
  {
    setUserType(theUserType);
  }
// fills widget with initial values (list or single value)
  void fillWidgetWithValues(QAD_EntityEdit* theWidget)
  {
    QStringList list;
    switch(getUserType()) {
    case SALOMEGUI_StudyPropertiesDlg::prpModeId:
      {
	list << QObject::tr("PRP_MODE_FROM_SCRATCH")<< QObject::tr("PRP_MODE_FROM_COPYFROM");
	theWidget->insertList(list);
	break;
      }
    case SALOMEGUI_StudyPropertiesDlg::prpLockedId:
      {
	list << QObject::tr("PRP_NO") << QObject::tr("PRP_YES");
	theWidget->insertList(list, getValue() == QObject::tr("PRP_NO") ? 0 : 1);    
	break;
      }
    case SALOMEGUI_StudyPropertiesDlg::prpModificationsId:
      {
	QAD_Study* study = QAD_Application::getDesktop()->getActiveStudy();
	if (study) { 
	  SALOMEDS::Study_var studyDoc = study->getStudyDocument();
	  SALOMEDS::AttributeStudyProperties_var propAttr;
	  if (!studyDoc->_is_nil()) {
	    propAttr = studyDoc->GetProperties();
	    if (!propAttr->_is_nil()) {
	      SALOMEDS::StringSeq_var aUsers;
	      SALOMEDS::LongSeq_var  aMins, aHours, aDays, aMonths, aYears;
	      propAttr->GetModificationsList(aUsers, aMins, aHours, aDays, aMonths, aYears, false);
	      int aCnt = aUsers->length();
	      for ( int i = 0; i < aCnt; i++ ) {
		QString val;
		val.sprintf("%2.2d/%2.2d/%2d %2.2d:%2.2d", 
			    aDays  [i], 
			    aMonths[i], 
			    aYears [i], 
			    aHours [i], 
			    aMins  [i]);
		val = val + " : " + QString(aUsers[i]);
		list.prepend(val);
	      }
	      theWidget->setDuplicatesEnabled(true);
	      theWidget->insertList(list);    
	    }
	  }
	}
	break;
      }
    default:
      {
	QAD_ListViewItem::fillWidgetWithValues(theWidget);
	break;
      }
    }
  }
// finishes editing of entity
  virtual UpdateType finishEditing(QAD_EntityEdit* theWidget) {
    if ( getUserType() == SALOMEGUI_StudyPropertiesDlg::prpModificationsId )
      return utCancel;
    else
      return QAD_ListViewItem::finishEditing(theWidget);
  }
};

SALOMEGUI_StudyPropertiesDlg::SALOMEGUI_StudyPropertiesDlg(QWidget* parent)
     : QDialog(parent, "", TRUE, WStyle_Customize | WStyle_NormalBorder | WStyle_Title | WStyle_SysMenu ),
       myChanged( false )
{
  setCaption(tr("TLT_STUDY_PROPERTIES"));
  setSizeGripEnabled( true );

  clearWFlags(Qt::WStyle_ContextHelp);

  QGridLayout* mainLayout = new QGridLayout(this);
  mainLayout->setMargin(DEFAULT_MARGIN);
  mainLayout->setSpacing(DEFAULT_SPACING);
  
  myPropList = new QAD_ListView(this);
  myPropList->addColumn("");
  myPropList->addColumn("");
  myPropList->enableEditing(TRUE);
  myPropList->setMinimumSize(MIN_LIST_WIDTH, MIN_LIST_HEIGHT);
  mainLayout->addMultiCellWidget(myPropList, 0, 0, 0, 2);

  myOKBtn = new QPushButton(tr("BUT_OK"), this);
  mainLayout->addWidget(myOKBtn, 1, 0);

  myCancelBtn = new QPushButton(tr("BUT_CANCEL"), this);
  mainLayout->addWidget(myCancelBtn, 1, 2);

  QSpacerItem* spacer1 = new QSpacerItem(SPACER_SIZE, SPACER_SIZE, QSizePolicy::Expanding, QSizePolicy::Minimum);
  mainLayout->addItem(spacer1, 1, 1);

  // Display study properties
  QAD_Study* study = ((QAD_Desktop*)parent)->getActiveStudy();
  if (study) myStudyDoc = study->getStudyDocument();

  initData();

  connect(myOKBtn,     SIGNAL(clicked()), this, SLOT(onOK()));
  connect(myCancelBtn, SIGNAL(clicked()), this, SLOT(reject()));
}

SALOMEGUI_StudyPropertiesDlg::~SALOMEGUI_StudyPropertiesDlg()
{
}

void SALOMEGUI_StudyPropertiesDlg::initData()
{
  bool hasData = !myStudyDoc->_is_nil();
  SALOMEDS::AttributeStudyProperties_var propAttr;
  if (hasData) propAttr = myStudyDoc->GetProperties();
  hasData = hasData && !propAttr->_is_nil();

  // Study author's name
  SALOMEGUI_PropItem* item = new SALOMEGUI_PropItem(myPropList, tr("PRP_AUTHOR")+":", true, prpAuthorId);
  if (hasData) item->setValue(propAttr->GetUserName());

  // Date of creation
  item = new SALOMEGUI_PropItem(myPropList, item, tr("PRP_DATE")+":", false, prpDateId);
  if (hasData) {
    CORBA::Long minutes, hours, day, month, year;
    if (propAttr->GetCreationDate(minutes, hours, day, month, year)) {
      QString strDate;
      strDate.sprintf("%2.2d/%2.2d/%2d %2.2d:%2.2d", day, month, year, hours, minutes);
      item->setValue(strDate);
    }
  }
  
  // Creation mode
//  item = new SALOMEGUI_PropItem(myPropList, item, tr("PRP_MODE")+":", true, prpModeId);
//  item->setEditingType(QAD_EntityEdit::etComboBox);
//  if (hasData) item->setValue(propAttr->GetCreationMode());

  // Locked or not
  item = new SALOMEGUI_PropItem(myPropList, item, tr("PRP_LOCKED")+":", true, prpLockedId);
  item->setEditingType(QAD_EntityEdit::etComboBox);  
  if (hasData) (propAttr->IsLocked()) ? item->setValue(tr("PRP_YES")) : item->setValue(tr("PRP_NO"));

  // Saved or not
  item = new SALOMEGUI_PropItem(myPropList, item, tr("PRP_MODIFIED")+":", false, prpSavedId);
  if (hasData) {
    if (propAttr->IsModified())
      item->setValue(tr("PRP_YES"));
    else
      item->setValue(tr("PRP_NO"));
  }

  // Modifications list
  item = new SALOMEGUI_PropItem(myPropList, item, tr("PRP_MODIFICATIONS")+":", true, prpModificationsId); 
  item->setEditingType(QAD_EntityEdit::etComboBox);  
  if (hasData) { 
    SALOMEDS::StringSeq_var aUsers;
    SALOMEDS::LongSeq_var   aMins, aHours, aDays, aMonths, aYears;
    propAttr->GetModificationsList(aUsers, aMins, aHours, aDays, aMonths, aYears, false);
    int aLast = aUsers->length()-1;
    if (aLast >= 0) {
      QString val;
      val.sprintf("%2.2d/%2.2d/%2d %2.2d:%2.2d", 
		  aDays  [aLast], 
		  aMonths[aLast], 
		  aYears [aLast], 
		  aHours [aLast], 
		  aMins  [aLast]);
      val = val + " : " + QString(aUsers[aUsers->length()-1]);
      item->setValue(val);
    }
  }

  myOKBtn->setEnabled(hasData);
}

bool SALOMEGUI_StudyPropertiesDlg::acceptData()
{
  return TRUE;
}

void SALOMEGUI_StudyPropertiesDlg::onOK()
{
  myPropList->accept();

  if (acceptData()) {
    SALOMEDS::AttributeStudyProperties_var propAttr = myStudyDoc->GetProperties();
    myChanged = propChanged();
    if ( !propAttr->_is_nil() && myChanged ) {
      QListViewItemIterator it( myPropList );
      // iterate through all items of the listview
      for ( ; it.current(); ++it ) {
	SALOMEGUI_PropItem* item = (SALOMEGUI_PropItem*)(it.current());
	switch (item->getUserType()) {
	case prpAuthorId:
	  propAttr->SetUserName(item->getValue().stripWhiteSpace().latin1());
	  break;
	case prpModeId:
	  propAttr->SetCreationMode(item->getValue().stripWhiteSpace().latin1());
	  break;
	case prpLockedId:
	  propAttr->SetLocked(item->getValue().compare(tr("PRP_YES")) == 0);
	  break;
	default:
	  break;
	}
      }
    }
    accept();
  }
}

bool SALOMEGUI_StudyPropertiesDlg::propChanged() {
  SALOMEDS::AttributeStudyProperties_var propAttr = myStudyDoc->GetProperties();
  if ( !propAttr->_is_nil() ) {
    QListViewItemIterator it( myPropList );
    // iterate through all items of the listview
    for ( ; it.current(); ++it ) {
      SALOMEGUI_PropItem* item = (SALOMEGUI_PropItem*)(it.current());
      switch (item->getUserType()) {
      case prpAuthorId:
	if ( QString( propAttr->GetUserName() ) != item->getValue().stripWhiteSpace() ) {
	  return true;
	}
	break;
      case prpModeId:
	if ( QString( propAttr->GetCreationMode() ) != item->getValue().stripWhiteSpace() ) {
	  return true;
        }
	break;
      case prpLockedId:
	{
	  bool bLocked = item->getValue().compare( tr( "PRP_YES" ) ) == 0;
	  if ( propAttr->IsLocked() != bLocked ) {
	    return true;
	  }
	  break;
	}
      default:
	break;
      }
    }
  }
  return false;
}
