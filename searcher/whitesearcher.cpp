#include "whitesearcher.h"

WhiteSearcher::WhiteSearcher(QWidget *parent) :
    QWidget(parent)
{
    // создаём виджеты
    white_table = new WhiteTable(this);
    catalog = new Catalog(this);
    input = new Input(this);
    ParamsSelector *param = 0;
    for(int i = 0; i < MAX_PARAMS; i++){
        param = new ParamsSelector(i, this);
        selectors << param;
    }
    params_reset = new QPushButton("X", this);
    panel_button_size = 50;
    edit_prices_permission = get_privilege(Privileges::Prices_edit_access);

    buttons_lt = new QHBoxLayout();
    buttons_lt->setMargin(0);
    buttons_lt->setSpacing(0);

    new_button = create_panel_button("Новая запись", ":/new", SLOT(new_detail_slot()));
    edit_button = create_panel_button("Редактировать запись", ":/edit", SLOT(edit_detail_slot()));
    delete_button = create_panel_button("Удалить запись", ":/delete", SLOT(delete_detail_slot()));
    if(edit_prices_permission){
        prices_button = create_panel_button("Редактировать цены", ":/prices", SLOT(edit_prices_slot()));
        discounts_button = create_panel_button("Скидки", ":/discounts", SLOT(edit_discounts_slot()));
    }
    settings_button = create_panel_button("Настройки", ":/settings", SIGNAL(open_settings()));

    // заполняем переменные
    single_group = -1;
    single_group_without_limits = -1;
    boxes_filled = false;

    success = true;
    // размещаем все элементы
    set_layout();

    clear_where_strs();

    // соединяем сигналы со слотами
    white_connects();
}

QPushButton* WhiteSearcher::create_panel_button(QString tooltip, QString image_file, const char *method){
    QPushButton *button = new QPushButton(this);
    button->setFixedSize(panel_button_size, panel_button_size);
    button->setToolTip(tooltip);
    QPixmap new_pixmap(image_file);
    QIcon new_icon(new_pixmap);
    button->setIcon(new_icon);
    button->setIconSize(button->rect().size());
    buttons_lt->addWidget(button);
    QObject::connect(button, SIGNAL(clicked()), method);
    return button;
}

inline void WhiteSearcher::white_connects(){
    // смена группы
    QObject::connect(this->catalog, SIGNAL(group_changed(int, QString)), SIGNAL(group_changed(int, QString)));
    QObject::connect(this->catalog, SIGNAL(hide_catalog()), SLOT(hide_catalog()));
    // смена пределов
    QObject::connect(this->white_table, SIGNAL(limits_changed(pair)), SIGNAL(limits_changed(pair)));
    // снятие лимита
    QObject::connect(this->white_table, SIGNAL(limits_removed()), SIGNAL(limits_removed()));
    // установка лимита
    QObject::connect(this->white_table, SIGNAL(limits_restored()), SIGNAL(limits_restored()));
    // изменение текста
    QObject::connect(this->input, SIGNAL(text_changed_signal(int,QString)), SLOT(text_changed_slot(int,QString)));
    // очистка текста
    QObject::connect(this->input, SIGNAL(reset_signal()), SIGNAL(reset_signal()));
    // изменение порядка сортировки
    QObject::connect(this->white_table, SIGNAL(sort_order_changed(int,Qt::SortOrder)), SIGNAL(sort_order_changed(int,Qt::SortOrder)));
    for(int i = 0; i < selectors.size(); i++){
        // заполнение селектора по щелчку
        QObject::connect(this->selectors.at(i), SIGNAL(fill_box_signal(int)), SLOT(fill_box(int)));
        // сброс данного селектора при открытии
        QObject::connect(this->selectors.at(i), SIGNAL(fill_box_signal(int)), SIGNAL(reset_param_signal(int)));
        // выбор элемента из селектора
        QObject::connect(this->selectors.at(i), SIGNAL(item_selected(QString,int)), SIGNAL(item_selected(QString,int)));
        // нажатие на кнопку над селектором
        QObject::connect(this->selectors.at(i), SIGNAL(button_clicked(int)), SIGNAL(button_clicked(int)));
    }

    // сброс селекторов
    QObject::connect(this->params_reset, SIGNAL(clicked()), SIGNAL(params_reset_clicked()));
    // изменение фильтра по цене
    QObject::connect(this->white_table, SIGNAL(prices_clicked()), SIGNAL(prices_clicked()));
    // изменение фильтра по кол-ву
    QObject::connect(this->white_table, SIGNAL(quantity_clicked()), SIGNAL(quantity_clicked()));
    // открыть карточку детали
    QObject::connect(this->white_table, SIGNAL(right_click(int)), SLOT(detail_info(int)));
    // открыть серый экран
    QObject::connect(this->white_table, SIGNAL(double_click(QModelIndex)), SLOT(open_grey(QModelIndex)));
    //
    QObject::connect(this->white_table, SIGNAL(section_resized(int, int)), SIGNAL(section_resized(int, int)));
    QObject::connect(this->white_table, SIGNAL(section_moved()), SIGNAL(section_moved()));
}

