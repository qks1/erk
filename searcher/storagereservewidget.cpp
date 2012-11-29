#include "storagereservewidget.h"
#include "ui_storagereservewidget.h"

StorageReserveWidget::StorageReserveWidget(ReserveStruct rstruct,
                                           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StorageReserveWidget)
{
    ui->setupUi(this);
    this->source = rstruct.source_model;
    model = 0;

    set_columns_names();
    set_layout();
    fill_table();

    connects();
}

StorageReserveWidget::~StorageReserveWidget()
{
    delete ui;
}

void StorageReserveWidget::set_layout(){
    // сохраняем размер шрифта, чтобы на его основе ресайзить виджеты
    int fsize = qApp->font().pointSize();

    ui->plus_button->setFixedSize(fsize*4, fsize*10);
    ui->minus_button->setFixedSize(fsize*4, fsize*10);

    ui->clear_button->setFixedSize(fsize*12, fsize*4);
    ui->close_button->setFixedSize(ui->clear_button->size());
    ui->tobase_button->setFixedSize(ui->plus_button->height(), ui->plus_button->height());

    ui->main_frame->setFixedHeight(fsize * 18);
    ui->right_frame->setFixedWidth(fsize * 36);
    //ui->main_frame->setFixedWidth(qApp->desktop()->maximumWidth());

    ui->table_widget->verticalHeader()->setDefaultSectionSize(qApp->font().pointSize()*2);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->setFixedHeight(ui->main_frame->frameSize().height());
}

void StorageReserveWidget::connects(){
    QObject::connect(ui->plus_button, SIGNAL(clicked()), SLOT(add_position()));
    QObject::connect(ui->minus_button, SIGNAL(clicked()), SLOT(remove_position()));
    QObject::connect(ui->clear_button, SIGNAL(clicked()), SLOT(clear()));
    QObject::connect(ui->close_button, SIGNAL(clicked()), SIGNAL(switch_pipes()));
    QObject::connect(ui->tobase_button, SIGNAL(clicked()), SLOT(to_base()));
}

void StorageReserveWidget::set_blue_table(GreyTable *b){
    this->blue_table = b;
}

void StorageReserveWidget::fill_table(){
    QSqlQuery query(base);
    delete ui->table_widget->model();
    QString query_str = QString("SELECT g.id as id, "                               // 0
                                "g.trademark_id as trademark_id, "                  // 1
                                "g.place_id as place_id, "                          // 2
                                "g.inspection_id as inspection_id, "                // 3
                                "g.category_id as category_id, "                    // 4
                                "t.name as name, "                                  // 5
                                "g.quantity as quantity, "                          // 6
                                "g.year as year, "                                  // 7
                                "i.insp_name as inspection, "                       // 8
                                "p.storage as storage, "                            // 9
                                "p.rack as rack, "                                  // 10
                                "p.board as board, "                                // 11
                                "p.box as box, "                                    // 12
                                "g.pack as nu, "                                    // 13
                                "c.category_name as category, "                     // 14
                                "g.add_info as add_info, "                          // 15
                                "g.defect as defect, "                              // 16
                                "g.photo as photo "                                 // 17
                                "FROM greytable g "
                                "JOIN trademarks t ON g.trademark_id = t.id "
                                "JOIN places p ON g.place_id = p.id "
                                "JOIN categories c ON g.category_id = c.id "
                                "JOIN inspections i ON g.inspection_id = i.id "
                                "WHERE g.add_user_id = %1 AND g.status = 4")
            .arg(USER_ID);
    qDebug() << query_str;
    source->setQuery(query_str, base);
    if(source->lastError().isValid()){
        QMessageBox::information(this, "Не удалось сформировать таблицу", source->lastError().text(), QMessageBox::Ok);
        return;
    }
    model = new QSortFilterProxyModel(this);
    model->setSourceModel(source);
    set_headers();
    ui->table_widget->setModel(model);
    ui->table_widget->hideColumn(0);
    ui->table_widget->hideColumn(1);
    ui->table_widget->hideColumn(2);
    ui->table_widget->hideColumn(3);
    ui->table_widget->hideColumn(4);
    ui->table_widget->hideColumn(17);
}

void StorageReserveWidget::set_columns_names(){
    columns_names["id"] = "id";
    columns_names["trademark_id"] = "Белый id";
    columns_names["name"] = "Название";
    columns_names["quantity"] = "Количество";
    columns_names["year"] = "Год";
    columns_names["inspection"] = "Приёмка";
    columns_names["storage"] = "Склад";
    columns_names["rack"] = "Стеллаж";
    columns_names["board"] = "Полка";
    columns_names["box"] = "Ящик";
    columns_names["nu"] = "Норма упаковки";
    columns_names["category"] = "Категория";
    columns_names["add_info"] = "Доп.пар. 1";
    columns_names["defect"] = "Доп.пар. 2";
}

void StorageReserveWidget::set_headers(){
    QString hdr;
    for(int i = 0; i < model->columnCount(); i++){
        hdr = columns_names[model->headerData(i, Qt::Horizontal).toString()];
        if(hdr != QString())
            model->setHeaderData(i, Qt::Horizontal, hdr);
    }
}

void StorageReserveWidget::add_position(){
    if(GLOBAL_MODE != SEARCHER_BLUE_MODE)
        return;
    if(blue_table == 0)
        return;

    // меняем статус на 4 для каждой выделенной строки в синем экране
    //foreach(QModelIndex i, this->blue_table)
    QList<QVariant> ids = blue_table->table_multi_data(columns_grey_ids["id"]).toList();
    QSqlQuery query(base);
    QString query_str;
    foreach(QVariant i, ids){
        query_str = QString("UPDATE greytable SET status = 4 WHERE id = %1 AND status = 3").arg(i.toInt());
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Не удалось добавить запись", query.lastError().text(), QMessageBox::Ok);
            return;
        }
    }
    fill_table();
    emit need_blue_refresh();
}

