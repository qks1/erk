#include "filters.h"

// конструктор, в котором присваиваем значения фильтров по умолчанию
Filters::Filters(){
    group = 0;
}

// установить значение group_filter, переданное в параметре
void Filters::set_group_filter(int g){
    this->group = g;
}

// очистить columns_filter
void Filters::clear_columns(){
    this->columns.clear();
}

// установить значение columns_filter, переданное в параметре
void Filters::set_columns_filter(QStringList c){
    // предварительно необходимо очистить его
    this->columns.clear();
    this->columns = c;
}

// установить значение переменной limits
void Filters::set_limits(pair l){
    this->limits = l;
}

void Filters::set_grey_limits(pair l){
    this->grey_limits = l;
}

void Filters::remove_limits(){
    this->nolimits = true;
}

void Filters::remove_grey_limits(){
    this->grey_nolimits = true;
}

void Filters::restore_limits(){
    this->nolimits = false;
}

void Filters::restore_grey_limits(){
    this->grey_nolimits = false;
}

void Filters::set_white_id(int id){
    this->white_id = id;
}

void Filters::reset_white_id(){
    this->white_id = 0;
}

void Filters::set_beginname(QString name){
    this->beginname = name;
}

void Filters::reset_beginname(){
    this->beginname = "";
}

void Filters::set_white_id_mode(int mode){
    this->white_id_mode = mode;
}

void Filters::set_parts(QStringList parts){
    this->parts = parts;
}

