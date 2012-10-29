#include "greysearcher.h"

GreySearcher::GreySearcher(QWidget *parent) :
    QWidget(parent)
{
    grey_table = new GreyTable(this);
    input = new Input(this);
    storages = new CustomComboBox(this);
    racks = new CustomComboBox(this);
    boards = new CustomComboBox(this);
    boxes = new CustomComboBox(this);
    storages->setEnabled(false);
    racks->setEnabled(false);
    boards->setEnabled(false);
    boxes->setEnabled(false);
    reset_boxes = new QPushButton("X", this);
    one_year = new CustomComboBox(this);
    from_year = new CustomComboBox(this);
    to_year = new CustomComboBox(this);
    oneyear_mode = new QRadioButton("год", this);
    multiyear_mode = new QRadioButton("диап.", this);
    oneyear_mode->setChecked(true);
    reset_years_button = new QPushButton("X", this);
    insp_box = new QComboBox(this);
    reset_insp_button = new QPushButton("X", this);
    // для доп. параметров
    add_info_mode = new QRadioButton("доп.пар.1");
    defect_mode = new QRadioButton("доп.пар.2");
    category_mode = new QRadioButton("категория");
    add_info_mode->setChecked(true);
    add_info_box = new CustomComboBox();
    defect_box = new CustomComboBox();
    category_box = new CustomComboBox();
    reset_add_boxes = new QPushButton("X");
    add_info_box->setAutoCompletion(true);

    // BUTTONS
    new_button = new QPushButton();
    edit_button = new QPushButton();
    del_button = new QPushButton();
    reset_button = new QPushButton();

    filled = false;
    years_box_filled = false;
    fill_insp_box();
    clear_where_strs();
    set_layout();
    hide_second_box();
    connects();
}

void GreySearcher::connects(){
    QObject::connect(this->grey_table, SIGNAL(double_click(QModelIndex)), SIGNAL(close_grey(QModelIndex)));
    // смена пределов
    QObject::connect(this->grey_table, SIGNAL(limits_changed(pair)), SIGNAL(limits_changed(pair)));
    // снятие лимита
    QObject::connect(this->grey_table, SIGNAL(limits_removed()), SIGNAL(limits_removed()));
    // установка лимита
    QObject::connect(this->grey_table, SIGNAL(limits_restored()), SIGNAL(limits_restored()));
    // изменение порядка сортировки
    QObject::connect(this->grey_table, SIGNAL(sort_order_changed(int,Qt::SortOrder)), SIGNAL(sort_order_changed(int,Qt::SortOrder)));
    // изменение текста
    QObject::connect(this->input, SIGNAL(text_changed_signal(int,QString)), SLOT(text_changed_slot(int,QString)));
    // очистка текста
    QObject::connect(this->input, SIGNAL(reset_signal()), SIGNAL(reset_signal()));
    QObject::connect(this->storages, SIGNAL(activated(QString)), SLOT(storages_slot(QString)));
    QObject::connect(this->racks, SIGNAL(activated(QString)), SLOT(racks_slot(QString)));
    QObject::connect(this->boards, SIGNAL(activated(QString)), SLOT(boards_slot(QString)));
    QObject::connect(this->boxes, SIGNAL(activated(QString)), SLOT(boxes_slot(QString)));
    QObject::connect(this->reset_boxes, SIGNAL(clicked()), SLOT(reset_boxes_slot()));
    QObject::connect(this->one_year, SIGNAL(activated(QString)), SLOT(one_year_changed(QString)));
    QObject::connect(this->from_year, SIGNAL(activated(QString)), SLOT(from_year_changed(QString)));
    QObject::connect(this->to_year, SIGNAL(activated(QString)), SLOT(to_year_changed(QString)));
    QObject::connect(this->oneyear_mode, SIGNAL(clicked()), SLOT(change_mode()));
    QObject::connect(this->multiyear_mode, SIGNAL(clicked()), SLOT(change_mode()));
    QObject::connect(this->reset_years_button, SIGNAL(clicked()), SIGNAL(reset_years()));
    QObject::connect(this->one_year, SIGNAL(fill_box_signal()), SLOT(fill_one_year_box()));
    QObject::connect(this->from_year, SIGNAL(fill_box_signal()), SLOT(fill_from_year_box()));
    QObject::connect(this->to_year, SIGNAL(fill_box_signal()), SLOT(fill_to_year_box()));
    QObject::connect(this->storages, SIGNAL(fill_box_signal()), SLOT(fill_storages_box()));
    QObject::connect(this->racks, SIGNAL(fill_box_signal()), SLOT(fill_racks_box()));
    QObject::connect(this->boards, SIGNAL(fill_box_signal()), SLOT(fill_boards_box()));
    QObject::connect(this->boxes, SIGNAL(fill_box_signal()), SLOT(fill_boxes_box()));
    QObject::connect(this->insp_box, SIGNAL(activated(QString)), SIGNAL(change_insp_signal(QString)));
    QObject::connect(this->reset_insp_button, SIGNAL(clicked()), SIGNAL(reset_insp_signal()));
    QObject::connect(this->add_info_mode, SIGNAL(clicked()), SLOT(change_add_mode()));
    QObject::connect(this->defect_mode, SIGNAL(clicked()), SLOT(change_add_mode()));
    QObject::connect(this->category_mode, SIGNAL(clicked()), SLOT(change_add_mode()));
    QObject::connect(this->add_info_box, SIGNAL(fill_box_signal()), SLOT(fill_add_info_box()));
    QObject::connect(this->defect_box, SIGNAL(fill_box_signal()), SLOT(fill_defect_box()));
    QObject::connect(this->category_box, SIGNAL(fill_box_signal()), SLOT(fill_category_box()));
    QObject::connect(this->add_info_box, SIGNAL(activated(QString)), SIGNAL(change_add_info_signal(QString)));
    QObject::connect(this->defect_box, SIGNAL(activated(QString)), SIGNAL(change_defect_signal(QString)));
    QObject::connect(this->category_box, SIGNAL(activated(QString)), SIGNAL(change_category_signal(QString)));
    QObject::connect(this->reset_add_boxes, SIGNAL(clicked()), SIGNAL(reset_add_boxes_signal()));
    QObject::connect(this->reset_button, SIGNAL(clicked()), SLOT(total_reset_slot()));
    //
    QObject::connect(this->grey_table, SIGNAL(section_resized(int, int)), SIGNAL(section_resized(int, int)));
    QObject::connect(this->grey_table, SIGNAL(section_moved()), SIGNAL(section_moved()));

    //QObject::connect(this)
}


