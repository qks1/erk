#include "document.h"

Document::Document(QStackedWidget *stack,
                   QStack<QWidget *> prev_wgts,
                   int id,
                   QStandardItemModel *list_model,
                   QWidget *parent) : QWidget(parent){
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->stack = stack;
    this->prev_wgts = prev_wgts;

    QSqlQuery query(base);
    QString query_str = QString("SELECT dl.doctype_id, dt.name as doctype, "
                                " dl.sumprice, dl.discount_percents, dl.discount_sum, "
                                " dl.docnum, dl.docdate, dl.podbor, dl.notes, dl.r_notes, dl.r_print, "
                                " dl.provider_id, c.name as provider, "
                                " dl.contragent_id, cc.name as contragent, "
                                " dl.consignee_id, ccc.name as consignee, "
                                " (SELECT count(*) FROM docs_details WHERE doc_id = %1 AND podbor = true) as podbor_count "
                                " FROM docslist dl "
                                " JOIN doctypes dt ON dt.id = dl.doctype_id "
                                " LEFT JOIN contragents c ON c.id = dl.provider_id "
                                " LEFT JOIN contragents cc ON cc.id = dl.contragent_id "
                                " LEFT JOIN contragents ccc ON ccc.id = dl.consignee_id "
                                " WHERE dl.id = %1").arg(id);
    qDebug() << query_str;
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (document)").arg(query.lastError().text()));
        return;
    }
    if(query.size() == 0){
        QMessageBox::warning(this, "Ошибка", QString("Неверный id документа: %1 (document)").arg(id));
        return;
    }
    query.next();
    QSqlRecord rec = query.record();

    this->doc_id = id;
    this->doctype = query.value(rec.indexOf("doctype")).toString();
    this->doctype_id = query.value(rec.indexOf("doctype_id")).toInt();
    this->sum_without_discount = query.value(rec.indexOf("sumprice")).toDouble();
    this->disc_percs = query.value(rec.indexOf("discount_percents")).toDouble();
    this->discount_sum = query.value(rec.indexOf("discount_sum")).toDouble();
    this->old_doc_num = query.value(rec.indexOf("docnum")).toInt();
    this->old_date = query.value(rec.indexOf("docdate")).toDate();
    this->podbor = query.value(rec.indexOf("podbor")).toInt();
    this->settings = get_settings();
    this->settings_section = "DOCUMENT_TABLE";
    this->cstruct = settings->value(QString("%1/%2").arg(USERNAME).arg(settings_section), QVariant()).value<ColumnsStruct>();
    this->all_discounts = false;
    this->all_selected = false;
    this->something_changed = false;
    form_sql_columns();
    this->list_model = list_model;

    // заголовки
    int podbor_count = query.value(rec.indexOf("podbor_count")).toInt();
    headers_lt = new QWidget();
    docnum_label = new QLabel("<b>Документ №</b>", headers_lt);
    date_label = new QLabel("<b>от</b>", headers_lt);
    docnum_edit = new QLineEdit(QString::number(old_doc_num), headers_lt);
    date_edit = new QDateEdit(old_date, headers_lt);
    date_edit->setCalendarPopup(true);
    date_edit->calendarWidget()->setFirstDayOfWeek(Qt::Monday);
    nopodbor_checkbox = new QCheckBox("Не подбирать", headers_lt);
    QFont f = nopodbor_checkbox->font();
    f.setBold(true);
    nopodbor_checkbox->setFont(f);
    if(podbor_count > 0) nopodbor_checkbox->setEnabled(false);
    right_move_button = new QPushButton("Переместить", this);
    right_naklad_button = new QPushButton("Накладная", this);
    right_sf_button = new QPushButton("Счёт-фактура", this);

    // виджеты
    wgt = new QTabWidget(this);
    int provider_id = query.value(rec.indexOf("provider_id")).toInt();
    QString provider = query.value(rec.indexOf("provider")).toString();
    int contragent_id = query.value(rec.indexOf("contragent_id")).toInt();
    QString contragent = query.value(rec.indexOf("contragent")).toString();
    int consignee_id = query.value(rec.indexOf("consignee_id")).toInt();
    QString consignee = query.value(rec.indexOf("consignee")).toString();
    QString notes = query.value(rec.indexOf("notes")).toString();
    QString r_notes = query.value(rec.indexOf("r_notes")).toString();
    bool print_r = query.value(rec.indexOf("r_print")).toBool();
    form = new DocumentForm(stack,
                            prev_wgts,
                            doctype_id,
                            provider_id, provider,
                            contragent_id, contragent,
                            consignee_id, consignee,
                            notes,
                            r_notes,
                            print_r,
                            this);
    table = new BaseTableView(settings_section, false, &cstruct, columns_default_widths, this);
    model = new DocTableModel(columns_ids);
    // подвал
    bottom_wgt = new QWidget(this);
    write_bottom = new QPushButton("Записать", bottom_wgt);
    sf_bottom = new QPushButton("Счёт-фактура", bottom_wgt);
    naklad_bottom = new QPushButton("Накладная", bottom_wgt);
    spis_bottom = new QPushButton("Списать", bottom_wgt);
    wodisc_hlabel = new QLabel("Без скидки:", bottom_wgt);
    disc_hlabel = new QLabel("Скидка:", bottom_wgt);
    wdisc_hlabel = new QLabel("Со скидкой:", bottom_wgt);
    wodisc_label = new QLabel(bottom_wgt);
    wdisc_label = new QLabel(bottom_wgt);
    disc_label = new QLabel(bottom_wgt);
    chdisc_button = new QPushButton("%", bottom_wgt);
    round_box = new QCheckBox("Округлить");
    round_box->setChecked(round);
    nopodbor_checkbox->setChecked(podbor == 5 ? true : false);

    set_bottom_labels(this->sum_without_discount, this->discount_sum, this->disc_percs);
    model->percents = disc_percs;

    buttons_lt = new QHBoxLayout();
    buttons_lt->setSpacing(0);
    buttons_lt->setMargin(0);
    escape_button = create_panel_button(this, buttons_lt, "Закрыть окно", ":back", SLOT(escape_slot()));
    subtract_quantity_button = create_panel_button(this, buttons_lt, "Вычесть количество", ":minus_big", SLOT(subtract_quantity_slot()));
    correct_record_button = create_panel_button(this, buttons_lt, "Корректировать строку", ":edit", SLOT(correct_record_slot()));

    buttons_lt->addStretch(1);

    table->setModel(model);
    RightAlignmentDelegate *d = new RightAlignmentDelegate(table);
    table->setItemDelegateForColumn(columns_ids["price"], d);
    table->setItemDelegateForColumn(columns_ids["discount_price"], d);
    table->setItemDelegateForColumn(columns_ids["sumprice"], d);
    table->setItemDelegateForColumn(columns_ids["num"], d);
    table->setItemDelegateForColumn(columns_ids["grey_id"], d);
    table->setItemDelegateForColumn(columns_ids["weight"], d);
    table->setItemDelegateForColumn(columns_ids["quantity"], d);

    // воплощаем всё в реальность
    set_columns_names();
    set_layout();
    connects();
    int n = fill_table();
    table->set_filled();
    rename_columns();
    table->restore_state();
    table->hideColumn(columns_ids["id"]);
    table->hideColumn(columns_ids["print_string"]);
    table->hideColumn(columns_ids["pure_price"]);
    table->hideColumn(columns_ids["pure_discount_price"]);
    table->hideColumn(columns_ids["trademark_id"]);


}

