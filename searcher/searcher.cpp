#include "searcher.h"

Searcher::Searcher(ReserveStruct *rstruct,
                   ColumnsStruct *white_columns,
                   ColumnsStruct *grey_columns,
                   QWidget *parent) :
    QWidget(parent)
{
    need_blue = get_privilege(Privileges::Blue_screen_access);
    //reserve = new ManagerReserveWidget(this);
    stack = new QStackedWidget();
    white_searcher = new WhiteSearcher(rstruct, need_blue, white_columns, stack, prev_widgets, this);
    grey_searcher = new GreySearcher(rstruct, need_blue, false, grey_columns, this);
    blue_searcher = 0;
    if(need_blue)
        blue_searcher = new GreySearcher(rstruct, need_blue, true, grey_columns, this);
    filters = new Filters();
    filters_default();
    //prev_widget = 0;
    current_reserve_doc = -1;
    white_searcher->fill_table(filters, false, false);
    success = white_searcher->success;
    GLOBAL_MODE = SEARCHER_WHITE_MODE;
    set_layout();
    connects();
    grey_connects(grey_searcher);
    if(need_blue)
        grey_connects(blue_searcher);
}


inline void Searcher::set_layout(){
    stack->addWidget(white_searcher);
    stack->addWidget(grey_searcher);
    if(need_blue)
        stack->addWidget(blue_searcher);
    stack->setCurrentWidget(white_searcher);

    //grey_searcher->hide();

    QHBoxLayout *hlt = new QHBoxLayout();
    hlt->addWidget(stack);
    this->setLayout(hlt);
}

void Searcher::switch_reserves(){
    white_searcher->switch_reserve();
    if(grey_searcher != 0)
        grey_searcher->switch_reserve();
    if(need_blue && blue_searcher != 0)
        blue_searcher->switch_reserve();
}

void Searcher::set_reserve_header(){
    if(!need_blue){
        if(!(this->white_searcher->isHidden()))
            this->white_searcher->set_reserve_header();
        else if(!(this->grey_searcher->isHidden()))
            this->grey_searcher->set_reserve_header();
    }
}

void Searcher::set_reserve_contragent(int id, QString name){
    if(!need_blue){
        if(this->white_searcher != 0)
            this->white_searcher->set_reserve_contragent(id, name);
        else if(this->grey_searcher != 0)
            this->grey_searcher->set_reserve_contragent(id, name);
    }
}

void Searcher::clear_reserve_contragent(){
    if(!need_blue){
        if(this->white_searcher != 0)
            this->white_searcher->clear_reserve_contragent();
        else if(this->grey_searcher != 0)
            this->grey_searcher->clear_reserve_contragent();
    }
}

void Searcher::resize_all(){
    white_searcher->resize_all();
}

void Searcher::set_date(){
    this->white_searcher->set_date();
}

void Searcher::restore_white_width(int index, int width){
    if(white_searcher != 0)
        white_searcher->restore_width(index, width);
}

void Searcher::restore_grey_width(int index, int width){
    if(grey_searcher != 0)
        grey_searcher->restore_width(index, width);
}

void Searcher::restore_manager_reserve_width(int index, int width){
    if(white_searcher != 0)
        white_searcher->restore_manager_reserve_width(index, width);
    if(grey_searcher != 0)
        grey_searcher->restore_manager_reserve_width(index, width);
}

void Searcher::restore_white_order(int logical, int newvisua){
    if(white_searcher != 0)
        white_searcher->restore_order(logical, newvisua);
}

void Searcher::restore_grey_order(int logical, int newvisual){
    if(grey_searcher != 0)
        grey_searcher->restore_order(logical, newvisual);
}

void Searcher::restore_manager_reserve_order(int logical, int newvisual){
    if(white_searcher != 0)
        white_searcher->restore_manager_reserve_order(logical, newvisual);
    if(grey_searcher != 0)
        grey_searcher->restore_manager_reserve_order(logical, newvisual);
}

int Searcher::open_white_columns_list(){
    if(white_searcher != 0)
        return white_searcher->open_columns_list();
    return -1;
}

int Searcher::open_grey_columns_list(){
    if(grey_searcher != 0)
        return grey_searcher->open_columns_list();
    return -1;
}

void Searcher::hide_show_white_columns(){
    if(white_searcher != 0)
        white_searcher->hide_show_columns();
}

void Searcher::hide_show_grey_columns(){
    if(grey_searcher != 0)
        grey_searcher->hide_show_columns();
}


