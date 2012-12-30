#include "managerreservewidget.h"
#include "ui_managerreservewidget.h"

ManagerReserveWidget::ManagerReserveWidget(ReserveStruct *rstruct,
                                           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManagerReserveWidget)
{
    QList<int> default_widths;
    int fs = qApp->font().pointSize();
    default_widths << fs*3      // select
                   << fs*5      // id
                   << fs*8      // grey_id
                   << fs*24     // name
                   << fs*8      // quantity
                   << fs*8      // price
                   << fs*8      // sumprice
                   << fs*8      // year
                   << fs*6      // storage
                   << fs*6      // rack
                   << fs*6      // board
                   << fs*6      // box
                   << fs*24     // note
                   << fs*6;     //weight
    ui->setupUi(this);
    ui->table_widget->set_variables("MANAGER_RESERVE", false, &(rstruct->columns), default_widths);

    QModelIndex cur = rstruct->selection_model->currentIndex();
    max_docs = 10;
    oldmodel = 0;
    grey_table = 0;
    contragent_id = 0;
    this->source = rstruct->source_model;
    this->list_model = rstruct->list_model;
    this->selection_model = rstruct->selection_model;
    ui->docs_list->setSelectionMode(QListView::SingleSelection);
    model = new ReserveProxyModel(this);
    all_selected = false;
    set_columns_names();
    set_layout();

    fill_list();
    ui->table_widget->restore_columns_width();
    ui->table_widget->restore_columns_order();
    if(cur.isValid())
        this->ui->docs_list->setCurrentIndex(cur);

    connects();
}

ManagerReserveWidget::~ManagerReserveWidget()
{
    delete ui;
}

void ManagerReserveWidget::connects(){
    connect(ui->docs_list, SIGNAL(clicked(QModelIndex)), SLOT(change_doc(QModelIndex)));
    connect(ui->docs_list->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), SLOT(change_doc(QModelIndex, QModelIndex)));
    connect(ui->plus_button, SIGNAL(clicked()), SLOT(add_position()));
    connect(ui->minus_button, SIGNAL(clicked()), SLOT(subtr_quant()));
    connect(ui->delete_button, SIGNAL(clicked()), SLOT(delete_position()));
    //connect(ui->show_button, SIGNAL(clicked()), SLOT(view_doc()));
    connect(ui->show_button, SIGNAL(pressed()), SLOT(view_doc()));
    connect(ui->show_button, SIGNAL(released()), SLOT(restore_doc()));
    connect(ui->clear_button, SIGNAL(clicked()), SLOT(clear_doc()));
    connect(ui->close_button, SIGNAL(clicked()), SIGNAL(switch_reserves()));
    connect(ui->move_button, SIGNAL(clicked()), SLOT(move_doc()));
    connect(ui->tobase_button, SIGNAL(clicked()), SLOT(tobase()));
    connect(ui->note_edit, SIGNAL(double_clicked()), SLOT(open_contragents_slot()));
    connect(ui->note_edit, SIGNAL(textChanged(QString)), SLOT(text_changed_slot()));
    connect(ui->table_widget->horizontalHeader(), SIGNAL(sectionClicked(int)), SLOT(header_clicked(int)));
    QObject::connect(ui->table_widget, SIGNAL(section_resized(int, int, QString)), SIGNAL(section_resized(int, int, QString)));
    QObject::connect(ui->table_widget, SIGNAL(section_moved(int, int, QString)), SIGNAL(section_moved(int, int, QString)));
    QObject::connect(ui->table_widget, SIGNAL(clicked(QModelIndex)), SLOT(selection_changed(QModelIndex)));

}

void ManagerReserveWidget::set_layout(){
    // сохраняем размер шрифта, чтобы на его основе ресайзить виджеты
    int fsize = qApp->font().pointSize();

    ui->plus_button->setFixedSize(fsize*2.5, fsize*5.5);
    ui->minus_button->setFixedSize(fsize*2.5, fsize*2.5);
    ui->delete_button->setFixedSize(ui->minus_button->size());

    ui->show_button->setFixedSize(fsize*10, fsize*2.5);
    ui->clear_button->setFixedSize(ui->show_button->size());
    ui->move_button->setFixedSize(ui->show_button->size());
    ui->close_button->setFixedSize(ui->show_button->size());
    ui->tobase_button->setFixedSize(ui->plus_button->height(), ui->plus_button->height());

    ui->docs_list->setFixedHeight(fsize*16);
    ui->docs_list->setFixedWidth(fsize*10);
    ui->note_edit->setFixedHeight(fsize*2.5);
    ui->table_widget->setFixedHeight(ui->docs_list->height());
    ui->right_splitter_frame->setFixedHeight(ui->table_widget->height());
    //ui->main_frame->setFixedHeight(ui->docs_list->height());
    ui->splitter->setFixedWidth(qApp->desktop()->width());
    ui->buttons_widget->setFixedWidth(fsize*32);
    ui->right_frame->setMinimumWidth(ui->buttons_widget->width());
    ui->smaller_splitter->setMaximumWidth(qApp->desktop()->width());
    ui->fake_label->setMinimumWidth(0);
    ui->right_frame->setMaximumWidth(ui->buttons_widget->width()*2);

    this->setFixedSize(qApp->desktop()->width(), ui->table_widget->height());
}

void ManagerReserveWidget::set_grey_table(GreyTable *g){
    this->grey_table = g;
}

void ManagerReserveWidget::restore_width(int index, int width){
    this->ui->table_widget->restore_width(index, width);
}

void ManagerReserveWidget::restore_order(int logical, int newvisual){
    this->ui->table_widget->restore_order(logical, newvisual);
}