inline void WhiteSearcher::set_layout(){
    // виджет, содержащий кнопки и текстовое поле
    QWidget *top_panel = new QWidget(this);
    // виджет, содержащий выпадающие списки параметров
    QWidget *selectors_panel = new QWidget(this);
    // на эти два виджета будут наложены горизонтальные лайауты

    // устанавливаем размеры элементов
    int input_width = 300;      // ширина текстового поля
    int button_size = 25;       // ширина кнопки сброса
    int spacing = 5;            // отступ до кнопки сброса

    // поправка на ширину боковой панели в бубунте. Осторожно, костыль.
    int panel_width = 0;
#ifdef Q_OS_LINUX
    panel_width = 100;
#endif
    // да и это тоже костыль. Ширина селектора при максимизированном окне.
    int selector_width = (qApp->desktop()->width() - button_size - panel_width - spacing)/MAX_PARAMS;

    // лайаут для виджета с кнопками и текстовым полем
    QHBoxLayout *top_lt = new QHBoxLayout();
    // добавляем кнопки
    top_lt->addLayout(buttons_lt);
    top_lt->addStretch(1);
    // добавляем текстовое поле
    top_lt->addWidget(input);
    input->setFixedSize(input_width, 70);
    top_lt->addStretch(1);
    top_panel->setLayout(top_lt);
    top_panel->setFixedHeight(input->height());

    // лайаут для виджета с селекторами
    QHBoxLayout *wlt = new QHBoxLayout;
    wlt->setSpacing(0);
    wlt->setMargin(0);
    for(int i = 0; i < MAX_PARAMS; i++){
        selectors.at(i)->setFixedWidth(selector_width);
        selectors.at(i)->setGeometry(selector_width*i,0,selector_width,selectors.at(i)->height());
        //selectors.at(i)->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        wlt->addWidget(selectors.at(i));
    }
    wlt->addSpacing(spacing);
    wlt->addWidget(params_reset);
    params_reset->setFixedSize(button_size, button_size);
    selectors_panel->setLayout(wlt);
    selectors_panel->setMinimumWidth(500);
    selectors_panel->setFixedHeight(selectors.at(0)->height());

    // формируем разделитель, содержащий каталог и таблицу
    QSplitter *splitter = split();

    // главный layout
    QVBoxLayout *mainlt= new QVBoxLayout;
    mainlt->addWidget(top_panel);
    mainlt->addWidget(selectors_panel);
    mainlt->addWidget(splitter);

    this->setLayout(mainlt);

    QSettings *settings = get_settings();
    if(!settings->value(QString("%1/show_catalog").arg(USERNAME), true).toBool())
        catalog->hide();
    delete settings;
}

QSplitter* WhiteSearcher::split(){
    // формируем разделитель, содержащий каталог и таблицу
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(this->catalog);
    splitter->addWidget(this->white_table);

    // устанавливаем соотношение размеров 1:4
    QList<int> sz;
    sz << 100 << 400;
    splitter->setSizes(sz);

    return splitter;
}


void WhiteSearcher::set_date(){
    this->white_table->set_date();
}

void WhiteSearcher::hide_catalog(){
    QSettings *settings = get_settings();
    settings->setValue(QString("%1/show_catalog").arg(USERNAME), false);
    settings->sync();
    delete settings;
    this->catalog->hide();
    emit catalog_hides();
}