inline void Searcher::filters_default(){
    // заполняем фильтры по умолчанию
    pair limits = {0, ITEMS_ON_PAGE};
    filters->set_limits(limits);
    filters->reset_white_id();
    filters->reset_beginname();
    filters->set_group_filter(0, "");
    filters->set_columns_filter(columns_white_table);
    filters->set_prices(ALL_PRICES);
    filters->set_quantity(ALL_QUANTITIES);
    SortingOrder order = {DEFAULT_WHITE_SORT_COLUMN, DEFAULT_WHITE_SORT_ORDER};
    filters->add_white_sort_column(order);
    filters->reset_params();

    filters->set_mode(WHITE_MODE);
    filters->set_last_filter(GROUPS_FILTER);
    grey_filters_default();

}

inline void Searcher::grey_filters_default(){
    pair limits = {0, ITEMS_ON_PAGE};
    filters->reset_grey_beginname();
    filters->reset_grey_text_id();
    filters->reset_grey_id();
    filters->set_grey_storage(NOPLACE_TEXT);
    filters->set_grey_rack(NOPLACE_TEXT);
    filters->set_grey_board(NOPLACE_TEXT);
    filters->set_grey_box(NOPLACE_TEXT);
    filters->set_grey_one_year("");
    filters->set_grey_from_year("");
    filters->set_grey_to_year("");
    filters->set_grey_limits(limits);
    filters->restore_grey_limits();
    SortingOrder grey_order = {"id", Qt::AscendingOrder};
    filters->add_grey_sort_column(grey_order);
    filters->clear_grey_insp();
    filters->clear_grey_add_info();
    filters->clear_grey_defect();
    filters->clear_grey_category();
    filters->set_grey_id_mode(EQUAL);
}

inline void Searcher::connects(){
    // сигналы от белого поисковика
    QObject::connect(this->white_searcher, SIGNAL(group_changed(int, QString)), SLOT(set_group_filter(int, QString)));
    QObject::connect(this->white_searcher, SIGNAL(limits_changed(pair)), SLOT(set_limits_filter(pair)));
    QObject::connect(this->white_searcher, SIGNAL(limits_removed()), SLOT(set_nolimits()));
    QObject::connect(this->white_searcher, SIGNAL(limits_restored()), SLOT(set_limits()));
    QObject::connect(this->white_searcher, SIGNAL(text_changed_signal(int, int, QString)), SLOT(set_text_filter(int, int, QString)));
    QObject::connect(this->white_searcher, SIGNAL(reset_signal()), SLOT(reset_white_text_filters()));
    QObject::connect(this->white_searcher, SIGNAL(sort_order_changed(int, Qt::SortOrder)), SLOT(change_sort_order(int, Qt::SortOrder)));
    QObject::connect(this->white_searcher, SIGNAL(item_selected(QString, int)), SLOT(set_param_filter(QString, int)));
    QObject::connect(this->white_searcher, SIGNAL(button_clicked(int)), SLOT(button_sort(int)));
    QObject::connect(this->white_searcher, SIGNAL(params_reset_clicked()), SLOT(reset_params_filter()));
    QObject::connect(this->white_searcher, SIGNAL(prices_clicked()), SLOT(switch_prices_filter()));
    QObject::connect(this->white_searcher, SIGNAL(quantity_clicked()), SLOT(switch_quantity_filter()));
    QObject::connect(this->white_searcher, SIGNAL(last_filter_changed(int)), SLOT(set_last_filter(int)));
    QObject::connect(this->white_searcher, SIGNAL(create_grey(int, QString)), SLOT(open_grey(int, QString)));
    QObject::connect(this->white_searcher, SIGNAL(create_blue(int, QString)), SLOT(open_blue(int, QString)));
    QObject::connect(this->white_searcher, SIGNAL(section_resized(int, int, QString)), SIGNAL(section_resized(int, int, QString)));
    QObject::connect(this->white_searcher, SIGNAL(section_moved(int, int, QString)), SIGNAL(section_moved(int, int, QString)));
    QObject::connect(this->white_searcher, SIGNAL(reset_param_signal(int)), SLOT(reset_param_filter(int)));
    QObject::connect(this->white_searcher, SIGNAL(open_settings()), SIGNAL(open_settings()));
    QObject::connect(this->white_searcher, SIGNAL(catalog_hides()), SIGNAL(catalog_hides()));
    QObject::connect(this->white_searcher, SIGNAL(catalog_shows()), SIGNAL(catalog_shows()));
    QObject::connect(this->white_searcher, SIGNAL(columns_changed()), SIGNAL(white_columns_changed()));
    QObject::connect(this->white_searcher, SIGNAL(need_refresh()), SIGNAL(need_white_refresh()));
    QObject::connect(this->white_searcher, SIGNAL(switch_reserve_signal()), SIGNAL(switch_reserve_signal()));
    QObject::connect(this->white_searcher, SIGNAL(refresh_searcher()), SIGNAL(need_total_refresh()));
    QObject::connect(this->white_searcher, SIGNAL(open_contragents(int, int)), SLOT(open_contragents(int, int)));
    QObject::connect(this->white_searcher, SIGNAL(clear_contragent()), SIGNAL(clear_reserve_contragent_signal()));
    QObject::connect(this->white_searcher, SIGNAL(open_docslist()), SLOT(open_docslist()));
}


