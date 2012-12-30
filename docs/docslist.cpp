#include "docslist.h"

DocsList::DocsList(bool opendocs,
                   QStackedWidget *stack,
                   QStack<QWidget*> prev_wgts,
                   QStandardItemModel *list_model,
                   QWidget *parent) :
    QWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->opendocs = opendocs;
    if(opendocs){
        this->stack = stack;
        this->prev_wgts = prev_wgts;
    }
    settings = get_settings();
    QString section = "DOCSLIST";
    cstruct = settings->value(QString("%1/%2").arg(USERNAME).arg(section), QVariant()).value<ColumnsStruct>();
    form_sql_columns();


    from_date = new QDateEdit();
    to_date = new QDateEdit;
    docnum = new QRadioButton("номер");
    client = new QRadioButton("клиент");
    textline = new QLineEdit();
    reset_text = new QPushButton("X");
    type_box = new CustomComboBox();
    remove_dates = new QCheckBox("ВЫКЛ.");
    refresh_button = new QPushButton("ОБНОВИТЬ");
    table = new BaseTableView(section, false, &cstruct, columns_default_widths, this);
    controls = new QWidget();
    from_label = new QLabel("С:");
    to_label = new QLabel("По:");
    type_label = new QLabel("Тип:");
    num_label = new QLabel("Номер:");
    model = new DocslistTableModel(columns_ids);

    contragent_string = QString();
    rnotes_string = QString();
    author_string = QString();
    searchtype_box = new QComboBox();
    search_box = new CustomComboBox();
    search_box->setEditable(true);
    search_box->setInsertPolicy(QComboBox::NoInsert);
    searchtype_box->addItem("Клиент");
    searchtype_box->addItem("Для кого");
    searchtype_box->addItem("Автор");
    fill_search_box();


    top_lt = new QHBoxLayout();
    escape_button = create_panel_button(this, top_lt, "Закрыть", ":back", SLOT(escape_slot()));
    if(opendocs){
        delete_button = create_panel_button(this, top_lt, "Удалить документ", ":delete", SLOT(delete_slot()));
    }
    this->list_model = list_model;

    filters["date"] = QString();
    filters["string"] = QString();
    filters["type"] = QString();
    filters["user"] = QString();
    filters["number"] = QString();
    order = " ORDER BY docdate, doctime";

    docnum->setChecked(true);
    reset_text->setToolTip("Сброс фильтра");
    refresh_button->setToolTip("Обновить список документов");

    table->setModel(model);
    // воплощаем всё в реальность
    set_columns_names();
    fill_combobox();
    custom_dates();
    set_layout();
    connects();
    form_date_filter();
    fill_table();
    table->set_filled();
    rename_columns();
    table->restore_state();
    table->hideColumn(columns_ids["id"]);
    table->hideColumn(columns_ids["doctype_id"]);
    table->hideColumn(columns_ids["provider_id"]);
    table->hideColumn(columns_ids["contragent_id"]);
    table->hideColumn(columns_ids["consignee_id"]);
    table->hideColumn(columns_ids["r_print"]);
    table->hideColumn(columns_ids["sumprice"]);
    table->hideColumn(columns_ids["price"]);
    table->hideColumn(columns_ids["round"]);

    RightAlignmentDelegate *d = new RightAlignmentDelegate(table);
    table->setItemDelegateForColumn(columns_ids["docnum"], d);
    table->setItemDelegateForColumn(columns_ids["discount_sum"], d);
    table->setItemDelegateForColumn(columns_ids["discount"], d);

    table->setFocus();
    qDebug() << "scrollbar minimum" << table->horizontalScrollBar()->minimum();
    qDebug() << "scrollbar maximum" << table->horizontalScrollBar()->maximum();
}

DocsList::~DocsList(){
    settings->setValue(QString("%1/%2").arg(USERNAME).arg("DOCSLIST"), QVariant::fromValue<ColumnsStruct>(cstruct));
    delete settings;
}

