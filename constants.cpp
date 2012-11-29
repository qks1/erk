#include "constants.h"

QSqlDatabase base;

// таблицы БД
QString GROUPS_TABLE = "groups";
QString SUBGROUPS_TABLE = "subgroups";
QString TOVMARKS_TABLE = "trademarks";
QString GREY_TABLE = "greytable";

// основной шрифт
int SYSTEM_FONT_SIZE = 11;
QString SYSTEM_FONT_FAMILY = "Tahoma";

// мелкий шрифт (для переключателя страниц)
int SMALL_FONT_SIZE = SYSTEM_FONT_SIZE-2;

// настройки поисковика
int ITEMS_ON_PAGE = 50;

// начиная со скольких элементов спрашивать подвтерждение пользователя при выводе их на одной странице
int MAX_ITEMS_WITHOUT_WARNING = 300;

// сколько параметров может быть у детали
const int MAX_PARAMS = 12;
const int max_reserves = 10;

// ширина столбца по умолчанию
int DEFAULT_COLUMN_WIDTH = 70;

// столбец для сортировки в белом экране по умолчанию
QString DEFAULT_WHITE_SORT_COLUMN = "id";

// порядок сортиорвки в белом экране по умолчанию
Qt::SortOrder DEFAULT_WHITE_SORT_ORDER = Qt::AscendingOrder;


QString ANY_ITEM_TEXT;
QString NOPLACE_TEXT;
QString NOPAR_TEXT;
QString NOYEAR_TEXT;
QString NOINFO_TEXT;
QString USERNAME;
QString PHOTOS_PATH;
QString SETTINGS_PATH;
QString DEFAULT_WEIGHT_UNIT;
QString DEFAULT_UNIT;
Usergroups::Group USERGROUP;
int USER_ID;
QString old_tab_name = "";

const int top_panel_size = 50;
const int bottom_panel_size = 50;

int GLOBAL_MODE;
bool is_connect;
bool searcher_show_reserve = false;

QStackedWidget *mwidget;
CustomTabWidget *search_tabs;