inline void Searcher::grey_connects(GreySearcher *g){
    // сигналы от серого поисковика
    QObject::connect(g, SIGNAL(close_grey(QModelIndex)), SLOT(close_grey(QModelIndex)));
    QObject::connect(g, SIGNAL(limits_changed(pair)), SLOT(set_grey_limits_filter(pair)));
    QObject::connect(g, SIGNAL(limits_removed()), SLOT(set_grey_nolimits()));
    QObject::connect(g, SIGNAL(limits_restored()), SLOT(set_grey_limits()));
    QObject::connect(g, SIGNAL(sort_order_changed(int, Qt::SortOrder)), SLOT(change_grey_sort_order(int, Qt::SortOrder)));
    QObject::connect(g, SIGNAL(text_changed_signal(int, int, QString)), SLOT(set_text_filter(int, int, QString)));
    QObject::connect(g, SIGNAL(reset_signal()), SLOT(reset_grey_text_filters()));
    QObject::connect(g, SIGNAL(storage_signal(QString)), SLOT(change_grey_storage(QString)));
    QObject::connect(g, SIGNAL(rack_signal(QString)), SLOT(change_grey_rack(QString)));
    QObject::connect(g, SIGNAL(board_signal(QString)), SLOT(change_grey_board(QString)));
    QObject::connect(g, SIGNAL(box_signal(QString)), SLOT(change_grey_box(QString)));
    QObject::connect(g, SIGNAL(reset_boxes_signal()), SLOT(reset_grey_boxes()));
    QObject::connect(g, SIGNAL(change_one_year_signal(QString,int)), SLOT(change_grey_one_year(QString,int)));
    QObject::connect(g, SIGNAL(change_from_year_signal(QString,int)), SLOT(change_grey_from_year(QString,int)));
    QObject::connect(g, SIGNAL(change_to_year_signal(QString,int)), SLOT(change_grey_to_year(QString,int)));
    QObject::connect(g, SIGNAL(refresh()), SLOT(refresh_grey()));
    QObject::connect(g, SIGNAL(reset_years()), SLOT(reset_grey_years()));
    QObject::connect(g, SIGNAL(change_insp_signal(QString)), SLOT(change_grey_insp(QString)));
    QObject::connect(g, SIGNAL(reset_insp_signal()), SLOT(reset_grey_insp()));
    QObject::connect(g, SIGNAL(change_add_info_signal(QString)), SLOT(change_grey_add_info(QString)));
    QObject::connect(g, SIGNAL(change_defect_signal(QString)), SLOT(change_grey_defect(QString)));
    QObject::connect(g, SIGNAL(change_category_signal(QString)), SLOT(change_grey_category(QString)));
    QObject::connect(g, SIGNAL(reset_add_boxes_signal()), SLOT(reset_add_boxes()));
    QObject::connect(g, SIGNAL(total_reset_signal()), SLOT(grey_reset_slot()));
    QObject::connect(g, SIGNAL(section_resized(int, int, QString)), SIGNAL(section_resized(int, int, QString)));
    QObject::connect(g, SIGNAL(section_moved(int, int, QString)), SIGNAL(section_moved(int, int, QString)));
    QObject::connect(g, SIGNAL(columns_changed()), SIGNAL(grey_columns_changed()));
    QObject::connect(g, SIGNAL(open_settings()), SIGNAL(open_settings()));
    QObject::connect(g, SIGNAL(need_refresh()), SIGNAL(need_grey_refresh()));
    QObject::connect(g, SIGNAL(switch_reserve_signal()), SIGNAL(switch_reserve_signal()));
    QObject::connect(g, SIGNAL(refresh_searcher()), SIGNAL(need_total_refresh()));
    QObject::connect(g, SIGNAL(open_contragents(int, int)), SLOT(open_contragents(int, int)));
    QObject::connect(g, SIGNAL(clear_contragent()), SIGNAL(clear_reserve_contragent_signal()));

    if(g == blue_searcher){
        QObject::connect(g, SIGNAL(need_blue_refresh()), SIGNAL(need_blue_refresh()));
    }
}

void Searcher::refresh_white_table(){
    white_searcher->refresh_table();
}

void Searcher::refresh_grey_table(){
    if(grey_searcher != 0)
        grey_searcher->refresh_table();
}