Document::~Document(){
    settings->setValue(QString("%1/%2").arg(USERNAME).arg(settings_section), QVariant::fromValue<ColumnsStruct>(cstruct));
    delete settings;
}

inline void Document::set_layout(){
    resize();
    // верх
    int f = qApp->font().pointSize();
    nopodbor_checkbox->move(f*2, 3);
    docnum_label->move(f*24, 3);
    docnum_edit->move(docnum_label->x() + docnum_label->width() + 5, 0);
    date_label->move(docnum_edit->x() + f*12, docnum_label->y());
    date_edit->move(date_label->x() + date_label->width() + 5, 0);
    headers_lt->setFixedHeight(f*3);
    headers_lt->setFixedWidth(date_edit->x() + f*16);
    //headers_lt->show();
    QHBoxLayout *h_lt = new QHBoxLayout();
    h_lt->addWidget(headers_lt);
    h_lt->addStretch(1);

    // низ
    QGridLayout *btns_btm_wgt = new QGridLayout();
    btns_btm_wgt->addWidget(write_bottom, 0, 0);
    btns_btm_wgt->addWidget(sf_bottom, 0, 1);
    btns_btm_wgt->addWidget(naklad_bottom, 1, 0);
    btns_btm_wgt->addWidget(spis_bottom, 1, 1);
    QGridLayout *lbls_btm_wgt = new QGridLayout();
    lbls_btm_wgt->addWidget(wodisc_hlabel, 0, 0);
    lbls_btm_wgt->addWidget(wdisc_hlabel, 2, 0);
    lbls_btm_wgt->addWidget(disc_hlabel, 1, 0);
    lbls_btm_wgt->addWidget(wodisc_label, 0, 1);
    lbls_btm_wgt->addWidget(wdisc_label, 2, 1);
    lbls_btm_wgt->addWidget(disc_label, 1, 1);
    lbls_btm_wgt->addWidget(round_box, 2, 2);
    lbls_btm_wgt->addWidget(chdisc_button, 0, 2, 2, 1);
    QHBoxLayout *bottom_wgt_lt = new QHBoxLayout();
    bottom_wgt_lt->addLayout(btns_btm_wgt);
    bottom_wgt_lt->addLayout(lbls_btm_wgt);
    bottom_wgt_lt->addStretch(1);
    bottom_wgt->setLayout(bottom_wgt_lt);

    QWidget *table_wgt = new QWidget(this);
    QVBoxLayout *rw = new QVBoxLayout();
    rw->addWidget(right_move_button);
    rw->addWidget(right_naklad_button);
    rw->addWidget(right_sf_button);
    rw->addStretch(1);
    QHBoxLayout *lw = new QHBoxLayout();
    lw->addWidget(table);
    lw->addLayout(rw);
    table_wgt->setLayout(lw);
    wgt->addTab(form, "Шапка");
    wgt->addTab(table_wgt, "Табличная часть");
    wgt->setCurrentIndex(global_document_tab);

    QVBoxLayout *vlt = new QVBoxLayout(this);
    vlt->setSpacing(0);
    vlt->addLayout(buttons_lt);
    vlt->addLayout(h_lt);
    vlt->addWidget(wgt);
    vlt->addWidget(bottom_wgt);
    vlt->addStretch(1);

    this->setLayout(vlt);
}

inline void Document::resize(){
    int f = qApp->font().pointSize();
    docnum_label->adjustSize();
    docnum_label->setFixedSize(docnum_label->size());
    date_label->adjustSize();
    date_label->setFixedSize(date_label->size());
    docnum_edit->setFixedWidth(f*8);
    date_edit->setFixedWidth(f*12);
    chdisc_button->setFixedSize(f*2.5, f*2.5);
    bottom_wgt->setFixedHeight(f*8);

    wgt->setMaximumWidth(f*100);
    wgt->setFixedHeight(f*44);
    //wgt->adjustSize();
    //wgt->setFixedSize(wgt->size());
    //this->setMinimumHeight(f*80);
}

inline void Document::connects(){
    connect(table->horizontalHeader(), SIGNAL(sectionClicked(int)), SLOT(header_clicked(int)));
    connect(model, SIGNAL(clicked()), SLOT(model_clicked()));
    connect(chdisc_button, SIGNAL(clicked()), SLOT(open_discount_editor()));
    connect(round_box, SIGNAL(clicked(bool)), SLOT(round_sum(bool)));
    connect(form, SIGNAL(something_changed()), SLOT(something_changed_slot()));
    connect(docnum_edit, SIGNAL(textChanged(QString)), SLOT(something_changed_slot()));
    connect(date_edit, SIGNAL(dateChanged(QDate)), SLOT(something_changed_slot()));
    connect(write_bottom, SIGNAL(clicked()), SLOT(write_slot()));
    connect(nopodbor_checkbox, SIGNAL(clicked(bool)), SLOT(switch_podbor(bool)));
    connect(right_move_button, SIGNAL(clicked()), SLOT(move_positions()));
    connect(right_naklad_button, SIGNAL(clicked()), SLOT(naklad_positions()));
    connect(right_sf_button, SIGNAL(clicked()), SLOT(sf_positions()));
    connect(wgt, SIGNAL(currentChanged(int)), SLOT(tab_changed_slot(int)));
    connect(table, SIGNAL(clicked(QModelIndex)), SLOT(selection_changed(QModelIndex)));
}

void Document::tab_changed_slot(int t){
    global_document_tab = t;
}

void Document::subtract_quantity_slot(){

}

