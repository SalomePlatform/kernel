//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_FileDlg.h
//  Author : 
//  Module : SALOME
//  $Header$

#ifndef QAD_FILEDIALOG_H
#define QAD_FILEDIALOG_H

#include <qcombobox.h>

#include <qfiledialogP.h>
#include "QAD.h"
#include "QAD_FileValidator.h"

class QAD_EXPORT QAD_FileDlg : public QFileDialogP
{
    Q_OBJECT

public:
    QAD_FileDlg( QWidget* parent, bool open, bool showQuickDir = true, bool modal = true );
    virtual ~QAD_FileDlg();

public:    
    bool               isOpenDlg()    const;    
    QString            selectedFile() const;

    void               setValidator( QAD_FileValidator* );

    static QString     getFileName( QWidget*           parent, 
				    const QString&     initial, 
				    const QStringList& filters, 
				    const QString&     caption,
				    bool               open,
				    bool               showQuickDir = true,
				    QAD_FileValidator* validator = 0);
    static QStringList getOpenFileNames( QWidget*           parent, 
					 const QString&     initial, 
					 const QStringList& filters, 
					 const QString&     caption,
					 bool               showQuickDir = true, 
					 QAD_FileValidator* validator = 0);
    static QString     getExistingDirectory ( QWidget*       parent,
					      const QString& initial,
					      const QString& caption, 
					      bool           showQuickDir = true);

protected:
  virtual bool         acceptData();
                            
private:
    void               addExtension();
    bool               processPath( const QString& path );

protected slots:
    void               accept();        
    void               reject(); 
    void               quickDir(const QString&);

protected:
    bool               myOpen;             /* open/save selector  */      
    QString            mySelectedFile;     /* selected filename   */    
    QAD_FileValidator* myValidator;        /* file validator      */
    static QString     myLastVisitedPath;  /* last visited path   */
    QComboBox*         myQuickCombo;       /* quick dir combo box */
};

#endif