void Searcher::refresh_blue_table(){
    if(need_blue && blue_searcher != 0){
        blue_searcher->refresh_table();
    }
}

void Searcher::show_white_catalog(){
    white_searcher->show_catalog();
}

void Searcher::hide_white_catalog(){
    white_searcher->hide_catalog();
}

int Searcher::mode(){
    return filters->mode();
}

void Searcher::reset_white_text_filters(){
    reset_text_filters(WHITE_MODE);
}

void Searcher::reset_grey_text_filters(){
    reset_text_filters(GREY_MODE);
}

void Searcher::reset_text_filters(int mode){
    if(mode == WHITE_MODE){
        this->filters->reset_params();
        this->filters->reset_white_id();
        this->filters->reset_beginname();
        this->white_searcher->clear_text();
    }
    else if(mode == GREY_MODE){
        this->filters->reset_grey_beginname();
        this->filters->reset_grey_text_id();
        this->grey_searcher->clear_text();
    }
    else if(mode == BLUE_MODE){
        this->filters->reset_grey_beginname();
        this->filters->reset_grey_text_id();
        this->blue_searcher->clear_text();
    }
    emit rename_tab("");
}

void Searcher::set_group_filter(int group, QString name){
    this->filters->reset_params();
    this->reset_text_filters(WHITE_MODE);
    this->filters->set_group_filter(group, name);
    filters->set_last_filter(GROUPS_FILTER);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, true, false);
    emit rename_tab(name);
}

void Searcher::reset_group_filter(){
    this->filters->reset_params();
    this->filters->set_group_filter(0, "");
    this->filters->set_last_filter(GROUPS_FILTER);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, true, false);
    emit rename_tab("");
}

void Searcher::set_limits_filter(pair limits){
    this->filters->set_limits(limits);
    filters->set_last_filter(LIMITS_FILTER);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, false, false);
}

void Searcher::set_nolimits(){
    this->filters->remove_limits();
    filters->set_last_filter(NOLIMITS_FILTER);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, false, false);
}

void Searcher::set_limits(){
    this->filters->restore_limits();
    filters->set_last_filter(SETLIMITS_FILTER);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, false, false);
}

void Searcher::set_grey_limits_filter(pair limits){
    this->filters->set_grey_limits(limits);
    filters->set_last_filter(GREY_LIMITS_FILTER);
    if(filters->mode() == GREY_MODE)
        grey_searcher->fill_table(this->filters, false);
    else if(filters->mode() == BLUE_MODE)
        blue_searcher->fill_table(this->filters, false);
}

void Searcher::set_grey_nolimits(){
    this->filters->remove_grey_limits();
    filters->set_last_filter(GREY_NOLIMITS_FILTER);
    if(filters->mode() == GREY_MODE)
        grey_searcher->fill_table(this->filters, false);
    else if(filters->mode() == BLUE_MODE)
        blue_searcher->fill_table(this->filters, false);
}

void Searcher::set_grey_limits(){
    this->filters->restore_grey_limits();
    filters->set_last_filter(GREY_SETLIMITS_FILTER);
    if(filters->mode() == GREY_MODE)
        grey_searcher->fill_table(this->filters, false);
    else if(filters->mode() == BLUE_MODE)
        blue_searcher->fill_table(this->filters, false);
}

void Searcher::change_grey_one_year(QString year, int mode){
    if(mode == ONE_YEAR_MODE)
        filters->set_grey_one_year(year);
    filters->set_last_filter(GREY_YEARS_FILTER);
    if(filters->mode() == GREY_MODE)
        grey_searcher->fill_table(this->filters, false);
    else if(filters->mode() == BLUE_MODE)
        blue_searcher->fill_table(this->filters, false);
}

void Searcher::change_grey_from_year(QString year, int mode){
    if(mode == MULTI_YEAR_MODE)
        filters->set_grey_from_year(year);
    filters->set_last_filter(GREY_YEARS_FILTER);
    if(filters->mode() == GREY_MODE)
        grey_searcher->fill_table(this->filters, false);
    else if(filters->mode() == BLUE_MODE)
        blue_searcher->fill_table(this->filters, false);
}

void Searcher::change_grey_to_year(QString year, int mode){
    if(mode == MULTI_YEAR_MODE)
        filters->set_grey_to_year(year);
    filters->set_last_filter(GREY_YEARS_FILTER);
    if(filters->mode() == GREY_MODE)
        grey_searcher->fill_table(this->filters, false);
    else if(filters->mode() == BLUE_MODE)
        blue_searcher->fill_table(this->filters, false);
}

void Searcher::reset_grey_years(){
    filters->set_grey_one_year("");
    filters->set_grey_from_year("");
    filters->set_grey_to_year("");
    if(filters->mode() == GREY_MODE)
        grey_searcher->fill_table(this->filters, false);
    else if(filters->mode() == BLUE_MODE)
        blue_searcher->fill_table(this->filters, false);
}