QVBoxLayout* GreySearcher::create_box_layout(QComboBox *box, QString label_str, int width){
    QVBoxLayout *vbox = new QVBoxLayout();
    QLabel *label = new QLabel(label_str);
    label->adjustSize();
    label->setFixedSize(label->width(), label->height());
    box->setFixedWidth(width);
    vbox->addWidget(label);
    vbox->addWidget(box);
    return vbox;
}

void GreySearcher::set_layout(){
    QWidget *top_panel = new QWidget(this);
    QWidget *bottom_panel = new QWidget(this);

    // устанавливаем размеры элементов
    int input_width = 300;      // ширина текстового поля
    int button_size = 25;       // ширина кнопки сброса
    int spacing = 5;            // отступ до кнопки сброса

    // лайаут для виджета с кнопками и текстовым полем
    QHBoxLayout *top_lt = new QHBoxLayout();
    top_lt->setMargin(0);
    top_lt->setSpacing(0);

    // добавляем кнопки, которые пока не созданы, лол
    int action_button_size = 45;
    new_button->setFixedSize(action_button_size, action_button_size);
    edit_button->setFixedSize(action_button_size, action_button_size);
    del_button->setFixedSize(action_button_size, action_button_size);
    reset_button->setFixedSize(action_button_size, action_button_size);
    reset_button->setIcon(QPixmap(":/reset"));
    reset_button->setToolTip("Сброс");
    QHBoxLayout *btn_lt = new QHBoxLayout();
    btn_lt->addWidget(new_button);
    btn_lt->addWidget(edit_button);
    btn_lt->addWidget(del_button);
    btn_lt->addWidget(reset_button);
    QWidget *btn_wgt = new QWidget(this);
    btn_wgt->setFixedWidth((action_button_size+2)*4);
    btn_wgt->setLayout(btn_lt);
    top_lt->addWidget(btn_wgt);
    top_lt->addStretch(1);

    // добавляем текстовое поле
    top_lt->addWidget(input);
    input->setFixedSize(input_width, 70);
    top_panel->setLayout(top_lt);
    top_panel->setFixedHeight(input->height());

    // лайаут для панели с комбобоксами
    QHBoxLayout *bottom_lt = new QHBoxLayout();
    int selector_width = qApp->desktop()->width()/14;
    bottom_lt->setSpacing(0);
    bottom_lt->setMargin(0);
    // над каждым комбобоксом должна быть надпись,
    // так что надпись+комбобокс запихнуть в отдельный лайаут
    QVBoxLayout *storages_vbox = create_box_layout(storages, "Склад", selector_width);
    QVBoxLayout *racks_vbox = create_box_layout(racks, "Стеллаж", selector_width);
    QVBoxLayout *boards_vbox = create_box_layout(boards, "Полка", selector_width);
    QVBoxLayout *boxes_vbox = create_box_layout(boxes, "Ящик", selector_width);
    //storages_vbox->setGeometry(selector_width,0,selector_width,storages_vbox);
    //racks_vbox->setGeometry(selector_width*2,0,selector_width,storages_vbox->heightForWidth(selector_width));
    //boards_vbox->setGeometry(selector_width*3,0,selector_width,storages_vbox->heightForWidth(selector_width));
    //boxes_vbox->setGeometry(selector_width*4,0,selector_width,storages_vbox->heightForWidth(selector_width));
    reset_boxes->setFixedSize(button_size, button_size);

    // элементы для выбора годов:
    // лейбл "Год", две радиокнопки друг над другом, лейбл "С", комбобокс 1, лейбл "по", комбобокс 2
    // два лейбла и комбобокс 2 периодически скрываются
    // уместно поместить всё это в отдельный виджет
    from_year_label = new QLabel("С");
    to_year_label = new QLabel("По");
    from_year_label->adjustSize();
    from_year_label->setFixedSize(from_year_label->width(), from_year_label->height());
    to_year_label->adjustSize();
    to_year_label->setFixedSize(to_year_label->width(), to_year_label->height());
    one_year->setFixedWidth(selector_width);
    from_year->setFixedWidth(selector_width);
    to_year->setFixedWidth(selector_width);
    reset_years_button->setFixedSize(button_size, button_size);
    reset_insp_button->setFixedSize(button_size, button_size);
    QVBoxLayout *years_buttons_lt = new QVBoxLayout();
    years_buttons_lt->addWidget(oneyear_mode);
    years_buttons_lt->addWidget(multiyear_mode);
    QHBoxLayout *years_hlt = new QHBoxLayout();
    years_hlt->addLayout(years_buttons_lt);
    years_hlt->addWidget(one_year);
    years_hlt->addWidget(from_year_label);
    years_hlt->addWidget(from_year);
    years_hlt->addWidget(to_year_label);
    years_hlt->addWidget(to_year);
    years_hlt->addStretch(1);
    years_hlt->addWidget(reset_years_button);
    QLabel *year_label = new QLabel("Год");
    year_label->adjustSize();
    year_label->setFixedSize(year_label->width(), year_label->height());
    QVBoxLayout *years_lt = new QVBoxLayout();
    years_lt->addWidget(year_label);
    years_lt->addLayout(years_hlt);
    QWidget *years_wgt = new QWidget(this);
    years_wgt->setFixedWidth(multiyear_mode->width() + from_year_label->width()
                             + from_year->width() + to_year_label->width() + to_year->width() + button_size);
    years_wgt->setLayout(years_lt);

    // приёмка
    QVBoxLayout *insp_lt = create_box_layout(insp_box, "Приёмка", selector_width);

    // лайаут для доп. параметров
    category_mode->adjustSize();
    category_mode->setFixedWidth(category_mode->width());
    int add_box_width = selector_width * 2;
    QVBoxLayout *add_mode_lt = new QVBoxLayout();
    add_mode_lt->addWidget(add_info_mode);
    add_mode_lt->addWidget(defect_mode);
    add_mode_lt->addWidget(category_mode);
    QHBoxLayout *add_lt = new QHBoxLayout();
    add_info_box->setFixedWidth(add_box_width);
    defect_box->setFixedWidth(add_box_width);
    category_box->setFixedWidth(add_box_width);
    reset_add_boxes->setFixedSize(button_size, button_size);
    add_lt->addLayout(add_mode_lt);
    add_lt->addWidget(add_info_box);
    add_lt->addWidget(defect_box);
    add_lt->addWidget(category_box);
    add_lt->addSpacing(spacing);
    add_lt->addWidget(reset_add_boxes);
    QWidget *add_wgt = new QWidget();
    add_wgt->setFixedWidth(category_mode->width() + add_box_width + spacing + button_size + 20);
    add_wgt->setLayout(add_lt);

    bottom_lt->addLayout(storages_vbox);
    bottom_lt->addLayout(racks_vbox);
    bottom_lt->addLayout(boards_vbox);
    bottom_lt->addLayout(boxes_vbox);
    bottom_lt->addSpacing(spacing);
    bottom_lt->addWidget(reset_boxes);
    bottom_lt->addSpacing(spacing);
    bottom_lt->addWidget(years_wgt);
    bottom_lt->addSpacing(spacing);
    bottom_lt->addLayout(insp_lt);
    bottom_lt->addSpacing(spacing);
    bottom_lt->addWidget(reset_insp_button);
    bottom_lt->addSpacing(spacing);
    bottom_lt->addStretch(1);
    bottom_lt->addWidget(add_wgt);
    //top_lt->addWidget(add_wgt);
    //params_reset->setFixedSize(button_size, button_size);
    bottom_panel->setLayout(bottom_lt);
    bottom_panel->setFixedWidth(selector_width*5 + spacing*5 + button_size*2 + years_wgt->width() + add_wgt->width());
    bottom_panel->setFixedHeight(storages->height() + 40);


    QVBoxLayout *vbl = new QVBoxLayout();
    vbl->addWidget(top_panel);
    vbl->addWidget(bottom_panel);
    vbl->addWidget(grey_table);
    this->setLayout(vbl);
    hide_second_box();
    show_add_info_box();
}

