#ifndef CATALOG_H
#define CATALOG_H

#include <QtGui>
#include <QtSql>
#include "helpers.h"
#include "constants.h"

/*
  Класс каталога групп и подгрупп.
  Состоит из дерева и кнопки сброса.
  Каждый элемент дерева - товарная группа, содаржащая одну или несколько подгрупп.
*/

class Catalog : public QWidget
{
    Q_OBJECT
public:
    explicit Catalog(QWidget *parent = 0);
    bool ok;                                    // чтобы другие виджеты могли проверить, всё ли нормально
    void resize_all();

private:
    QTreeWidget *groups;                        // древовидный каталог групп
    QHeaderView *header;                        // заголовок каталога
    QPushButton *reset_groups;                  // кнопка сброса фильтра по группам
    QString default_header;                     // заголовок каталога по умолчанию

    // функции добавления групп и подгрупп в каталог
    bool addGroups();
    //bool addSubgroup(QTreeWidgetItem *groupItem, int groupNum);

    inline void connects();                     // функция соединения сигналов и слотов

protected:
    virtual void resizeEvent(QResizeEvent *);           // реакция на изменение размеров каталога
    virtual void showEvent(QShowEvent *);               // реакция на появление каталога на экране
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    
signals:
    void group_changed(int, QString);           // испускается при изменении выбранной группы
    void hide_catalog();                        // испускается при нажатии на кнопку "скрыть каталог"
    void show_catalog();                        // испускается при нажатии на кнопку "показать каталог"

private slots:
    void change_group(QTreeWidgetItem*, int);   // передать в поисковик номер группы
    void clear_group();                         // передать сигнал сброса группы
    void change_header(QTreeWidgetItem*, int);  // изменить заголовок каталога
    void clear_header();                        // установить заголовок по умолчанию
    
public slots:
    void move_button();   // перемещение кнопки сброса на место

};

#endif // CATALOG_H