void Filters::add_white_sort_column(SortingOrder item){
    int index = this->white_sort_order.indexOf(item);
    // если столбец первый в списке, то инвертируем порядок сортировки во всех столбцах
    if(index == 0){
        for(int i = 0; i < white_sort_order.size(); i++){
            white_sort_order[i].order = (white_sort_order[i].order == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
        }
    }
    else{
        // если столбец уже есть в списке и он не первый, удаляем его и вставляем в начало
        if(index >= 1){
            this->white_sort_order.removeAt(index);
        }
        this->white_sort_order.push_front(item);
    }
}

void Filters::add_grey_sort_column(SortingOrder item){
    int index = this->grey_sort_order.indexOf(item);
    // если столбец первый в списке, то инвертируем порядок сортировки во всех столбцах
    if(index == 0){
        for(int i = 0; i < grey_sort_order.size(); i++){
              grey_sort_order[i].order = (grey_sort_order[i].order == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
        }
    }
    else{
        // если столбец уже есть в списке и он не первый, удаляем его и вставляем в начало
        if(index >= 1)
            this->grey_sort_order.removeAt(index);
        this->grey_sort_order.push_front(item);
    }
}

void Filters::set_param(int num, QString str){
    this->params[num] = str;
}

void Filters::reset_params(){
    this->params.clear();
    for(int i = 0; i < MAX_PARAMS; i++)
        params << "";
}

void Filters::switch_prices(){
    if(this->prices == ALL_PRICES)
        this->prices = POSITIVE_PRICES;
    else if(this->prices == POSITIVE_PRICES)
        this->prices = ZERO_PRICES;
    else if(this->prices == ZERO_PRICES)
        this->prices = ALL_PRICES;
}

void Filters::set_prices(int p){
    this->prices = p;
}

void Filters::switch_quantity(){
    if(this->quantity == ALL_QUANTITIES)
        this->quantity = POSITIVE_QUANTITIES;
    else if(this->quantity == POSITIVE_QUANTITIES)
        this->quantity = ALL_QUANTITIES;
}

void Filters::set_quantity(int q){
    this->quantity = q;
}

void Filters::set_mode(int m){
    this->search_mode = m;
}

void Filters::set_last_filter(int l){
    this->last_filter = l;
}

void Filters::set_grey_id(int id){
    this->grey_id = id;
}

void Filters::set_grey_text_id(int id){
    this->grey_text_id = id;
}

void Filters::reset_grey_text_id(){
    this->grey_text_id = 0;
}

void Filters::reset_grey_id(){
    this->grey_id = 0;
}

void Filters::set_grey_beginname(QString name){
    this->grey_beginname = name;
}

void Filters::reset_grey_beginname(){
    this->grey_beginname = "";
}

void Filters::set_grey_id_mode(int mode){
    this->grey_id_mode = mode;
}

void Filters::set_grey_parts(QStringList parts){
    this->grey_parts = parts;
}

void Filters::set_grey_storage(QString s){
    this->grey_storage = ((s == ANY_ITEM_TEXT) ? NOPLACE_TEXT : s);
}

void Filters::set_grey_rack(QString s){
    this->grey_rack = ((s == ANY_ITEM_TEXT) ? NOPLACE_TEXT : s);
}

void Filters::set_grey_board(QString s){
    this->grey_board = ((s == ANY_ITEM_TEXT) ? NOPLACE_TEXT : s);
}

void Filters::set_grey_box(QString s){
    this->grey_box = ((s == ANY_ITEM_TEXT) ? NOPLACE_TEXT : s);
}

void Filters::set_grey_from_year(QString s){
    this->grey_from_year = (s == NOYEAR_TEXT ? "0" : ((s == "-1" || s == ANY_ITEM_TEXT) ? "" : s));
}

void Filters::set_grey_to_year(QString s){
    this->grey_to_year = (s == NOYEAR_TEXT ? "0" : ((s == "-1" || s == ANY_ITEM_TEXT) ? "" : s));
}

void Filters::set_grey_one_year(QString s){
    this->grey_one_year = (s == NOYEAR_TEXT ? "0" : ((s == "-1" || s == ANY_ITEM_TEXT) ? "" : s));
}

void Filters::set_grey_year_mode(int i){
    this->grey_year_mode = i;
}

void Filters::set_grey_insp(int i){
    this->grey_insp = (i >= 0 ? i : -1);
}

void Filters::set_grey_add_info(QString s){
    this->grey_add_info = ((s == ANY_ITEM_TEXT) ? NOINFO_TEXT : s);
}

void Filters::set_grey_defect(QString s){
    this->grey_defect = ((s == ANY_ITEM_TEXT) ? NOINFO_TEXT : s);
}

void Filters::set_grey_category(int i){
    this->grey_category = (i >= 0 ? i : -1);
}

void Filters::set_grey_string_category(QString s){
    this->grey_string_category = ((s == ANY_ITEM_TEXT) ? NOINFO_TEXT : s);
}




void Filters::clear_grey_storage(){
    this->grey_storage = NOPLACE_TEXT;
}

void Filters::clear_grey_rack(){
    this->grey_rack = NOPLACE_TEXT;
}

void Filters::clear_grey_board(){
    this->grey_board = NOPLACE_TEXT;
}

void Filters::clear_grey_box(){
    this->grey_box = NOPLACE_TEXT;
}

void Filters::clear_grey_one_year(){
    this->grey_one_year = "";
}

void Filters::clear_grey_from_year(){
    this->grey_from_year = "";
}

void Filters::clear_grey_to_year(){
    this->grey_to_year = "";
}

void Filters::clear_grey_insp(){
    this->grey_insp = -1;
}

void Filters::clear_grey_add_info(){
    this->grey_add_info = NOINFO_TEXT;
}

void Filters::clear_grey_defect(){
    this->grey_defect = NOINFO_TEXT;
}

void Filters::clear_grey_category(){
    this->grey_category = -1;
    this->grey_string_category = NOINFO_TEXT;
}

//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//

// вернуть значение group_filter
int Filters::group_filter(){
    return this->group;
}

// вернуть значение columns_filter
QStringList Filters::columns_filter(){
    return this->columns;
}

// вернуть значение limits
pair Filters::limits_filter(){
    return this->limits;
}

pair Filters::grey_limits_filter(){
    return this->grey_limits;
}

// вернуть начало выборки
int Filters::begin(){
    return this->limits.begin;
}

int Filters::grey_begin(){
    return this->grey_limits.begin;
}

bool Filters::are_there_limits(){
    return (nolimits ? false : true);
}

bool Filters::are_there_grey_limits(){
    return (grey_nolimits ? false : true);
}

// вернуть "белый" id
int Filters::white_id_filter(){
    return this->white_id;
}

// вернуть шаблон дял поиска с начала
QString Filters::beginname_filter(){
    return this->beginname;
}

int Filters::white_id_mode_filter(){
    return this->white_id_mode;
}

QStringList Filters::parts_filter(){
    return this->parts;
}

QList<SortingOrder> Filters::white_sort_order_filter(){
    return this->white_sort_order;
}

QList<SortingOrder> Filters::grey_sort_order_filter(){
    return this->grey_sort_order;
}

QStringList Filters::params_filter(){
    return this->params;
}

int Filters::prices_filter(){
    return this->prices;
}

int Filters::quantity_filter(){
    return this->quantity;
}

int Filters::mode(){
    return this->search_mode;
}

int Filters::last_applied_filter(){
    return this->last_filter;
}

int Filters::grey_text_id_filter(){
    return this->grey_text_id;
}

int Filters::grey_id_filter(){
    return this->grey_id;
}

// вернуть шаблон для поиска с начала
QString Filters::grey_beginname_filter(){
    return this->grey_beginname;
}

int Filters::grey_id_mode_filter(){
    return this->grey_id_mode;
}

QStringList Filters::grey_parts_filter(){
    return this->grey_parts;
}

QString Filters::grey_storage_filter(){
    return this->grey_storage;
}

QString Filters::grey_rack_filter(){
    return this->grey_rack;
}

QString Filters::grey_board_filter(){
    return this->grey_board;
}

QString Filters::grey_box_filter(){
    return this->grey_box;
}

QString Filters::grey_from_year_filter(){
    return this->grey_from_year;
}

QString Filters::grey_to_year_filter(){
    return this->grey_to_year;
}

QString Filters::grey_one_year_filter(){
    return this->grey_one_year;
}

int Filters::grey_year_mode_filter(){
    return this->grey_year_mode;
}


int Filters::grey_insp_filter(){
    return this->grey_insp;
}

QString Filters::grey_add_info_filter(){
    return this->grey_add_info;
}

QString Filters::grey_defect_filter(){
    return this->grey_defect;
}

int Filters::grey_category_filter(){
    return this->grey_category;
}

QString Filters::grey_string_category_filter(){
    return this->grey_string_category;
}
