#include "constants.h"

// параметры соединения с БД
/*
QString DB_NAME = "postgres";
QString DB_USER = "postgres";
QString DB_PASSWORD = "aurora2174_";
QString DB_HOST = "192.168.1.101";
*/

QString DB_NAME = "erkbase";
QString DB_USER = "erkprog";
QString DB_PASSWORD = "aurora2174_";
QString DB_HOST = "localhost";

/*
QString DB_NAME = "erkbase";
QString DB_USER = "df";
QString DB_PASSWORD = "j39ljzuzy";
QString DB_HOST = "localhost";
*/
// таблицы БД
QString GROUPS_TABLE = "groups";
QString SUBGROUPS_TABLE = "subgroups";
QString TOVMARKS_TABLE = "trademarks";
QString GREY_TABLE = "greytable";

// основной шрифт
int SYSTEM_FONT_SIZE = 11;
QString SYSTEM_FONT_FAMILY = "Ubuntu";

// мелкий шрифт (для переключателя страниц)
int SMALL_FONT_SIZE = SYSTEM_FONT_SIZE-2;

// настройки поисковика
int ITEMS_ON_PAGE = 50;

// начиная со скольких элементов спрашивать подвтерждение пользователя при выводе их на одной странице
int MAX_ITEMS_WITHOUT_WARNING = 300;

// сколько вкладок поисковика создаётся по умолчанию
int TABS_DEFAULT = 1;

// сколько параметров может быть у детали
int MAX_PARAMS = 12;

// ширина столбца по умолчанию
int DEFAULT_COLUMN_WIDTH = 70;

// столбец для сортировки в белом экране по умолчанию
QString DEFAULT_WHITE_SORT_COLUMN = "id";

// порядок сортиорвки в белом экране по умолчанию
Qt::SortOrder DEFAULT_WHITE_SORT_ORDER = Qt::AscendingOrder;

QStringList ALL_WHITE_COLUMNS;
QStringList WHITE_TABLE_COLUMNS;
QStringList ALL_GREY_COLUMNS;
QStringList GREY_TABLE_COLUMNS;

// имена столбцов в белом и сером экране
QMap<QString,QString> WHITE_COLUMNS_NAMES;
QMap<QString,QString> GREY_COLUMNS_NAMES;

QString ANY_ITEM_TEXT;
QString NOPLACE_TEXT;
QString NOPAR_TEXT;
QString NOYEAR_TEXT;
QString NOINFO_TEXT;

void init_vars(){
    ALL_WHITE_COLUMNS << "t.id" << "t.subgroup_id" << "t.name" << "s.name as subgroup" << "t.quantity" << "t.price_ret" << "t.par1_val" << "t.par2_val" << "t.par3_val" << "t.par4_val"<< "t.par5_val" << "t.par6_val" << "t.par7_val" << "t.par8_val" << "t.par9_val" << "t.par10_val" << "t.par11_val" << "t.par12_val";
    WHITE_TABLE_COLUMNS << "t.id" << "t.name" << "s.name as subgroup" << "t.quantity" << "t.price_ret" << "t.par1_val" << "t.par2_val" << "t.par3_val" << "t.par4_val"<< "t.par5_val" << "t.par6_val" << "t.par7_val" << "t.par8_val" << "t.par9_val" << "t.par10_val" << "t.par11_val" << "t.par12_val";
    ALL_GREY_COLUMNS << "g.id" << "g.trademark_id" << "t.name" << "t.pattern" << "g.quantity" << "g.year" << "g.place_id" << "g.price_ret" << "p.storage" << "p.rack" << "p.board" << "p.box" << "i.insp_name" << "g.add_info" << "g.defect" << "g.category_id" << "c.category_name";
    GREY_TABLE_COLUMNS << "g.id" << "t.name" << "g.quantity" << "g.year" << "g.price_ret" << "p.storage" << "p.rack" << "p.board" << "p.box" << "i.insp_name" << "g.add_info" << "g.defect" << "c.category_name";
    QCoreApplication::setOrganizationName("erk");
    QCoreApplication::setApplicationName("base");

    WHITE_COLUMNS_NAMES["id"] = "id";
    WHITE_COLUMNS_NAMES["name"] = "Имя";
    WHITE_COLUMNS_NAMES["subgroup"] = "Группа";
    WHITE_COLUMNS_NAMES["quantity"] = "Кол-во";
    WHITE_COLUMNS_NAMES["price_ret"] = "Цена";
    WHITE_COLUMNS_NAMES["par1_val"] = "1";
    WHITE_COLUMNS_NAMES["par2_val"] = "2";
    WHITE_COLUMNS_NAMES["par3_val"] = "3";
    WHITE_COLUMNS_NAMES["par4_val"] = "4";
    WHITE_COLUMNS_NAMES["par5_val"] = "5";
    WHITE_COLUMNS_NAMES["par6_val"] = "6";
    WHITE_COLUMNS_NAMES["par7_val"] = "7";
    WHITE_COLUMNS_NAMES["par8_val"] = "8";
    WHITE_COLUMNS_NAMES["par9_val"] = "9";
    WHITE_COLUMNS_NAMES["par10_val"] = "10";
    WHITE_COLUMNS_NAMES["par11_val"] = "11";
    WHITE_COLUMNS_NAMES["par12_val"] = "12";

    GREY_COLUMNS_NAMES["id"] = "id";
    GREY_COLUMNS_NAMES["quantity"] = "Кол-во";
    GREY_COLUMNS_NAMES["year"] = "Год";
    GREY_COLUMNS_NAMES["name"] = "Имя";
    GREY_COLUMNS_NAMES["storage"] = "Склад";
    GREY_COLUMNS_NAMES["rack"] = "Стеллаж";
    GREY_COLUMNS_NAMES["board"] = "Полка";
    GREY_COLUMNS_NAMES["box"] = "Ящик";
    GREY_COLUMNS_NAMES["insp_name"] = "Приёмка";
    GREY_COLUMNS_NAMES["add_info"] = "Доп.пар.1";
    GREY_COLUMNS_NAMES["defect"] = "Доп.пар.2";
    GREY_COLUMNS_NAMES["category_name"] = "Категория";
    GREY_COLUMNS_NAMES["price_ret"] = "Цена";
    // текст для пункта "любой" в комбобоксе
    ANY_ITEM_TEXT = "<любой>";
    // чем заполняются пустые фильтры по местам
    NOPLACE_TEXT = "NOPLACE";
    // пустые фильтры по доп. параметрам
    NOPAR_TEXT = "NOPAR";
    // отсутствие года
    NOYEAR_TEXT = "бг";
    // пустые фильтры по доп. параметрам
    NOINFO_TEXT = "NOINFO";

}