void ManagerReserveWidget::open_contragents_slot(){
    int doc_num = ui->docs_list->currentIndex().row() + 1;
    emit open_contragents(doc_num, 1);
}

void ManagerReserveWidget::text_changed_slot(){
    // если поле "для кого" очищено и контрагент в объекте класса установлен, то очистить его
    if(ui->note_edit->text().length() == 0 && this->contragent_id > 0)
        initial_clear_contragent();
}

void ManagerReserveWidget::set_contragent(int id, QString name){
    this->contragent_id = id;
    ui->note_edit->setText(name);
}

void ManagerReserveWidget::clear_contragent(){
    this->contragent_id = 0;
    ui->note_edit->clear();
}

void ManagerReserveWidget::set_doc_color(int num, bool red){
    list_model->setData(list_model->index(num, 0), red ? Qt::red : Qt::black, Qt::TextColorRole);
    change_doc(ui->docs_list->currentIndex());
}

void ManagerReserveWidget::fill_list(){
    /*for(int i = 1; i <= max_docs; i++){
        QListWidgetItem *ql = new QListWidgetItem(QString("Заявка %1").arg(i), ui->docs_list);
        ql->setData(Qt::UserRole, i);
    }
*/
    ui->docs_list->setModel(list_model);
    ui->docs_list->setSelectionModel(selection_model);
    ui->docs_list->setCurrentIndex(list_model->index(0, 0));
    change_doc(list_model->index(0, 0));

}

void ManagerReserveWidget::initial_set_contragent(){
    clear_contragent();
    QSqlQuery query(base);
    QString query_str = QString("SELECT rd.contragent_id, c.name "
                                " FROM reserve_docs rd "
                                " JOIN contragents c ON rd.contragent_id = c.id "
                                " WHERE rd.user_id = %1 AND rd.doc_num = %2")
            .arg(USER_ID)
            .arg(ui->docs_list->selectionModel()->currentIndex().row() + 1);
    if(!query.exec(query_str)){
        qDebug() << "Ошибка sql-запроса: " << query.lastError().text();
        return;
    }
    if(query.size() < 1)
        return;
    query.next();
    this->contragent_id = query.value(0).toInt();
    ui->note_edit->setText(query.value(1).toString());
}

bool ManagerReserveWidget::initial_clear_contragent(){
    // очищаем поле и атрибут класса
    clear_contragent();
    // из базы удалить же
    QSqlQuery query(base);
    QString query_str = QString("UPDATE reserve_docs SET contragent_id = NULL WHERE user_id = %1 AND doc_num = %2")
            .arg(USER_ID)
            .arg(ui->docs_list->currentIndex().data(Qt::UserRole).toInt());
    //qDebug() << query_str;
    if(!query.exec(query_str)){
        qDebug() << "Ошибка sql-запроса: " << query.lastError().text();
        return false;
    }
    // послать сигнал наверх, чтобы удалить контрагента из всех вкладок
    emit clear_contragent_signal();
    return true;
}

void ManagerReserveWidget::set_columns_names(){
    columns_names["id"] = "";
    columns_names["grey_id"] = "id";
    columns_names["name"] = "Название";
    columns_names["quantity"] = "Кол-во";
    columns_names["price"] = "Цена";
    columns_names["sumprice"] = "Сумма";
    columns_names["year"] = "Год";
    columns_names["storage"] = "Склад";
    columns_names["rack"] = "Стеллаж";
    columns_names["board"] = "Полка";
    columns_names["box"] = "Ящик";
    columns_names["note"] = "Требование";
    columns_names["weight"] = "Вес";
    columns_names["selected"] = " ";
}

void ManagerReserveWidget::set_headers(){
    QString hdr;
    for(int i = 0; i < model->columnCount(); i++){
        //qDebug() << model->headerData(i, Qt::Horizontal).toString();
        hdr = columns_names[model->headerData(i, Qt::Horizontal).toString()];
        if(hdr != QString())
            model->setHeaderData(i, Qt::Horizontal, hdr);
    }
}

void ManagerReserveWidget::change_doc(QModelIndex cur, QModelIndex prev){
    //set_reserve_header();
    ui->num_label->setText(QString("Заявка %1").arg(list_model->data(cur, Qt::UserRole).toString()));
    // если в базе нет такого документа (ну программа первый раз запускается) - создаём его в reserve_docs
    // а вообще, это стыд и срам, убрать впоследствии
    QSqlQuery query(base);
    QString query_str = QString("SELECT count(*) FROM reserve_docs WHERE doc_num = %1 AND user_id = %2").arg(cur.data(Qt::UserRole).toInt()).arg(USER_ID);
    if(!query.exec(query_str)){
        QMessageBox::information(this, "Всё плохо", query.lastError().text(), QMessageBox::Ok);
        return;
    }
    query.next();
    if(query.value(0).toInt() == 0){
        // тут-то и создаём его
        query_str = QString("INSERT INTO reserve_docs (user_id, doc_num) VALUES (%1, %2)").arg(USER_ID).arg(cur.data(Qt::UserRole).toInt());
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Всё снова плохо", query.lastError().text(), QMessageBox::Ok);
            return;
        }
    }
    // после - показываем сожержимое
    //ui->table_widget->delete_model();
    //if(model != 0)
      //  delete model;
    query_str = QString("SELECT r.selected as selected, "
                        "r.id as id, "
                        "r.grey_id as grey_id,  "
                        "t.name as name, "
                        "r.quantity as quantity, "
                        "r.price as price, "
                        "(r.price*r.quantity) as sumprice, "
                        "g.year as year, "
                        "p.storage as storage, "
                        "p.rack as rack, "
                        "p.board as board, "
                        "p.box as box, "
                        "r.note as note, "
                        "t.weight as weight "
                        "FROM reserve r JOIN greytable g ON r.grey_id = g.id JOIN trademarks t ON g.trademark_id = t.id JOIN places p ON g.place_id = p.id "
                        "WHERE r.doc_id = (SELECT id FROM reserve_docs WHERE user_id = %1 AND doc_num = %2) ORDER BY r.id")
            .arg(USER_ID)
            .arg(cur.data(Qt::UserRole).toInt());
    //qDebug() << query_str;
    source->setQuery(query_str, base);
    if(source->lastError().isValid()){
        QMessageBox::information(this, "Не удалось сформировать таблицу", source->lastError().text(), QMessageBox::Ok);
        return;
    }
    model->setSourceModel(source);

    set_headers();
    ui->table_widget->setModel(model);
    //ui->table_widget->resizeRowsToContents();
    ui->table_widget->hideColumn(1);

    double sum = count_sum();
    list_model->setData(ui->docs_list->currentIndex(), (sum > 0 ? Qt::red : Qt::black), Qt::TextColorRole);
    ui->table_widget->set_filled();
    initial_set_contragent();
}