void WhiteSearcher::show_catalog(){
    QSettings *settings = get_settings();
    settings->setValue(QString("%1/show_catalog").arg(USERNAME), true);
    settings->sync();
    delete settings;
    this->catalog->show();
    emit catalog_shows();
}

void WhiteSearcher::new_detail_slot(){
    int group = 0;
    QString name = "";
    if(filters->group_filter() <= 0 || filters->group_name_filter().length() == 0)
        qDebug() << QString("Group filters: invalid values: group = %1, name = %2").arg(filters->group_filter()).arg(filters->group_name_filter().length());
    else{
        group = filters->group_filter();
        name = filters->group_name_filter();
    }
    WhiteAddDialog *add = new WhiteAddDialog(group, name, DIALOG_WHITE_NEW);
    if(add->exec() == QDialog::Accepted)
        fill_table(filters, false, false);
}

void WhiteSearcher::edit_detail_slot(){
    int id = this->white_table->table_data("id").toInt();
    if(id <= 0)
        error("Ошибка", "Выберите строку");
    else{
        WhiteAddDialog *edit = new WhiteAddDialog(id, DIALOG_WHITE_EDIT);
        if(edit->exec() == QDialog::Accepted)
            fill_table(filters, false, false);
    }
}

void WhiteSearcher::delete_detail_slot(){
    int id = this->white_table->table_data("id").toInt();
    if(id <= 0)
        error("Ошибка", "Выберите строку");
    else{
        int quantity = this->white_table->table_data("quantity").toInt();
        QString name = this->white_table->table_data("name").toString();
        if(quantity > 0)
            error("Обнулите количество", "Удалить можно только деталь с нулевым количеством!");
        else{
            if(question("Удалить?", QString("Удалить деталь %1?").arg(name))){
                // собственно, удаление
                QSqlQuery query(base);
                QString query_str = QString("UPDATE trademarks SET status = 2 WHERE id = %1").arg(id);
                if(!query.exec(query_str)){
                    error("Ошибка", QString("Ошибка при удалении детали: %1").arg(query.lastError().text()));
                    return;
                }
                fill_table(filters, false, false);
                //QMessageBox::information(this, "Успешно", "Деталь удалена.", QMessageBox::Ok);
            }
        }
    }
}

void WhiteSearcher::edit_prices_slot(){
    if(edit_prices_permission){     // я знаю, что проверять это незачем, но МАЛО ЛИ ЧТО
        int id = this->white_table->table_data("id").toInt();
        current_row = this->white_table->current_row();
        if(id <= 0)
            error("Ошибка", "Выберите строку");
        else{
            WhitePricesEditor *pred = new WhitePricesEditor
                    (id,
                     white_table->table_data(WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_RETAILPRICE)).toDouble(),
                     white_table->table_data(WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_WHOLEPRICE)).toDouble(),
                     white_table->table_data(WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_WHOLEBEGIN)).toInt(),
                     white_table->table_data(WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_UNIT)).toString(),
                     this);
            QObject::connect(pred, SIGNAL(accepted()), this, SLOT(refresh_table()));
            pred->exec();
            //if(pred->exec() == QDialog::Accepted)
                //refresh_table();
        }
    }
    else
        error("", "Вам запрещено редактировать цены.");
}

void WhiteSearcher::edit_discounts_slot(){
    if(edit_prices_permission){     // я знаю, что проверять это незачем, но МАЛО ЛИ ЧТО
        int id = this->white_table->table_data("id").toInt();
        current_row = this->white_table->current_row();
        if(id <= 0)
            error("Ошибка", "Выберите строку");
        else{
            YearsDiscountsDialog *dlg = new YearsDiscountsDialog
                    (id,
                     white_table->table_data(WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_NAME)).toString(),
                     white_table->table_data(WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_RETAILPRICE)).toDouble(),
                     white_table->table_data(WHITE_TABLE_COLUMNS.indexOf(COLUMN_WHITE_WHOLEPRICE)).toDouble(),
                     this);
            QObject::connect(dlg, SIGNAL(destroyed()), this, SLOT(refresh_table()));
            QObject::connect(dlg, SIGNAL(accepted()), this, SLOT(refresh_table()));
            QObject::connect(dlg, SIGNAL(rejected()), this, SLOT(refresh_table()));
            dlg->exec();
        }
    }
    else
        error("", "Вам запрещено просматривать скидки.");

}

