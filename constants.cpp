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

void init_vars(){
    ALL_WHITE_COLUMNS << "t.id" << "t.subgroup_id" << "t.name" << "s.name as subgroup" << "t.quantity" << "t.price_ret" << "t.par1_val" << "t.par2_val" << "t.par3_val" << "t.par4_val"<< "t.par5_val" << "t.par6_val" << "t.par7_val" << "t.par8_val" << "t.par9_val" << "t.par10_val" << "t.par11_val" << "t.par12_val";
    WHITE_TABLE_COLUMNS << "t.id" << "t.name" << "s.name as subgroup" << "t.quantity" << "t.price_ret" << "t.par1_val" << "t.par2_val" << "t.par3_val" << "t.par4_val"<< "t.par5_val" << "t.par6_val" << "t.par7_val" << "t.par8_val" << "t.par9_val" << "t.par10_val" << "t.par11_val" << "t.par12_val";
    QCoreApplication::setOrganizationName("erk");
    QCoreApplication::setApplicationName("base");
}