void ManagerReserveWidget::add_position(){
    if(GLOBAL_MODE != SEARCHER_GREY_MODE)
        return;

    int grey_id = grey_table->table_data(columns_grey_ids["id"]).toInt();
    int doc_num = ui->docs_list->currentIndex().data(Qt::UserRole).toInt();
    int quantity = grey_table->table_data(columns_grey_ids["quantity"]).toInt();
    double retail_price = grey_table->table_data(columns_grey_ids["price_ret"]).toDouble();
    double whole_price = grey_table->table_data(columns_grey_ids["price_whole"]).toDouble();
    int whole_begin = grey_table->table_data(columns_grey_ids["whole_begin"]).toInt();

    ReserveAddPosition *add = new ReserveAddPosition(grey_id, doc_num, quantity, retail_price, whole_price, whole_begin, this);
    if(add->exec() == QDialog::Accepted){
        //refresh_table();
        change_doc(ui->docs_list->currentIndex());
        emit need_refresh();
    }

}

void ManagerReserveWidget::refresh_table(){
    if(GLOBAL_MODE != SEARCHER_GREY_MODE)
        return;
    grey_table->refresh_table();
}

double ManagerReserveWidget::count_sum(){
    double sum = 0;
    for(int i = 0; i < ui->table_widget->model()->rowCount(); i++)
        sum += ui->table_widget->table_data(i, 5).toDouble();
    ui->summ_label->setText(QString::number(sum, 'f', 2));
    return sum;
}

void ManagerReserveWidget::subtr_quant(){
    if(this->selection_model->selectedRows().count() > 1)
        return;
    int id = ui->table_widget->table_data(0).toInt();
    int quantity = ui->table_widget->table_data(3).toInt();
    ReserveSubtract *rs = new ReserveSubtract(id, quantity, this);
    if(rs->exec() == QDialog::Accepted){
        //refresh_table();
        change_doc(ui->docs_list->currentIndex());
    }
    emit need_refresh();
}

void ManagerReserveWidget::delete_position(){
    // удалить позицию из таблицы и вернуть в базу
    if(QMessageBox::question(this, "Удалить позиции?", "Удалить выбранные позиции?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
        foreach(QModelIndex i, ui->table_widget->selectionModel()->selectedRows()){
            int id = ui->table_widget->table_data(i.row(),0).toInt();
            delpos(id);
        }
        //refresh_table();
        change_doc(ui->docs_list->currentIndex());
        emit need_refresh();
    }
}

void ManagerReserveWidget::delpos(int id){
    QSqlQuery query(base);
    QString query_str = QString("DELETE FROM reserve WHERE id = %1").arg(id);
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Не удалось удалить", query.lastError().text(), QMessageBox::Ok);
        return;
    }
    //refresh_table();
}