void WhiteSearcher::refresh_table(){
    fill_table(filters, false, false);
    white_table->set_current_row(current_row);
}

void WhiteSearcher::resize_all(){
    catalog->resize_all();
    white_table->resize_all();
    for(int i = 0; i < selectors.count(); i++)
        selectors[i]->resize_all();
}

int WhiteSearcher::open_columns_list(){
    if(white_table != 0)
        return white_table->open_columns_list();
    return -1;
}

void WhiteSearcher::hide_show_columns(){
    if(white_table != 0)
        white_table->hide_show_columns();
}

QMap<QString, QString> WhiteSearcher::get_params_names(int group){
    // получить список названий параметров для данной подгруппы
    QMap<QString, QString> params;
    QSqlQuery q(base);
    QString query = "SELECT * FROM " + SUBGROUPS_TABLE + " WHERE id=" + QString::number(group);
    if(!q.exec(query)){
        error("Ошибка", QString("Не удалось выполнить запрос:\n").append(query));
        // params в этом случае остаётся пустым, и названия столбцов просто не заполнятся
    }
    else {
        QSqlRecord rec = q.record();
        q.next();
        for(int i = 1; i <= MAX_PARAMS; i++){
            QString field_name = "par" + QString::number(i);
            params[field_name] = q.value(rec.indexOf(field_name)).toString();
        }
    }
    return params;
}


QString WhiteSearcher::rename_column(QString str, int group){
    // Если группа определена, то столбцы с параметрами обзываем названиями этих параметров.
    // Иначе берём название из WHITE_COLUMNS_NAMES.
    QRegExp rx("^(par[0-9]+)_val$");
    if(group > 0 && str.contains(rx)){
        int pos = rx.indexIn(str);
        return (pos > -1 ? params_names[rx.cap(1)] : "?");
    }
    else
        return WHITE_COLUMNS_NAMES[str];
}


void WhiteSearcher::set_button_labels(){
    // устанавливаем названия кнопок над селекторами.
    // Собственно, они уже хранятся в params_names, хорошо бы ещё прибавить количество
    // если single_group_without_limits > 0, то кнопки будут называться
    // именами параметров для этой подгруппы, иначе - номерами.
    QString str;
    int cnt;
    for(int i = 0; i < MAX_PARAMS; i++){
        str = (single_group_without_limits > 0 ? params_names["par" + QString::number(i+1)] : QString::number(i+1));
        cnt = selectors.at(i)->count();
        selectors.at(i)->set_button_label(str, cnt > 0 ? cnt-1 : cnt);
    }
}

void WhiteSearcher::set_button_labels(QSqlQuery query){
    query.setForwardOnly(true);
    query.next();
    QString str;
    int cnt;
    for(int i = 0; i < MAX_PARAMS; i++){
        str = (single_group_without_limits > 0 ? params_names["par" + QString::number(i+1)] : QString::number(i+1));
        cnt = query.value(0).toInt();
        query.next();
        selectors.at(i)->set_button_label(str, cnt > 0 ? cnt-1 : cnt);
    }
}

void WhiteSearcher::set_text_button_labels(){
    for(int i = 0; i < MAX_PARAMS; i++)
        selectors.at(i)->set_button_label(single_group_without_limits > 0 ? params_names["par" + QString::number(i+1)] : QString::number(i+1));
}

void WhiteSearcher::clear_button_labels(){
    // очищаем названия кнопок над селекторами.
    for(int i = 0; i < MAX_PARAMS; i++){
        selectors.at(i)->clear_button_label();
    }
}

int WhiteSearcher::set_totals(QString query){
    QSqlQuery sql(base);
    int total;
    if(!sql.exec(query)){
        error("Ошибка", QString("Не удалось выполнить запрос:\n").append(query));
        total = -1;
    } else {
        sql.next();
        total = sql.value(0).toInt();
    }
    return total;
}

void WhiteSearcher::restore_width(int index, int width){
    this->white_table->restore_width(index, width);
}

void WhiteSearcher::restore_order(){
    this->white_table->restore_order();
}

void WhiteSearcher::detail_info(int id){
    DetailCard card(id, this);
    card.exec();
}

void WhiteSearcher::clear_text(){
    this->input->set_text("");
}