inline void DocsList::set_layout(){
    // кнопки наверху добавили, дальше - две радиокнопки, текстовое поле и кнопка сброса
    // радиокнопки в вертикальном лайауте
    //QVBoxLayout *rblt = new QVBoxLayout();
    //rblt->addWidget(docnum);
    //rblt->addWidget(client);
    top_lt->setSpacing(0);
    top_lt->addSpacing(20);
    /*
    //top_lt->addLayout(rblt);
    top_lt->addWidget(num_label);
    top_lt->addWidget(textline);
    //////////////--------------------------------------*/
    QGridLayout *glt = new QGridLayout();
    glt->addWidget(type_label, 0, 0);
    glt->addWidget(type_box, 0, 1);
    glt->addWidget(num_label, 1, 0);
    glt->addWidget(textline, 1, 1);
    top_lt->addLayout(glt);

    top_lt->addSpacing(20);
    QVBoxLayout *search_lt = new QVBoxLayout();
    search_lt->addWidget(searchtype_box);
    search_lt->addWidget(search_box);
    search_lt->setSpacing(0);
    search_lt->setMargin(0);
    top_lt->addLayout(search_lt);
    //////////////--------------------------------------
/*
    QGridLayout *glt = new QGridLayout();
    glt->addWidget(type_label, 0, 0);
    glt->addWidget(type_box, 0, 1);
    glt->addWidget(num_label, 1, 0);
    glt->addWidget(textline, 1, 1);

    glt->addWidget(searchtype_box, 0, 2);
    glt->addWidget(search_box, 1, 2);
    top_lt->addLayout(glt);
*/
    top_lt->addSpacing(20);
    top_lt->addWidget(reset_text);
    top_lt->addStretch(1);

    // все элементы управления запихнём в виджет
    QHBoxLayout *bottom_lt = new QHBoxLayout();
    bottom_lt->addWidget(from_label);
    bottom_lt->addWidget(from_date);
    bottom_lt->addWidget(to_label);
    bottom_lt->addWidget(to_date);
    bottom_lt->addWidget(remove_dates);
    bottom_lt->addSpacing(20);
    //bottom_lt->addWidget(type_label);
    //bottom_lt->addWidget(type_box);
    // далее кнопка обновить
    bottom_lt->addStretch(1);
    bottom_lt->addWidget(refresh_button);
    bottom_lt->addStretch(1);
    resize();

    //controls->setLayout(hlt);

    // виджет и таблицу пихаем в лайаут
    QVBoxLayout *vlt = new QVBoxLayout();
    //vlt->addWidget(controls);
    vlt->addLayout(top_lt);
    vlt->addLayout(bottom_lt);
    vlt->addWidget(table);

    this->setLayout(vlt);

}

void DocsList::resize(){
    int font_size = qApp->font().pointSize();
    from_label->adjustSize();
    from_date->adjustSize();
    to_label->adjustSize();
    to_date->adjustSize();
    remove_dates->adjustSize();
    type_label->adjustSize();
    type_box->setFixedWidth(font_size * 15);
    docnum->adjustSize();
    num_label->adjustSize();
    client->adjustSize();
    textline->setFixedWidth(font_size * 15);
    reset_text->setFixedSize(font_size+15, font_size+15);
    refresh_button->adjustSize();

    searchtype_box->setFixedWidth(font_size * 16);
    search_box->setFixedWidth(searchtype_box->width());
    //int total_width = from_label->width() + from_date->width() + to_label->width() + to_date->width() + remove_dates->width() + type_label->width() +
            //type_box->width() + client->width() + textline->width() + reset_text->width() + refresh_button->width() + 115;
    //int total_height = docnum->height()*2 + 15;
    //controls->setFixedSize(total_width, total_height);
    table->verticalHeader()->setDefaultSectionSize(table->verticalHeader()->fontMetrics().height() + 2);
    table->resizeColumnsToContents();
}


inline void DocsList::connects(){
    connect(type_box, SIGNAL(fill_box_signal()), SLOT(fill_combobox()));
    connect(type_box, SIGNAL(fill_box_signal()), SLOT(reset_type_filter()));
    connect(from_date, SIGNAL(dateChanged(QDate)), SLOT(change_date(QDate)));
    connect(textline, SIGNAL(textChanged(QString)), SLOT(form_number_filter()));
    connect(type_box, SIGNAL(activated(int)), SLOT(form_type_filter(int)));
    connect(remove_dates, SIGNAL(clicked(bool)), SLOT(change_dateoff(bool)));
    connect(search_box, SIGNAL(fill_box_signal()), SLOT(fill_search_box()));
    connect(search_box, SIGNAL(editTextChanged(QString)), SLOT(change_textsearch(QString)));
    connect(reset_text, SIGNAL(clicked()), SLOT(reset_text_slot()));
    if(opendocs)
        connect(table, SIGNAL(double_clicked(QModelIndex)), SLOT(open_doc_slot(QModelIndex)));
    else connect(table, SIGNAL(double_clicked(QModelIndex)), SLOT(emit_id_signal(QModelIndex)));
    connect(refresh_button, SIGNAL(clicked()), SLOT(refresh()));
}