void Searcher::grey_reset_slot(){
    grey_filters_default();
    if(filters->mode() == GREY_MODE)
        grey_searcher->fill_table(this->filters, false);
    else if(filters->mode() == BLUE_MODE)
        blue_searcher->fill_table(this->filters, false);
}

void Searcher::set_text_filter(int mode, int type, QString text){
    if(mode == WHITE_MODE){
        // сбрасываем фильтр по группам
        this->filters->set_group_filter(0, "");
        // и по параметрам
        this->filters->reset_params();
        // и текущее значение текстового фильтра - тоже, если ранее текстовое поле было очищено
        //this->reset_text_filters();
    }
    else if(mode == GREY_MODE  || mode == BLUE_MODE){
        // сбрасываем фильтры по местам
        clear_grey_boxes();
    }
    if(type == ID_MODE){
        if(mode == WHITE_MODE){
            // устанавливаем фильтр по id
            this->filters->set_white_id(text.toInt());
            if(this->white_searcher->input_id_combobox_value() == 0)
                this->filters->set_white_id_mode(GREATER_OR_EQUAL);
            else if(this->white_searcher->input_id_combobox_value() == 1)
                this->filters->set_white_id_mode(EQUAL);
            filters->set_last_filter(ID_FILTER);
            emit rename_tab(QString("id: ").append(text));
        }
        else if(mode == GREY_MODE || mode == BLUE_MODE){
            GreySearcher *t = (mode == GREY_MODE ? grey_searcher : blue_searcher);
            this->filters->reset_grey_id();
            // устанавливаем фильтр по id
            this->filters->set_grey_text_id(text.toInt());
            if(t->input_id_combobox_value() == 0)
                this->filters->set_grey_id_mode(GREATER_OR_EQUAL);
            else if(t->input_id_combobox_value() == 1)
                this->filters->set_grey_id_mode(EQUAL);
            filters->set_last_filter(GREY_TEXT_ID_FILTER);
            emit rename_tab(QString("серый id: ").append(text));
        }
    }
    else if(type == BEGIN_MODE){
        // поиск с начала
        text = replases(text);
        if(mode == WHITE_MODE){
            this->filters->set_beginname(text.toUpper());
            filters->set_last_filter(BEGIN_TEXT_FILTER);
            emit rename_tab(QString("с нач.: ").append(text));
        }
        else if(mode == GREY_MODE || mode == BLUE_MODE){
            this->filters->reset_grey_id();
            this->filters->set_grey_beginname(text.toUpper());
            filters->set_last_filter(GREY_BEGIN_TEXT_FILTER);
            emit rename_tab(QString("серый с нач.: ").append(text));
        }
    }
    else if(type == PARTS_MODE){
        // поиск по фрагментам
        text = replases(text);
        // текст в текстовом поле надо преобразовать в список фрагментов
        QStringList parts = text.split(" ", QString::SkipEmptyParts);
        QStringList patterns;
        if(parts.size() != 0){
            patterns = valid_strings(parts);
            // а если ни одного валидного паттерна не нашлось,
            // то сразу вызываем fill_table, чтобы она удалила последний символ
            if(patterns.size() == 0 && filters->mode() == WHITE_MODE){
                white_searcher->fill_table(this->filters, false, true);
                return;
            }
            else if(patterns.size() == 0 && filters->mode() == GREY_MODE){
                grey_searcher->fill_table(this->filters, true);
                return;
            }
            else if(patterns.size() == 0 && filters->mode() == BLUE_MODE){
                blue_searcher->fill_table(this->filters, true);
                return;
            }
        }
        else{
            patterns.clear();
        }
        if(mode == WHITE_MODE){
            filters->set_parts(patterns);
            filters->set_last_filter(PARTS_TEXT_FILTER);
            emit rename_tab(QString("фрагм.: ").append(text));
        }
        else if(mode == GREY_MODE || mode == BLUE_MODE){
            this->filters->reset_grey_id();
            filters->set_grey_parts(patterns);
            filters->set_last_filter(GREY_PARTS_TEXT_FILTER);
            emit rename_tab(QString("серый фрагм.: ").append(text));
        }
    }
    else{
        // какая-то хрень, ничего не делаем
    }
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, false, false);
    else if(filters->mode() == GREY_MODE)
        grey_searcher->fill_table(this->filters, false);
    else if(filters->mode() == BLUE_MODE)
        blue_searcher->fill_table(this->filters, false);
}

