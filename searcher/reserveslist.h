#ifndef RESERVESLIST_H
#define RESERVESLIST_H

#include <QtGui>
#include <QtSql>
#include "common/constants.h"
#include "common/helpers.h"
#include "common/basetableview.h"

class ReservesList : public QDialog
{
    Q_OBJECT
public:
    explicit ReservesList(int id, QString name, QWidget *parent = 0);
    ~ReservesList();

private:
    QPushButton *escape_button;
    QHBoxLayout *buttons_lt;
    BaseTableView *docs_list;
    QListWidget *reserves_list;
    QStatusBar *bar;
    ColumnsStruct columns;
    QSettings *settings;
    
signals:
    
public slots:
    
};

#endif // RESERVESLIST_H
