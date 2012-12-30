#ifndef CONTRAGENTSLIST_H
#define CONTRAGENTSLIST_H

#include <QtGui>
#include <QtSql>
#include "common/constants.h"
#include "common/helpers.h"
#include "common/basetable.h"


class ContragentsList : public QWidget
{
    Q_OBJECT
public:
    explicit ContragentsList(bool returning, int type = 0, QWidget *parent = 0);
    ~ContragentsList();

private:
    bool returning;
    int type;

    // BUTTONS:
    QPushButton *escape_button;

    QHBoxLayout *top_lt;

    QRadioButton *all, *buyers, *shippers;
    ColumnsStruct columns;
    QSettings *settings;
    QString section;
    BaseTable *table;
    QSqlQueryModel *model;
    QRadioButton *name_button;
    QRadioButton *inn_button;
    QRadioButton *contact_button;
    QLineEdit *text_field;
    QStatusBar *top_bar, *bottom_bar;

    // строки состояния
    QLabel *bar1_header, *bar2_header, *bar1_text, *bar2_text;
    int left_width;

    QMap<QString, QString> columns_names;
    void set_columns_names();
    void set_headers();

    void set_layout();

    void closeEvent(QCloseEvent *);
    
signals:
    void delete_me();
    void selected(int, QString);
    
public slots:

private slots:
    void fill_table();
    void escape_slot();
    void set_statusbars(QModelIndex);
    void return_selected(QModelIndex);
    
};

#endif // CONTRAGENTSLIST_H
