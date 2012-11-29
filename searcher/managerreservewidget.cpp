#include "managerreservewidget.h"
#include "ui_managerreservewidget.h"

ManagerReserveWidget::ManagerReserveWidget(ReserveStruct rstruct,
                                           QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManagerReserveWidget)
{
    ui->setupUi(this);
    ui->table_widget->setSelectionMode(QTableView::ExtendedSelection);
    QModelIndex cur = rstruct.selection_model->currentIndex();
    max_docs = 10;
    oldmodel = 0;
    grey_table = 0;
    this->source = rstruct.source_model;
    this->list_model = rstruct.list_model;
    this->selection_model = rstruct.selection_model;
    this->table_header = rstruct.table_header;
    //this->table_header->setParent(ui->table_widget);
    //QHeaderView *table_headerd = new QHeaderView(Qt::Horizontal);
    //ui->table_widget->setHorizontalHeader(table_header);
    //ui->table_widget->horizontalHeader()->setVisible(true);
    ui->table_widget->horizontalHeader()->setHighlightSections(false);

    set_columns_names();
    set_layout();

    fill_list();
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

    ui->note_button->setFixedSize(fsize*2.5, fsize*2.5);

    ui->docs_list->setFixedHeight(fsize*16);
    ui->docs_list->setFixedWidth(fsize*10);
    ui->note_edit->setFixedHeight(fsize*2.5);
    //ui->table_widget->setFixedHeight(ui->docs_list->height() - ui->note_edit->height());
    //ui->right_frame->setFixedHeight(ui->docs_list->height());
    ui->main_frame->setFixedHeight(ui->docs_list->height());

    ui->table_widget->verticalHeader()->setDefaultSectionSize(qApp->font().pointSize()*2);

    this->setFixedSize(qApp->desktop()->maximumWidth(), ui->main_frame->frameSize().height());
}

void ManagerReserveWidget::set_grey_table(GreyTable *g){
    this->grey_table = g;
}

int ManagerReserveWidget::resize_section(int num, int w){
    int old = ui->table_widget->horizontalHeader()->sectionSize(num);
    ui->table_widget->horizontalHeader()->resizeSection(num, w);
    return old;
}

