#include "catalog.h"

//--------------------------------------------------------------------------//
//--------------------------- КОНСТРУКТОР-----------------------------------//
//--------------------------------------------------------------------------//

Catalog::Catalog(QWidget *parent) :
    QWidget(parent){

    // В конструкторе класса создаём объекты
    groups = new QTreeWidget();
    header = groups->header();
    default_header = "Выберите группу";

    // кнопка сброса фильтра по группам должна лежать на заголовке каталога,
    // поэтому передаём указатель на него в качестве виджета-предка
    reset_groups = new QPushButton("X", header);

    // устанавливаем, чтобы у каталога всегда была видна полоса прокрутки
    // (нужно, чтобы корректно отображалась кнопка сброса фильтра)
    groups->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    // устанавливаем заголовок каталога по умолчанию
    groups->setHeaderLabel(default_header);

    // заполняем каталог
    // если не удалось (т.е. какие-то проблемы с БД), устанавливаем ok=false,
    // чтобы другие виджеты могли увидеть, что с БД проблемы
    ok = (addGroups() ? true : false);

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

inline void Catalog::connects(){
    // при выборе группы из каталога - установить фильтр по группам
    QObject::connect(this->groups, SIGNAL(itemActivated(QTreeWidgetItem*, int)),
                     this, SLOT(change_group(QTreeWidgetItem*, int)));

    // при нажатии кнопки сброса фильтра по группам - сбросить его
    //QObject::connect(this->reset_groups, SIGNAL(clicked()), SLOT(clear_group()));
    QObject::connect(this->reset_groups, SIGNAL(clicked()), SIGNAL(hide_catalog()));

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

bool Catalog::addGroups(){
    return add_groups(this->groups);
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

void Catalog::resize_all(){
    move_button();
}

//--------------------------------------------------------------------------//

void Catalog::change_group(QTreeWidgetItem *qtwi, int column){
    // слот, выполняющийся при выборе элемента из каталога
    // получаем номер группы из данных, хранящихся в этом элементе
    QStringList data = qtwi->data(column, Qt::UserRole).toStringList();
    // испускаем сигнал, который принимает Searcher
    emit group_changed(data.at(0).toInt(), data.at(1));
}

void Catalog::clear_group(){
    // испускаем сигнал, который принимает Searcher
    emit group_changed(0, "");
}

void Catalog::change_header(QTreeWidgetItem *qtwi, int column){
    // получаем название группы из данных, хранящихся в выбранном элементе
    QString group_name = qtwi->data(column, Qt::UserRole).toStringList().at(1);
    // устанавливаем его в качестве заголовка
    this->groups->setHeaderLabel(group_name);
}

void Catalog::clear_header(){
    // устанавливаем заголовок по умолчанию
    this->groups->setHeaderLabel(default_header);
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

void Catalog::showEvent(QShowEvent *){
    // при отрисовке каталога на экране корректировать положение кнопки сброса
    move_button();
}

//--------------------------------------------------------------------------//