void Document::correct_record_slot(){
    if(!table->currentIndex().isValid())
        return;

    int row = table->currentIndex().row();
    int record_id = table->table_data(row, columns_ids["id"]).toInt();
    int trademark_id = table->table_data(row, columns_ids["trademark_id"]).toInt();
    QString trademark = table->table_data(row, columns_ids["name"]).toString();
    QString print_string = table->table_data(row, columns_ids["print_string"]).toString();
    QString storage = table->table_data(row, columns_ids["storage"]).toString();
    QString rack = table->table_data(row, columns_ids["rack"]).toString();
    QString board = table->table_data(row, columns_ids["board"]).toString();
    QString box = table->table_data(row, columns_ids["box"]).toString();
    QString year = table->table_data(row, columns_ids["year"]).toString();
    QString notes = table->table_data(row, columns_ids["notes"]).toString();
    int quantity = table->table_data(row, columns_ids["quantity"]).toInt();
    double price = table->table_data(row, columns_ids["pure_price"]).toDouble();
    double discount_price = table->table_data(row, columns_ids["pure_discount_price"]).toDouble();
    bool is_discount = table->table_data(row, columns_ids["is_discount"], Qt::CheckStateRole) == Qt::Checked ? true : false;
    double weight = table->table_data(row, columns_ids["weight"]).toDouble();

    CorrectDocRecord *cdr = new CorrectDocRecord(record_id,
                                                 trademark_id,
                                                 trademark,
                                                 print_string,
                                                 storage,
                                                 rack,
                                                 board,
                                                 box,
                                                 year,
                                                 notes,
                                                 quantity,
                                                 price,
                                                 discount_price,
                                                 is_discount,
                                                 weight,
                                                 sum_without_discount,
                                                 disc_percs,
                                                 discount_sum,
                                                 this);
    if(cdr->exec() == QDialog::Accepted){
        model->setQuery(model->query().lastQuery(), base);
        emit need_refresh();
        renew_bottom_labels();
    }
}

void Document::selection_changed(QModelIndex i){
    if(!i.isValid())
        return;

    if(count_selected() <= 1){
        // снять выделение
        table->model()->setData(table->model()->index(last_selected_row, columns_ids["select"]), Qt::Unchecked, Qt::CheckStateRole);
        // установить выделение
        table->model()->setData(table->model()->index(i.row(), columns_ids["select"]), Qt::Checked, Qt::CheckStateRole);
        table->setCurrentIndex(i);
    }
}


inline void Document::set_columns_names(){
    columns["id"] = "id";
    columns["num"] = "№";
    columns["grey_id"] = "id серого";
    columns["name"] = "Наименование";
    columns["quantity"] = "Кол-во";
    columns["unit"] = "Ед.";
    columns["price"] = "Цена без ск.";
    columns["is_discount"] = "Ск.";
    columns["discount_price"] = "Цена со ск.";
    columns["sumprice"] = "Сумма";
    columns["notes"] = "Требование";
    columns["year"] = "Год";
    columns["storage"] = "Склад";
    columns["rack"] = "Стеллаж";
    columns["board"] = "Полка";
    columns["box"] = "Ящик";
    columns["category_name"] = "Категория";
    columns["add_info"] = "Доп.пар.1";
    columns["defect"] = "Доп.пар.2";
    columns["weight"] = "Вес";
    columns["select"] = " ";
}

void Document::rename_columns(){
    // переименовываем столбцы, беря названия из columns
    for(int i = 0; i < model->columnCount(); i++){
        if(columns[model->headerData(i, Qt::Horizontal).toString()] != "")
            model->setHeaderData(i, Qt::Horizontal, columns[model->headerData(i, Qt::Horizontal).toString()]);
    }
}

inline void Document::form_sql_columns(){
    int i = 0;
    int fontsize = qApp->font().pointSize();

    columns_sql << "dd.selected as select";
    columns_default_widths << fontsize * 4;
    columns_ids["select"] = i++;

    columns_sql << "dd.id as id";
    columns_default_widths << fontsize * 5;
    columns_ids["id"] = i++;

    columns_sql << "dd.num as num";
    columns_default_widths << fontsize * 5;
    columns_ids["num"] = i++;

    columns_sql << "dd.grey_id as grey_id";
    columns_default_widths << fontsize * 8;
    columns_ids["grey_id"] = i++;

    columns_sql << "t.name as name";
    columns_default_widths << fontsize * 20;
    columns_ids["name"] = i++;

    columns_sql << "dd.quantity as quantity";
    columns_default_widths << fontsize * 8;
    columns_ids["quantity"] = i++;

    columns_sql << "un.unit_name as unit";
    columns_default_widths << fontsize * 5;
    columns_ids["unit"] = i++;

    columns_sql << "dd.price as price";
    columns_default_widths << fontsize * 10;
    columns_ids["price"] = i++;

    columns_sql << "dd.is_discount as is_discount";
    columns_default_widths << fontsize * 8;
    columns_ids["is_discount"] = i++;

    columns_sql << "dd.discount_price as discount_price";
    columns_default_widths << fontsize * 10;
    columns_ids["discount_price"] = i++;

    columns_sql << "dd.discount_price*dd.quantity as sumprice";
    columns_default_widths << fontsize * 10;
    columns_ids["sumprice"] = i++;

    columns_sql << "dd.notes as notes";
    columns_default_widths << fontsize * 20;
    columns_ids["notes"] = i++;

    columns_sql << "g.year as year";
    columns_default_widths << fontsize * 8;
    columns_ids["year"] = i++;

    columns_sql << "p.storage as storage";
    columns_default_widths << fontsize * 8;
    columns_ids["storage"] = i++;

    columns_sql << "p.rack as rack";
    columns_default_widths << fontsize * 8;
    columns_ids["rack"] = i++;

    columns_sql << "p.board as board";
    columns_default_widths << fontsize * 8;
    columns_ids["board"] = i++;

    columns_sql << "p.box as box";
    columns_default_widths << fontsize * 8;
    columns_ids["box"] = i++;

    columns_sql << "c.category_name as category_name";
    columns_default_widths << fontsize * 20;
    columns_ids["category_name"] = i++;

    columns_sql << "g.add_info as add_info";
    columns_default_widths << fontsize * 20;
    columns_ids["add_info"] = i++;

    columns_sql << "g.defect as defect";
    columns_default_widths << fontsize * 20;
    columns_ids["defect"] = i++;

    columns_sql << "t.id as trademark_id";
    columns_default_widths << fontsize * 20;
    columns_ids["trademark_id"] = i++;

    columns_sql << "t.weight as weight";
    columns_default_widths << fontsize * 20;
    columns_ids["weight"] = i++;

    columns_sql << "dd.print_string as print_string";
    columns_default_widths << fontsize * 20;
    columns_ids["print_string"] = i++;

    columns_sql << "dd.price as pure_price";
    columns_default_widths << fontsize * 10;
    columns_ids["pure_price"] = i++;

    columns_sql << "dd.discount_price as pure_discount_price";
    columns_default_widths << fontsize * 10;
    columns_ids["pure_discount_price"] = i++;


    sql_from = columns_sql.join(", ");
    sql_tables = "docs_details dd "
            " JOIN greytable g ON dd.grey_id = g.id "
            " JOIN trademarks t ON g.trademark_id = t.id "
            " JOIN places p ON g.place_id = p.id "
            " JOIN categories c ON g.category_id = c.id "
            " JOIN units un ON t.unit_id = un.id ";
}

int Document::fill_table(){
    QString query_str = QString("SELECT %1 FROM %2 WHERE dd.doc_id = %3")
            .arg(sql_from)
            .arg(sql_tables)
            .arg(this->doc_id);
    query_str += " ORDER BY num";
    //qDebug() << query_str;
    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        qDebug() << model->lastError().text();
        return -1;
    }

    return model->rowCount();
}