void Searcher::change_sort_order(int column, Qt::SortOrder order){
    QString column_name = this->white_searcher->get_original_name(column);
    SortingOrder s = {column_name, order};
    this->filters->add_white_sort_column(s);
    filters->set_last_filter(SORT_FILTER);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, false, false);
}

void Searcher::change_grey_sort_order(int column, Qt::SortOrder order){
    QString column_name = this->grey_searcher->get_original_name(column);
    SortingOrder s = {column_name, order};
    this->filters->add_grey_sort_column(s);
    filters->set_last_filter(GREY_SORT_FILTER);
    if(filters->mode() == GREY_MODE)
        grey_searcher->fill_table(this->filters, false);
    if(filters->mode() == BLUE_MODE)
        blue_searcher->fill_table(this->filters, false);
}

void Searcher::button_sort(int num){
    int column_index = this->white_searcher->get_original_index("par" + QString::number(num+1) + "_val");
    int current_sort_index = this->white_searcher->get_original_index(this->filters->white_sort_order_filter().first().column);
    Qt::SortOrder order;
    if(column_index == current_sort_index)
        order = (filters->white_sort_order_filter().first().order == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
    else
        order = Qt::AscendingOrder;
    change_sort_order(column_index, order);
}

void Searcher::set_param_filter(QString param, int num){
    this->filters->set_param(num, param);
    filters->set_last_filter(PARAMS_FILTER);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, false, false);
}

void Searcher::reset_params_filter(){
    this->filters->reset_params();
    filters->set_last_filter(PARAMS_FILTER);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, false, false);
}

void Searcher::reset_param_filter(int index){
    this->filters->reset_param(index);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, false, false);
}

void Searcher::switch_prices_filter(){
    this->filters->switch_prices();
    filters->set_last_filter(PRICES_FILTER);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, false, false);
}

void Searcher::switch_quantity_filter(){
    this->filters->switch_quantity();
    filters->set_last_filter(QUANTITY_FILTER);
    if(filters->mode() == WHITE_MODE)
        white_searcher->fill_table(this->filters, false, false);
}

void Searcher::set_last_filter(int lf){
    filters->set_last_filter(lf);
}

void Searcher::change_grey_storage(QString st){
    filters->set_grey_storage(st);
    filters->set_grey_rack(NOPLACE_TEXT);
    filters->set_grey_board(NOPLACE_TEXT);
    filters->set_grey_box(NOPLACE_TEXT);
    set_last_filter(GREY_PLACE_FILTER);
    grey_searcher->fill_table(filters, false);
}

void Searcher::change_grey_rack(QString st){
    filters->set_grey_rack(st);
    filters->set_grey_board(NOPLACE_TEXT);
    filters->set_grey_box(NOPLACE_TEXT);
    set_last_filter(GREY_PLACE_FILTER);
    grey_searcher->fill_table(filters, false);
}

void Searcher::change_grey_board(QString st){
    filters->set_grey_board(st);
    filters->set_grey_box(NOPLACE_TEXT);
    set_last_filter(GREY_PLACE_FILTER);
    grey_searcher->fill_table(filters, false);
}

void Searcher::change_grey_box(QString st){
    filters->set_grey_box(st);
    set_last_filter(GREY_PLACE_FILTER);
    grey_searcher->fill_table(filters, false);
}

void Searcher::change_grey_add_info(QString s){
    if(s == ANY_ITEM_TEXT)
        filters->clear_grey_add_info();
    else
        filters->set_grey_add_info(s);
    set_last_filter(GREY_ADD_FILTER);
    grey_searcher->fill_table(filters, false);
}

void Searcher::change_grey_defect(QString s){
    if(s == ANY_ITEM_TEXT)
        filters->clear_grey_defect();
    else
        filters->set_grey_defect(s);
    set_last_filter(GREY_ADD_FILTER);
    grey_searcher->fill_table(filters, false);
}

void Searcher::reset_add_boxes(){
    filters->clear_grey_add_info();
    filters->clear_grey_defect();
    filters->clear_grey_category();
    set_last_filter(GREY_ADD_FILTER);
    grey_searcher->fill_table(filters, false);
}

void Searcher::clear_grey_boxes(){
    filters->clear_grey_storage();
    filters->clear_grey_rack();
    filters->clear_grey_board();
    filters->clear_grey_box();
    filters->clear_grey_one_year();
    filters->clear_grey_from_year();
    filters->clear_grey_to_year();
    filters->clear_grey_insp();
    filters->clear_grey_add_info();
    filters->clear_grey_defect();
    filters->clear_grey_category();
    filters->clear_grey_add_info();
    filters->clear_grey_defect();
    filters->clear_grey_category();
}

void Searcher::reset_grey_boxes(){
    filters->clear_grey_storage();
    filters->clear_grey_rack();
    filters->clear_grey_board();
    filters->clear_grey_box();
    grey_searcher->fill_table(filters, false);
}

