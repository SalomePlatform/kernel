/********************************************************************
**  Class:   QAD_MessageBox
**  Descr:   Message dialog box for QAD-based application
**  Module:  QAD
**  Created: UI team, 02.10.00
*********************************************************************/
#ifndef QAD_MESSAGEBOX_H
#define QAD_MESSAGEBOX_H

#include <qstring.h>
#include <qwidget.h>

#include "QAD.h"

class QAD_EXPORT QAD_MessageBox
{
public:
    /* show a box with one button */
    static int info1 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0 );
    static int warn1 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0 );
    static int error1 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0 );

    /* show a box with two buttons */
    static int info2 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       int idButton0, int idButton1, int idDefault );
    static int warn2 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       int idButton0, int idButton1, int idDefault );
    static int error2 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       int idButton0, int idButton1, int idDefault );

    /* show a box with three buttons */
    static int info3 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       const QString& textButton2, int idButton0, int idButton1,
                       int idButton2, int idDefault );
    static int warn3 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       const QString& textButton2, int idButton0, int idButton1,
                       int idButton2, int idDefault );
    static int error3 ( QWidget* parent, const QString& caption, const QString& text,
                       const QString& textButton0, const QString& textButton1,
                       const QString& textButton2, int idButton0, int idButton1,
                       int idButton2, int idDefault );
};

#endif

