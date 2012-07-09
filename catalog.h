#ifndef CATALOG_H
#define CATALOG_H

#include <QtGui>

/*
  Класс каталога групп и подгрупп.
  Состоит из дерева и кнопки сброса.
*/

class Catalog : public QWidget
{
    Q_OBJECT
public:
    explicit Catalog(QWidget *parent = 0);

private:
    QTreeWidget *groups;                        // древовидный каталог групп
    QHeaderView *header;                        // заголовок каталога
    QPushButton *reset_groups;                  // кнопка сброса фильтра по группам
    QString default_header;                    // заголовок каталога по умолчанию

    // функции добавления групп и подгрупп в каталог
    void addGroups();
    void addSubgroup(QTreeWidgetItem *groupItem, int groupNum);

    inline void connects();                     // функция соединения сигналов и слотов

    void resizeEvent(QResizeEvent *);           // реакция на изменение размеров окна
    
signals:
    void group_changed(int);                    // испускается при изменении выбранной группы

private slots:
    void change_group(QTreeWidgetItem*, int);   // передать в поисковик номер группы
    void clear_group();                         // передать сигнал сброса группы
    void change_header(QTreeWidgetItem*, int);  // изменить заголовок каталога
    void clear_header();                        // установить заголовок по умолчанию
    
public slots:
    void move_button();   // перемещение кнопки сброса на место

};

#endif // CATALOG_H