void GreySearcher::restore_width(int index, int width){
    if(grey_table != 0)
        this->grey_table->restore_width(index, width);
}

int GreySearcher::open_columns_list(){
    if(grey_table != 0)
        return grey_table->open_columns_list();
}

void GreySearcher::restore_order(){
    this->grey_table->restore_order();
}

void GreySearcher::hide_show_columns(){
    if(grey_table != 0)
        grey_table->hide_show_columns();
}

void GreySearcher::clear_where_strs(){
    where_strings["id"] = "";
    where_strings["text"] = "";
    where_strings["places"] = "";
    where_strings["years"] = "";
    where_strings["insp"] = "";
    where_strings["add_info"] = "";
    where_strings["defect"] = "";
    where_strings["category"] = "";
}

void GreySearcher::clear_text(){
    this->input->set_text("");
}

void GreySearcher::change_mode(){
    if(oneyear_mode->isChecked()){
        // выбран режим одного года.
        // скрываем второй бокс и лейбл
        hide_second_box();
    }
    else if(multiyear_mode->isChecked()){
        // показываем второй бокс и лейбл
        show_second_box();
    }
}

void GreySearcher::hide_second_box(){
    this->from_year_label->hide();
    this->from_year->hide();
    this->to_year_label->hide();
    this->to_year->hide();
    this->one_year->show();
}