bool Document::write_changes(){
    /* Изменения могут быть следующими:
       В форме:
        1-3) provider_id, contragent_id, consignee_id
        4-5) notes
       Иное:
        6) номер документа
        7) дата документа
       Для пунктов 1-3: если значение соответствующей переменной 0, то записываем в базу NULL,
            иначе - это самое значение.
       4-5: тупо переписываем значения.
       6: проверяем на уникальность в течение года
       7 - blahblahblah

       Как обычно, заводим qmap changed_params, куда будем складывать изменённые параметры
    */
    QMap<QString, QString> changed_params;
    QSqlQuery query(base);
    QString query_str;
    // Первоочерёдно проверяем уникальность номеров! Если они не уникальны, остальное сохранять и не нужно.
    // Проверяем, изменены ли они по сравнению с первоначальным. Если изменены, выполняем проверку.
    if(old_doc_num != docnum_edit->text().toInt()){
        query_str = QString("SELECT * FROM docslist WHERE year = %1 AND docnum = %2 AND doctype_id = %3")
                .arg(QDate::currentDate().year())
                .arg(docnum_edit->text().toInt())
                .arg(this->doctype_id);
        qDebug() << query_str;
        if(!query.exec(query_str)){
            QMessageBox::warning(this, "Ошибка", QString("Невозможно выполнить запрос: %1").arg(query.lastError().text()), QMessageBox::Ok);
            return false;
        }
        if(query.size() > 0){
            QMessageBox::warning(this, "Ошибка", "Номер не уникален", QMessageBox::Ok);
            return false;
        }
        qDebug("Номер уникален");
        changed_params["docnum"] = QString::number(docnum_edit->text().toInt());
    }
    // Далее проверяем id контрагентов. Если они нули, то подставляем NULL в базу
    if(form->old_provider_id != form->provider_id)
        changed_params["provider_id"] = form->provider_id == 0 ? "NULL" : QString::number(form->provider_id);
    if(form->old_provider_id != form->provider_id)
        changed_params["contragent_id"] = form->contragent_id == 0 ? "NULL" : QString::number(form->contragent_id);
    if(form->old_consignee_id != form->consignee_id)
        changed_params["consignee_id"] = form->consignee_id == 0 ? "NULL" : QString::number(form->consignee_id);

    // с notes и r_id всё просто
    if(form->old_notes != form->notes)
        changed_params["notes"] = form->notes;
    //if(form->old_r_id != form->r_id)
        //changed_params["r_id"] = form->r_id == 0 ? "NULL" : QString::number(form->r_id);

    // с р-строкой теперь всё не так
    // если такая строка содержится в базе, то записываем её id
    // если не содержится - то добавляем (с типом 2)
    QString s = form->r_string();
    if(s.length() == 0)
        changed_params["r_notes"] = "";
    else{
        query_str = QString("SELECT id FROM r_strings WHERE string = '%1'").arg(s);
        if(!query.exec(query_str)){
            qDebug() << "Не удалось выполнить запрос: " << query.lastError().text();
            return false;
        }
        if(query.size() == 0){
            query_str = QString("INSERT INTO r_strings (string, pattern, type) "
                                "VALUES ('%1', '%2', 2) RETURNING id")
                    .arg(s).arg(create_pattern(s));
            if(!query.exec(query_str)){
                qDebug() << "Не удалось выполнить запрос: " << query.lastError().text();
                return false;
            }
            query.next();
            qDebug() << "Строка вставлена: " << s << "id " << query.value(0).toString();
        }
        changed_params["r_notes"] = s;
    }

    // с датой, в общем-то, тоже
    if(old_date != date_edit->date())
        changed_params["docdate"] = date_edit->date().toString("yyyy-MM-dd");

    // DONE
    // склеиваем всё в строку
    query_str = QString("UPDATE docslist SET ");
    QMapIterator<QString,QString> it(changed_params);
    QStringList values;
    while(it.hasNext()){
        it.next();
        if(it.key() == "provider_id"
                || it.key() == "contragent_id"
                || it.key() == "consignee_id")
            values << QString("%1 = %2").arg(it.key()).arg(it.value());
        else
            values << QString("%1 = '%2'").arg(it.key()).arg(it.value());
    }
    query_str = QString("UPDATE docslist SET %1 WHERE id = %2")
            .arg(values.join(", "))
            .arg(this->doc_id);
    qDebug() << query_str;

    // записываем в транзакции
    if(!base.transaction()){
        QMessageBox::warning(this, "Ошибка", QString("Невозможно начать транзакцию: %1").arg(query.lastError().text()), QMessageBox::Ok);
        return false;
    }
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Невозможно выполнить запрос: %1").arg(query.lastError().text()), QMessageBox::Ok);
        base.rollback();
        return false;
    }
    // ИСТОРИЯ!!!!!!!!!!!!!!!!!!!!!!!!!!


    base.commit();

    old_doc_num = docnum_edit->text().toInt();
    form->old_provider_id = form->provider_id;
    form->old_provider_id = form->provider_id;
    form->old_consignee_id = form->consignee_id;
    form->old_notes = form->notes;
    old_date = date_edit->date();

    emit need_refresh();
    return true;
}

void Document::escape_slot(){
    write_slot();
    emit delete_me();
    this->close();
}

