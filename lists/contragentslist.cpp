#include "contragentslist.h"

ContragentsList::ContragentsList(bool returning, int type, QWidget *parent) :
    QWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->returning = returning;
    this->type = type;
    model = new QSqlQueryModel(this);
    all = new QRadioButton("Все", this);
    buyers = new QRadioButton("Покупатели", this);
    shippers = new QRadioButton("Поставщики", this);
    name_button = new QRadioButton("Клиент", this);
    inn_button = new QRadioButton("ИНН", this);
    contact_button = new QRadioButton("Конт. лицо", this);
    section = "CONTRAGENTS";
    settings = get_settings();
    columns = settings->value(QString("%1/%2").arg(USERNAME).arg(section), QVariant()).value<ColumnsStruct>();
    table = new BaseTable(section, false, false, &columns, this);
    table->create_new_table();
    model = new QSqlQueryModel(this);
    text_field = new QLineEdit(this);
    top_bar = new QStatusBar(this);
    bottom_bar = new QStatusBar(this);
    bar1_header = new QLabel("Реквизиты:");
    bar2_header = new QLabel("Контакты:");
    bar1_text = new QLabel();
    bar2_text = new QLabel();
    bar1_header->setAlignment(Qt::AlignRight);
    bar2_header->setAlignment(Qt::AlignRight);

    // Типы: 0 - все, 1 - покупатели, 2 - поставщики
    // В зависимости от типа, активируем нужную радиокнопку
    if(type == 1) buyers->setChecked(true);
    else if(type == 2) shippers->setChecked(true);
    else all->setChecked(true);

    left_width = 0;

    top_lt = new QHBoxLayout();
    top_lt->setSpacing(0);
    top_lt->setMargin(0);
    escape_button = create_panel_button(this, top_lt, "Закрыть", ":/escape.png", SLOT(escape_slot()));

    set_layout();
    fill_table();
    table->set_filled();
    table->set_model(model);
    table->restore_state();
    for(int i = 5; i <= 13; i++)
        table->hide_column(i);

    // статусбары
    bar1_header->setFixedWidth(left_width);
    bar2_header->setFixedWidth(left_width);
    top_bar->addWidget(bar1_header);
    top_bar->addWidget(bar1_text);
    bottom_bar->addWidget(bar2_header);
    bottom_bar->addWidget(bar2_text);

    set_columns_names();
    set_headers();

    connect(table, SIGNAL(clicked(QModelIndex)), SLOT(set_statusbars(QModelIndex)));
    connect(all, SIGNAL(clicked()), SLOT(fill_table()));
    connect(buyers, SIGNAL(clicked()), SLOT(fill_table()));
    connect(shippers, SIGNAL(clicked()), SLOT(fill_table()));
    if(returning)
        connect(table, SIGNAL(double_clicked(QModelIndex)), SLOT(return_selected(QModelIndex)));
}

void ContragentsList::set_layout(){
    // лайаут для двух радиокнопок возле текстовой строки
    QVBoxLayout *threebuttons_lt = new QVBoxLayout();
    threebuttons_lt->addWidget(name_button);
    threebuttons_lt->addWidget(inn_button);
    threebuttons_lt->addWidget(contact_button);

    // добавляем поисковик
    top_lt->addLayout(threebuttons_lt);
    top_lt->addWidget(text_field);
    // подвигаем влево
    top_lt->addStretch(1);
    // упаковываем во фрейм, чтобы легко манипулировать размером
    QFrame *top_panel = new QFrame(this);
    top_panel->setLayout(top_lt);


    // три радиокнопки сбоку
    QVBoxLayout *left_lt = new QVBoxLayout();
    left_lt->addWidget(all);
    left_lt->addWidget(buyers);
    left_lt->addWidget(shippers);
    left_lt->addStretch(1);
    // упаковываем во фрейм, чтобы легко манипулировать размером
    QFrame *left_frame = new QFrame(this);
    left_frame->setLayout(left_lt);
    left_frame->setFixedWidth(qApp->font().pointSize()*15);
    left_width = left_frame->width();

    // нижний лайаут, да
    QHBoxLayout *bottom_lt = new QHBoxLayout();
    bottom_lt->addWidget(left_frame);
    bottom_lt->addWidget(table);

    // самый главный лт
    QVBoxLayout *main_lt = new QVBoxLayout(this);
    main_lt->setSpacing(0);
    main_lt->addWidget(top_panel);
    main_lt->addWidget(top_bar);
    main_lt->addWidget(bottom_bar);
    main_lt->addLayout(bottom_lt);

    this->setLayout(main_lt);
    name_button->setChecked(true);

    connect(text_field, SIGNAL(textChanged(QString)), SLOT(fill_table()));
}

void ContragentsList::set_columns_names(){
    columns_names["id"] = "id";
    columns_names["name"] = "Контрагент";
    columns_names["phone"] = "Телефон";
    columns_names["inn"] = "ИНН";
    columns_names["kpp"] = "КПП";
}

void ContragentsList::set_headers(){
    QString hdr;
    for(int i = 0; i < model->columnCount(); i++){
        hdr = columns_names[model->headerData(i, Qt::Horizontal).toString()];
        if(hdr != QString())
            model->setHeaderData(i, Qt::Horizontal, hdr);
    }
}