void GreySearcher::change_add_mode(){
    if(add_info_mode->isChecked())
        show_add_info_box();
    else if(defect_mode->isChecked())
        show_defect_box();
    else if(category_mode->isChecked())
        show_category_box();
}

void GreySearcher::show_add_info_box(){
    this->defect_box->hide();
    this->category_box->hide();
    this->add_info_box->show();
}

void GreySearcher::show_defect_box(){
    this->add_info_box->hide();
    this->category_box->hide();
    this->defect_box->show();
}

void GreySearcher::show_category_box(){
    this->add_info_box->hide();
    this->defect_box->hide();
    this->category_box->show();
}

void GreySearcher::show_second_box(){
    this->from_year_label->show();
    this->from_year->show();
    this->to_year_label->show();
    this->to_year->show();
    this->one_year->hide();
}

void GreySearcher::reset_boxes_slot(){
    clear_boxes();
    emit reset_boxes_signal();
}

void GreySearcher::text_changed_slot(int type, QString text){
    clear_boxes();
    emit text_changed_signal(GREY_MODE, type, text);
}

int GreySearcher::set_totals(QString query){
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

int GreySearcher::input_id_combobox_value(){
    return this->input->id_combobox_value();
}


int GreySearcher::get_original_index(QString name){
    return original_columns_names.indexOf(name);
}

QString GreySearcher::get_original_name(int index){
    return original_columns_names[index];
}

void GreySearcher::fill_storages_box(){
    fill_places_box(1);
}

void GreySearcher::fill_racks_box(){
    fill_places_box(2);
}

void GreySearcher::fill_boards_box(){
    fill_places_box(3);
}

void GreySearcher::fill_boxes_box(){
    fill_places_box(4);
}

bool GreySearcher::fill_places_box(int mode){
    QSqlQueryModel *query = new QSqlQueryModel();
    QString which, cur_filter, query_str;
    CustomComboBox *box = 0, *next = 0;
    int cur_index;
    switch(mode){
    case 1:
        which = "storage";
        box = storages;
        next = racks;
        cur_filter = filters->grey_storage_filter();
        break;
    case 2:
        which = "rack";
        box = racks;
        next = boards;
        cur_filter = filters->grey_rack_filter();
        break;
    case 3:
        which = "board";
        box = boards;
        next = boxes;
        cur_filter = filters->grey_board_filter();
        break;
    case 4:
        which = "box";
        box = boxes;
        cur_filter = filters->grey_box_filter();
        break;
    default:
        return false;
    }

    query_str = QString("(SELECT '") + ANY_ITEM_TEXT + "') UNION (SELECT DISTINCT p." + which + tables_str + glue_where("places");
    if(mode >= 2 && filters->grey_storage_filter() != NOPLACE_TEXT){
        query_str += " AND p.storage = '" + filters->grey_storage_filter() + "'";
        if(mode >= 3 && filters->grey_rack_filter() != NOPLACE_TEXT){
            query_str += " AND p.rack = '" + filters->grey_rack_filter() + "'";
            if(mode >= 4 && filters->grey_board_filter() != NOPLACE_TEXT)
                query_str += " AND p.board = '" + filters->grey_board_filter() + "'";
        }
    }
    query_str += " ORDER BY 1)";
    query->setQuery(query_str, base);
    if(query->lastError().isValid()){
        error("Ошибка при заполнении комбобокса " + which, query->lastError().text());
        //box->setEnabled(false);
        return false;
    }
    box->setModel(query);
    if(mode < 4) next->setEnabled(true);
    if(cur_filter == NOPLACE_TEXT){
        cur_index = find_index(box->model(), ANY_ITEM_TEXT);
        if(box->count() == 2 && cur_index >= 0){
            cur_index = (cur_index== 0 ? 1 : 0);
            box->setCurrentIndex(cur_index);
            // EMIT!
            switch(mode){
            case 1:
                filters->set_grey_storage(box->itemText(cur_index));
                break;
            case 2:
                filters->set_grey_rack(box->itemText(cur_index));
                break;
            case 3:
                filters->set_grey_board(box->itemText(cur_index));
                break;
            case 4:
                filters->set_grey_box(box->itemText(cur_index));
                break;
            }
            return (mode <= 3 ? fill_places_box(++mode) : true);
        }
        else{
            box->setCurrentIndex(find_index(box->model(), ANY_ITEM_TEXT));
            box->setEnabled(true);
            return true;
        }
    }
    else{
        box->setEnabled(true);
        cur_index = find_index(box->model(), cur_filter);
        cur_index = (cur_index >= 0 ? cur_index : find_index(box->model(), ANY_ITEM_TEXT)); // защита от дурака
        box->setCurrentIndex(cur_index);
        return true;
    }
    return true;
}

void GreySearcher::storages_slot(QString str){
    racks->setEditText("");
    boards->setEditText("");
    boxes->setEditText("");
    boards->setEnabled(false);
    boxes->setEnabled(false);
    emit storage_signal(str);
}

void GreySearcher::racks_slot(QString str){
    boards->setEditText("");
    boxes->setEditText("");
    boxes->setEnabled(false);
    emit rack_signal(str);
}

void GreySearcher::boards_slot(QString str){
    boxes->setEditText("");
    emit board_signal(str);
}

void GreySearcher::boxes_slot(QString str){
    emit box_signal(str);
}

QString GreySearcher::year_text(QString year){
    if(year == "-1" || year == "")
        return ANY_ITEM_TEXT;
    else if(year == "0")
        return NOYEAR_TEXT;
    else
        return year;
}

void GreySearcher::fill_add_info_box(){
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query_str;
    QString cur = filters->grey_add_info_filter();

    query_str = "(SELECT '" + ANY_ITEM_TEXT + "') UNION (SELECT DISTINCT g.add_info " + tables_str + glue_where("add_info") + ") ORDER BY 1";
    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        error("Ошибка при заполнении доп. параметров 1", model->lastError().text());
        add_info_box->setEnabled(false);
        return;
    }
    else{
        add_info_box->setModel(model);
        if(add_info_box->count() == 2){
            if(add_info_box->itemText(0) == ANY_ITEM_TEXT)
                add_info_box->setCurrentIndex(1);
            else if(add_info_box->itemText(1) == ANY_ITEM_TEXT)
                add_info_box->setCurrentIndex(0);
        }
        else{
            int index = -1;
            if(cur.size() > 0) index = find_index(model, cur);
            if(index >= 0)
                add_info_box->setCurrentIndex(index);
            else
                add_info_box->setEditText(cur == NOINFO_TEXT ? ANY_ITEM_TEXT : cur);
        }
    }
}

