#ifndef STORAGERESERVEWIDGET_H
#define STORAGERESERVEWIDGET_H

#include <QtGui>
#include <QtSql>
#include "greytable.h"
#include "common/constants.h"
#include "common/helpers.h"
#include "common/basetableview.h"

namespace Ui {
class StorageReserveWidget;
}

class StorageReserveWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit StorageReserveWidget(ReserveStruct *rstruct, QWidget *parent = 0);
    ~StorageReserveWidget();
    void set_blue_table(GreyTable *b);
    void delpos(int);
    
private:
    Ui::StorageReserveWidget *ui;
    void set_layout();
    void connects();
    void set_columns_names();
    void set_headers();

    QMap<QString, QString> columns_names;
    QSqlQueryModel *source;
    QSortFilterProxyModel *model;
    GreyTable *blue_table;

private slots:
    void fill_table();
    void add_position();
    void remove_position();
    void clear();
    void to_base();

signals:
    void need_blue_refresh();
    void switch_pipes();
};

#endif // STORAGERESERVEWIDGET_H
