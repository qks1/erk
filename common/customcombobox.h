#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QtGui>
#include <QMouseEvent>
#include "constants.h"
#include "qglobal.h"
#include "helpers.h"

class CustomComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CustomComboBox(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);
    void closeEvent(QCloseEvent *);
    void showPopup();

    
signals:
    void fill_box_signal();
    
public slots:
    
};

#endif // CUSTOMCOMBOBOX_H