void GreySearcher::fill_defect_box(){
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query_str;
    QString cur = filters->grey_defect_filter();

    query_str = "(SELECT '" + ANY_ITEM_TEXT + "') UNION (SELECT DISTINCT g.defect " + tables_str + glue_where("defect") + ") ORDER BY 1";
    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        error("Ошибка при заполнении доп. параметров 2", model->lastError().text());
        defect_box->setEnabled(false);
        return;
    }
    else{
        defect_box->setModel(model);
        if(defect_box->count() == 2){
            if(defect_box->itemText(0) == ANY_ITEM_TEXT)
                defect_box->setCurrentIndex(1);
            else if(defect_box->itemText(1) == ANY_ITEM_TEXT)
                defect_box->setCurrentIndex(0);
        }
        else{
            int index = -1;
            if(cur.size() > 0) index = find_index(model, cur);
            if(index >= 0)
                defect_box->setCurrentIndex(index);
            else
                defect_box->setEditText(cur == NOINFO_TEXT ? ANY_ITEM_TEXT : cur);
        }
    }
}

void GreySearcher::fill_category_box(){
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query_str;
    QString cur = filters->grey_string_category_filter();

    query_str = "(SELECT '" + ANY_ITEM_TEXT + "') UNION (SELECT DISTINCT c.category_name " + tables_str + glue_where("category") + ") ORDER BY 1";
    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        error("Ошибка при заполнении категорий", model->lastError().text());
        category_box->setEnabled(false);
        return;
    }
    else{
        category_box->setModel(model);
        if(category_box->count() == 2){
            if(category_box->itemText(0) == ANY_ITEM_TEXT)
                category_box->setCurrentIndex(1);
            else if(category_box->itemText(1) == ANY_ITEM_TEXT)
                category_box->setCurrentIndex(0);
        }
        else{
            int index = -1;
            if(cur.size() > 0) index = find_index(model, cur);
            if(index >= 0)
                category_box->setCurrentIndex(index);
            else
                category_box->setEditText(cur == NOINFO_TEXT ? ANY_ITEM_TEXT : cur);
        }
    }
}