int WhiteSearcher::input_id_combobox_value(){
    return this->input->id_combobox_value();
}

int WhiteSearcher::get_original_index(QString name){
    return original_column_names.indexOf(name);
}

QString WhiteSearcher::get_original_name(int index){
    return original_column_names[index];
}

void WhiteSearcher::fill_box(int index){
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query_str, cur_par;
    cur_par = filters->params_filter().at(index);
    query_str = QString("SELECT DISTINCT t.par") + QString::number(index+1) + "_val " + tables_str + glue_where(QString("par%1_val").arg(index+1)) + " ORDER BY 1";
    //qDebug() << query_str;
    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        error("Ошибка при заполнении комбобокса " + QString::number(index), model->lastError().text());
        return;
    }
    else{
        selectors.at(index)->set_model(model);
    }
}

void WhiteSearcher::text_changed_slot(int type, QString text){
    // очистить селекторы
    for(int i = 0; i < MAX_PARAMS; i++){
        selectors.at(i)->set_edit_text("");
        selectors.at(i)->clear_items();
    }
    emit text_changed_signal(WHITE_MODE, type, text);
}

void WhiteSearcher::open_grey(QModelIndex i){
    int id = white_table->data(i, 0).toInt();
    emit(create_grey(id));
}

void WhiteSearcher::clear_where_strs(){
    where_strings["groups"] = "";
    where_strings["text"] = "";
    where_strings["prices"] = "";
    where_strings["quantites"] = "";
    for(int i = 1; i <= MAX_PARAMS; i++)
        where_strings[QString("par%1_val").arg(i)] = "";
}

QString WhiteSearcher::glue_where(QStringList *ex){
    QString where;
    QStringList lst;
    QMap<QString, QString>::iterator it = where_strings.begin();
    for(; it != where_strings.end(); it++){
        if(ex != 0)
            if(ex->indexOf(it.key()) >= 0)
                continue;
        if(it.value().size() > 0)
            lst << it.value();
    }
    lst << "status = 1";
    where = lst.join(" AND ");
    if(where.size() > 0)
        where = " WHERE " + where;
    return where;
}

QString WhiteSearcher::glue_where(QString str){
    QStringList lst;
    lst << str;
    return glue_where(&lst);
}