void ManagerReserveWidget::set_reserve_header(){
    this->ui->table_widget->setHorizontalHeader(table_header);
    //this->table_header->setParent(ui->table_widget);
    this->table_header->show();
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

void ManagerReserveWidget::set_columns_names(){
    columns_names["id"] = "";
    columns_names["grey_id"] = "id";
    columns_names["name"] = "Название";
    columns_names["quantity"] = "Количество";
    columns_names["price"] = "Цена";
    columns_names["sumprice"] = "Сумма";
    columns_names["year"] = "Год";
    columns_names["storage"] = "Склад";
    columns_names["rack"] = "Стеллаж";
    columns_names["board"] = "Полка";
    columns_names["box"] = "Ящик";
    columns_names["note"] = "Требование";
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
    delete ui->table_widget->model();
    query_str = QString("SELECT r.id as id, "
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
                        "r.note as note "
                        "FROM reserve r JOIN greytable g ON r.grey_id = g.id JOIN trademarks t ON g.trademark_id = t.id JOIN places p ON g.place_id = p.id "
                        "WHERE r.doc_id = (SELECT id FROM reserve_docs WHERE user_id = %1 AND doc_num = %2)")
            .arg(USER_ID)
            .arg(cur.data(Qt::UserRole).toInt());
    //qDebug() << query_str;
    model = new QSortFilterProxyModel(this);
    source->setQuery(query_str, base);
    if(source->lastError().isValid()){
        QMessageBox::information(this, "Не удалось сформировать таблицу", source->lastError().text(), QMessageBox::Ok);
        return;
    }
    model->setSourceModel(source);
    set_headers();
    ui->table_widget->setModel(model);
    ui->table_widget->hideColumn(0);
    ui->table_widget->resizeColumnsToContents();
    //ui->table_widget->resizeRowsToContents();

    double sum = count_sum();
    list_model->setData(ui->docs_list->currentIndex(), (sum > 0 ? Qt::red : Qt::black), Qt::TextColorRole);
}

void ManagerReserveWidget::add_position(){
    if(GLOBAL_MODE != SEARCHER_GREY_MODE)
        return;

    int grey_id = grey_table->table_data(columns_grey_ids["id"]).toInt();
    int doc_num = ui->docs_list->currentIndex().data(Qt::UserRole).toInt();
    int quantity = grey_table->table_data(columns_grey_ids["quantity"]).toInt();
    int retail_price = grey_table->table_data(columns_grey_ids["price_ret"]).toDouble();
    int whole_price = grey_table->table_data(columns_grey_ids["price_whole"]).toDouble();
    int whole_begin = grey_table->table_data(columns_grey_ids["whole_begin"]).toInt();

    ReserveAddPosition *add = new ReserveAddPosition(grey_id, doc_num, quantity, retail_price, whole_price, whole_begin, this);
    if(add->exec() == QDialog::Accepted){
        //refresh_table();
        change_doc(ui->docs_list->currentIndex());
    }

    emit need_refresh();
}

void ManagerReserveWidget::refresh_table(){
    if(GLOBAL_MODE != SEARCHER_GREY_MODE)
        return;
    grey_table->refresh_table();
}

double ManagerReserveWidget::count_sum(){
    double sum = 0;
    for(int i = 0; i < ui->table_widget->model()->rowCount(); i++)
        sum += ui->table_widget->model()->data(ui->table_widget->model()->index(i, 5)).toDouble();
    ui->summ_label->setText(QString::number(sum, 'f', 2));
    return sum;
}

void ManagerReserveWidget::subtr_quant(){
    if(ui->table_widget->selectionModel()->selectedRows().count() > 1)
        return;
    int id = ui->table_widget->model()->data(ui->table_widget->model()->index(ui->table_widget->currentIndex().row(), 0)).toInt();
    int quantity = ui->table_widget->model()->data(ui->table_widget->model()->index(ui->table_widget->currentIndex().row(), 3)).toInt();
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
            int id = ui->table_widget->model()->data(model->index(i.row(), 0)).toInt();
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
            int id = ui->table_widget->model()->data(ui->table_widget->model()->index(c, 0)).toInt();
            delpos(id);
            c++;
        }
    }
    change_doc(ui->docs_list->currentIndex());
    emit need_refresh();
}

void ManagerReserveWidget::move_doc(){


}

void ManagerReserveWidget::view_doc(){
    oldmodel = static_cast<QSortFilterProxyModel*>(ui->table_widget->model());

    QString query_str = QString("SELECT t.name, sum(r.quantity) "
                        "FROM reserve r JOIN greytable g ON r.grey_id = g.id JOIN trademarks t ON g.trademark_id = t.id "
                        "WHERE r.doc_id = (SELECT id FROM reserve_docs WHERE user_id = %1 AND doc_num = %2) GROUP BY t.name")
            .arg(USER_ID)
            .arg(ui->docs_list->currentIndex().data(Qt::UserRole).toInt());
    //qDebug() << query_str;
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        QMessageBox::information(this, "Не удалось сформировать таблицу", model->lastError().text(), QMessageBox::Ok);
        return;
    }
    ui->table_widget->setModel(model);
    ui->table_widget->resizeColumnsToContents();

}