bool GreySearcher::fill_year_box(int mode){
    TableModelForYears *model = new TableModelForYears();
    QString query_str, cur_year;
    CustomComboBox *box;
    switch(mode){
    case 1:
        cur_year = filters->grey_one_year_filter();
        box = one_year;
        break;
    case 2:
        cur_year = filters->grey_from_year_filter();
        box = from_year;
        break;
    case 3:
        cur_year = filters->grey_to_year_filter();
        box = to_year;
        break;
    default:
        return false;
    }

    query_str = "(SELECT -1) UNION (SELECT DISTINCT year " + tables_str + glue_where("years");
    if(mode == 2)
        query_str += (filters->grey_to_year_filter().toInt() > 0 ? " AND (year <= " + filters->grey_to_year_filter() + ")" : "");
    else if(mode == 3)
        query_str += (filters->grey_from_year_filter().toInt() > 0 ? " AND (year >= " + filters->grey_from_year_filter() + ")" : "");
    query_str += ") ORDER BY 1";
    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        error("Ошибка при заполнении комбобокса " + QString::number(mode), model->lastError().text());
        box->setEnabled(false);
        return false;
    }
    else{
        box->setModel(model);
        if(box->count() == 2 && box->itemText(0) == "")
            box->setCurrentIndex(1);
        else{
            int index = find_index(model, cur_year);
            if(index >= 0)
                box->setCurrentIndex(find_index(model, cur_year));
            else
                box->setEditText(year_text(cur_year));
        }
    }
    return true;
}

void GreySearcher::fill_one_year_box(){
    fill_year_box(1);
}

void GreySearcher::fill_from_year_box(){
    fill_year_box(2);
}

void GreySearcher::fill_to_year_box(){
    fill_year_box(3);
}

void GreySearcher::fill_insp_box(){
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query_str = QString("(SELECT '") + ANY_ITEM_TEXT + "') UNION (SELECT DISTINCT insp_name FROM inspections ORDER BY 1)";
    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        error("Ошибка при получении яписка приёмок", model->lastError().text());
        return;
    }
    insp_box->setModel(model);
}

void GreySearcher::one_year_changed(QString str){
    int mode = (oneyear_mode->isChecked() ? ONE_YEAR_MODE : MULTI_YEAR_MODE);
    emit change_one_year_signal(str, mode);
}

void GreySearcher::from_year_changed(QString str){
    int mode = (oneyear_mode->isChecked() ? ONE_YEAR_MODE : MULTI_YEAR_MODE);
    emit change_from_year_signal(str, mode);
}

void GreySearcher::to_year_changed(QString str){
    int mode = (oneyear_mode->isChecked() ? ONE_YEAR_MODE : MULTI_YEAR_MODE);
    emit change_to_year_signal(str, mode);
}

void GreySearcher::clear_boxes(){
    storages->setEditText("");
    racks->setEditText("");
    boards->setEditText("");
    boxes->setEditText("");
    storages->clear();
    racks->clear();
    boards->clear();
    boxes->clear();
    storages->setEnabled(false);
    racks->setEnabled(false);
    boards->setEnabled(false);
    boxes->setEnabled(false);
    where_strings["places"] = "";
}

void GreySearcher::total_reset_slot(){
    clear_boxes();
    emit total_reset_signal();
}

void GreySearcher::set_years_filters(){
    if(oneyear_mode->isChecked()){
        if(filters->grey_one_year_filter().size() > 0)
            where_strings["years"] = " year = " + filters->grey_one_year_filter();
        this->one_year->setEditText(year_text(filters->grey_one_year_filter()));
    }
    if(multiyear_mode->isChecked()){
        if(filters->grey_from_year_filter().size() > 0)
            where_strings["years"] = " year >= " + filters->grey_from_year_filter();
        this->from_year->setEditText(year_text(filters->grey_from_year_filter()));
    }
    if(multiyear_mode->isChecked()){
        if(filters->grey_to_year_filter().size() > 0){
            if(where_strings["years"].size() > 0) where_strings["years"] += " AND ";
            where_strings["years"] += " year <= " + filters->grey_to_year_filter();
        }
        this->to_year->setEditText(year_text(filters->grey_to_year_filter()));
    }
}

bool GreySearcher::set_place_filter(int mode){
    QString cur, which;
    CustomComboBox *box = 0;
    switch(mode){
    case 0:
        cur = filters->grey_storage_filter();
        which = "storage ";
        box = storages;
        break;
    case 1:
        cur = filters->grey_rack_filter();
        which = "rack ";
        box = racks;
        break;
    case 2:
        cur = filters->grey_board_filter();
        which = "board ";
        box = boards;
        break;
    case 3:
        cur = filters->grey_box_filter();
        which = "box ";
        box = boxes;
        break;
    }

    if(cur == NOPLACE_TEXT){
        QSqlQuery *q = new QSqlQuery("(SELECT '" + ANY_ITEM_TEXT + "') UNION (SELECT DISTINCT p." + which + tables_str + glue_where("places") + " ORDER BY 1)", base);
             if(q->lastError().isValid()){
            error("Ошибка", q->lastError().text());
            return false;
        }
        if(q->size() == 2){
            q->next();
            if(q->value(0) == ANY_ITEM_TEXT) q->next();
            switch(mode){
            case 0:
                filters->set_grey_storage(q->value(0).toString());
                cur = filters->grey_storage_filter();
                break;
            case 1:
                filters->set_grey_rack(q->value(0).toString());
                cur = filters->grey_rack_filter();
                break;
            case 2:
                filters->set_grey_board(q->value(0).toString());
                cur = filters->grey_board_filter();
                break;
            case 3:
                filters->set_grey_box(q->value(0).toString());
                cur = filters->grey_box_filter();
                break;
            }
        }

        else return false;
    }
    if(where_strings["places"].size() > 0) where_strings["places"] += " AND ";
    where_strings["places"] += " p." + which + "= '" + cur + "' ";
    box->setEditText(cur);
    box->setEnabled(true);
    return true;
}