void ManagerReserveWidget::clear_doc(){
    if(QMessageBox::question(this, "Очистить заявку?", "Очистить заявку и вернуть всё в базу?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
        int c = 0;
        while(c < ui->table_widget->model()->rowCount()){
            int id = ui->table_widget->table_data(c, 0).toInt();
            delpos(id);
            c++;
        }
    }
    change_doc(ui->docs_list->currentIndex());
    emit need_refresh();
}

void ManagerReserveWidget::selection_changed(QModelIndex i){
    if(!i.isValid())
        return;

    if(count_selected() <= 1){
        // снять выделение
        model->setData(model->index(last_selected_row, 0), Qt::Unchecked, Qt::CheckStateRole);
        // установить выделение
        model->setData(model->index(i.row(), 0), Qt::Checked, Qt::CheckStateRole);
        ui->table_widget->setCurrentIndex(i);
    }
}


void ManagerReserveWidget::move_doc(){
    if(ui->table_widget->model()->rowCount() == 0)
        return;

    select_all = false;
    last_selected_row = -1;
    if(count_selected() == 0){
        // если всего одна строка, перемещаем её
        // если много - предлагаем выбрать позиции
        if(model->rowCount() == 1){
            select_all = true;
        }
        else{
            QMessageBox::information(this, "Ошибка", "Выберите строки", QMessageBox::Ok);
            return;
        }
    }
    MovePositions *mp = new MovePositions(list_model, this);
    connect(mp, SIGNAL(doc_selected(int)), SLOT(move_to_doc(int)));
    connect(mp, SIGNAL(reserve_selected(int)), SLOT(move_to_reserve(int)));
    mp->exec();
}

int ManagerReserveWidget::count_selected(){
    int n = 0;
    for(int i = 0; i < model->rowCount(); i++){
        if(model->data(model->index(i, 0), Qt::CheckStateRole) == Qt::Checked){
            n++;
            last_selected_row = i;
        }
    }
    qDebug() << "selected" << n << "strings";
    return n;
}

void ManagerReserveWidget::tobase(){
    if(this->model->rowCount() == 0)
        return;

    ReserveSelectDocType *wgt = new ReserveSelectDocType(this);
    connect(wgt, SIGNAL(selected(int, bool)), this, SLOT(create_doc(int, bool)));

    wgt->exec();
}

void ManagerReserveWidget::move_to_doc(int doc_id){
    // получили, например, id документа
    QSqlQuery query(base);
    QString query_str;
    // сохраняем в булевой переменной, просмотрен ли документ (если побирать не нужно - также false)
    query_str = QString("SELECT podbor, discount_percents, (SELECT max(num) FROM docs_details WHERE doc_id = %1) FROM docslist WHERE id = %1").arg(doc_id);
    if(!query.exec(query_str)){
        QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_1)").arg(query.lastError().text()), "ОК");
        return;
    }
    if(query.size() == 0){
        QMessageBox::information(this, "Ошибка", QString("Документ с id = %1 не найден.)").arg(doc_id), "ОК");
        return;
    }
    query.next();
    bool viewed = (query.value(0).toInt() <= 1 ? false : true);
    double discount = query.value(1).toDouble();
    int maxnum = query.value(2).toInt();

    if(!base.transaction()){
        QMessageBox::information(this, "Ошибка", QString("Не удаётся начать транзакцию: %1 (move_to_doc)").arg(base.lastError().text()), "ОК");
        return;
    }
    // проходимся по всем строкам, ища среди них выделенные
    // если select_all == true, то перемещаем всё подряд
    for(int j = 0; j < model->rowCount(); j++){
        QModelIndex i = model->index(j, 0);
        if(!select_all && ui->table_widget->table_data(i.row(), 0, Qt::CheckStateRole) == Qt::Unchecked)
            continue;
        /* Копируем значения полей из таблицы reserve в таблицу docs_details, где doc_id = id:
           - grey_id - в grey_id
           - quantity - в quantity
           - price - в price
           - note - в notes
           А также:
           - is_discount = true
           - discount_price = price*(docslist.discount)
           - num = max(num)+1
           - podbor = false
           Предварительно проверить: если документ не просмотрен (viewed == false) и там присутствует деталь с таким grey_id, склеить количество
           После всего этого удалить из reserve строку с id = rec_id
        */
        int rec_id = ui->table_widget->table_data(i.row(),1).toInt();
        int grey_id = ui->table_widget->table_data(i.row(),2).toInt();
        int quantity = ui->table_widget->table_data(i.row(),4).toInt();
        double price = ui->table_widget->table_data(i.row(),5).toDouble();
        QString notes = ui->table_widget->table_data(i.row(),12).toString();

        // схоронили? а теперь удоляем строку из резерва
        // удалить её нужно прежде чем создадим новую в документе, иначе в сером экране создастся отрицательное кол-во и триггер будет ругаться
        query_str = QString("DELETE FROM reserve WHERE id = %1").arg(rec_id);
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_5)").arg(query.lastError().text()), "ОК");
            base.rollback();
            return;
        }

        // делаем ту самую проверку
        if(!viewed){
            query_str = QString("SELECT id FROM docs_details WHERE doc_id = %1 AND grey_id = %2").arg(doc_id).arg(grey_id);
            if(!query.exec(query_str)){
                QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_2)").arg(query.lastError().text()), "ОК");
                base.rollback();
                return;
            }
            if(query.size() > 0){
                query.next();
                int t_id = query.value(0).toInt();
                query_str = QString("UPDATE docs_details SET quantity = quantity + %1, selected = false WHERE id = %2").arg(quantity).arg(t_id);
                if(!query.exec(query_str)){
                    QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_3)").arg(query.lastError().text()), "ОК");
                    base.rollback();
                    return;
                }
                qDebug() << ui->table_widget->table_data(i.row(),2).toString() << "Кол-во суммировано";
                continue;
            }
            // если записи с таким grey_id не нашлось, выходим из блока и вставляем новую строку по общим правилам
        }
        query_str = QString("INSERT INTO docs_details (doc_id, grey_id, quantity, price, notes, is_discount, discount_price, num, podbor, selected) "
                            " VALUES (%1, %2, %3, %4, '%5', true, %6, %7, false, false)")
                .arg(doc_id)
                .arg(grey_id)
                .arg(quantity)
                .arg(price)
                .arg(notes)
                .arg(price*(100-discount)/100)
                .arg(++maxnum);
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_4)").arg(query.lastError().text()), "ОК");
            base.rollback();
            return;
        }
    }

    if(!base.commit()){
        QMessageBox::information(this, "Ошибка", QString("Не удаётся завершить транзакцию: %1 (move_to_doc)").arg(base.lastError().text()), "ОК");
        base.rollback();
        return;
    }
    change_doc(ui->docs_list->currentIndex());
}

