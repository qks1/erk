#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QComboBox>
#include <QWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include "constants.h"

class CustomComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CustomComboBox(QWidget *parent = 0);

private:
    void mousePressEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);
    void closeEvent(QCloseEvent *);

    
signals:
    void fill_boxes_signal();
    
public slots:
    
};

#endif // CUSTOMCOMBOBOX_H