void GreySearcher::set_storages_filter(){
    where_strings["places"] = "";
    for(int i = 0; i < 4; i++)
        if(!set_place_filter(i)) break;
}

void GreySearcher::set_insp_filter(){
    where_strings["insp"] = "";
    if(filters->grey_insp_filter() >= 0)
        where_strings["insp"] += QString(" i.id = ") + QString::number(filters->grey_insp_filter());
    else
        insp_box->setCurrentIndex(find_index(insp_box->model(), ANY_ITEM_TEXT));
}

void GreySearcher::set_add_filter(){
    QString cur;
    cur = filters->grey_add_info_filter();
    if(cur != NOINFO_TEXT)
        where_strings["add_info"] = " g.add_info = '" + cur + "'";
    add_info_box->setEditText(cur == NOINFO_TEXT ? ANY_ITEM_TEXT : cur);
    cur = filters->grey_defect_filter();
    if(cur != NOINFO_TEXT)
        where_strings["defect"] = " g.defect = '" + cur + "'";
    defect_box->setEditText(cur == NOINFO_TEXT ? ANY_ITEM_TEXT : cur);
    int cat = filters->grey_category_filter();
    if(cat >= 0)
        where_strings["category"] = " g.category_id = " + QString::number(cat);
    category_box->setEditText(cat < 0 ? ANY_ITEM_TEXT : filters->grey_string_category_filter());
}

QString GreySearcher::glue_where(QStringList *ex){
    QString where;
    QStringList lst;
    QMap<QString, QString>::iterator it = where_strings.begin();
    for(; it != where_strings.end(); it++){
        if(ex != 0)
            if(ex->indexOf(it.key()) >= 0) continue;
        if(it.value().size() > 0)
            lst << it.value();
    }
    where = lst.join(" AND ");
    if(where.size() > 0)
        where = " WHERE " + where;
    return where;
}

QString GreySearcher::glue_where(QString str){
    QStringList lst;
    lst << str;
    return glue_where(&lst);
}

void GreySearcher::fill_table(Filters *f, bool delete_last_symbol){
    MyTableModel *model = new MyTableModel();
    this->filters = f;

    if(delete_last_symbol &&
            (filters->last_applied_filter() == GREY_TEXT_ID_FILTER ||
             filters->last_applied_filter() == GREY_BEGIN_TEXT_FILTER ||
             filters->last_applied_filter() == GREY_PARTS_TEXT_FILTER)){
        input->delete_last_symbol();
        return;
    }

    bool reset_page = false;
    if(filters->last_applied_filter() != GREY_LIMITS_FILTER){
        reset_page = true;
        pair l = {0, grey_table->get_items_on_page()};
        filters->set_grey_limits(l);
    }

    QString query_str = apply_filters();
    //qDebug() << query_str;
    if(query_str == "FAIL"){
        // если последний фильтр - текстовый, удаляем последний символ из текстового поля, таблица после этого перезаполнится
        // не удаляем, если это поиск по id в режиме '='
        if((filters->last_applied_filter() == GREY_TEXT_ID_FILTER && filters->grey_id_mode_filter() == GREATER_OR_EQUAL) ||
                filters->last_applied_filter() == GREY_BEGIN_TEXT_FILTER ||
                filters->last_applied_filter() == GREY_PARTS_TEXT_FILTER)
            input->delete_last_symbol();
    }
    else{
        model->setQuery(query_str, base);
        if(model->rowCount() == 0){
            QString empty = "SELECT '' as ", tmp;
            empty += GREY_TABLE_COLUMNS.join(", '' as ");
            qDebug() << empty;
            model->setQuery(empty, base);
        }
        original_columns_names.clear();
        for(int i = 0; i < model->columnCount(); i++)
            original_columns_names << model->headerData(i, Qt::Horizontal).toString();

        for(int i = 0; i < model->columnCount(); i++)
            model->setHeaderData(i, Qt::Horizontal, GREY_COLUMNS_NAMES[model->headerData(i, Qt::Horizontal).toString()]);
        // сохраняем столбец и порядок сортировки
        int sort_column;
        Qt::SortOrder sort_order;
        if(!(filters->grey_sort_order_filter().isEmpty())){
            SortingOrder s = filters->grey_sort_order_filter().first();
            sort_column = original_columns_names.indexOf(s.column);
            sort_order = s.order;
        }
        else {
            sort_column = original_columns_names.indexOf("id");
            sort_order = Qt::AscendingOrder;
            error("Внимание!", "Установлен порядок сортировки по умолчанию");
        }

        grey_table->fill(model, original_columns_names, sort_column, sort_order, reset_page);
    }
}

