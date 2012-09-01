#include "detailcard.h"

DetailCard::DetailCard(int id, QWidget *parent) :
    QWidget(parent)
{
    this->id = id;
    this->parent = parent;
    //exec();
}

int DetailCard::exec(){
    CardDialog *info = new CardDialog(parent);

    QLabel *info_label = new QLabel();
    QLabel *quantity_label = new QLabel();
    settings = new QSettings("erk", "base");

    // выбираем название, группу, подгруппу, общее кол-во
    QString tm_query = QString("SELECT t.name as trademark, t.quantity, t.photo, s.name as subgroup, g.name as group") +
            " FROM trademarks t " +
            " JOIN subgroups s ON (t.subgroup_id = s.id) " +
            " JOIN groups g ON (g.id=s.group_id) " +
            " WHERE t.id = " + QString::number(id);
    QSqlQuery *sqlquery = new QSqlQuery(tm_query, base);
    if(sqlquery->lastError().isValid()){
        error("Ошибка", sqlquery->lastError().text());
    }
    QSqlRecord rec = sqlquery->record();
    sqlquery->next();
    QString trademark = sqlquery->value(rec.indexOf("trademark")).toString();
    QString subgroup = sqlquery->value(rec.indexOf("subgroup")).toString();
    QString group = sqlquery->value(rec.indexOf("group")).toString();
    QString photo = sqlquery->value(rec.indexOf("photo")).toString();
    int quantity = sqlquery->value(rec.indexOf("quantity")).toInt();

    info_label->setText(QString("<h2>%1</h2><br /><h3>%2, %3</h3><br />").arg(trademark).arg(group).arg(subgroup));
    quantity_label->setText(QString("<h4>Общее кол-во на складе: %1").arg(quantity));

    // картинкоъ

    QPixmap picture;
    QString address;
    QString path = settings->value(QString("%1PHOTOS_PATH").arg(USERNAME), PHOTOS_PATH).toString();
    if(path.at(path.size()-1) != QDir::separator())
        path += QDir::separator();
    if(photo.toInt() > 0){
        address = QString("%1%2.jpg").arg(path).arg(photo);
        if(!picture.load(address))
            error("Ошибка при загрузке картинки", QString("Не удалось загрузить картинку по адресу %1.").arg(address));
    }
    else{
        address = path + "00000.jpg";
        if(!picture.load(address))
            error("Ошибка при загрузке картинки", QString("Не удалось загрузить картинку по адресу %1.").arg(address));
    }

    QLabel *picture_label = new QLabel();
    picture_label->setPixmap(picture.scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation ));




    // формирование таблицы

    QString query = QString("SELECT g.id, g.quantity, g.year, p.storage, p.rack, p.board, p.box, i.insp_name, g.add_info, g.defect, c.category_name ") +
                        " FROM greytable g " +
                        " JOIN trademarks t ON (t.id=g.trademark_id) " +
                        " JOIN places p ON (p.id=g.place_id) " +
                        " JOIN categories c ON (g.category_id=c.id) " +
                        " JOIN inspections i ON (g.inspection_id = i.id) " +
                        " WHERE t.id = " + QString::number(id);
    MyTableModel *model = new MyTableModel();
    model->setQuery(query, base);
    if(model->lastError().isValid()){
        error("Ошибка", model->lastError().text());
    }

    column_names.clear();
    for(int i = 0; i < model->columnCount(); i++)
       column_names << model->headerData(i, Qt::Horizontal).toString();

    // переименование столбцов
    for(int i = 0; i < model->columnCount(); i++){
        model->setHeaderData(i, Qt::Horizontal, GREY_COLUMNS_NAMES[model->headerData(i, Qt::Horizontal).toString()]);
    }

    table = new QTableView();
    table->setModel(model);
    table->verticalHeader()->hide();
    table->horizontalHeader()->setMovable(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->verticalHeader()->setDefaultSectionSize(table->verticalHeader()->fontMetrics().height() + 2);
    table->horizontalHeader()->setHighlightSections(false);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    table->setContextMenuPolicy(Qt::CustomContextMenu);

    // переименование столбцов
    //for(int i = 0; i < model->columnCount(); i++)
        //model->setHeaderData(i, Qt::Horizontal, GREY_COLUMNS_NAMES[model->headerData(i, Qt::Horizontal).toString()]);


    table->resizeColumnsToContents();

    // расположение элементов

    QVBoxLayout *vlt = new QVBoxLayout();
    vlt->addWidget(info_label);
    vlt->addWidget(quantity_label);

    QHBoxLayout *hlt = new QHBoxLayout();
    hlt->addLayout(vlt);
    hlt->addStretch(1);
    hlt->addWidget(picture_label);

    QVBoxLayout *main_lt = new QVBoxLayout();
    main_lt->addLayout(hlt);
    main_lt->addWidget(table);

    info->setLayout(main_lt);
    info->resize(600,500);
    info->setWindowTitle("Карточка детали");

    QObject::connect(this->table->horizontalHeader(), SIGNAL(sectionResized(int,int,int)), SLOT(column_width_changed(int,int,int)));
    QObject::connect(this->table->horizontalHeader(), SIGNAL(sectionMoved(int,int,int)), SLOT(column_moved(int,int,int)));

    restore_state();

    return info->exec();
}

