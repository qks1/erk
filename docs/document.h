#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QWidget>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include "common/constants.h"
#include "common/helpers.h"
#include "common/basetableview.h"
#include "documentform.h"
#include "lists/contragentslist.h"
#include "docs/correctdocrecord.h"


// МОДЕЛЬ ТАБЛИЦЫ
class DocTableModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit DocTableModel(QMap<QString,int> ids, QWidget *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    double percents;

private:
    QMap<QString, int> ids;

signals:
    void clicked();
};


class Document : public QWidget
{
    Q_OBJECT
public:
    explicit Document(QStackedWidget *stack,
                      QStack<QWidget*> prev_wgts,
                      int id,
                      QStandardItemModel *list_model,
                      QWidget *parent = 0);
    ~Document();

private:
    QStackedWidget *stack;
    QStack<QWidget*> prev_wgts;
    // constructor
    QString doctype;
    int doctype_id;
    // buttons
    QHBoxLayout *buttons_lt;
    QWidget *headers_lt;
    QPushButton *escape_button;
    QPushButton *subtract_quantity_button;
    QPushButton *correct_record_button;
    // widgets
    QTabWidget *wgt;
    DocumentForm *form;
    BaseTableView *table;
    DocTableModel *model;
    // headers
    QLabel *docnum_label, *date_label;
    QLineEdit *docnum_edit;
    QDateEdit *date_edit;
    QCheckBox *nopodbor_checkbox;
    // right part of table tab
    QPushButton *right_move_button;
    QPushButton *right_naklad_button;
    QPushButton *right_sf_button;
    QStandardItemModel *list_model;
    int temp_quantity;
    bool select_all;
    int last_selected_row;
    // bottom
    QWidget *bottom_wgt;
    QPushButton *write_bottom;
    QPushButton *sf_bottom;
    QPushButton *naklad_bottom;
    QPushButton *spis_bottom;
    // лейблы снизу: сумма без скидки, сумма со скидкой, сумма скидки
    QLabel *wodisc_hlabel, *wdisc_hlabel, *disc_hlabel;
    QLabel *wodisc_label, *wdisc_label, *disc_label;
    QCheckBox *round_box;
    QPushButton *chdisc_button;

    double sum_without_discount, disc_percs, discount_sum;
    QSettings *settings;
    QString settings_section;
    ColumnsStruct cstruct;      // для табличной части

    QMap<QString, QString> columns;
    QStringList columns_sql;
    QMap<QString, int> columns_ids;
    QList<int> columns_default_widths;
    QString sql_from, sql_tables;
    int doc_id;
    bool all_discounts, all_selected;
    bool something_changed;
    int old_doc_num;
    int podbor;
    QDate old_date;
    QList<bool> selected_items;

    inline void set_layout();
    inline void resize();
    inline void connects();
    inline void set_columns_names();
    inline void form_sql_columns();
    void rename_columns();
    void renew_bottom_labels();

    int fill_table();
    bool write_changes();
    int count_selected();

    
signals:
    void delete_me();
    void fail_to_write_discounts();
    void need_refresh();
    void renew_prices(double, double);
    void create_move_dialog(Document*);
    void set_reserve_doc_color(int);
    
public slots:
    void move_to_doc(int);
    void move_to_reserve(int);

private slots:
    void escape_slot();
    bool write_slot();
    void header_clicked(int);
    void model_clicked();
    void open_discount_editor();
    void recount_discount(double);
    void set_bottom_labels(double, double, double);
    void round_sum(bool);
    void switch_podbor(bool);
    void something_changed_slot();
    void move_positions();
    void naklad_positions();
    void sf_positions();
    void change_temp_quantity(int);
    void tab_changed_slot(int);
    void subtract_quantity_slot();
    void correct_record_slot();
    void selection_changed(QModelIndex);
};




class EditOrderDiscount : public QDialog
{
    Q_OBJECT
public:
    explicit EditOrderDiscount(double cur_disc_perc, double cur_disc_sum, double order_sum, QWidget *parent = 0);

private:
    QLineEdit *percents_edit;
    QPushButton *count;
    QPushButton *ok_button, *cancel_button;
    QLabel *percs_label;
    QLabel *fact_name_label, *value_name_label;
    QLabel *fact_label, *value_label;

    double current_percents, current_discount_sum, order_sum;
    inline void set_layout();
    void resize();

signals:
    void recount(double);

public slots:
    void renew_labels(double, double);

private slots:
    void count_slot();
    void ok_clicked();
    void cancel_clicked();
};



#endif // DOCUMENT_H