bool Document::write_slot(){
    if(something_changed){
        something_changed = false;

        if(QMessageBox::question(this, "Сохранить?", "Сохранить изменения?", QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
            if(!write_changes()) return false;
        }
        form->fill_r_box();
        something_changed = false;
    }
    return true;
}

void Document::header_clicked(int index){
    if(index != columns_ids["is_discount"] && index != columns_ids["select"])
        return;

    QString query_str;
    if(index == columns_ids["is_discount"]){
        if(!all_discounts){
            all_discounts = true;
            // выделяем всё
            query_str = QString("UPDATE docs_details SET is_discount = true WHERE doc_id = %1").arg(doc_id);
        }
        else{
            all_discounts = false;
            // наоборот, снимаем
            query_str = QString("UPDATE docs_details SET is_discount = false WHERE doc_id = %1").arg(doc_id);
        }
    }
    else if(index == columns_ids["select"]){
        if(!all_selected){
            all_selected = true;
            // выделяем всё
            query_str = QString("UPDATE docs_details SET selected = true WHERE doc_id = %1").arg(doc_id);
        }
        else{
            all_selected = false;
            // наоборот, снимаем
            query_str = QString("UPDATE docs_details SET selected = false WHERE doc_id = %1").arg(doc_id);
        }
    }
    QSqlQuery query(base);
    if(!query.exec(query_str)){
        qDebug() << "Header: " << query.lastError().text();
        return;
    }
    model->setQuery(model->query().lastQuery(), base);
}

void Document::model_clicked(){
    all_discounts = false;
    // надо перезаполнить
    model->setQuery(model->query().lastQuery(), base);
    emit need_refresh();
    // переЩЩитываем сумму без скидки, чтобы посчитать размер скидки и перезаполнить лейблы снизу
    QSqlQuery query(base);
    if(!query.exec(QString("SELECT discount_sum FROM docslist WHERE id = %1").arg(this->doc_id))){
        qDebug() << query.lastError().text();
        return;
    }
    query.next();
    discount_sum = query.value(0).toDouble();
    wdisc_label->setText(QString("<b>%1 р.</b>").arg(QString::number(discount_sum, 'f', 2)));
    disc_label->setText(QString("<b>%1 \% (%2 р.)</b>").arg(model->percents).arg(QString::number(sum_without_discount-discount_sum, 'f', 2)));
}

void Document::open_discount_editor(){
    // текущая скидка в процентах, текущая сумма скидки, текущая сумма заказа

    EditOrderDiscount *eod = new EditOrderDiscount(disc_percs, sum_without_discount - discount_sum, discount_sum, this);
    connect(eod, SIGNAL(recount(double)), SLOT(recount_discount(double)));
    connect(this, SIGNAL(renew_prices(double,double)), eod, SLOT(renew_labels(double,double)));
    eod->exec();
}

void Document::recount_discount(double d){
    if(!base.transaction()){
        qDebug() << "fail to begin transaction: " << base.lastError().text();
        return;
    }
    double total_discount = 0;
    // пройтись по каждой позиции в таблице, где отмечена скидка - посчитать новую цену, записать её (колонка discount_value),
    // посчитать получившийся размер скидки, приплюсовать к discount_value
    QSqlQuery query(base);
    QString query_str;
    double baseprice, newprice;
    int rec_id;
    QString tmp;
    for(int i = 0; i < model->rowCount(); i++){
        if(model->data(model->index(i, columns_ids["is_discount"]), Qt::CheckStateRole) == Qt::Checked){
            tmp = model->data(model->index(i, columns_ids["price"])).toString();
            baseprice = tmp.left(tmp.indexOf(" ")).toDouble();
            // округлить до копейки
            newprice = baseprice * ((100-d)/100.0);
            newprice = floor((newprice+0.005)*100.0) / 100.0;
            total_discount += (baseprice-newprice) * model->data(model->index(i, columns_ids["quantity"])).toInt();
            // ищем id
            rec_id = model->data(model->index(i, columns_ids["id"])).toInt();
            query_str = QString("UPDATE docs_details SET discount_price = %1 WHERE id = %2").arg(newprice).arg(rec_id);
            if(!query.exec(query_str)){
                QMessageBox::warning(this, "Не удалось обновить скидку", query.lastError().text(), QMessageBox::Ok);
                base.rollback();
                return;
            }
        }
    }

    // перезаполнить таблицу
    model->setQuery(model->query().lastQuery(), base);
    // в docslist записать discount_percents (передано) и discount_sum (посчитано)
    query_str = QString("UPDATE docslist SET discount_percents = '%1', discount_sum = '%2' WHERE id = %3")
            .arg(d)
            .arg(sum_without_discount - total_discount)
            .arg(this->doc_id);
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Не удалось обновить скидку", query.lastError().text(), QMessageBox::Ok);
        base.rollback();
        return;
    }

    if(!base.commit()){
        qDebug() << "fail to begin transaction: " << base.lastError().text();
        base.rollback();
        return;
    }

    this->discount_sum = sum_without_discount - total_discount;
    this->disc_percs = d;
    model->percents = d;
    // обновить, соответственно, суммы внизу окна
    set_bottom_labels(sum_without_discount, discount_sum, disc_percs);

    emit renew_prices(disc_percs, total_discount);
    emit need_refresh();
}

void Document::set_bottom_labels(double sum_without_discount, double discount_sum, double discount_percs){
    wodisc_label->setText(QString("<b>%1 р.</b>").arg(QString::number(sum_without_discount, 'f', 2)));
    wdisc_label->setText(QString("<b>%1 р.</b>").arg(QString::number(discount_sum, 'f', 2)));
    disc_label->setText(QString("<b>%1 \% (%2 р.)</b>").arg(discount_percs).arg(QString::number(sum_without_discount-discount_sum, 'f', 2)));
}

void Document::round_sum(bool round){
    // обновляем round и цену в базе
    // существующая цена - discount_sum
    double newprice;
    QSqlQuery query(base);
    QString query_str;
    if(round)
        newprice = ceil(this->discount_sum);
    else{
        // заново подсчитываем сумму
        query_str = QString("SELECT sum(discount_price*quantity) FROM docs_details WHERE doc_id = %1").arg(this->doc_id);
        if(!query.exec(query_str)){
            qDebug() << "Ошибка: " << query.lastError().text();
            return;
        }
        query.next();
        newprice = query.value(0).toDouble();
    }
    this->discount_sum = newprice;
    query_str = QString("UPDATE docslist SET round = %1, discount_sum = %2 WHERE id = %3")
            .arg(round ? "true" : "false")
            .arg(newprice)
            .arg(doc_id);
    if(!query.exec(query_str)){
        qDebug() << "Ошибка: " << query.lastError().text();
        return;
    }
    set_bottom_labels(sum_without_discount, discount_sum, disc_percs);
    emit need_refresh();
}

void Document::switch_podbor(bool s){
    QSqlQuery query(base);
    QString query_str = QString();
    if(s){
        // если галка поставлена, т.е. мы запрещаем подбор, проверим, точно ли не подобрана ни одна деталь из заказа
        query_str = QString("SELECT count(*) FROM docs_details WHERE doc_id = %1 AND podbor = true").arg(doc_id);
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (switch_podbor1)").arg(query.lastError().text()), QMessageBox::Ok);
            nopodbor_checkbox->setChecked(false);
            return;
        }
        if(query.size() == 0){
            QMessageBox::information(this, "?", "Неведомая ошибка", "OK");
            nopodbor_checkbox->setChecked(false);
            return;
        }
        query.next();
        int count = query.value(0).toInt();
        if(count > 0){
            QMessageBox::information(this, "Ошибка", "Заявка частично подобрана!", "OK");
            nopodbor_checkbox->setChecked(false);
            return;
        }
        // если всё нормально, то для этого документа ставим podbor = 5, предварительно спросив об этом
        if(QMessageBox::question(this, "Запретить подбор?", "Запретить подбор данного документа?", "Да", "Нет") != 0){
            nopodbor_checkbox->setChecked(false);
            return;
        }
        query_str = QString("UPDATE docslist SET podbor = 5 WHERE id = %1").arg(this->doc_id);
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (switch_podbor2)").arg(query.lastError().text()), QMessageBox::Ok);
            nopodbor_checkbox->setChecked(false);
            return;
        }
    }
    else{
        // если галка снимается, то ставим podbor = 1
        if(QMessageBox::question(this, "Разрешить подбор?", "Разрешить подбор данного документа?", "Да", "Нет") != 0){
            nopodbor_checkbox->setChecked(true);
            return;
        }
        query_str = QString("UPDATE docslist SET podbor = 1 WHERE id = %1").arg(this->doc_id);
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (switch_podbor2)").arg(query.lastError().text()), QMessageBox::Ok);
            nopodbor_checkbox->setChecked(true);
            return;
        }
    }

    // ИСТОРИЯ!!!!!!!!!!!!
    emit need_refresh();
}