inline void DocsList::set_columns_names(){
    columns["id"] = "id";
    columns["spisan"] = "Спис.";
    columns["oplata"] = "Опл.";
    columns["podbor"] = "Гот.";
    columns["vydan"] = "Выд.";
    columns["typename"] = "Документ";
    columns["docnum"] = "Номер";
    columns["date"] = "Дата";
    columns["time"] = "Время";
    columns["discount_sum"] = "Сумма";
    columns["discount"] = "Скидка %";
    columns["contragent"] = "Покупатель";
    columns["provider"] = "Поставщик";
    columns["consignee"] = "Грузополучатель";
    columns["user"] = "Автор";
    columns["notes"] = "Примечание";
    columns["r_notes"] = "Для кого";

}

void DocsList::rename_columns(){
    // переименовываем столбцы, беря названия из columns
    for(int i = 0; i < model->columnCount(); i++){
        if(columns[model->headerData(i, Qt::Horizontal).toString()] != "")
            model->setHeaderData(i, Qt::Horizontal, columns[model->headerData(i, Qt::Horizontal).toString()]);
    }
}


inline void DocsList::custom_dates(){
    // по умолчанию устанавливаем в оба поля выбора даты сегодняшнее число
    QDate date = QDate::currentDate();
    from_date->setDate(date);
    to_date->setDate(date);

    // активируем выпадающий календарь
    from_date->setCalendarPopup(true);
    to_date->setCalendarPopup(true);
    from_date->calendarWidget()->setFirstDayOfWeek(Qt::Monday);
    to_date->calendarWidget()->setFirstDayOfWeek(Qt::Monday);

}

void DocsList::switch_hidden(){
    fill_table();
}

void DocsList::fill_combobox(){
    type_box->clear();
    // выбираем типы документов, присутствующие в базе за выбранный период времени
    // не забываем про скрытые
    QSqlQuery query(base);
    QString condition = "";
    if(!remove_dates->isChecked())
        condition = QString("WHERE docdate >= '%1' AND docdate <= '%2'")
                .arg(from_date->date().toString("yyyy-MM-dd"))
                .arg(to_date->date().toString("yyyy-MM-dd"));
    QString hidden = show_hidden ? "" : " AND type = 1";
    QString query_str = QString("SELECT id, name FROM doctypes WHERE id IN "
            "(SELECT DISTINCT doctype_id FROM docslist %1) %2 ORDER BY id").arg(condition).arg(hidden);
    if(!query.exec(query_str)){
        qDebug() << "SQL error: " << query.lastError().text();
        return;
    }
    type_box->addItem("", -1);
    while(query.next())
        type_box->addItem(query.value(1).toString(), query.value(0).toInt());
}

void DocsList::fill_search_box(){
    search_box->clear();
    // выбрать блаблаааааааааааааааааааааабьаваывыджм
    // заводим QStringList, куда будем складывать непустые фильтры
    int type = searchtype_box->currentIndex();
    qDebug() << "type: " << type;
    QString column = "";
    if(type == 0) column = "c.name";
    else if(type == 1) column = "d.r_notes";
    else if(type == 2) column = "u.login";

    QStringList f;
    QMapIterator<QString, QString> it(filters);
    while(it.hasNext()){
        it.next();
        if(it.value() != QString() && it.key() != "string" && it.key() != "number") f << it.value();
    }
    f << QString("%1 IS NOT NULL").arg(column);
    if(!show_hidden)
        f << "dt.type = 1";

    QSqlQuery query(base);
    QString query_str = QString("SELECT DISTINCT %1 FROM %2 ").arg(column).arg(sql_tables);
    if(f.count() > 0)
        query_str += QString(" WHERE %1").arg(f.join((" AND ")));
    query_str += " ORDER BY 1";
    qDebug() << query_str;
    if(!query.exec(query_str)){
        qDebug() << "SQL error: " << query.lastError().text();
        return;
    }
    search_box->addItem("");
    while(query.next())
        search_box->addItem(query.value(0).toString());
}

