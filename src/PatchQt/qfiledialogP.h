/////////////////////////////////////////////////////////////////////////////
// Module      : PatchQt
// File        : qfiledialogP.h
// Description : the patch for Qt's QFileDialog class (qfiledialog.h)
/////////////////////////////////////////////////////////////////////////////

/****************************************************************************
** $Id$
**
** Definition of QFileDialog class
**
** Created : 950428
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of the dialogs module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef QFILEDIALOGP_H
#define QFILEDIALOGP_H

#include <qglobal.h>
#if (QT_VERSION >= 0x030303)

#include <qfiledialog.h>
// mpv: do not use patches for qt version >= 3.3.3
#define QFileIconProviderP QFileIconProvider
#define QFilePreviewP QFilePreview
#define QFileDialogP QFileDialog

#else

class QPushButton;
class QButton;
class QLabel;
class QWidget;
class QFileDialog;
class QTimer;
class QNetworkOperation;
class QLineEdit;
class QListViewItem;
class QListBoxItem;
class QProgressBar;
class QFileDialogPrivate;
class QFileDialogQFileListView;

#ifndef QT_H
#include "qdir.h"
#include "qdialog.h"
#include "qurloperator.h"
#include "qurlinfo.h"
#include "qlistbox.h"
#include "qlistview.h"
#include "qlineedit.h"
#endif // QT_H

#ifndef QT_NO_FILEDIALOG

class Q_EXPORT QFileIconProviderP : public QObject
{
    Q_OBJECT
public:
    QFileIconProviderP( QObject * parent = 0, const char* name = 0 );
    virtual const QPixmap * pixmap( const QFileInfo & );

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QFileIconProviderP( const QFileIconProviderP & );
    QFileIconProviderP& operator=( const QFileIconProviderP & );
#endif
};

class Q_EXPORT QFilePreviewP
{
public:
    QFilePreviewP();
    virtual void previewUrl( const QUrl &url ) = 0;

};

class Q_EXPORT QFileDialogP : public QDialog
{
    Q_OBJECT
    Q_ENUMS( Mode ViewMode PreviewMode )
    // ##### Why are this read-only properties ?
    Q_PROPERTY( QString selectedFile READ selectedFile )
    Q_PROPERTY( QString selectedFilter READ selectedFilter )
    Q_PROPERTY( QStringList selectedFiles READ selectedFiles )
    // #### Should not we be able to set the path ?
    Q_PROPERTY( QString dirPath READ dirPath )
    Q_PROPERTY( bool showHiddenFiles READ showHiddenFiles WRITE setShowHiddenFiles )
    Q_PROPERTY( Mode mode READ mode WRITE setMode )
    Q_PROPERTY( ViewMode viewMode READ viewMode WRITE setViewMode )
    Q_PROPERTY( PreviewMode previewMode READ previewMode WRITE setPreviewMode )
    Q_PROPERTY( bool infoPreview READ isInfoPreviewEnabled WRITE setInfoPreviewEnabled )
    Q_PROPERTY( bool contentsPreview READ isContentsPreviewEnabled WRITE setContentsPreviewEnabled )

public:
    QFileDialogP( const QString& dirName, const QString& filter = QString::null,
		 QWidget* parent=0, const char* name=0, bool modal = FALSE );
    QFileDialogP( QWidget* parent=0, const char* name=0, bool modal = FALSE );
    ~QFileDialogP();

    // recommended static functions

    static QString getOpenFileName( const QString &initially = QString::null,
				    const QString &filter = QString::null,
				    QWidget *parent = 0, const char* name = 0,
				    const QString &caption = QString::null,
				    QString *selectedFilter = 0,
				    bool resolveSymlinks = TRUE);
    static QString getSaveFileName( const QString &initially = QString::null,
				    const QString &filter = QString::null,
				    QWidget *parent = 0, const char* name = 0,
				    const QString &caption = QString::null,
				    QString *selectedFilter = 0,
				    bool resolveSymlinks = TRUE);
    static QString getExistingDirectory( const QString &dir = QString::null,
					 QWidget *parent = 0,
					 const char* name = 0,
					 const QString &caption = QString::null,
					 bool dirOnly = TRUE,
					 bool resolveSymlinks = TRUE);
    static QStringList getOpenFileNames( const QString &filter= QString::null,
					 const QString &dir = QString::null,
					 QWidget *parent = 0,
					 const char* name = 0,
					 const QString &caption = QString::null,
					 QString *selectedFilter = 0,
					 bool resolveSymlinks = TRUE);

    // other static functions

    static void setIconProvider( QFileIconProviderP * );
    static QFileIconProviderP* iconProvider();

    // non-static function for special needs

    QString selectedFile() const;
    QString selectedFilter() const;
    virtual void setSelectedFilter( const QString& );
    virtual void setSelectedFilter( int );

    void setSelection( const QString &);

    void selectAll( bool b );

    QStringList selectedFiles() const;

    QString dirPath() const;

    void setDir( const QDir & );
    const QDir *dir() const;

    void setShowHiddenFiles( bool s );
    bool showHiddenFiles() const;

    void rereadDir();
    void resortDir();

    enum Mode { AnyFile, ExistingFile, Directory, ExistingFiles, DirectoryOnly };
    void setMode( Mode );
    Mode mode() const;

    enum ViewMode { Detail, List };
    enum PreviewMode { NoPreview, Contents, Info };
    void setViewMode( ViewMode m );
    ViewMode viewMode() const;
    void setPreviewMode( PreviewMode m );
    PreviewMode previewMode() const;

    bool eventFilter( QObject *, QEvent * );

    bool isInfoPreviewEnabled() const;
    bool isContentsPreviewEnabled() const;
    void setInfoPreviewEnabled( bool );
    void setContentsPreviewEnabled( bool );

    void setInfoPreview( QWidget *w, QFilePreviewP *preview );
    void setContentsPreview( QWidget *w, QFilePreviewP *preview );

    QUrl url() const;

    void addFilter( const QString &filter );

public slots:
    void done( int );
    void setDir( const QString& );
    void setUrl( const QUrlOperator &url );
    void setFilter( const QString& );
    void setFilters( const QString& );
    void setFilters( const char ** );
    void setFilters( const QStringList& );

protected:
    void resizeEvent( QResizeEvent * );
    void keyPressEvent( QKeyEvent * );

    void addWidgets( QLabel *, QWidget *, QPushButton * );
    void addToolButton( QButton *b, bool separator = FALSE );
    void addLeftWidget( QWidget *w );
    void addRightWidget( QWidget *w );

signals:
    void fileHighlighted( const QString& );
    void fileSelected( const QString& );
    void filesSelected( const QStringList& );
    void dirEntered( const QString& );
    void filterSelected( const QString& );

private slots:
    void detailViewSelectionChanged();
    void listBoxSelectionChanged();
    void changeMode( int );
    void fileNameEditReturnPressed();
    void stopCopy();
    void removeProgressDia();

    void fileSelected( int );
    void fileHighlighted( int );
    void dirSelected( int );
    void pathSelected( int );

    void updateFileNameEdit( QListViewItem *);
    void selectDirectoryOrFile( QListViewItem * );
    void popupContextMenu( QListViewItem *, const QPoint &, int );
    void popupContextMenu( QListBoxItem *, const QPoint & );
    void updateFileNameEdit( QListBoxItem *);
    void selectDirectoryOrFile( QListBoxItem * );
    void fileNameEditDone();

    void okClicked();
    void filterClicked(); // not used
    void cancelClicked();

    void cdUpClicked();
    void newFolderClicked();

    void fixupNameEdit();

    void doMimeTypeLookup();

    void updateGeometries();
    void modeButtonsDestroyed();
    void urlStart( QNetworkOperation *op );
    void urlFinished( QNetworkOperation *op );
    void dataTransferProgress( int bytesDone, int bytesTotal, QNetworkOperation * );
    void insertEntry( const QValueList<QUrlInfo> &fi, QNetworkOperation *op );
    void removeEntry( QNetworkOperation * );
    void createdDirectory( const QUrlInfo &info, QNetworkOperation * );
    void itemChanged( QNetworkOperation * );
    void goBack();

private:
    enum PopupAction {
	PA_Open = 0,
	PA_Delete,
	PA_Rename,
	PA_SortName,
	PA_SortSize,
	PA_SortType,
	PA_SortDate,
	PA_SortUnsorted,
	PA_Cancel,
	PA_Reload,
	PA_Hidden
    };

    void init();
    bool trySetSelection( bool isDir, const QUrlOperator &, bool );
    void deleteFile( const QString &filename );
    void popupContextMenu( const QString &filename, bool withSort,
			   PopupAction &action, const QPoint &p );

    QDir reserved; // was cwd
    QString fileName;

    friend class QFileDialogQFileListView;
    friend class QFileListBox;

    QFileDialogPrivate *d;
    QFileDialogQFileListView  *files;

    QLineEdit  *nameEdit; // also filter
    QPushButton *okB;
    QPushButton *cancelB;

#if defined(Q_WS_WIN)
    static QString winGetOpenFileName( const QString &initialSelection,
				       const QString &filter,
				       QString* workingDirectory,
				       QWidget *parent = 0,
				       const char* name = 0,
				       const QString& caption = QString::null,
				       QString* selectedFilter = 0 );
    static QString winGetSaveFileName( const QString &initialSelection,
				       const QString &filter,
				       QString* workingDirectory,
				       QWidget *parent = 0,
				       const char* name = 0,
				       const QString& caption = QString::null,
    				       QString* selectedFilter = 0 );
    static QStringList winGetOpenFileNames( const QString &filter,
					    QString* workingDirectory,
					    QWidget *parent = 0,
					    const char* name = 0,
					    const QString& caption = QString::null,
					    QString* selectedFilter = 0 );
    static QString winGetExistingDirectory( const QString &initialDirectory,
					    QWidget* parent = 0,
					    const char* name = 0,
					    const QString& caption = QString::null);
    static QString resolveLinkFile( const QString& linkfile );
#endif
#if defined(Q_WS_MACX) || defined(Q_WS_MAC9)
    static QString macGetSaveFileName( const QString &, const QString &, 
				       QString *, QWidget *, const char*,
				       const QString& );
    static QStringList macGetOpenFileNames( const QString &, QString*,
					    QWidget *, const char *,
					    const QString&, bool = TRUE,
					    bool = FALSE );
#endif


private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QFileDialogP( const QFileDialogP & );
    QFileDialogP &operator=( const QFileDialogP & );
#endif
};

/******************************************************************
 *
 * Definitions of view classes
 *
 ******************************************************************/

