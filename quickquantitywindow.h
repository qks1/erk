#ifndef QUICKQUANTITYWINDOW_H
#define QUICKQUANTITYWINDOW_H

#include <QDialog>
#include <QCheckBox>
#include <QtSql>
#include <QString>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "constants.h"
#include "helpers.h"

class QuickQuantityWindow : public QDialog
{
    Q_OBJECT
public:
    explicit QuickQuantityWindow(int trademark_id, QWidget *parent = 0);

private:
    int trademark_id;
    QCheckBox *category;
    QTableWidget *table;
    QLabel *label;

    int count_quantity(int category_mode);
    void set_header_labels();
    void set_label(int);
    void resize_all(bool);
    
signals:
    
public slots:

private slots:
    void fill_table(bool);
    
};

#endif // QUICKQUANTITYWINDOW_H