void DocsList::delete_last_symbol(){
    if(this->textline->text() != ""){
        this->textline->setText(this->textline->text().remove(this->textline->text().size()-1, 1));
        fill_table();
    }
}

void DocsList::escape_slot(){
    qDebug() << "sender" << this;
    emit delete_me();
    qDebug("EMITTED");
    //this->close();
}

void DocsList::delete_slot(){

}

void DocsList::form_date_filter(){
    if(!(from_date->date().isValid() && to_date->date().isValid()))
        return;
    filters["date"] = QString("docdate >= '%1' AND docdate <= '%2'")
            .arg(from_date->date().toString("yyyy-MM-dd"))
            .arg(to_date->date().toString("yyyy-MM-dd"));
}

void DocsList::form_number_filter(){
    if(textline->text().length() == 0){
        filters["number"] = QString();
        order = " ORDER BY docdate, doctime";
        fill_table();
    }
    else if(textline->text().toInt() > 0){
        filters["number"] = QString("docnum >= %1").arg(textline->text());
        order = " ORDER BY docnum, doctype_id, docdate, doctime";
        if(fill_table() == 0)
            delete_last_symbol();
    }
    textline->setFocus();
}

void DocsList::form_type_filter(int){
    int type = type_box->itemData(type_box->currentIndex(), Qt::UserRole).toInt();
    filters["type"] = (type == -1 ? QString() : QString("doctype_id = %1").arg(type));
    fill_table();
}

void DocsList::reset_type_filter(){
    filters["type"] = QString();
    fill_table();
}

void DocsList::form_text_filter(){

}

void DocsList::change_date(QDate){
    form_date_filter();
    fill_table();
}

void DocsList::change_dateoff(bool b){
    from_label->setEnabled(!b);
    from_date->setEnabled(!b);
    to_label->setEnabled(!b);
    to_date->setEnabled(!b);
    if(b)
        filters["date"] = QString();
    else
        form_date_filter();
    fill_table();
}

void DocsList::change_searchtype(int t){
    if(t == 0)
        search_box->setEditText(contragent_string);
    else if(t == 1)
        search_box->setEditText(rnotes_string);
    else if(t == 2)
        search_box->setEditText(author_string);
    fill_search_box();
    change_textsearch(search_box->currentText());
}

void DocsList::change_textsearch(QString s){
    if(s.length() == 0){
        reset_text_slot();
        return;
    }
    filters["number"] = QString();
    textline->setText(filters["number"]);
    int type = searchtype_box->currentIndex();
    if(type == 0){
        contragent_string = s;
        filters["string"] = QString("c.name ~* '%1'").arg(s);
        order = " ORDER BY c.name, docdate, doctime";
    }
    else if(type == 1){
        rnotes_string = s;
        filters["string"] = QString("d.r_notes ~* '%1'").arg(s);
        order = " ORDER BY d.r_notes, docdate, doctime";
    }
    else if(type == 2){
        author_string = s;
        filters["string"] = QString("u.login ~* '%1'").arg(s);
        order = " ORDER BY u.login, docdate, doctime";
    }
    fill_table();
    textline->setFocus();
}

void DocsList::reset_text_slot(){
    search_box->setEditText("");
    filters["string"] = QString();
    order = " ORDER BY docdate, doctime";
    contragent_string = "";
    rnotes_string = "";
    author_string = "";
    fill_table();
}

void DocsList::open_doc_slot(QModelIndex index){
    //disconnect(opened_doc);
    if(!table->currentIndex().isValid())
        return;

    int id = table->table_data(index.row(), 0).toInt();
    prev_wgts.push(stack->currentWidget());
    opened_doc = new Document(stack, prev_wgts, id, list_model, this);
    stack->addWidget(opened_doc);
    stack->setCurrentWidget(opened_doc);

    connect(opened_doc, SIGNAL(delete_me()), SLOT(close_current()));
    connect(opened_doc, SIGNAL(need_refresh()), SLOT(refresh()));
    connect(opened_doc, SIGNAL(create_move_dialog(Document*)), SIGNAL(create_move_dialog(Document*)));
    connect(opened_doc, SIGNAL(set_reserve_doc_color(int)), SIGNAL(set_reserve_doc_color(int)));
}