void Document::something_changed_slot(){
    something_changed = true;
}

int Document::count_selected(){
    int n = 0;
    for(int i = 0; i < table->model()->rowCount(); i++){
        if(model->data(model->index(i, columns_ids["select"]), Qt::CheckStateRole) == Qt::Checked){
            n++;
            last_selected_row = i;
        }
    }
    return n;
}

void Document::move_positions(){
    /* Расклад такой:
       - если не выделено ни одной позиции - предлагаем переместить все
       - если выделена одна позиция - предлагаем ввести количество (спинбокс, значение по умолчанию - полное кол-во)
       - если выделено несколько позиций - переносим их все
    */
    temp_quantity = 0;
    select_all = false;
    last_selected_row = -1;
    int n = count_selected();

    if(n == 0){
        if(QMessageBox::question(this, "Переместить все?", "Переместить все позиции?", "Да", "Нет") == 0)
            select_all = true;
        else return;
    }
    else if(n == 1){
        if(last_selected_row < 0)
            return;

        temp_quantity = table->table_data(last_selected_row, columns_ids["quantity"]).toInt();
        QDialog *dlg = new QDialog(this);
        dlg->setWindowTitle("Введите кол-во");
        QSpinBox *spin = new QSpinBox(dlg);
        spin->setMaximum(temp_quantity);
        QPushButton *ok_button = new QPushButton("ОК", dlg);
        QPushButton *cancel_button = new QPushButton("Отмена", dlg);
        QHBoxLayout *hlt = new QHBoxLayout();
        QVBoxLayout *vlt = new QVBoxLayout();
        hlt->addWidget(ok_button);
        hlt->addWidget(cancel_button);
        vlt->addWidget(spin);
        vlt->addLayout(hlt);
        dlg->setLayout(vlt);
        connect(ok_button, SIGNAL(clicked()), dlg, SLOT(accept()));
        connect(cancel_button, SIGNAL(clicked()), dlg, SLOT(reject()));
        connect(spin, SIGNAL(valueChanged(int)), this, SLOT(change_temp_quantity(int)));
        spin->setValue(temp_quantity);
        spin->setFocus();
        spin->selectAll();

        if(dlg->exec() == QDialog::Rejected)
            return;
    }

    emit create_move_dialog(this);
}

void Document::change_temp_quantity(int q){
    this->temp_quantity = q;
}

void Document::move_to_doc(int tdoc_id){
    QSqlQuery query(base);
    QString query_str;
    bool onestring = temp_quantity > 0 ? true : false;
    // сохраняем в булевой переменной, просмотрен ли документ (если побирать не нужно - также false)
    query_str = QString("SELECT podbor, discount_percents, (SELECT max(num) FROM docs_details WHERE doc_id = %1) FROM docslist WHERE id = %1").arg(tdoc_id);
    if(!query.exec(query_str)){
        QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_1)").arg(query.lastError().text()), "ОК");
        return;
    }
    if(query.size() == 0){
        QMessageBox::information(this, "Ошибка", QString("Документ с id = %1 не найден.)").arg(tdoc_id), "ОК");
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
    for(int j = 0; j < table->model()->rowCount(); j++){
        QModelIndex i = table->model()->index(j, 0);
        if(!select_all && table->table_data(i.row(), columns_ids["select"], Qt::CheckStateRole) == Qt::Unchecked)
            continue;
        /* Изменяем у документа doc_id, добавляя podbor=false
           Предварительно проверить: если документ не просмотрен (viewed == false) и там присутствует деталь с таким grey_id, склеить количество
           После всего этого удалить из reserve строку с id = rec_id
        */
        int rec_id = table->table_data(i.row(),columns_ids["id"]).toInt();
        int grey_id = table->table_data(i.row(),columns_ids["grey_id"]).toInt();
        int quantity = onestring ? temp_quantity : table->table_data(i.row(),columns_ids["quantity"]).toInt();
        QString tmp = table->table_data(i.row(),columns_ids["price"]).toString();
        double price = tmp.left(tmp.indexOf(" ")).toDouble();
        bool is_discount = table->table_data(i.row(),columns_ids["is_discount"]).toBool();
        QString notes = table->table_data(i.row(),columns_ids["notes"]).toString();

        // если temp_quantity таки больше нуля, надо вычесть количество из старой строчки и создать новую
        // а если в старой осталось ноль, удалить её к фигам
        if(onestring){
            query_str = QString("UPDATE docs_details SET quantity = quantity - %1 WHERE id = %2 RETURNING quantity")
                    .arg(quantity).arg(rec_id);
            qDebug() << "first" << query_str;
            if(!query.exec(query_str)){
                QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_1)").arg(query.lastError().text()), "ОК");
                base.rollback();
                return;
            }
            if(query.size() == 0){
                QMessageBox::information(this, "Ошибка", QString("Странная ошибка (move_to_doc_2)"), "ОК");
                base.rollback();
                return;
            }
            query.next();
            int q = query.value(0).toInt();
            if(q == 0){
                // удаляем
                query_str = QString("DELETE FROM docs_details WHERE id = %1").arg(rec_id);
                if(!query.exec(query_str)){
                    QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_3)").arg(query.lastError().text()), "ОК");
                    base.rollback();
                    return;
                }
            }
        }

        // делаем ту самую проверку
        if(!viewed){
            query_str = QString("SELECT id FROM docs_details WHERE doc_id = %1 AND grey_id = %2").arg(tdoc_id).arg(grey_id);
            if(!query.exec(query_str)){
                QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_4)").arg(query.lastError().text()), "ОК");
                base.rollback();
                return;
            }
            if(query.size() > 0){
                query.next();
                int t_id = query.value(0).toInt();
                query_str = QString("UPDATE docs_details SET quantity = quantity + %1 WHERE id = %2").arg(quantity).arg(t_id);
                if(!query.exec(query_str)){
                    QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_5)").arg(query.lastError().text()), "ОК");
                    base.rollback();
                    return;
                }
                qDebug() << table->table_data(i.row(),2).toString() << "Кол-во суммировано";
                onestring = false;
                continue;
            }
            // если записи с таким grey_id не нашлось, выходим из блока и вставляем новую строку
        }
        if(onestring){
            query_str = QString("INSERT INTO docs_details (doc_id, grey_id, quantity, price, notes, is_discount, discount_price, num, podbor) "
                                " VALUES (%1, %2, %3, %4, '%5', %6, %7, %8, false)")
                    .arg(tdoc_id)
                    .arg(grey_id)
                    .arg(quantity)
                    .arg(price)
                    .arg(notes)
                    .arg(is_discount ? "true" : "false")
                    .arg(is_discount ? price*(100-discount)/100 : price)
                    .arg(++maxnum);
            qDebug() << "second" << query_str;
            if(!query.exec(query_str)){
                QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_6)").arg(query.lastError().text()), "ОК");
                base.rollback();
                return;
            }
        }
        else{
            // делаем апд
            query_str = QString("UPDATE docs_details SET doc_id = %1, discount_price = %2, num = %3, podbor = false WHERE id = %4")
                    .arg(tdoc_id)
                    .arg(is_discount ? price*(100-discount)/100 : price)
                    .arg(++maxnum)
                    .arg(rec_id);
            if(!query.exec(query_str)){
                QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_doc_7)").arg(query.lastError().text()), "ОК");
                base.rollback();
                return;
            }
        }
    }

    if(!base.commit()){
        QMessageBox::information(this, "Ошибка", QString("Не удаётся завершить транзакцию: %1 (move_to_doc)").arg(base.lastError().text()), "ОК");
        base.rollback();
        return;
    }
    temp_quantity = 0;
    QSqlQueryModel *m = (QSqlQueryModel*)table->model();
    m->setQuery(m->query().lastQuery(), base);
    emit need_refresh();
    // обновить лейблы снизу
    renew_bottom_labels();
}