QString GreySearcher::apply_filters(){
    clear_where_strs();
    QString query, base, columns, order, limits, count, where;
    int total;
    int begin = filters->grey_begin();
    int limit = this->grey_table->get_items_on_page();
    short mode = filters->grey_id_mode_filter();
    QStringList parts = filters->grey_parts_filter();
    base = "SELECT ";
    columns = GREY_TABLE_COLUMNS.join(",");
    tables_str = QString(" FROM greytable g JOIN trademarks t ON g.trademark_id = t.id ")
            + " JOIN places p ON g.place_id = p.id "
            + " JOIN categories c ON g.category_id = c.id"
            + " JOIN inspections i ON g.inspection_id = i.id";


    /*
                ФИЛЬТР ПО ID
                                        */
    // результат - в строке результат в where_strings["id"]
    if(filters->grey_id_filter() > 0)
        where_strings["id"] = " trademark_id = " + QString::number(filters->grey_id_filter());

    /*
              ТЕКСТОВЫЙ ФИЛЬТР
                                        */
    // из трёх режимов может одновременно применяться только один, результат в where_strings["text"]
    if(filters->grey_text_id_filter() > 0)
        // фильтр по id (серому), введённому в текстовое поле
        where_strings["text"] = QString(" g.id ") + (mode == EQUAL ? " = " : " >= ") + QString::number(filters->grey_text_id_filter());
    else if(filters->grey_beginname_filter() != "")
        // применяем фильтр по имени (с начала)
        // ищем по началу, поэтому в конце запроса добавляем %
        where_strings["text"] = " t.pattern LIKE '" + filters->grey_beginname_filter() + "%'";
    else if(!parts.empty())
        // применяем фильтр по фрагментам
        where_strings["text"] = " t.pattern LIKE '" + parts.join("' OR t.pattern LIKE '") + "'";

    /*
              ФИЛЬТР ПО СКЛАДАМ
                                        */
    // результат в where_strings["places"]
    set_storages_filter();

    /*
              ФИЛЬТР ПО ГОДАМ
                                        */
    // результат в where_strings["years"]
    set_years_filters();

    /*
            ФИЛЬТР ПО ПРИЁМКАМ
                                        */
    // результат в where_strings["insp"]
    set_insp_filter();

    /*
          ФИЛЬТР ПО ДОП. ПАРАМЕТРАМ
                                        */
    // результат в where_strings["add_info", "defect" и "category"]
    set_add_filter();


    // склеиваем получившиеся куски в запрос where
    where = glue_where();

    // Для переключателя страниц необходимо подсчитать общее количество записей, удовлетворяющих фильтрам (кроме limits).
    // В table же мы передаём передаём только записи по количеству limits. Поэтому сформируем запрос для подсчёта общего кол-ва записей,
    // выполним его и забудем про это дело.
    count = "SELECT count(*) " + tables_str + where;
    total = set_totals(count);
    if(total <= 0){
        if(!(filters->last_applied_filter() == GREY_TEXT_ID_FILTER && filters->grey_text_id_filter() > 0 && mode == EQUAL)
            && ((filters->last_applied_filter() == GREY_TEXT_ID_FILTER && filters->grey_id_mode_filter() == GREATER_OR_EQUAL) ||
                filters->last_applied_filter() == GREY_BEGIN_TEXT_FILTER ||
                filters->last_applied_filter() == GREY_PARTS_TEXT_FILTER))
            return "FAIL";
        else{
            QString str = "SELECT '' as ";
            int i;
            for(i = 0; i < GREY_TABLE_COLUMNS.size()-1; i++){
                str += GREY_TABLE_COLUMNS.at(i).mid(GREY_TABLE_COLUMNS.at(i).indexOf('.')+1);
                str += ", '' as ";
            }
            str += GREY_TABLE_COLUMNS.at(i).mid(GREY_TABLE_COLUMNS.at(i).indexOf('.')+1);
            return str;
        }
    }
    else
        grey_table->set_totals(total);

    // порядок сортировки
    order = " ORDER BY " + sorting_order_to_string(filters->grey_sort_order_filter());

    // лимиты
    if(filters->are_there_grey_limits())
        limits = " OFFSET " + QString::number(begin) + " LIMIT " + QString::number(limit);

    query = base + columns + tables_str + where + order + limits;
    storages->setEnabled(true);

    return query;
}

TableModelForYears::TableModelForYears(QObject *parent) :
    QSqlQueryModel(parent){
}

QVariant TableModelForYears::data(const QModelIndex &index, int role) const{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole && QSqlQueryModel::data(index).toInt() == -1)
        return QString(ANY_ITEM_TEXT);

    if(role == Qt::DisplayRole && QSqlQueryModel::data(index).toInt() == 0 && QSqlQueryModel::data(index).toString().size() > 0)
        return QString(NOYEAR_TEXT);

    return QSqlQueryModel::data(index, role);
}