void Searcher::change_grey_insp(QString s){
    if(s == ANY_ITEM_TEXT)
        reset_grey_insp();
    else{
        QSqlQuery q("SELECT id FROM inspections WHERE insp_name = '" + s + "'", base);
        if(q.lastError().isValid()){
            error("Ошибка при установке приёмки", q.lastError().text());
            return;
        }
        if(q.size() != 1){
            error("Ошибка при установке приёмки", QString::number(q.size()));
            return;
        }
        q.next();
        int id = q.value(0).toInt();
        filters->set_grey_insp(id);
        filters->set_last_filter(GREY_INSP_FILTER);
        grey_searcher->fill_table(filters, false);
    }
}

void Searcher::change_grey_category(QString s){
    if(s == ANY_ITEM_TEXT)
        filters->clear_grey_category();
    else{
        QSqlQuery q("SELECT id FROM categories WHERE category_name = '" + s + "'", base);
        if(q.lastError().isValid()){
            error("Ошибка при установке категории", q.lastError().text());
            return;
        }
        if(q.size() != 1){
            error("Ошибка при установке категории", QString::number(q.size()));
            return;
        }
        q.next();
        int id = q.value(0).toInt();
        filters->set_grey_category(id);
        filters->set_grey_string_category(s);
        filters->set_last_filter(GREY_ADD_FILTER);
        grey_searcher->fill_table(filters, false);
    }
}

void Searcher::reset_grey_insp(){
    filters->clear_grey_insp();
    grey_searcher->fill_table(filters, false);
}

void Searcher::open_blue(int id, QString name){
    old_grey_filters = this->filters;
    grey_filters_default();
    filters->set_grey_id(id);
    set_last_filter(GREY_ID_FILTER);
    blue_searcher->fill_table(this->filters, false);
    blue_searcher->set_reserve_header();
    stack->setCurrentWidget(blue_searcher);
    filters->set_mode(BLUE_MODE);
    GLOBAL_MODE = SEARCHER_BLUE_MODE;

    emit rename_tab(name, true);
}

void Searcher::close_blue(){
    this->filters = old_grey_filters;
    stack->setCurrentWidget(grey_searcher);
    filters->set_mode(GREY_MODE);
    GLOBAL_MODE = SEARCHER_WHITE_MODE;
    //emit rename_tab(old_tab_name);
}

void Searcher::open_grey(int id, QString name){
    //if(need_blue )
    grey_filters_default();
    filters->set_grey_id(id);
    set_last_filter(GREY_ID_FILTER);
    //grey_searcher->open_trademark(id, filters);
    grey_searcher->fill_table(this->filters, false);
    grey_searcher->set_reserve_header();
    stack->setCurrentWidget(grey_searcher);
    filters->set_mode(GREY_MODE);
    GLOBAL_MODE = SEARCHER_GREY_MODE;

    emit rename_tab(name, true);
}

void Searcher::close_grey(QModelIndex i){
    set_last_filter(CLOSE_GREY_FILTER);
    stack->setCurrentWidget(white_searcher);
    filters->set_mode(WHITE_MODE);
    white_searcher->set_reserve_header();
    GLOBAL_MODE = SEARCHER_WHITE_MODE;
    //refresh_white_table();

    emit rename_tab(old_tab_name);
}

void Searcher::refresh_grey(){
    grey_searcher->fill_table(filters, false);
}

void Searcher::open_contragents(int doc_num, int type){
    current_reserve_doc = doc_num;
    ContragentsList *contragents = new ContragentsList(true, type);
    prev_widgets.push(stack->currentWidget());
    stack->addWidget(contragents);
    stack->setCurrentWidget(contragents);
    contragents->show();
    connect(contragents, SIGNAL(delete_me()), SLOT(close_current()));
    connect(contragents, SIGNAL(selected(int,QString)), SLOT(contragent_returned_slot(int,QString)));
}

void Searcher::open_docslist(){
    /*DocsList *docs = new DocsList();
    prev_widgets.push(stack->currentWidget());
    stack->addWidget(docs);
    stack->setCurrentWidget(docs);
    docs->show();
    connect(docs, SIGNAL(delete_me()), SLOT(close_current()));
    connect(this, SIGNAL(switch_hidden_signal()), docs, SLOT(switch_hidden()));
    connect(docs, SIGNAL(open_doc(int)), SLOT(open_doc(int)));*/
}

void Searcher::open_doc(int id){
    /*Document *doc = new Document(id);
    prev_widgets.push(stack->currentWidget());
    stack->addWidget(doc);
    stack->setCurrentWidget(doc);
    doc->show();

    connect(doc, SIGNAL(delete_me()), SLOT(close_current()));*/
}

