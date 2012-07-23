#include "constants.h"

// параметры соединения с БД
/*
QString DB_NAME = "postgres";
QString DB_USER = "postgres";
QString DB_PASSWORD = "aurora2174_";
QString DB_HOST = "192.168.1.101";
*/
/*
QString DB_NAME = "erkbase";
QString DB_USER = "erkprog";
QString DB_PASSWORD = "aurora2174_";
QString DB_HOST = "localhost";
*/

QString DB_NAME = "erkbase";
QString DB_USER = "df";
QString DB_PASSWORD = "j39ljzuzy";
QString DB_HOST = "localhost";

// таблицы БД
QString GROUPS_TABLE = "groups";
QString SUBGROUPS_TABLE = "subgroups_ex";
QString TOVMARKS_TABLE = "tovmarks_ex";

// основной шрифт
int SYSTEM_FONT_SIZE = 12;
QString SYSTEM_FONT_FAMILY = "Arial";

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

// столбец для сортировки в белом экране по умолчанию
QString DEFAULT_WHITE_SORT_COLUMN = "id";

// порядок сортиорвки в белом экране по умолчанию
Qt::SortOrder DEFAULT_WHITE_SORT_ORDER = Qt::AscendingOrder;