void Document::move_to_reserve(int id){
    int docnum = id+1;
    QSqlQuery query(base);
    QString query_str;
    bool onestring = temp_quantity > 0 ? true : false;
    if(!base.transaction()){
        QMessageBox::information(this, "Ошибка", QString("Не удаётся начать транзакцию: %1 (move_to_reserve)").arg(base.lastError().text()), "ОК");
        return;
    }

    // проходимся по всем строкам, ища среди них выделенные
    // если select_all == true, то перемещаем всё подряд
    for(int j = 0; j < table->model()->rowCount(); j++){
        QModelIndex i = table->model()->index(j, 0);
        if(!select_all && table->table_data(i.row(), columns_ids["select"], Qt::CheckStateRole) == Qt::Unchecked)
            continue;

        int rec_id = table->table_data(i.row(),columns_ids["id"]).toInt();
        int grey_id = table->table_data(i.row(),columns_ids["grey_id"]).toInt();
        int quantity = onestring ? temp_quantity : table->table_data(i.row(),columns_ids["quantity"]).toInt();
        QString tmp = table->table_data(i.row(),columns_ids["price"]).toString();
        double price = tmp.left(tmp.indexOf(" ")).toDouble();
        QString notes = table->table_data(i.row(),columns_ids["notes"]).toString();

        // если temp_quantity таки больше нуля, надо вычесть количество из старой строчки и создать новую
        // а если в старой осталось ноль, удалить её к фигам
        if(onestring){
            query_str = QString("UPDATE docs_details SET quantity = quantity - %1 WHERE id = %2 RETURNING quantity")
                    .arg(quantity).arg(rec_id);
            if(!query.exec(query_str)){
                QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_1)").arg(query.lastError().text()), "ОК");
                base.rollback();
                return;
            }
            if(query.size() == 0){
                QMessageBox::information(this, "Ошибка", QString("Странная ошибка (move_to_reserve_2)"), "ОК");
                base.rollback();
                return;
            }
            query.next();
            int q = query.value(0).toInt();
            if(q == 0){
                // удаляем
                query_str = QString("DELETE FROM docs_details WHERE id = %1").arg(rec_id);
                if(!query.exec(query_str)){
                    QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_3)").arg(query.lastError().text()), "ОК");
                    base.rollback();
                    return;
                }
            }
        }
        else{
            query_str = QString("DELETE FROM docs_details WHERE id = %1").arg(rec_id);
            if(!query.exec(query_str)){
                QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_4)").arg(query.lastError().text()), "ОК");
                base.rollback();
                return;
            }
        }

        // проверить, есть в нужном документе позиция с таким же grey_id, и если есть - склеить кол-во
        query_str = QString("SELECT id FROM reserve WHERE grey_id = %1 AND doc_id = (SELECT id FROM reserve_docs WHERE user_id = %2 AND doc_num = %3)")
                .arg(grey_id)
                .arg(USER_ID)
                .arg(docnum);
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_5)").arg(query.lastError().text()), "ОК");
            base.rollback();
            return;
        }
        if(query.size() > 0){
            query.next();
            int t_id = query.value(0).toInt();

            query_str = QString("UPDATE reserve SET quantity = quantity + %1 WHERE id = %2").arg(quantity).arg(t_id);
            if(!query.exec(query_str)){
                QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_6)").arg(query.lastError().text()), "ОК");
                base.rollback();
                return;
            }
            qDebug() << table->table_data(i.row(),2).toString() << "Кол-во суммировано";
            continue;
        }
        // если мы до сюда добрались - такой позиции нет, тогда вставляем новую
        // сначала сохраним doc_id, так будет проще
        query_str = QString("SELECT id FROM reserve_docs WHERE doc_num = %1 AND user_id = %2").arg(docnum).arg(USER_ID);
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_7)").arg(query.lastError().text()), "ОК");
            base.rollback();
            return;
        }
        query.next();
        int t_id = query.value(0).toInt();

        query_str = QString("INSERT INTO reserve (doc_id, grey_id, quantity, note, price, num) "
                            " VALUES (%1, %2, %3, '%4', %5, (SELECT max(num) FROM reserve WHERE doc_id = %1)+1)")
                .arg(t_id).arg(grey_id).arg(quantity).arg(notes).arg(price);
        if(!query.exec(query_str)){
            QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (move_to_reserve_8)").arg(query.lastError().text()), "ОК");
            base.rollback();
            return;
        }
    }

    if(!base.commit()){
        QMessageBox::information(this, "Ошибка", QString("Не удаётся завершить транзакцию: %1 (move_to_reserve)").arg(base.lastError().text()), "ОК");
        base.rollback();
        return;
    }
    temp_quantity = 0;
    QSqlQueryModel *m = (QSqlQueryModel*)table->model();
    m->setQuery(m->query().lastQuery(), base);
    emit set_reserve_doc_color(id);
    emit need_refresh();
    renew_bottom_labels();
}

void Document::renew_bottom_labels(){
    QSqlQuery query(base);
    QString query_str;
    query_str = QString("SELECT sumprice, discount_percents, discount_sum, round FROM docslist WHERE id = %1").arg(doc_id);
    if(!query.exec(query_str)){
        QMessageBox::information(this, "Ошибка", QString("Не удалось выполнить запрос: %1 (renew_bottom_labels)").arg(query.lastError().text()), "ОК");
        return;
    }
    if(query.size() == 0){
        QMessageBox::information(this, "Ошибка", QString("Документ с id = %1 не найден.)").arg(doc_id), "ОК");
        return;
    }
    query.next();
    bool round = query.value(3).toBool();
    sum_without_discount = query.value(0).toDouble();
    disc_percs = query.value(1).toDouble();
    discount_sum = query.value(2).toDouble();
    if(round) discount_sum = ceil(discount_sum);
    set_bottom_labels(sum_without_discount, discount_sum, disc_percs);
    model->percents = disc_percs;

}