class QRenameEdit : public QLineEdit
{
    Q_OBJECT

public:
    QRenameEdit( QWidget *parent )
	: QLineEdit( parent, "qt_rename_edit" )
    {}

protected:
    void keyPressEvent( QKeyEvent *e );
    void focusOutEvent( QFocusEvent *e );

signals:
    void escapePressed();

};

class QFileListBox : public QListBox
{
    friend class QFileDialogP;

    Q_OBJECT

private:
    QFileListBox( QWidget *parent, QFileDialogP *d );

    void clear();
    void show();
    void startRename( bool check = TRUE );
    void viewportMousePressEvent( QMouseEvent *e );
    void viewportMouseReleaseEvent( QMouseEvent *e );
    void viewportMouseDoubleClickEvent( QMouseEvent *e );
    void viewportMouseMoveEvent( QMouseEvent *e );
#ifndef QT_NO_DRAGANDDROP
    void viewportDragEnterEvent( QDragEnterEvent *e );
    void viewportDragMoveEvent( QDragMoveEvent *e );
    void viewportDragLeaveEvent( QDragLeaveEvent *e );
    void viewportDropEvent( QDropEvent *e );
    bool acceptDrop( const QPoint &pnt, QWidget *source );
    void setCurrentDropItem( const QPoint &pnt );
#endif
    void keyPressEvent( QKeyEvent *e );

private slots:
    void rename();
    void cancelRename();
    void doubleClickTimeout();
    void changeDirDuringDrag();
    void dragObjDestroyed();
    void contentsMoved( int, int );

private:
    QRenameEdit *lined;
    QFileDialogP *filedialog;
    bool renaming;
    QTimer* renameTimer;
    QListBoxItem *renameItem, *dragItem;
    QPoint pressPos, oldDragPos;
    bool mousePressed;
    int urls;
    QString startDragDir;
    QListBoxItem *currDropItem;
    QTimer *changeDirTimer;
    bool firstMousePressEvent;
    QUrlOperator startDragUrl;

};