void DetailCard::save_state(){
    // сохраняем состояние (столбцы и их ширина)
    for(int i = 0; i < table->model()->columnCount(); i++){
        settings->beginGroup(QString("%1/CARD_COLUMNS").arg(USERNAME));
        // записываем в настройки ширину столбца, называя ключ оригинальным именем этого столбца
        // оригинальное имя хранится в column_names в столбце по номеру логического индекса в модели,
        // который не меняется никогда, в отличие от визуального
        settings->setValue(column_names[table->horizontalHeader()->logicalIndex(i)], table->columnWidth(table->horizontalHeader()->logicalIndex(i)));
        // записываем в настройки соответствие визуального индекса и логического,
        // чтобы потом восстановить порядок.
        // Визуальный индекс тождественен счётчику i, логический же зашит в модели.
        settings->setValue(QString(column_names[table->horizontalHeader()->logicalIndex(i)]).append("_index"), i);
        settings->endGroup();
    }
}

void DetailCard::restore_state(){
    QString name;
    int index;
    for(int i = 0; i < table->model()->columnCount(); i++){
        settings->beginGroup(QString("%1/CARD_COLUMNS").arg(USERNAME));
        // Восстанавливаем ширину столбца и его визуальный индекс.
        // Ширина хранится в настройках по ключу, названному оригинальным именем столбца,
        // поэтому надо восстановить оригинальное имя.
        name = column_names[i];
        table->setColumnWidth(i, settings->value(name, DEFAULT_COLUMN_WIDTH).toInt());
        // Визуальный индекс хранится по ключу, названному оригинальным именем столбца
        // с суффиксом "_index". Восстанавливаем индекс из настроек и двигаем столбец.
        index = settings->value(QString(name).append("_index"), i).toInt();
        table->horizontalHeader()->moveSection(table->horizontalHeader()->visualIndex(i), index);
        settings->endGroup();
    }
}

void DetailCard::column_width_changed(int,int,int){
    save_state();
}

void DetailCard::column_moved(int, int, int){
    save_state();
}

CardDialog::CardDialog(QWidget *parent){
    settings = new QSettings("erk", "base");
    restoreGeometry(settings->value(QString("%1/CARD/geometry").arg(USERNAME)).toByteArray());
}

void CardDialog::closeEvent(QCloseEvent *event){
    settings->setValue(QString("%1/CARD/geometry").arg(USERNAME), saveGeometry());
    QDialog::closeEvent(event);
}

