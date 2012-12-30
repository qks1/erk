#ifndef DOCSLIST_H
#define DOCSLIST_H

#include <QtGui>
#include <QtSql>
#include "common/helpers.h"
#include "common/constants.h"
#include "common/basetableview.h"
#include "common/customcombobox.h"
#include "document.h"


class DocslistTableModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit DocslistTableModel(QMap<QString,int> ids, QWidget *parent = 0);

private:
    QMap<QString, int> ids;
    int iconsize;
    QVariant data(const QModelIndex &index, int role) const;
};

class DocsList : public QWidget
{
    Q_OBJECT
public:
    explicit DocsList(bool opendocs,
                      QStackedWidget *stack,
                      QStack<QWidget*> prev_wgts,
                      QStandardItemModel *list_model,
                      QWidget *parent = 0);
    ~DocsList();
    int current_id();

private:
    QStackedWidget *stack;
    QStack<QWidget*> prev_wgts;

    /* элементы формы:
        - надписи "С", "По", "Тип"
        - два DateEdit (с, по)
        - CheckBox "снять временной интервал"
        - PushButton открыть диалог выбора даты
        - ComboBox фильтр по типу
        - PushButton обновить
        - две RadioButton поиск по клиенту/номеру документа
        - LineEdit для поиска
        - ComboBox выбор режима доп. поиска
        - EditableComboBox доп. поиск
        - PushButton сброс фильтра
        - PushButton вернуться, удалить, печать, эксель, реестр
    */
    QLabel *from_label;
    QLabel *to_label;
    QLabel *type_label;
    QLabel *num_label;
    QDateEdit *from_date;
    QDateEdit *to_date;
    CustomComboBox *type_box;
    QCheckBox *remove_dates;
    QPushButton *refresh_button;
    QRadioButton *docnum;
    QRadioButton *client;
    QLineEdit *textline;
    QPushButton *reset_text;
    QWidget *controls;
    BaseTableView *table;
    DocslistTableModel *model;
    QSettings *settings;
    QComboBox *searchtype_box;
    CustomComboBox *search_box;
    Document *opened_doc;

    QHBoxLayout *top_lt;
    QPushButton *escape_button;
    QPushButton *delete_button;
    ColumnsStruct cstruct;
    QStandardItemModel *list_model;

    QString contragent_string, rnotes_string, author_string;
    bool opendocs;

    QMap<QString, QString> columns;
    //QMap<QString, QString> columns_sql;
    QStringList columns_sql;
    QMap<QString, int> columns_ids;
    QList<int> columns_default_widths;
    QString sql_from, sql_tables;
    QMap<QString, QString> filters;
    QString order;
    inline void set_layout();
    inline void resize();
    inline void connects();
    inline void set_columns_names();
    inline void form_sql_columns();
    void rename_columns();

    inline void custom_dates();
    void delete_last_symbol();
    void form_date_filter();
    int fill_table();

    
signals:
    void delete_me();
    void open_doc(int);
    void id_signal(int);
    void create_move_dialog(Document*);
    void set_reserve_doc_color(int);

public slots:
    void switch_hidden();
    void move_to_doc(int);
    void move_to_reserve(int);

private slots:
    void fill_combobox();
    void fill_search_box();
    void escape_slot();
    void delete_slot();
    void change_date(QDate);
    void form_text_filter();
    void form_number_filter();
    void form_type_filter(int);
    void reset_type_filter();
    void change_dateoff(bool);
    void change_searchtype(int);
    void change_textsearch(QString);
    void reset_text_slot();
    void open_doc_slot(QModelIndex);
    void emit_id_signal(QModelIndex);
    void close_current();
    void refresh();
    
};

#endif // DOCSLIST_H