void ManagerReserveWidget::move_to_reserve(int id){
    // полученный id на единицу меньше, чем номер документа
    int docnum = id+1;

    QSqlQuery query(base);
    QString query_str;
    if(!base.transaction()){
        QMessageBox::information(this, "Ошибка", QString("Не удаётся начать транзакцию: %1 (move_to_reserve)").arg(base.lastError().text()), "ОК");
        return;
    }
    // проходимся по всем строкам, ища среди них выделенные
    // если select_all == true, то перемещаем всё подряд
    for(int j = 0; j < model->rowCount(); j++){
        QModelIndex i = model->index(j, 0);
        if(!select_all && ui->table_widget->table_data(i.row(), 0, Qt::CheckStateRole) == Qt::Unchecked)
            continue;

        int rec_id = ui->table_widget->table_data(i.row(),1).toInt();
        int grey_id = ui->table_widget->table_data(i.row(),2).toInt();
        int quantity = ui->table_widget->table_data(i.row(),4).toInt();
        // проверить, есть в нужном документе позиция с таким же grey_id, и если есть - склеить кол-во
        query_str = QString("SELECT id FROM reserve WHERE grey_id = %1 AND doc_id = (SELECT id FROM reserve_docs WHERE user_id = %2 AND doc_num = %3)")
                .arg(grey_id)
                .arg(USER_ID)
                .arg(docnum);
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_1)").arg(query.lastError().text()), "ОК");
            base.rollback();
            return;
        }
        if(query.size() > 0){
            query.next();
            int t_id = query.value(0).toInt();

            // и если есть - то предыдущую строку из резерва надо удалить
            query_str = QString("DELETE FROM reserve WHERE id = %1").arg(rec_id);
            if(!query.exec(query_str)){
                QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_2)").arg(query.lastError().text()), "ОК");
                base.rollback();
                return;
            }

            query_str = QString("UPDATE reserve SET quantity = quantity + %1, selected = false WHERE id = %2").arg(quantity).arg(t_id);
            if(!query.exec(query_str)){
                QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_3)").arg(query.lastError().text()), "ОК");
                base.rollback();
                return;
            }
            qDebug() << ui->table_widget->table_data(i.row(),2).toString() << "Кол-во суммировано";
            continue;
        }
        // если мы до сюда добрались - такой позиции нет, тогда просто меняем doc_id
        query_str = QString("UPDATE reserve SET doc_id = (SELECT id FROM reserve_docs WHERE user_id = %1 AND doc_num = %2), selected = false WHERE id = %3")
                .arg(USER_ID).arg(docnum).arg(rec_id);
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_4)").arg(query.lastError().text()), "ОК");
            base.rollback();
            return;
        }
    }

    if(!base.commit()){
        QMessageBox::information(this, "Ошибка", QString("Не удаётся завершить транзакцию: %1 (move_to_reserve)").arg(base.lastError().text()), "ОК");
        base.rollback();
        return;
    }
    set_doc_color(id);
}

void ManagerReserveWidget::header_clicked(int index){
    if(index != 0)
        return;

    QString query_str;
    if(!all_selected){
        all_selected = true;
        // выделяем всё
        query_str = QString("UPDATE reserve SET selected = true WHERE doc_id = (SELECT id FROM reserve_docs WHERE user_id = %1 AND doc_num = %2)")
                .arg(USER_ID)
                .arg(ui->docs_list->currentIndex().row()+1);
    }
    else{
        all_selected = false;
        // наоборот, снимаем
        query_str = QString("UPDATE reserve SET selected = false WHERE doc_id = (SELECT id FROM reserve_docs WHERE user_id = %1 AND doc_num = %2)")
                .arg(USER_ID)
                .arg(ui->docs_list->currentIndex().row()+1);
    }
    QSqlQuery query(base);
    if(!query.exec(query_str)){
        qDebug() << "Header: " << query.lastError().text();
        return;
    }
    source->setQuery(source->query().lastQuery(), base);
}