void ManagerReserveWidget::restore_doc(){
    if(oldmodel != 0){
        delete ui->table_widget->model();
        ui->table_widget->setModel(oldmodel);
        ui->table_widget->resizeColumnsToContents();
    }
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
    QObject::connect(this->quantity_spin, SIGNAL(valueChanged(int)), SLOT(quantity_changed(int)));

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

    QLabel *price_retail_label = new QLabel(QString("� озн.: %1 р.").arg(base_retail_price));
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
            qDebug() << query_str;
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

void ReserveAddPosition::quantity_changed(int q){
    //set_base_price_text(q < base_whole_begin ? base_retail_price : base_whole_price);
    //if(!price_changed)
    //    price->setValue(q < base_whole_begin ? base_retail_price : base_whole_price);
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
//////////////////////////////////// ПЕ� ЕМЕСТИТЬ ПОЗИЦИИ //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

ReserveMovePositions::ReserveMovePositions(QTableView *table, QModelIndexList indexes, QWidget *parent) : QDialog(parent){
    create_widgets();
    set_layout();

    max_docs = 10;
    reserves_button->setChecked(true);
    form_reserves_list();

    switch_widget();

    connects();
}

inline void ReserveMovePositions::create_widgets(){
    reserves_button = new QRadioButton("Заявки");
    docs_button = new QRadioButton("Документы");
    offers_button = new QRadioButton("Комм. предложения");
    ok_button = new QPushButton("Переместить");
    cancel_button = new QPushButton("Отмена");
    layout = new QStackedLayout();
    docs_widget = new QWidget(this);
    offers_widget = new QWidget(this);
    reserves_list = new QListView();
    docs_list = new QTableView();
    offers_list = new QTableView();


}

inline void ReserveMovePositions::set_layout(){
    // лайаут с тремя радиокнопками, они будут расположены вертикально и сдвинуты кверху
    QVBoxLayout *radiobuttons_vlt = new QVBoxLayout();
    radiobuttons_vlt->addWidget(reserves_button);
    radiobuttons_vlt->addWidget(docs_button);
    radiobuttons_vlt->addWidget(offers_button);
    radiobuttons_vlt->addStretch(1);

    // лайаут с кнопками ОК и Отмена, они будут в самом низу справа
    QHBoxLayout *buttons_hlt = new QHBoxLayout();
    buttons_hlt->addStretch(1);
    buttons_hlt->addWidget(ok_button);
    buttons_hlt->addWidget(cancel_button);

    // формируем docs_list
    //

    // формируем offers_list
    //

    // формируем stacked_layout
    layout->addWidget(reserves_list);
    layout->addWidget(docs_list);
    layout->addWidget(offers_list);

    // главный лайаут
    // состоит из двух: сверху hbox с радиокнопками и стакедлайаутом, снизу кнопки ок и отмена
    QVBoxLayout *main_lt = new QVBoxLayout();
    QHBoxLayout *top_lt = new QHBoxLayout();
    top_lt->addLayout(radiobuttons_vlt);
    top_lt->addLayout(layout);
    main_lt->addLayout(top_lt);
    main_lt->addLayout(buttons_hlt);

    this->setLayout(main_lt);
}

inline void ReserveMovePositions::connects(){
    // перекючение виджетов по радиокнопкам
    connect(this->reserves_button, SIGNAL(clicked()), SLOT(switch_widget()));
    connect(this->docs_button, SIGNAL(clicked()), SLOT(switch_widget()));
    connect(this->offers_button, SIGNAL(clicked()), SLOT(switch_widget()));
}

inline void ReserveMovePositions::form_reserves_list(){
    // в таблице будут два столбца - "Заявка №" и "Для кого"
    // к каждой ячейке с номером заявки присовокупляем оный в UserRole

}

inline void ReserveMovePositions::form_docs_list(){

}

inline void ReserveMovePositions::form_offers_list(){

}



////// СЛОТЫ
void ReserveMovePositions::switch_widget(){
    if(this->reserves_button->isChecked())
        layout->setCurrentWidget(reserves_list);
    else if(this->docs_button->isChecked())
        layout->setCurrentWidget(docs_list);
    else if(this->offers_button->isChecked())
        layout->setCurrentWidget(offers_list);
}