void Document::naklad_positions(){

}

void Document::sf_positions(){

}



DocTableModel::DocTableModel(QMap<QString,int> ids, QWidget *parent) : QSqlQueryModel(parent){
    this->ids = ids;
    percents = 0;
}

Qt::ItemFlags DocTableModel::flags(const QModelIndex &index) const{
    if(index.column() == ids["is_discount"] || index.column() == ids["select"])
        return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
    return QSqlQueryModel::flags(index);
}

QVariant DocTableModel::data(const QModelIndex &index, int role) const{

    // цену отображаем в виде двух знаков после запятой
    if((index.column() == ids["sumprice"]
        || index.column() == ids["price"]
        || index.column() == ids["discount_price"])
            && role == Qt::DisplayRole)
        return QString("%1").arg(QString::number(QSqlQueryModel::data(index, role).toDouble(), 'f', 2));

    // чекбоксы
    if(index.column() == ids["is_discount"] || index.column() == ids["select"]){
        if(role == Qt::CheckStateRole)
            return QSqlQueryModel::data(index, Qt::DisplayRole).toBool() ? Qt::Checked : Qt::Unchecked;
        if(role == Qt::DisplayRole)
            return QVariant();
    }
    return QSqlQueryModel::data(index, role);
}

bool DocTableModel::setData(const QModelIndex &index, const QVariant &value, int role){
    if(index.column() == ids["is_discount"] && role == Qt::CheckStateRole){
        QSqlQuery query(base);
        QModelIndex id_index = QSqlQueryModel::index(index.row(), ids["id"]);
        // надо посчитать новую цену
        // если галка снята, то discount_price = price
        // иначе считаем и округляем до копейки
        double baseprice = QSqlQueryModel::data(QSqlQueryModel::index(index.row(), ids["price"])).toDouble();
        // нет времени объяснять, оно работает
        double newprice = value.toBool() ?  floor(((baseprice * ((100-percents)/100.0))+0.005)*100.0) / 100.0 : baseprice;
        QString query_str = QString("UPDATE docs_details SET is_discount = %1, discount_price = %2 WHERE id = %3")
                .arg(value.toBool() ? "true" : "false")
                .arg(newprice)
                .arg(QSqlQueryModel::data(id_index).toInt());
        if(!query.exec(query_str)){
            qDebug() << query.lastError().text();
            return false;
        }

        setQuery(this->query().lastQuery(), base);
        emit clicked();
        return true;
    }
    if(index.column() == ids["select"] && role == Qt::CheckStateRole){
        QSqlQuery query(base);
        QModelIndex id_index = QSqlQueryModel::index(index.row(), ids["id"]);
        QString query_str = QString("UPDATE docs_details SET selected = %1 WHERE id = %2")
                .arg(value.toBool() ? "true" : "false")
                .arg(QSqlQueryModel::data(id_index).toInt());
        if(!query.exec(query_str)){
            qDebug() << query.lastError().text();
            return false;
        }

        setQuery(this->query().lastQuery(), base);
        return true;
    }
}



EditOrderDiscount::EditOrderDiscount(double cur_disc_perc, double cur_disc_sum, double order_sum, QWidget *parent)
    : QDialog(parent){
    //while(!base.transaction());
    this->setWindowTitle("Скидка");
    this->current_percents = cur_disc_perc;
    this->current_discount_sum = cur_disc_sum;
    this->order_sum = order_sum;
    // создаём
    percents_edit = new QLineEdit();
    count = new QPushButton("!");
    ok_button = new QPushButton("Записать");
    cancel_button = new QPushButton("Отмена");
    percs_label = new QLabel("%");
    fact_name_label = new QLabel("Скидка:");
    value_name_label = new QLabel("Размер:");
    fact_label = new QLabel();
    value_label = new QLabel();
    count->setToolTip("Пересчитать");

    percents_edit->setText(QString::number(current_percents));
    percents_edit->setAlignment(Qt::AlignRight);
    renew_labels(current_percents, current_discount_sum);

    QDoubleValidator* double_validator = new QDoubleValidator(this);
    percents_edit->setValidator(double_validator);
    percents_edit->selectAll();

    set_layout();
    connect(count, SIGNAL(clicked()), SLOT(count_slot()));
    connect(ok_button, SIGNAL(clicked()), SLOT(ok_clicked()));
    connect(cancel_button, SIGNAL(clicked()), SLOT(cancel_clicked()));
}

inline void EditOrderDiscount::set_layout(){
    resize();

    // текстовое поле, символ процента и кнопка - горизонтально
    QHBoxLayout *hlt = new QHBoxLayout();
    hlt->addWidget(percents_edit);
    hlt->addWidget(percs_label);
    hlt->addWidget(count);

    // лейблы - гридом, ибо так красивше
    QGridLayout *mlt = new QGridLayout();
    mlt->addWidget(fact_name_label, 0, 0);
    mlt->addWidget(value_name_label, 1, 0);
    mlt->addWidget(fact_label, 0, 1);
    mlt->addWidget(value_label, 1, 1);

    // ок, отмена - горизонтально
    QHBoxLayout *bhlt = new QHBoxLayout();
    bhlt->addWidget(ok_button);
    bhlt->addWidget(cancel_button);

    // всё - вертикально
    QVBoxLayout *lt = new QVBoxLayout(this);
    lt->addLayout(hlt);
    lt->addLayout(mlt);
    lt->addLayout(bhlt);

    this->setLayout(lt);
}

void EditOrderDiscount::resize(){
    int q = qApp->font().pointSize();
    percents_edit->setFixedSize(q*5, q*2.5);
    count->setFixedSize(q*2.5, q*2.5);
    ok_button->setFixedSize(q*8, q*3);
    cancel_button->setFixedSize(ok_button->size());
    percs_label->adjustSize();
    fact_name_label->adjustSize();
    value_name_label->adjustSize();
    fact_label->adjustSize();
    value_label->adjustSize();
}

void EditOrderDiscount::count_slot(){
    emit recount(percents_edit->text().toDouble());
}

void EditOrderDiscount::renew_labels(double percents, double value){
    fact_label->setText(QString("%1 %").arg(QString::number(percents, 'f', 2)));
    value_label->setText(QString("%1 р.").arg(QString::number(value, 'f', 2)));
}

void EditOrderDiscount::ok_clicked(){
    //if(QMessageBox::question(this, "Сохранить?", "Записать изменения?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
      //  return;
    count_slot();
    QDialog::accept();
}

void EditOrderDiscount::cancel_clicked(){
    //if(QMessageBox::question(this, "Отменить?", "Отменить изменения?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
    //    return;
    QDialog::reject();
}
