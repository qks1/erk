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
QString USERNAME;
QString PHOTOS_PATH;
QString SETTINGS_PATH;
QString DEFAULT_WEIGHT_UNIT;
QString DEFAULT_UNIT;
Usergroups::Group USERGROUP;
int USER_ID;

int GLOBAL_MODE;
bool is_connect;
QStackedWidget *mwidget;
CustomTabWidget *search_tabs;