void ManagerReserveWidget::create_doc(int doctype, bool nocomplect){
    /*
      Сначала с таблицу docslist записать информацию о документа:
        - doctype - выбран в функции tobase и передан в конструкторе,
        - docnum - выбирается из базы максимальные docnum типа doctype в текущем году и увеличивается на 1,
            если такового нет - то просто 1
        - year - текущий год
        - datetime - sql-функция NOW()
        - user_id - глобальная переменная USER_ID
        - contragent_id - если this->contragent_id положителен, то он и подставляется
        - provider_id - id = 1
        - notes - если this->contragent_id неположителен, то копируется содержимое ui->note_edit
    */
    int year = QDate::currentDate().year();
    QMap<QString, QString> params;
    params["doctype_id"] = QString::number(doctype);
    params["docnum"] = QString("(SELECT coalesce(max(docnum), 0) + 1 FROM docslist WHERE doctype_id = %1 AND year = %2)").arg(doctype).arg(year);
    params["year"] = QString::number(year);
    params["docdate"] = "current_date";
    params["doctime"] = "current_time";
    params["user_id"] = QString::number(USER_ID);
    params["podbor"] = nocomplect ? "5" : "1";
    params["provider_id"] = "1";
    if(this->contragent_id > 0)
       params["contragent_id"] = QString::number(this->contragent_id);
    else if(ui->note_edit->text().length() > 0)
        params["notes"] = ui->note_edit->text();
    // формируем строкуу
    QStringList keys = params.keys();
    QStringList vals = params.values();
    QString query_str = QString("INSERT INTO docslist (%1) VALUES (%2) RETURNING id")
            .arg(keys.join(", "))
            .arg(vals.join(", "));
    //qDebug() << query_str;
    // выполняем запрос. Делаем всё в транзакции!
    QSqlQuery query(base);
    if(!base.transaction()){
        QMessageBox::warning(this, "Ошибка", QString("Невозможно начать транзакцию. ").append(base.lastError().text()), QMessageBox::Ok);
        return;
    }
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Невозможно выполнить запрос. ").append(query.lastError().text()), QMessageBox::Ok);
        base.rollback();
        return;
    }
    // так как мы затребовали id созданной записи, нужно его извлечь и сохранить
    if(query.size() < 1){
        base.rollback();
        return;
    }
    query.next();
    int doc_id = query.value(0).toInt();

    /*
      Далее каждую позицию из резерва нужно перенести в таблицу docs_details.
      doc_id - получили только что
      num - счётчик, начиная с 1
      grey_id, quantity, price, notes - копируем из резерва
      is_discount = false, discount = 0, т.к. скидок нет
      После чего из reserve строку удалить.
      Индексы в модели резерва:
        id = 0
        grey_id = 1
        quantity = 3
        price = 4
        note = 11
    */
    int id, grey_id, quantity, num;
    double price, weight;
    QString notes;
    for(int i = 0; i < this->model->rowCount(); i++){
        id = ui->table_widget->table_data(i, 0).toInt();
        num = i + 1;
        grey_id = ui->table_widget->table_data(i, 1).toInt();
        quantity = ui->table_widget->table_data(i, 3).toInt();
        price = ui->table_widget->table_data(i, 4).toDouble();
        notes = ui->table_widget->table_data(i, 11).toString();
        weight = ui->table_widget->table_data(i, 12).toDouble();

        query_str = QString("DELETE FROM reserve WHERE id = %1").arg(id);
        if(!query.exec(query_str)){
            QMessageBox::warning(this, "Ошибка", QString("Невозможно выполнить запрос. ").append(query.lastError().text()), QMessageBox::Ok);
            base.rollback();
            return;
        }

        query_str = QString("INSERT INTO docs_details (doc_id, num, grey_id, quantity, price, notes, is_discount, discount_price, print_string, weight) "
                            " VALUES (%1, %2, %3, %4, '%5', '%6', %7, '%8', %9, "
                            "(SELECT s.single_name || ' ' || t.name FROM greytable g "
                            " JOIN trademarks t ON t.id = g.trademark_id "
                            " JOIN subgroups s ON s.id = t.subgroup_id WHERE g.id = %3))")
                .arg(doc_id)
                .arg(num)
                .arg(grey_id)
                .arg(quantity)
                .arg(price)
                .arg(notes)
                .arg("false")
                .arg(price)
                .arg(weight);
        if(!query.exec(query_str)){
            QMessageBox::warning(this, "Ошибка", QString("Невозможно выполнить запрос. ").append(query.lastError().text()), QMessageBox::Ok);
            qDebug() << query_str;
            base.rollback();
            return;
        }

    }
    if(!initial_clear_contragent()){
        base.rollback();
        return;
    }
    base.commit();

    change_doc(ui->docs_list->currentIndex());
}

void ManagerReserveWidget::view_doc(){
    //oldmodel = static_cast<QSortFilterProxyModel*>(ui->table_widget->get_model());
    //oldmodel = this->model;
    ui->table_widget->showColumn(1);

    QString query_str = QString("SELECT t.name, sum(r.quantity) as sumprice "
                        "FROM reserve r JOIN greytable g ON r.grey_id = g.id JOIN trademarks t ON g.trademark_id = t.id "
                        "WHERE r.doc_id = (SELECT id FROM reserve_docs WHERE user_id = %1 AND doc_num = %2) GROUP BY t.name")
            .arg(USER_ID)
            .arg(ui->docs_list->currentIndex().data(Qt::UserRole).toInt());
    QSqlQueryModel *temp_model = new QSqlQueryModel(this);
    temp_model->setQuery(query_str, base);
    if(temp_model->lastError().isValid()){
        QMessageBox::information(this, "Не удалось сформировать таблицу", temp_model->lastError().text(), QMessageBox::Ok);
        return;
    }
    ui->table_widget->setModel(temp_model);
    set_headers();
}