void Searcher::switch_hidden(){
    this->white_searcher->switch_hidden_public();
}

void Searcher::open_reserves_list(){
    white_searcher->open_reserves_list();
}

void Searcher::contragent_returned_slot(int id, QString name){
    close_current();
    // в базу записать же
    if(current_reserve_doc >= 0){
        QSqlQuery query(base);
        QString query_str = QString("UPDATE reserve_docs SET contragent_id = %1 WHERE user_id = %2 AND doc_num = %3")
                .arg(id)
                .arg(USER_ID)
                .arg(current_reserve_doc);
        if(!query.exec(query_str)){
            qDebug() << "Ошибка sql-запроса: " << query.lastError().text();
            return;
        }
        current_reserve_doc = -1;
    }

    emit contragent_returned(id, name);
}

void Searcher::close_current(){
    //stack->currentWidget()->close();
    stack->removeWidget(stack->currentWidget());
    stack->setCurrentWidget(prev_widgets.last());
    prev_widgets.pop();
}

QStringList Searcher::valid_strings(QStringList parts){
    /*
       Это мы применяем поиск по фрагментам.
       В строку поиска может быть введено сколько угодно фрагментов.
       В требуемых строках фрагменты могут стоять в любом порядке,
       могут быть разделены любым количеством символов, но каждый фрагмент
       должен встречаться ровно столько раз, сколько в строке поиска.
       Задача этой функции - отобрать последовательности фрагментов,
       по которым будет что-то находиться, ибо при поиске
       проверять все n! последовательностей слишком долго.
       Фрагменты будем обрабатывать по очереди, т.е. сначала рассмотрим
       последовательность из 1 фрагмента, затем из 2, 3 и т.д.
       На каждом этапе будем отсекать те последовательности,
       по которым не находится ни одной позиции,
       а оставшиеся будем хранить в двумерном списке valid.
       Также нам понадобится временный двумерный список,
       куда мы будем складывать валидные варианты на каждом этапе.
    */
    QList<QStringList> valid;
    QList<QStringList> temp_valid;
    // Первоначально в списке valid будет храниться только первый фрагмент.
    // Если и по нему ничего не находится - прикрываем лавочку.
    // Дабы не перегружать функцию size_of_select(QStringList) -
    // запихнём единственный фрагмент в QStringList.
    QStringList templist;
    templist.insert(0, parts.at(0));
    int result_size = size_of_select(templist);
    /*
       Если поиск по первому фрагменту не вернул нифига,
       значит, поиск по введённым фрагментам не должен найти ничего.
       Поэтому вернём лист, состоящий из единственного этого первого фрагмента,
       чтобы потом установить его в parts_filter,
       тогда apply_filters сформирует строку запроса,
       возвращающую ноль строк, то есть FAIL.
    */
    if(result_size == 0)
        return templist;

    // Если вернул, то продолжаем извращения.
    else{
        valid.insert(0, templist);
        // для каждого фрагмента из списка (исходного, из поля ввода):
        for(int i = 1; i < parts.size(); i++){
            temp_valid.clear();
            // берём каждый валидный stringlist (на первом этапе эти стринглисты
            // состоят из 1 строки, на втором - из двух, и т.д.)
            for(int j = 0; j < valid.size(); j++){
                //между каждыми двумя строчками из этого листа, а также в начале и в конце
                for(int k = 0; k <= valid.at(j).size(); k++){
                    // по очереди вставляем очередной фрагмент из parts
                    templist = valid.at(j);
                    templist.insert(k, parts.at(i));
                    // и проверяем получившуюся строку на валидность.
                    result_size = size_of_select(templist);
                    // если валидна, вставляем её в temp_valid
                    if(result_size > 0)
                        temp_valid << templist;
                }
            }
            valid.clear();
            // если на данном этапе не нашлось ни одной валидной строки, то кранты - надо возвращать пустой лист
            if(temp_valid.size() == 0){
                break;
            }
            valid = temp_valid;
        }

        // Это уже под конец.
        // В valid лежат последовательности фрагментов, каждую из которых
        // нужно склеить в строку для запроса (их может быть и ноль)
        QStringList patterns;
        foreach (templist, valid)
            patterns << "%" + templist.join("%") + "%";

        return patterns;
    }
}

int Searcher::size_of_select(QStringList lst){
    // Вспомогательная функция для valid_strings,
    // склеивающая список фрагментов в строку для sql-запроса
    // и определяющая размер выборки по этому запросу.
    QSqlQuery query(base);
    query.exec("SELECT * FROM " + TOVMARKS_TABLE + " WHERE pattern LIKE '%" + lst.join("%") + "%'");
    return query.size();
}