void DocsList::move_to_doc(int doc_id){
    if(opened_doc)
        opened_doc->move_to_doc(doc_id);
}

void DocsList::move_to_reserve(int id){
    if(opened_doc)
        opened_doc->move_to_reserve(id);
}

void DocsList::emit_id_signal(QModelIndex index){
    if(!table->currentIndex().isValid())
        return;
    int id = table->table_data(index.row(), 0).toInt();
    emit id_signal(id);
}

void DocsList::close_current(){
    //stack->currentWidget()->close();
    stack->removeWidget(stack->currentWidget());
    stack->setCurrentWidget(prev_wgts.last());

    prev_wgts.pop();
    table->setFocus();
}

void DocsList::refresh(){
    QModelIndex current_index = this->table->currentIndex();
    model->setQuery(model->query().lastQuery(), base);
    table->setCurrentIndex(current_index);
}


inline void DocsList::form_sql_columns(){
    int i = 0;
    int fontsize = qApp->font().pointSize();

    columns_sql << "d.id as id";
    columns_default_widths << fontsize * 5;
    columns_ids["id"] = i++;

    columns_sql << "d.doctype_id as doctype_id";
    columns_default_widths << fontsize * 5;
    columns_ids["doctype_id"] = i++;

    columns_sql << "d.spisan as spisan";
    columns_default_widths << fontsize * 3;
    columns_ids["spisan"] = i++;

    columns_sql << "d.oplata as oplata";
    columns_default_widths << fontsize * 3;
    columns_ids["oplata"] = i++;

    columns_sql << "d.podbor as podbor";
    columns_default_widths << fontsize * 3;
    columns_ids["podbor"] = i++;

    columns_sql << "d.vydan as vydan";
    columns_default_widths << fontsize * 3;
    columns_ids["vydan"] = i++;

    columns_sql << "dt.name as typename";
    columns_default_widths << fontsize * 16;
    columns_ids["name"] = i++;

    columns_sql << "d.docnum as docnum";
    columns_default_widths << fontsize * 8;
    columns_ids["docnum"] = i++;

    columns_sql << "to_char(d.docdate, 'DD.MM.YYYY') as date";
    columns_default_widths << fontsize * 12;
    columns_ids["date"] = i++;

    columns_sql << "to_char(d.doctime, 'HH24:MI:SS') as time";
    columns_default_widths << fontsize * 12;
    columns_ids["time"] = i++;

    columns_sql << "d.sumprice as sumprice";
    columns_default_widths << fontsize * 12;
    columns_ids["sumprice"] = i++;

    columns_sql << "d.discount_percents as discount";
    columns_default_widths << fontsize * 12;
    columns_ids["discount"] = i++;

    columns_sql << "d.discount_sum as discount_sum";
    columns_default_widths << fontsize * 12;
    columns_ids["discount_sum"] = i++;

    columns_sql << "coalesce(c.name, '') as contragent";
    columns_default_widths << fontsize * 24;
    columns_ids["contragent"] = i++;

    columns_sql << "cc.name as provider";
    columns_default_widths << fontsize * 24;
    columns_ids["provider"] = i++;

    columns_sql << "ccc.name as consignee";
    columns_default_widths << fontsize * 24;
    columns_ids["consignee"] = i++;

    columns_sql << "u.login as user";
    columns_default_widths << fontsize * 16;
    columns_ids["user"] = i++;

    columns_sql << "d.notes as notes";
    columns_default_widths << fontsize * 32;
    columns_ids["notes"] = i++;

    columns_sql << "d.r_notes as r_notes";
    columns_default_widths << fontsize * 32;
    columns_ids["r_notes"] = i++;

    columns_sql << "d.r_print as r_print";
    columns_default_widths << fontsize * 32;
    columns_ids["r_print"] = i++;

    columns_sql << "d.provider_id as provider_id";
    columns_default_widths << fontsize * 5;
    columns_ids["provider_id"] = i++;

    columns_sql << "d.contragent_id as contragent_id";
    columns_default_widths << fontsize * 5;
    columns_ids["contragent_id"] = i++;

    columns_sql << "d.consignee_id as consignee_id";
    columns_default_widths << fontsize * 5;
    columns_ids["consignee_id"] = i++;

    columns_sql << "d.round as round";
    columns_default_widths << fontsize * 5;
    columns_ids["round"] = i++;


    sql_from = columns_sql.join(", ");
    sql_tables = "docslist d "
                " JOIN doctypes dt ON d.doctype_id = dt.id "
                " JOIN users u ON u.id = d.user_id"
                " LEFT JOIN contragents c ON c.id = d.contragent_id"
                " LEFT JOIN contragents cc ON cc.id = d.provider_id"
                " LEFT JOIN contragents ccc ON cc.id = d.consignee_id"
                " LEFT JOIN r_strings rs ON rs.id = d.r_id";
}

