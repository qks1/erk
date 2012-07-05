#include "catalog.h"
#include "helpers.h"
#include "constants.h"
#include <QtSql>

//--------------------------------------------------------------------------//
//--------------------------- КОНСТРУКТОР-----------------------------------//
//--------------------------------------------------------------------------//

Catalog::Catalog(QWidget *parent) :
    QWidget(parent){

    // инициализируем переменные
    initialize();

    // В конструкторе класса создаём необходимые объекты
    groups = new QTreeWidget();
    header = groups->header();

    // кнопка сброса фильтра по группам должна лежать на заголовке каталога,
    // поэтому передаём указатель на него в качестве виджета-предка
    reset_groups = new QPushButton("X", header);

    // устанавливаем, чтобы у каталога всегда была видна полоса прокрутки
    // (нужно, чтобы корректно отображалась кнопка сброса фильтра)
    groups->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // устанавливаем заголовок каталога по умолчанию
    groups->setHeaderLabel(DEFAULT_HEADER);

    // заполняем каталог
    addGroups();

    // помещаем на место кнопку сброса фильтра
    move_button();

    // помещаем дерево в layout (для корректного отображения)
    QHBoxLayout* lt = new QHBoxLayout;
    lt->addWidget(groups);
    setLayout(lt);

    // соединяем сигналы со слотами
    connects();

}

//--------------------------------------------------------------------------//

inline void Catalog::initialize(){
    DEFAULT_HEADER = "Выберите группу";
}

//--------------------------------------------------------------------------//

inline void Catalog::connects(){
    // при выборе группы из каталога - установить фильтр по группам
    QObject::connect(this->groups, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
                     this, SLOT(change_group(QTreeWidgetItem*, int)));

    // при нажатии кнопки сброса фильтра по группам - сбросить его
    QObject::connect(this->reset_groups, SIGNAL(clicked()),
                     this, SLOT(clear_group()));

    // при выборе группы из каталога - установить заголовок каталога
    QObject::connect(this->groups, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
                     this, SLOT(change_header(QTreeWidgetItem*,int)));

    // при сбросе фильтра по группам - установить заголовок каталога по умолчанию
    QObject::connect(this->reset_groups, SIGNAL(clicked()),
                     this, SLOT(clear_header()));
}

//--------------------------------------------------------------------------//
//--------------------------- ФУНКЦИИ --------------------------------------//
//--------------------------------------------------------------------------//

void Catalog::addGroups(){
    // выбираем список групп из БД
    QSqlQuery query;
    QString strSelect = "SELECT id, name FROM " + GROUPS_TABLE + " ORDER BY name";

    // если не удаётся выполнить запрос, выдаём ошибку
    if(!query.exec(strSelect)){
        error("Ошибка", QString("Не удалось получить список групп:\n").append(strSelect));
        return;
    }

    QString groupName;                              // название группы - для отображения в каталоге
    int groupNum;                                   // номер группы - для использования в фильтре
    QTreeWidgetItem* groupItem = 0;                 // элемент дерева

    // начинаем разбирать результат запроса
    QSqlRecord rec = query.record();
    while(query.next()){
        groupName = query.value(rec.indexOf("name")).toString();
        groupNum = query.value(rec.indexOf("id")).toInt();

        // формируем элемент дерева
        groupItem = new QTreeWidgetItem(this->groups);
        groupItem->setText(0, groupName);                       // устанавливаем groupName в качестве текста
        QStringList data;                                       // для передачи номера в фильтр прикрепляем данные к элементу
        data << QString::number(groupNum+ENLARGER) << groupName;  // чтобы отличить группу от подгруппы в фильтре, искусственно увеличим хранящийся номер
        groupItem->setData(0, Qt::UserRole, data);

        addSubgroup(groupItem, groupNum);                       // для каждой группы добавляем список подгрупп
    }
}

//--------------------------------------------------------------------------//

void Catalog::addSubgroup(QTreeWidgetItem *groupItem, int groupNum){
    // выбираем из БД список подгрупп для группы groupNum
    QSqlQuery query;
    QString strSelect = "SELECT id, name FROM " + SUBGROUPS_TABLE + " WHERE group_id=" + QString::number(groupNum);

    // если не удаётся выполнить запрос, выдаём ошибку
    if(!query.exec(strSelect)){
        error("Ошибка", QString("Не удалось получить список подгрупп:\n").append(strSelect));
        return;
    }

    // формируем элемент поддерева
    QString subgroupName;               // отображаемый текст
    int subgroupNum;                    // номер подгруппы для передачи в фильтр
    QTreeWidgetItem *subgroupItem = 0;  // элемент поддерева

    // начинаем разбирать результат запроса
    QSqlRecord rec = query.record();
    while(query.next()){
        subgroupName = query.value(rec.indexOf("name")).toString();
        subgroupNum = query.value(rec.indexOf("id")).toInt();

        // для элемента поддерева устанавливаем в качестве предка соответствующий элемент дерева
        subgroupItem = new QTreeWidgetItem(groupItem);

        // устанавливаем текст и данные для элемента поддерева
        subgroupItem->setText(0, subgroupName);
        QStringList data;
        data << QString::number(subgroupNum) << subgroupName;
        subgroupItem->setData(0, Qt::UserRole, data);
    }
}

//--------------------------------------------------------------------------//
//--------------------------- СЛОТЫ ----------------------------------------//
//--------------------------------------------------------------------------//

void Catalog::move_button(){
    // помещаем кнопку сброса фильтра в правый угол заголовка
    // ширина и высота кнопки должны быть равны высоте заголовка
    int size = header->height();

    // получим ширину вертикального скроллбара
    int scrollbar_width = groups->verticalScrollBar()->width();

    // отстоять от левого края кнопка должна на (ширина_каталога-ширина_скроллбара-ширина_кнопки-небольшой_отступ)
    int wdth = groups->width() - scrollbar_width - size - 5;

    // наконец, устанавливаем геометрию кнопки
    reset_groups->setGeometry(wdth, 0, size, size);
}

//--------------------------------------------------------------------------//

void Catalog::change_group(QTreeWidgetItem *qtwi, int column){
    // слот, выполняющийся при выборе элемента из каталога
    // получаем номер группы из данных, хранящихся в этом элементе
    int group = qtwi->data(column, Qt::UserRole).toStringList().at(0).toInt();
    // испускаем сигнал, который принимает Searcher
    emit group_changed(group);
}

void Catalog::clear_group(){
    // испускаем сигнал, который принимает Searcher
    emit group_changed(0);
}

void Catalog::change_header(QTreeWidgetItem *qtwi, int column){
    // получаем название группы из данных, хранящихся в выбранном элементе
    QString group_name = qtwi->data(column, Qt::UserRole).toStringList().at(1);
    // устанавливаем его в качестве заголовка
    this->groups->setHeaderLabel(group_name);
}

void Catalog::clear_header(){
    // устанавливаем заголовок по умолчанию
    this->groups->setHeaderLabel(DEFAULT_HEADER);
}


//--------------------------------------------------------------------------//
//--------------------------- СОБЫТИЯ --------------------------------------//
//--------------------------------------------------------------------------//

void Catalog::resizeEvent(QResizeEvent *){
    // при изменении размеров каталога (будь то изменение размеров окна
    // или перемещение разделителя) необходимо корректировать положение кнопки сброса
    move_button();
}

//--------------------------------------------------------------------------//
