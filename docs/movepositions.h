#ifndef MOVEPOSITIONS_H
#define MOVEPOSITIONS_H

#include <QtGui>
#include <QtSql>
#include "docslist.h"

class MovePositions : public QDialog{
    Q_OBJECT

public:
    explicit MovePositions(QStandardItemModel *list_model, QWidget *parent = 0);

private:
    // Элементы интерфейса:
    // - две радиокнопки ("заявки", "документы"
    // - кнопки ОК и Отмена
    // - QListView для заявок, два QTableView для документов и предложений
    // - QStackedLayout для их переключения
    // - два виджета - контейнера для докуметов+фильтры и предложений+фильтры
    // - фильтры для документов и комм. предложений
    QRadioButton *docs_button, *reserves_button;
    QPushButton *ok_button, *cancel_button;
    QStackedLayout *layout;
    QListView *reserves_list;
    QStandardItemModel *reserves_list_model;
    DocsList *docs_list;

    int max_docs;

    inline void create_widgets();
    inline void set_layout();
    inline void connects();
    inline void form_reserves_list();
    inline void form_docs_list();

signals:
    void doc_selected(int);
    void reserve_selected(int);

private slots:
    void switch_widget();
    void doc_selected_slot(int);
    void reserve_selected_slot(QModelIndex);
    void accept();
};


#endif // MOVEPOSITIONS_H