void WhiteSearcher::fill_table(Filters *f, bool reset_groups = false, bool delete_last_symbol = false){
    WhiteTableModel *query = new WhiteTableModel(this);
    QString strSelect;
    this->filters = f;
    success = true;

    if(delete_last_symbol &&
            (filters->last_applied_filter() == ID_FILTER || filters->last_applied_filter() == BEGIN_TEXT_FILTER ||
             filters->last_applied_filter() == PARTS_TEXT_FILTER)){
        input->delete_last_symbol();
        return;
    }

    // сбрасываем текущее значение single_group
    this->single_group = -1;
    this->single_group_without_limits = -1;

    // если надо перезаполнить боксы (т.е. если фильтр подразумевает изменение набора строк),
    // сбрасываем флаг boxes_filled и очищаем заголовки кнопок
    if(filters->last_applied_filter() != SORT_FILTER && filters->last_applied_filter() != LIMITS_FILTER
            && filters->last_applied_filter() != PRICES_FILTER && filters->last_applied_filter() != QUANTITY_FILTER){
        boxes_filled = false;
        clear_button_labels();
    }

    bool reset_page = false;
    if(filters->last_applied_filter() != LIMITS_FILTER){
        pair p = {0, white_table->get_items_on_page()};
        filters->set_limits(p);
        reset_page = true;
    }

    if(filters->last_applied_filter() != NOLIMITS_FILTER && filters->last_applied_filter() != LIMITS_FILTER){
        pair limits = {0, this->white_table->get_items_on_page()};
        //qDebug() << this->white_table->get_items_on_page();
        filters->set_limits(limits);
        filters->restore_limits();
        white_table->set_multipage_mode();
    }
    strSelect = apply_filters(filters);
    //qDebug() << strSelect;

    if(strSelect == "FAIL"){
        // если последний фильтр - текстовый, удаляем последний символ из текстового поля, таблица после этого перезаполнится
        if(filters->last_applied_filter() == ID_FILTER || filters->last_applied_filter() == BEGIN_TEXT_FILTER ||
                filters->last_applied_filter() == PARTS_TEXT_FILTER)
            input->delete_last_symbol();
    }
    else{
        // фильтры применены успешно. Начинаем заполнять поисковик данными.
        // применяем sql-запрос
        query->setQuery(strSelect, base);
        if(query->lastError().isValid()){
            // запрос не применился. success устанавливаем в false.
            success = false;
            QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query->lastError().text()));
            return;
        }
        // сохраняем оригинальные имена столбцов, чтобы узнавать потом индексы
        original_column_names.clear();
        for(int i = 0; i < query->columnCount(); i++)
            original_column_names << query->headerData(i, Qt::Horizontal).toString();

        // переименовываем столбцы
        // если single_group > 0, т.е. выбрана конкретная подгруппа,
        // столбцы с параметрами надо переименовать параметрами для этой подгруппы.
        // для этого выбираем их названия из БД и запихиваем в params_names
        if(single_group > 0)
            params_names = get_params_names(single_group);
        else params_names.clear();
        // и теперь для каждого столбца устанавливаем новое название
        for(int i = 0; i < query->columnCount(); i++)
            query->setHeaderData(i, Qt::Horizontal, rename_column(query->headerData(i, Qt::Horizontal).toString(), single_group));

        // установим названия кнопок над селекторами, только текст, без подсчёта
        set_text_button_labels();

        // если установлен фильтр по цене, надо соответствующим образом раскрасить заголовок
        if(filters->prices_filter() == POSITIVE_PRICES)
            query->setHeaderData(original_column_names.indexOf("price_ret"), Qt::Horizontal, QColor(0,192,0), Qt::BackgroundRole);
        else if(filters->prices_filter() == ZERO_PRICES)
            query->setHeaderData(original_column_names.indexOf("price_ret"), Qt::Horizontal, Qt::red, Qt::BackgroundRole);

        // если установлен фильтр по кол-ву, тоже красим заголовок
        if(filters->quantity_filter() == POSITIVE_QUANTITIES)
            query->setHeaderData(original_column_names.indexOf("quantity"), Qt::Horizontal, QColor(0,192,0), Qt::BackgroundRole);


        // устанавливаем названия кнопок над селекторами
        //set_button_labels();

        // сохраняем столбец и порядок сортировки
        int sort_column;
        Qt::SortOrder sort_order;
        if(!(filters->white_sort_order_filter().isEmpty())){
            SortingOrder s = filters->white_sort_order_filter().first();
            sort_column = original_column_names.indexOf(s.column);
            sort_order = s.order;
        }
        else {
            sort_column = original_column_names.indexOf(DEFAULT_WHITE_SORT_COLUMN);
            sort_order = DEFAULT_WHITE_SORT_ORDER;
            error("Внимание!", "Установлен порядок сортировки по умолчанию");
        }
        white_table->fill(query, original_column_names, sort_column, sort_order, reset_page);
    }
}