void ManagerReserveWidget::restore_doc(){
    ui->table_widget->delete_model();
    ui->table_widget->setModel(model);
    ui->table_widget->restore_columns_width();
    ui->table_widget->restore_columns_order();
    ui->table_widget->hideColumn(1);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// ДОБАВИТЬ ПОЗИЦИЮ ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

ReserveAddPosition::ReserveAddPosition(int id, int num, int quantity, double retail_price, double whole_price, int whole_begin, QWidget *parent) : QDialog(parent)
{
    editing = false;

    // сохраняем конструктор
    grey_id = id;
    doc_num = num;
    base_quantity = quantity;
    base_retail_price = retail_price;
    base_whole_price = whole_price;
    base_whole_begin = whole_begin;

    // создаём интерфейс
    quantity_total = new QLineEdit();
    quantity_spin = new QLineEdit();
    //base_price = new QLineEdit();
    price = new QDoubleSpinBox();
    note = new QLineEdit();
    accept_button = new QPushButton("Занести");
    reject_button = new QPushButton("Отменить");

    price->setMaximum(1000000);
    price->setSuffix(" р.");
    QIntValidator *intvalid = new QIntValidator(0, base_quantity, this);
    quantity_spin->setValidator(intvalid);
    quantity_total->setEnabled(false);
    //base_price->setEnabled(false);

    quantity_total->setText(QString("%1 шт.").arg(base_quantity));
    //quantity_spin->setText("0");
    set_base_price_text(base_retail_price);
    price->setValue(base_retail_price);

    set_layout();
    this->adjustSize();
    this->setFixedSize(this->size());
    this->setWindowTitle("Выписать");

    price_changed = false;  // если цена хоть раз изменена вручную, делаем это true и автоматически её боле не меняем

    QObject::connect(this->price, SIGNAL(editingFinished()), SLOT(price_entered()));

    QObject::connect(this->accept_button, SIGNAL(clicked()), SLOT(accept()));
    QObject::connect(this->reject_button, SIGNAL(clicked()), SLOT(reject()));
}

void ReserveAddPosition::set_layout(){
    QGridLayout *grid_lt = new QGridLayout(this);

    // лейблы для подписей
    QLabel *qtotal_label = new QLabel("В наличии");
    QLabel *bprice_label = new QLabel("Баз. цена");
    QLabel *quant_label = new QLabel("Выписать");
    QLabel *price_label = new QLabel("Цена");
    QLabel *note_label = new QLabel("Требование");

    QLabel *price_retail_label = new QLabel(QString("Розн.: %1 р.").arg(base_retail_price));
    QLabel *price_whole_label = new QLabel(QString("Опт.: %1 р.").arg(base_whole_price));
    QLabel *price_begin_label = new QLabel(QString("Опт с %1 шт.").arg(base_whole_begin));
    QLayout *prices_lt = new QVBoxLayout();
    prices_lt->addWidget(price_retail_label);
    prices_lt->addWidget(price_whole_label);
    prices_lt->addWidget(price_begin_label);


    grid_lt->addWidget(qtotal_label, 0, 0);
    grid_lt->addWidget(quantity_total, 0, 1);
    grid_lt->addWidget(bprice_label, 0, 2);
    //grid_lt->addWidget(base_price, 0, 3);
    grid_lt->addLayout(prices_lt, 0, 3);

    grid_lt->addWidget(quant_label, 1, 0);
    grid_lt->addWidget(quantity_spin, 1, 1);
    grid_lt->addWidget(price_label, 1, 2);
    grid_lt->addWidget(price, 1, 3);

    grid_lt->addWidget(note_label, 2, 0);
    grid_lt->addWidget(note, 2, 1, 1, 3);

    // для кнопок сделаем отдельный лайаут
    QHBoxLayout *b_lt = new QHBoxLayout();
    b_lt->addWidget(accept_button);
    b_lt->addWidget(reject_button);

    grid_lt->addLayout(b_lt, 3, 0, 1, 4);

    this->setLayout(grid_lt);
    this->setFixedWidth(350);
}

void ReserveAddPosition::set_base_price_text(double p){
    //base_price->setText(QString("%1 р.").arg(QString::number(p, 'f', 2)));
}

void ReserveAddPosition::accept(){
    if(!editing){
        if(quantity_spin->text().toInt() > base_quantity){
            QMessageBox::warning(this, "Ошибка", "Введённое количество больше, чем на складе", QMessageBox::Ok);
            return;
        }
        if(quantity_spin->text().toInt() <= 0){
            QMessageBox::warning(this, "Ошибка", "Введённое количество нулевое", QMessageBox::Ok);
            return;
        }
        if(price->value() < 0){
            QMessageBox::warning(this, "Ошибка", "Цена не может быть отрицательной!", QMessageBox::Ok);
            return;
        }
        if(quantity_spin->text().toInt() == 0){
            QDialog::reject();
            return;
        }

        // сохранить в базу, в таблицу reserve
        // если такой id уже есть в этой заявке - добавляем кол-во, иначе создаём новую запись
        QSqlQuery query(base);
        QString query_str = QString("SELECT count(*) FROM reserve WHERE grey_id = %1 AND doc_id = %2")
                .arg(grey_id)
                .arg(QString("(SELECT id FROM reserve_docs WHERE doc_num = %1 AND user_id = %2)").arg(doc_num).arg(USER_ID));
        if(!query.exec(query_str)){
            QMessageBox::warning(this, "Ошибка", QString("Не удалось: %1").arg(query.lastError().text()), QMessageBox::Ok);
            return;
        }
        query.next();
        if(query.value(0).toInt() == 1){
            query_str = QString("UPDATE reserve SET quantity = quantity + %1 WHERE grey_id = %2 AND doc_id = %3")
                    .arg(quantity_spin->text())
                    .arg(grey_id)
                    .arg(QString("(SELECT id FROM reserve_docs WHERE doc_num = %1 AND user_id = %2)").arg(doc_num).arg(USER_ID));
            if(!query.exec(query_str)){
                QMessageBox::warning(this, "Ошибка", QString("Не удалось добавить к количеству: %1").arg(query.lastError().text()), QMessageBox::Ok);
                return;
            }
        }
        else{
            query_str = QString("INSERT INTO reserve(doc_id, grey_id, quantity, price, note) "
                                        "VALUES(%1, %2, %3, %4, '%5')")
                    .arg(QString("(SELECT id FROM reserve_docs WHERE doc_num = %1 AND user_id = %2)").arg(doc_num).arg(USER_ID))
                    .arg(grey_id).arg(quantity_spin->text()).arg(price->value()).arg(note->text());
            //qDebug() << query_str;
            if(!query.exec(query_str)){
                QMessageBox::warning(this, "Ошибка", QString("Не удалось добавить в заявку: %1").arg(query.lastError().text()), QMessageBox::Ok);
                return;
            }
        }
    }
    QDialog::accept();
}


void ReserveAddPosition::price_entered(){
    if(!price_changed){
        qDebug("Цена введена вручную.");
        price_changed = true;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// МОДЕЛЬ ТАБЛИЦЫ /////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

ReserveProxyModel::ReserveProxyModel(QWidget *parent) : QSortFilterProxyModel(parent)
{
}

Qt::ItemFlags ReserveProxyModel::flags(const QModelIndex &index) const{
    if(index.column() == 0)
        return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
    return QSortFilterProxyModel::flags(index);
}


QVariant ReserveProxyModel::data(const QModelIndex &index, int role) const{
    if(role == Qt::DisplayRole && (index.column() == 5 || index.column() == 6))
        return QString::number(QSortFilterProxyModel::data(index, role).toDouble(), 'f', 2);
    // чекбоксы
    if(index.column() == 0){
        if(role == Qt::CheckStateRole)
            return QSortFilterProxyModel::data(index, Qt::DisplayRole).toBool() ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return QVariant();
    }

    return QSortFilterProxyModel::data(index, role);
}

bool ReserveProxyModel::setData(const QModelIndex &index, const QVariant &value, int role){
    qDebug("stavim...");
    if(index.column() == 0 && role == Qt::CheckStateRole){
        qDebug("here");
        QSqlQuery query(base);
        QModelIndex id_index = QSortFilterProxyModel::index(index.row(), 1);
        QString query_str = QString("UPDATE reserve SET selected = %1 WHERE id = %2")
                .arg(value.toBool() ? "true" : "false")
                .arg(QSortFilterProxyModel::data(id_index).toInt());
        if(!query.exec(query_str)){
            qDebug() << query.lastError().text();
            return false;
        }
        QSqlQueryModel *m = (QSqlQueryModel*)this->sourceModel();
        m->setQuery(m->query().lastQuery(), base);
        return true;
    }

    return QSortFilterProxyModel::setData(index, value, role);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// ВЫЧЕСТЬ КОЛИЧЕСТВО ///////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

ReserveSubtract::ReserveSubtract(int id, int quantity, QWidget *parent){
    c_id = id;
    c_quantity = quantity;

    subtr_spin = new QSpinBox();
    quant_line = new QLineEdit();
    accept_button = new QPushButton("Вычесть");
    reject_button = new QPushButton("Отмена");

    QLabel *s_label = new QLabel("Вычесть");
    QLabel *q_label = new QLabel("Кол-во");

    subtr_spin->setValue(0);
    subtr_spin->setMinimum(0);
    subtr_spin->setMaximum(c_quantity);
    quant_line->setText(QString("%1 шт.").arg(c_quantity));

    QGridLayout *lt = new QGridLayout(this);
    QVBoxLayout *vlt1 = new QVBoxLayout();
    QVBoxLayout *vlt2 = new QVBoxLayout();
    vlt1->addWidget(s_label);
    vlt1->addWidget(subtr_spin);
    vlt2->addWidget(q_label);
    vlt2->addWidget(quant_line);
    lt->addLayout(vlt1, 0, 0);
    lt->addLayout(vlt2, 0, 1);
    lt->addWidget(accept_button, 1, 0);
    lt->addWidget(reject_button, 1, 1);

    this->setWindowTitle("Вычесть");

    connect(subtr_spin, SIGNAL(valueChanged(int)), SLOT(change_quant(int)));
    connect(accept_button, SIGNAL(clicked()), SLOT(accept()));
    connect(reject_button, SIGNAL(clicked()), SLOT(reject()));
}

void ReserveSubtract::change_quant(int q){
    quant_line->setText(QString("%1 шт.").arg(c_quantity - q));
}

void ReserveSubtract::accept(){
    if(subtr_spin->value() > 0){
        QSqlQuery query(base);
        QString query_str = QString("UPDATE reserve SET quantity = quantity - %1 WHERE id = %2").arg(subtr_spin->value()).arg(c_id);
        if(!query.exec(query_str)){
            QMessageBox::warning(this, "Не удалось вычесть", query.lastError().text(), QMessageBox::Ok);
            return;
        }
    }
    QDialog::accept();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////// КЛАСС ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// ДИАЛОГ ВЫБОРА ТИПА ДОКУМЕНТА /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

ReserveSelectDocType::ReserveSelectDocType(QWidget *parent) : QDialog(parent){
    this->setWindowTitle("Документ");
    doctypes = new QComboBox();
    nocomplect = new QCheckBox("НЕ ПОДБИРАТЬ");
    okbutton = new QPushButton("OK");
    doctypes->setFixedWidth(qApp->font().pointSize() * 20);
    nocomplect->setChecked(false);
    okbutton->setFixedWidth(doctypes->width());
    QVBoxLayout *lt = new QVBoxLayout(this);
    lt->addWidget(doctypes);
    lt->addWidget(nocomplect);
    lt->addWidget(okbutton);
    this->setLayout(lt);
    this->setFixedSize(qApp->font().pointSize() * 25, qApp->font().pointSize() * 12);

    // fill combobox
    QSqlQuery query(base);
    QString query_str = "SELECT id, name FROM doctypes WHERE \"primary\" = 'true'";
    if(!show_hidden)
        query_str += " AND type = 1";
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", query.lastError().text());
        return;
    }
    while(query.next())
        doctypes->addItem(query.value(1).toString(), query.value(0));
    //connect(doctypes, SIGNAL())
    connect(okbutton, SIGNAL(clicked()), this, SLOT(ok_clicked()));

}

void ReserveSelectDocType::ok_clicked(){
    emit selected(doctypes->itemData(doctypes->currentIndex()).toInt(), nocomplect->isChecked());
    QDialog::accept();
}

SmartSplitter::SmartSplitter(QWidget *parent){
    connect(this, SIGNAL(mouse_released()), SLOT(splitter_moved_slot()));
}

void SmartSplitter::mouseReleaseEvent(QMouseEvent *e){
    qDebug() << "mouse released!";
    splitter_moved_slot();

    QSplitter::mouseReleaseEvent(e);
}

void SmartSplitter::splitter_moved_slot(){
    qDebug() << this->sizes()[0] << this->sizes()[1];

}