void ContragentsList::escape_slot(){
    this->close();
    emit delete_me();
}

void ContragentsList::set_statusbars(QModelIndex index){
    int row = index.row();
    bar1_text->setText(table->table_data(row, 1).toString());

    // со вторым статусбаром всё сложно
    // для каждого из трёх потенциальных контактных лиц нужно проверить наличие всех трёх полей
    // вернее, не так: если есть ФИО лица, то проверить наличие телефона и е-маил
    // если нет, не проверять более ничего
    // первое лицо: столбцы 5(имя), 6(телефон), 7(почта)
    // второе лицо: 8,9,10,
    // третье: 11,12,13
    QString bar2_str = "";
    if(table->table_data(5).toString().length() > 0){
        bar2_str += table->table_data(5).toString();
        if(table->table_data(6).toString().length() > 0)
            bar2_str += QString(", телефон: ").append(table->table_data(6).toString());
        if(table->table_data(7).toString().length() > 0)
            bar2_str += QString(", e-mail: %1").arg(table->table_data(7).toString());
    }
    if(table->table_data(8).toString().length() > 0){
        if(bar2_str.length()) bar2_str += ", ";
        bar2_str += table->table_data(8).toString();
        if(table->table_data(9).toString().length() > 0)
            bar2_str += QString(", телефон: ").append(table->table_data(9).toString());
        if(table->table_data(10).toString().length() > 0)
            bar2_str += QString(", e-mail: %1").arg(table->table_data(10).toString());
    }
    if(table->table_data(11).toString().length() > 0){
        if(bar2_str.length()) bar2_str += ", ";
        bar2_str += table->table_data(11).toString();
        if(table->table_data(12).toString().length() > 0)
            bar2_str += QString(", телефон: ").append(table->table_data(12).toString());
        if(table->table_data(13).toString().length() > 0)
            bar2_str += QString(", e-mail: %1").arg(table->table_data(13).toString());
    }
    bar2_text->setText(bar2_str);
}

void ContragentsList::closeEvent(QCloseEvent *e){
    //settings->setValue(QString("%1/%2").arg(USERNAME).arg(section), QVariant::fromValue<ColumnsStruct>(columns));
    //delete settings;
}

ContragentsList::~ContragentsList(){
    settings->setValue(QString("%1/%2").arg(USERNAME).arg(section), QVariant::fromValue<ColumnsStruct>(columns));
    delete settings;
}

void ContragentsList::fill_table(){
    QString query_str = "SELECT c.id, c.name, c.inn, c.kpp, c.phone, "
            "c.contact_name_1, c.contact_phone_1, c.contact_email_1, "
            "c.contact_name_2, c.contact_phone_2, c.contact_email_2, "
            "c.contact_name_3, c.contact_phone_3, c.contact_email_3 "
            "FROM contragents c";

    // заведём стринглист, куда будем складывать установленные фильтры.
    // потом их склеим и передадим в WHERE sql-запроса
    QStringList conditions;
    // фильтр по типу. 1 - покупатель, 2 - поставщик
    if(buyers->isChecked())
        conditions << "type = 1";
    else if(shippers->isChecked())
        conditions << "type = 2";
    // текстовый фильтр.
    // бывает по названию, по ИНН и по контактному лицу (алсо телефону, email)
    // по названию:
    if(name_button->isChecked()){
        conditions << QString("name ~* '%1'").arg(text_field->text());
    }
    // по ИНН
    else if(inn_button->isChecked()){
        conditions << QString("inn LIKE '\%%1\%'").arg(text_field->text());
    }
    // по контактным лицам
    else if(contact_button->isChecked()){
        // контактных лиц предусмотрено трое, по три поля для каждого (имя, телефон, email), и искать нужно по всем
        conditions << QString("(contact_name_1 ~* '%1'"
                              " OR contact_phone_1 ~* '%1'"
                              " OR contact_email_1 ~* '%1'"
                              " OR contact_name_2 ~* '%1'"
                              " OR contact_phone_2 ~* '%1'"
                              " OR contact_email_2 ~* '%1'"
                              " OR contact_name_3 ~* '%1'"
                              " OR contact_phone_3 ~* '%1'"
                              " OR contact_email_3 ~* '%1')"
                              ).arg(text_field->text());
    }
    QString conditions_str = conditions.join(" AND ");
    if(conditions_str.length() > 0)
        query_str += QString(" WHERE ").append(conditions_str);
    qDebug() << query_str;

    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        qDebug() << "Ошибка: " << model->lastError().text();
        return;
    }
    if(model->rowCount() == 0){
        QString cuted_text = text_field->text().left(text_field->text().length()-1);
        text_field->setText(cuted_text);
        return;
    }
    table->set_model(model);
    for(int i = 5; i <= 13; i++)
        table->hide_column(i);

}

void ContragentsList::return_selected(QModelIndex index){
    int row = index.row();
    emit selected(table->table_data(row, 0).toInt(), table->table_data(row, 1).toString());
    close();
}