QString WhiteSearcher::apply_filters(Filters *filters){
    clear_where_strs();
    base.transaction();
    QString query, where, groupby, order, limits = "", count, final;
    bool query_success = false;
    int total;

    // список таблиц
    tables_str = QString(" FROM ") + SUBGROUPS_TABLE + " s "
            + "JOIN " + TOVMARKS_TABLE + " t ON t.subgroup_id = s.id "
            + "JOIN units u ON t.unit_id = u.id "
            + "LEFT OUTER JOIN weight_units w ON t.weight_unit_id = w.id ";


    /*
              ФИЛЬТР ПО ГРУППАМ
           where_strings["groups"]
                                        */
    if(filters->group_filter() == 0)
        where_strings["groups"] = "";
    else if(filters->group_filter() < ENLARGER)
        where_strings["groups"] = QString("t.subgroup_id = %1").arg(QString::number(filters->group_filter()));
    else
        where_strings["groups"] = "t.subgroup_id IN (SELECT id FROM " + SUBGROUPS_TABLE + " WHERE group_id = " + QString::number(filters->group_filter()-ENLARGER) + ")";

    /*
              ТЕКСТОВЫЙ ФИЛЬТР
            where_strings["text"]
                                        */
    // из трёх режимов может одновременно применяться только один
    if(filters->white_id_filter() != 0)
        // фильтр по id (белому)
        where_strings["text"] += QString("t.id") + (filters->white_id_mode_filter() == EQUAL ? " = " : " >= ") + QString::number(filters->white_id_filter());
    else if(filters->beginname_filter() != "")
        // фильтр по имени (с начала)
        where_strings["text"] += "t.pattern LIKE '" + filters->beginname_filter() + "%'";
    else if(!filters->parts_filter().empty())
        // применяем фильтр по фрагментам
        where_strings["text"] = "t.pattern LIKE '" + filters->parts_filter().join("' OR t.pattern LIKE '") + "'";

    /*
           ФИЛЬТРЫ ПО ПАРАМЕТРАМ
          where_strings["par1..12"]
                                        */
    for(int i = 0; i < MAX_PARAMS; i++){
        if(filters->params_filter().at(i) != ANY_ITEM_TEXT){
            where_strings[QString("par%1_val").arg(i+1)] = QString("par%1_val = '%2'").arg(QString::number(i+1)).arg(filters->params_filter().at(i));
            selectors.at(i)->set_edit_text(filters->params_filter().at(i) == "" ? "пусто" : filters->params_filter().at(i));
        }
        else
            selectors.at(i)->set_edit_text("");
    }

    /*
              ФИЛЬТР ПО ЦЕНАМ
           where_strings["prices"]
                                        */
    if(filters->prices_filter() == POSITIVE_PRICES)
        where_strings["prices"] = "t.price_ret > 0";
    else if(filters->prices_filter() == ZERO_PRICES)
        where_strings["prices"] = "t.price_ret = 0";

    /*
            ФИЛЬТР ПО КОЛИЧЕСТВУ
         where_strings["quantities"]
                                        */
    if(filters->quantity_filter() == POSITIVE_QUANTITIES)
        where_strings["quantities"] = "t.quantity > 0";

    where = glue_where();
    // Для переключателя страниц необходимо подсчитать общее количество записей, удовлетворяющих фильтрам (кроме limits).
    // В table же мы передаём передаём только записи по количеству limits. Поэтому сформируем запрос для подсчёта общего кол-ва записей,
    // выполним его и забудем про это дело.
    count = "SELECT count(*) FROM " + TOVMARKS_TABLE + " t " + where;
    total = set_totals(count);
    if(total <= 0){
        while(!base.commit());
        return "FAIL";
    }
    else
        white_table->set_totals(total);
    // порядок сортировки
    order = " ORDER BY " + sorting_order_to_string(filters->white_sort_order_filter());

    // лимиты
    if(filters->are_there_limits())
        limits = " OFFSET " + QString::number(filters->begin()) + " LIMIT " + QString::number(this->white_table->get_items_on_page());

    // если все детали, удовлетворяющие запросу, принадлежат только одной подгруппе, её номер надо установить в single_group
    // то же, но без учёта лимитов - в single_group_without_limits. Потом объяснить, что к чему!
    // а вообще это не дело, впилить потом в куда-то
    QSqlQuery select_groups(base);
    query_success = true;
    query = "SELECT DISTINCT t.subgroup_id FROM (SELECT " + WHITE_TABLE_COLUMNS.join(",") + ", t.subgroup_id " + tables_str + where + ") AS t";
    if(!select_groups.exec(query))
        query_success = false;
    select_groups.next();
    single_group_without_limits = (select_groups.size() == 1 ? select_groups.value(0).toInt() : -1);
    if(single_group_without_limits != -1)
        single_group = single_group_without_limits;
    else{
        query = "SELECT DISTINCT t.subgroup_id FROM (SELECT " + WHITE_TABLE_COLUMNS.join(",") + ", t.subgroup_id " + tables_str + where + limits + ") AS t";
        if(!select_groups.exec(query))
            query_success = false;
        select_groups.next();
        single_group = (select_groups.size() == 1 ? select_groups.value(0).toInt() : -1);
    }
    if(!query_success)
        qDebug() << QString("Не удалось выполнить запрос. %1").arg(select_groups.lastError().text());

    // теперь сформируем строку, которую надо вернуть функции заполнения таблицы
    final = "SELECT " + filters->columns_filter().join(",") + tables_str + where + groupby + order + limits;
    while(!base.commit());
    //qDebug() << final;
    return final;
}


