#ifndef MANAGERRESERVEWIDGET_H
#define MANAGERRESERVEWIDGET_H

#include <QtGui>
#include <QtSql>
#include "whitetable.h"
#include "greytable.h"
#include "constants.h"
#include "helpers.h"

namespace Ui {
class ManagerReserveWidget;
}

class ManagerReserveWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ManagerReserveWidget(ReserveStruct rstruct,
                                  QWidget *parent = 0);
    ~ManagerReserveWidget();
    void set_grey_table(GreyTable *g);
    void set_reserve_header();
    int resize_section(int, int);

private:
    Ui::ManagerReserveWidget *ui;
    GreyTable *grey_table;
    QSqlQueryModel *source;
    QStandardItemModel *list_model;
    QItemSelectionModel *selection_model;
    QHeaderView *table_header;
    QSortFilterProxyModel *model;
    QMap<QString, QString> columns_names;
    void set_layout();
    void set_columns_names();
    void set_headers();
    void connects();

    int max_docs;
    QSortFilterProxyModel *oldmodel;
    void delpos(int);

    void fill_list();

signals:
    void need_refresh();
    void switch_reserves();

private slots:
    void change_doc(QModelIndex cur, QModelIndex prev = QModelIndex());
    void add_position();
    void subtr_quant();
    void delete_position();
    void view_doc();
    void restore_doc();
    void clear_doc();
    void move_doc();
    void refresh_table();
    double count_sum();
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// ДОБАВИТЬ ПОЗИЦИЮ ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class ReserveAddPosition : public QDialog{
    Q_OBJECT

public:
    explicit ReserveAddPosition(int id, int num, int quantity, double retail_price, double whole_price, int whole_begin, QWidget *parent = 0);

private:
    // интерфейс
    QPushButton *accept_button;
    QPushButton *reject_button;
    QLineEdit *quantity_spin;
    QDoubleSpinBox *price;
    QLineEdit *note;
    //QLineEdit *base_price;
    QLineEdit *quantity_total;

    // элементы конструктора
    int grey_id, doc_num;
    int base_quantity;
    double base_retail_price, base_whole_price;
    int base_whole_begin;

    bool price_changed;
    bool editing;

    void set_layout();
    void set_base_price_text(double);


private slots:
    void price_entered();
    void quantity_changed(int);
    void accept();

};


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// ВЫЧЕСТЬ КОЛИЧЕСТВО ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class ReserveSubtract : public QDialog{
    Q_OBJECT

public:
    explicit ReserveSubtract(int id, int quantity, QWidget *parent = 0);

private:
    QSpinBox *subtr_spin;
    QLineEdit *quant_line;
    QPushButton *accept_button;
    QPushButton *reject_button;

    int c_id, c_num, c_quantity;

private slots:
    void change_quant(int);
    void accept();

};


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// ПЕ� ЕМЕСТИТЬ ПОЗИЦИИ //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class ReserveMovePositions : public QDialog{
    Q_OBJECT

public:
    explicit ReserveMovePositions(QTableView *table, QModelIndexList indexes, QWidget *parent = 0);

private:
    // Элементы интерфейса:
    // - три радиокнопки ("заявки", "документы", "комм. предл."
    // - кнопки ОК и Отмена
    // - QListView для заявок, два QTableView для документов и предложений
    // - QStackedLayout для их переключения
    // - два виджета - контейнера для докуметов+фильтры и предложений+фильтры
    // - фильтры для документов и комм. предложений
    QRadioButton *reserves_button, *docs_button, *offers_button;
    QPushButton *ok_button, *cancel_button;
    QStackedLayout *layout;
    QWidget *docs_widget, *offers_widget;
    QListView *reserves_list;
    QTableView *docs_list, *offers_list;
    // виджеты фильтров добавим позднее

    int max_docs;

    inline void create_widgets();
    inline void set_layout();
    inline void connects();
    inline void form_reserves_list();
    inline void form_docs_list();
    inline void form_offers_list();

private slots:
    void switch_widget();
};


#endif // MANAGERRESERVEWIDGET_H