int DocsList::fill_table(){
    // заводим QStringList, куда будем складывать непустые фильтры
    QStringList f;
    QMapIterator<QString, QString> it(filters);
    while(it.hasNext()){
        it.next();
        if(it.value() != QString()) f << it.value();
    }
    if(!show_hidden)
        f << "dt.type = 1";

    QString query_str = QString("SELECT %1 FROM %2").arg(sql_from).arg(sql_tables);
    if(f.count() > 0)
        query_str += QString(" WHERE %1").arg(f.join((" AND ")));
    query_str += order;
    //qDebug() << query_str;
    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        qDebug() << model->lastError().text();
        return -1;
    }

    table->setFocus();
    return model->rowCount();
}

int DocsList::current_id(){
    int id = -1;
    if(table->currentIndex().isValid())
        id = table->table_data(0).toInt();
    return id;
}




DocslistTableModel::DocslistTableModel(QMap<QString,int> ids, QWidget *parent) : QSqlQueryModel(parent){
    this->ids = ids;
    iconsize = qApp->font().pointSize()+10;
}

QVariant DocslistTableModel::data(const QModelIndex &index, int role) const{
    /* столбец "списан":  0 - не списан (ничего не отображать),
                          4 - списан (галка),
                          5- аннулирован (крест)
       столбец "оплачен": 0 - не оплачен (не отображать ничего)
                          2 - оплачен частично (тильда)
                          4 - оплачен полностью (галка)
       столбец "готов":   1 - документ не просмотрен (отображать минус)
                          2 - документ просмотрен (тильда)
                          3 - подобрана хотя бы одна позиция (тильда)
                          4 - подобран полностью (галка)
                          5 - подбирать не нужно (крест)
       столбец "выдан":   0 - не выдан (не отображать ничего)
                          2 - выдан частично (тильда)
                          4 - выдан полностью (галка)
       Одинаковым иконкам соответствует одниаковый номер в базе, для упрощения этой функции:
            0 = пустота
            1 = минус  (green_minus)
            2,3 = тильда (green_tilda)
            4 = галка  (ok)
            5 = крест  (no)
       Соответственно, текст в них не отображать
    */

    if(index.column() == ids["spisan"] ||
            index.column() == ids["oplata"] ||
            index.column() == ids["podbor"] ||
            index.column() == ids["vydan"]){
        if(role == Qt::DecorationRole){
            if(QSqlQueryModel::data(index, Qt::DisplayRole).toInt() == 1)
                return QPixmap(":green_minus").scaled(iconsize, iconsize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            if(QSqlQueryModel::data(index, Qt::DisplayRole).toInt() == 2 || QSqlQueryModel::data(index, Qt::DisplayRole).toInt() == 3)
                return QPixmap(":green_tilda").scaled(iconsize, iconsize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            if(QSqlQueryModel::data(index, Qt::DisplayRole).toInt() == 4)
                return QPixmap(":ok").scaled(iconsize, iconsize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            if(QSqlQueryModel::data(index, Qt::DisplayRole).toInt() == 5)
                return QPixmap(":no").scaled(iconsize, iconsize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        }
        if(role == Qt::DisplayRole){
            return "";
        }
    }

    // цену отображаем в виде двух знаков после запятой"
    if((index.column() == ids["discount_sum"] || index.column() == ids["sumprice"])
            && role == Qt::DisplayRole)
        return QString("%1").arg(QString::number(QSqlQueryModel::data(index, role).toDouble(), 'f', 2));


    return QSqlQueryModel::data(index, role);
}
