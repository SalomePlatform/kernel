//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_ListView.h
//  Author : Vadim SANDLER
//  Module : SALOME
//  $Header$

#if !defined(_QAD_ListView_HeaderFile)
#define _QAD_ListView_HeaderFile

#include <qlistview.h>
#include <qlist.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qlineedit.h>
#include <qcombobox.h>

//VRV: porting on Qt 3.0.5
#if QT_VERSION >= 0x030005
#include <qtoolbutton.h> 
#endif
//VRV: porting on Qt 3.0.5

#include "QAD_Popup.h"

#include <TColStd_ListOfInteger.hxx>
#include <TColStd_ListOfReal.hxx>

// enumeration for ListView updating mode
enum UpdateType {
  utCancel        = -1,           // cancel updating
  utNone          =  0,           // needs no any update
  utUpdateItem,                   // update one item
  utUpdateParent,                 // update parent item too
  utUpdateViewer,                 // update all viewer
  utUpdateAll                     // strong update
};

class QAD_ListViewItem;
class QAD_EntityEdit;

//================================================================
// Class        : QAD_ListView
// Description  : parent class for Data Viewer and Properties Viewer
//================================================================
class QAD_ListView : public QListView , public QAD_PopupServer  {
  
  Q_OBJECT
    
public:
// constructor  
  QAD_ListView(QWidget *parent);
// destructor  
  virtual ~QAD_ListView();

// updates list viewer
  virtual void updateViewer();
// updtes currently selected item(s)
  virtual void updateSelected();

// fills popup with items
  void onCreatePopup();
// delete popup
  void deletePopupMenu();

// setting editing of items availbale/not available
  void enableEditing(bool theFlag);
// says if editing is enabled
  bool isEnableEditing();
// accepts user input by calling finishEditing(true)
  void accept();

// clears view
  void clear();

// event filter
  bool eventFilter(QObject* object, QEvent* event) ;

// gets current tooltip for list view
// returns valid rect in success
  QRect tip(QPoint aPos, QString& aText, QRect& dspRect, QFont& dspFnt) const;

protected:
// handler for mouse button release event (execute popup menu)
  void contentsMouseReleaseEvent(QMouseEvent* e);
// handler for resize event
  void resizeEvent(QResizeEvent* e);
// handler for paint event
  void viewportPaintEvent(QPaintEvent* e);
// finishes editing of entity
  virtual UpdateType finishEditing(bool ok);

// returns true if mouse events are enabled
  bool isMouseEnabled();
// enables/disables mouse events (excluding MouseMove)
  void enableMouse(bool enable);
  
protected slots:
// called when selection changed in list
  virtual void onSelectionChanged();
// called when user finishes in editing of item
  void onEditOk();
// called when user cancels item editing
  void onEditCancel();
// called when columns sizes are changed
  void onHeaderSizeChange(int, int, int);

protected:
  QAD_EntityEdit*       myEdit;
  QAD_ListViewItem*     myEditedItem;
  bool                  myEditingEnabled;
  bool                  myMouseEnabled;
};


//================================================================
// Class    : QAD_ListViewItem
// Purpose  : List Viewer entity
//================================================================
class QAD_ListViewItem : public QListViewItem {

public:
// constructor
  QAD_ListViewItem(QAD_ListView*     parent);
// constructor
  QAD_ListViewItem(QAD_ListView*     parent, 
		   QAD_ListViewItem* after);
// constructor  
  QAD_ListViewItem(QAD_ListView*     parent,
		   const QString     theName,
		   const bool        theEditable = false);
// constructor  
  QAD_ListViewItem(QAD_ListView*     parent,
		   const QString     theName,
		   const QString     theValue, 
		   const bool        theEditable = false);
// constructor
  QAD_ListViewItem(QAD_ListViewItem* parent,
		   const QString     theName,
		   const bool        theEditable = false);
// constructor
  QAD_ListViewItem(QAD_ListView*     parent,
		   QAD_ListViewItem* after,
		   const QString     theName,
		   const bool        theEditable = false);
// constructor
  QAD_ListViewItem(QAD_ListViewItem* parent,
		   QAD_ListViewItem* after,
		   const QString     theName,
		   const bool        theEditable = false);
// constructor  
  QAD_ListViewItem(QAD_ListViewItem* parent,
		   const QString     theName,
		   const QString     theValue, 
		   const bool        theEditable = false);
// constructor  
  QAD_ListViewItem(QAD_ListView*     parent,
		   QAD_ListViewItem* after,
		   const QString     theName,
		   const QString     theValue, 
		   const bool        theEditable = false);
// constructor  
  QAD_ListViewItem(QAD_ListViewItem* parent,
		   QAD_ListViewItem* after,
		   const QString     theName,
		   const QString     theValue, 
		   const bool        theEditable = false);
// destructor
  ~QAD_ListViewItem();

// returns full path to the entity (full name)
  QString            fullName();
  
// expands all entities beginning from this level
  void               openAllLevels();
// updates all entites beginning from this level
  virtual void       updateAllLevels();

// returns true if entity can be edited
  bool               isEditable() const;
// set entity as editable/uneditable
  void               setEditable(bool theEditable);

// returns true if entitiy is accepted after editing
  bool               isAccepted() const;
// set entity accepted or not after editing
  void               setAccepted(bool theAccepted);

// returns name of entity (as default it is text in first column)
  virtual QString    getName() const;
// sets name of entity (as default it is text in first column)
  virtual UpdateType setName(const QString theName);

// returns value of entity (as default it is text in second column)
  virtual QString    getValue() const;
// sets value of entity (as default it is text in second column)
  virtual UpdateType setValue(const QString theValue);

// creates control for editing and fills it with values
  QAD_EntityEdit*    startEditing();
// fills widget with initial values (list or single value)
  virtual void       fillWidgetWithValues(QAD_EntityEdit* theWidget);
// finishes editing of entity
  virtual UpdateType finishEditing(QAD_EntityEdit* theWidget);

// returns type of edit control (0 - edit box, 1 - combo box, 2 - editable combo box)
  virtual int        getEditingType();
// sets type of edit control (0 - edit box, 1 - combo box, 2 - editable combo box)
  virtual void       setEditingType(const int);
// returns edited column
  virtual int        getEditedColumn();
// returns type of edited value (string, int, double)
  virtual int        getValueType();
// sets type of edited value (string, int, double)
  virtual void       setValueType(const int);

// gets user type 
  virtual int        getUserType();
// sets user type 
  virtual void       setUserType(const int);

// returns buttons for editing widget
  virtual int        getButtons();
// sets  buttons for editing widget
  virtual void       setButtons(const int);
// returns text for tooltip
  QString            tipText();
// calculates rectangle which should contain items tip
  QRect              tipRect();
// calculates rect of item text in viewport coordinates
  QRect              textRect(const int column) const;
// calculates full rect of item data in viewport coordinates
  QRect              itemRect(const int column) const;

protected:
// initialization
  void               init();

private:
  bool myEditable;
  bool myAccepted;
  int  myEditingType;
  int  myValueType;
  int  myButtons;
  int  myUserType; // user are welcome to put additional data here and use it in fillWidgetWithValues()
};