void StorageReserveWidget::delpos(int id){
    // меняем статус на 3
    QSqlQuery query(base);
    QString query_str = QString("UPDATE greytable SET status =3 WHERE id = %1 AND status = 4").arg(id);
    if(!query.exec(query_str)){
        QMessageBox::information(this, "Не удалось вернуть запись", query.lastError().text(), QMessageBox::Ok);
        return;
    }
}

void StorageReserveWidget::remove_position(){
    int id = model->data(model->index(ui->table_widget->currentIndex().row(), 0)).toInt();
    delpos(id);
    fill_table();
    emit need_blue_refresh();
}

void StorageReserveWidget::clear(){
    int c = model->columnCount();
    int id;
    for(int i = 0; i < c; i++){
        id = model->data(model->index(i, 0)).toInt();
        delpos(id);
    }
    fill_table();
    emit need_blue_refresh();
}

void StorageReserveWidget::to_base(){
    /* для каждой записи:
        - определяем id
        - если есть деталь с такими параметрами и статус = 1, сложить кол-во и удалить запись
        - если нет - update set status = 1
        - запись в истории: в таблицу history_greytable_incoming
    */
    int c = model->rowCount();
    int id, grey_id, trademark_id, place_id, inspection_id, category_id, quantity;
    QString year, pack, add_info, defect, photo;
    QString query_str;
    QSqlQuery query(base);
    for(int i = 0; i < c; i++){
        id = model->data(model->index(i, 0)).toInt();
        trademark_id = model->data(model->index(i, 1)).toInt();
        place_id = model->data(model->index(i, 2)).toInt();
        inspection_id = model->data(model->index(i, 3)).toInt();
        category_id = model->data(model->index(i, 4)).toInt();
        quantity = model->data(model->index(i, 6)).toInt();
        year = model->data(model->index(i, 7)).toString();
        pack = model->data(model->index(i, 13)).toString();
        add_info = model->data(model->index(i, 15)).toString();
        defect = model->data(model->index(i, 16)).toString();
        photo = model->data(model->index(i, 17)).toString();
        grey_id = id;
        qDebug() << grey_id;

        // т.к. надо ещё и записать в историю, делать всё будем в транзакции
        while(!base.transaction());
        // подготавливаем запрос для проверки наличия такой же детали.
        query_str = QString("SELECT id, quantity FROM greytable WHERE trademark_id = %1 "
                            "AND place_id = %2 "
                            "AND year = '%3' "
                            "AND inspection_id = %4 "
                            "AND pack = '%5' "
                            "AND category_id = %6 "
                            "AND add_info = '%7' "
                            "AND defect = '%8' "
                            "AND status = %9")
                .arg(trademark_id)
                .arg(place_id)
                .arg(year)
                .arg(inspection_id)
                .arg(pack)
                .arg(category_id)
                .arg(add_info)
                .arg(defect)
                .arg(1);
        qDebug() << query_str;
        if(!query.exec(query_str)){
            QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()), QMessageBox::Ok);
            base.rollback();
            return;
        }
        // если есть!
        if(query.size() > 0){
            query.next();
            // добавляем кол-во к этой записи
            int tid = query.value(0).toInt(), tquant = query.value(1).toInt();
            grey_id = tid;
            query_str = QString("UPDATE greytable SET quantity = quantity + %1 WHERE id = %2")
                    .arg(quantity)
                    .arg(tid);
            //qDebug() << query_str;
            if(!query.exec(query_str)){
                QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()), QMessageBox::Ok);
                base.rollback();
                return;
            }
            // удаляем старую запись из синего экрана
            query_str = QString("DELETE FROM greytable WHERE id = %1").arg(id);
            if(!query.exec(query_str)){
                QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()), QMessageBox::Ok);
                base.rollback();
                return;
            }

            // выводим сообщение об успешном завершении операции
            qDebug() << QString("id %1: кол-во суммировано с id %2. id %1 удалён.").arg(id).arg(tid);
        }
        // а если нет, то тупо меняем статус
        else{
            query_str = QString("UPDATE greytable SET status = 1 WHERE id = %1").arg(id);
            qDebug() << query_str;
            if(!query.exec(query_str)){
                QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()), QMessageBox::Ok);
                base.rollback();
                return;
            }
        }

        // справились
        // теперь надо сделать запись в истории
        // все переменные из таблицы извлечены, user_id известен, datetime = now()
        query_str = QString("INSERT INTO history_greytable_incomings "
                            "(datetime, user_id, grey_id, quantity, place_id, year, inspection_id, pack, add_info, defect, category_id, photo) "
                            "VALUES (%1, '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', '%10', '%11', '%12' )")
                .arg("now()")
                .arg(USER_ID)
                .arg(grey_id)
                .arg(quantity)
                .arg(place_id)
                .arg(year)
                .arg(inspection_id)
                .arg(pack)
                .arg(add_info)
                .arg(defect)
                .arg(category_id)
                .arg(photo);
        qDebug() << query_str;
        if(!query.exec(query_str)){
            QMessageBox::warning(this, "Ошибка", QString("Не удалось записать в историю.\n%1").arg(query.lastError().text()), QMessageBox::Ok);
            base.rollback();
            return;
        }
        // уфф, вроде всё
        while(!base.commit());
    }
    fill_table();
    emit need_blue_refresh();
}