class QFileDialogQFileListView : public QListView
{
    Q_OBJECT

public:
    QFileDialogQFileListView( QWidget *parent, QFileDialogP *d );

    void clear();
    void startRename( bool check = TRUE );
    void setSorting( int column, bool increasing = TRUE );

    QRenameEdit *lined;
    bool renaming;
    QListViewItem *renameItem;

private:
    void viewportMousePressEvent( QMouseEvent *e );
    void viewportMouseDoubleClickEvent( QMouseEvent *e );
    void keyPressEvent( QKeyEvent *e );
    void viewportMouseReleaseEvent( QMouseEvent *e );
    void viewportMouseMoveEvent( QMouseEvent *e );
#ifndef QT_NO_DRAGANDDROP
    void viewportDragEnterEvent( QDragEnterEvent *e );
    void viewportDragMoveEvent( QDragMoveEvent *e );
    void viewportDragLeaveEvent( QDragLeaveEvent *e );
    void viewportDropEvent( QDropEvent *e );
    bool acceptDrop( const QPoint &pnt, QWidget *source );
    void setCurrentDropItem( const QPoint &pnt );
#endif

private slots:
    void rename();
    void cancelRename();
    void changeSortColumn2( int column );
    void doubleClickTimeout();
    void changeDirDuringDrag();
    void dragObjDestroyed();
    void contentsMoved( int, int );

private:
    QFileDialogP *filedialog;
    QTimer* renameTimer;
    QPoint pressPos, oldDragPos;
    bool mousePressed;
    int urls;
    QString startDragDir;
    QListViewItem *currDropItem, *dragItem;
    QTimer *changeDirTimer;
    bool firstMousePressEvent;
    bool ascending;
    int sortcolumn;
    QUrlOperator startDragUrl;

};

/****************************************************************************
 *
 * Classes for copy progress dialog
 *
 ****************************************************************************/

class QFDProgressAnimation : public QWidget
{
    Q_OBJECT

public:
    QFDProgressAnimation( QWidget *parent );
    void start();

private slots:
    void next();

protected:
    void paintEvent( QPaintEvent *e );

private:
    int step;
    QTimer *timer;

};

class QFDProgressDialog : public QDialog
{
    Q_OBJECT

public:
    QFDProgressDialog( QWidget *parent, const QString &fn, int steps );

    void setReadProgress( int p );
    void setWriteProgress( int p );
    void setWriteLabel( const QString &s );

signals:
    void cancelled();

private:
    QProgressBar *readBar;
    QProgressBar *writeBar;
    QLabel *writeLabel;
    QFDProgressAnimation *animation;

};

#endif

#endif

#endif // QFILEDIALOG_H