//================================================================
// Class        : QAD_EditBox
// Description  : edit box for list entity editing
//================================================================
class QAD_EditBox: public QLineEdit {

  Q_OBJECT

public:
// constructor  
  QAD_EditBox(QWidget* parent);
  
protected:
// event filter (for KeyPress event)
  void keyPressEvent(QKeyEvent* e);
  
signals:
  void escapePressed();

};


//================================================================
// Class        : QAD_ComboBox
// Description  : combo box for list entity editing
//================================================================
class QAD_ComboBox: public QComboBox {

  Q_OBJECT

public:
// constructor
  QAD_ComboBox(bool rw, QWidget* parent = 0, const char* name = 0);

// searches item in list and returns its index
  int findItem(const QString theText);
// adds item in combo box
  void insertItem(const QString& theValue, int theIndex = -1);
// adds list of items in combo box
  void insertList(const QStringList& theList);
// adds item in combo box
  void insertItem(const int theValue);
// adds list of items in combo box
  void insertList(const TColStd_ListOfInteger& theList);
// adds item in combo box
  void insertItem(const double theValue);
// adds list of items in combo box
  void insertList(const TColStd_ListOfReal& theList);
  
};


//================================================================
// Class        : QAD_EntityEdit
// Description  : edit control for QAD List View
//================================================================
class QAD_EntityEdit : public QWidget {

  Q_OBJECT
  
public:
// enum for edit control type
  enum {
    etLineEdit,           // simple edit box
    etComboBox,           // non-editable combo box
    etComboEdit           // editable combo box
  };
// enum for value type
  enum {
    vtString,             // string
    vtInteger,            // integer value
    vtDouble              // double value
  };
// enum for insertion order
  enum {
    atBottom,
    atTop,
    atBeforeCurrent,
    atAfterCurrent
  };
// enum for edit box buttons (Apply = Cancel)
  enum {
    btApply  = 0x0001,
    btCancel = 0x0002
  };

// constructor
  QAD_EntityEdit(QWidget* parent, 
                     int      controlType = etLineEdit, 
                     int      valueType   = vtString,
                     bool     butApply    = false, 
                     bool     butCancel   = false);
// destructor
  ~QAD_EntityEdit();

// finishes editing
  void finishEditing();
// clears edit/combo box
  void clear();
// returns current text in edit box or combo box
  QString getText();
// sets text
  void setText(const QString& theText);
// adds item in combo box
  void insertItem(const QString& theValue, 
                  bool           theSetCurrent = false,
                  int            theOrder = atBottom);
// adds list of items in combo box
  void insertList(const QStringList& theList, 
                  const int          theCurrent = -1);
// adds item in combo box
  void insertItem(const int theValue, 
                  bool  theSetCurrent = false);
// adds list of items in combo box
  void insertList(const TColStd_ListOfInteger& theList, 
                  const int                    theCurrent = -1);
// adds item in combo box
  void insertItem(const double theValue, 
                  bool         theSetCurrent = false);
// adds list of items in combo box
  void insertList(const TColStd_ListOfReal& theList, 
                  const int                 theCurrent = -1);
// gets actual widget
  QWidget* getControl();
// redirect focus to corresponding widget
  void setFocus();
// sets validator for the control
  void setValidator(const QValidator*);
// shows/hides buttons
  void showButtons(bool show);
// enables/disables data duplication (for combo box)
  void setDuplicatesEnabled(bool enabled);

protected:
// event filter for KeyPress event
  void keyPressEvent ( QKeyEvent * e);

private slots:
// called when item activated in combo box
  void onComboActivated(const QString&);
// called when text changed in control
  void onTextChanged(const QString&);
// called when user presses Apply button
  void onApply();
// called when user presses Cancel button
  void onCancel();

signals:
  void returnPressed();
  void escapePressed();

private:
// Widgets
  QAD_EditBox*  myEdit;
  QAD_ComboBox* myCombo;
  QToolButton*      myApplyBtn;
  QToolButton*      myCancelBtn;

  QString           myString;

};


#endif
