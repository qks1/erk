#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QtGui>
#include <QtSql>

// константы для переключателя страниц
#define PAGE_FIRST 1
#define PAGE_PREV 2
#define PAGE_NEXT 3
#define PAGE_LAST 4
#define PAGE_NUMBER 5

#define MULTIPAGE_MODE 10
#define SINGLEPAGE_MODE 11

// константы для последнего применённого фильтра
#define GROUPS_FILTER 100
#define COLUMNS_FILTER 101
#define LIMITS_FILTER 102
#define ID_FILTER 103
#define BEGIN_TEXT_FILTER 104
#define PARTS_TEXT_FILTER 105
#define NOLIMITS_FILTER 106
#define SETLIMITS_FILTER 107
#define SORT_FILTER 108
#define PARAMS_FILTER 109
#define PRICES_FILTER 110
#define QUANTITY_FILTER 111
#define GREY_ID_FILTER 112
#define CLOSE_GREY_FILTER 113
#define GREY_LIMITS_FILTER 114
#define GREY_NOLIMITS_FILTER 115
#define GREY_SETLIMITS_FILTER 116
#define GREY_SORT_FILTER 117
#define GREY_TEXT_ID_FILTER 118
#define GREY_BEGIN_TEXT_FILTER 119
#define GREY_PARTS_TEXT_FILTER 120
#define GREY_PLACE_FILTER 121
#define GREY_YEARS_FILTER 122
#define GREY_INSP_FILTER 123
#define GREY_ADD_FILTER 124

// константы для режима поиска по id
#define EQUAL 200
#define GREATER_OR_EQUAL 201

// константы для режимов поиска (текстовое поле)
#define ID_MODE 300
#define BEGIN_MODE 301
#define PARTS_MODE 302
#define ERROR_MODE 303

// режимы фильтра по ценам
#define ALL_PRICES 400
#define POSITIVE_PRICES 401
#define ZERO_PRICES 402

// режимы фильтра по количеству
#define ALL_QUANTITIES 500
#define POSITIVE_QUANTITIES 501

// режимы поисковика
#define WHITE_MODE 600
#define GREY_MODE 601
#define BLUE_MODE 602

// режимы фильтра по годам в сером экране
#define ONE_YEAR_MODE 700
#define MULTI_YEAR_MODE 701

// режимы
#define SEARCHER_WHITE_MODE 800
#define SEARCHER_GREY_MODE 801
#define SEARCHER_BLUE_MODE 802

// режимы диалогов в поисковике
#define DIALOG_WHITE_NEW 900
#define DIALOG_WHITE_EDIT 901

// в поисковике выборка может производиться как по группам, так и по подгруппам.
// чтобы в фильтре отличить группу от подгруппы, в каталоге (дереве) к номеру каждой группы прибавляется большое число.
#define ENLARGER 100000

///////////////////////
/////// СТОЛБЦЫ ///////
///////////////////////


// ПОИСКОВИК, БЕЛЫЙ ЭКРАН
extern QMap<QString, QString> columns_white_sql;
extern QMap<QString, int> columns_white_ids;


// ПОИСКОВИК, СЕРЫЙ ЭКРАН
extern QMap<QString, QString> columns_grey_sql;
extern QMap<QString, int> columns_grey_ids;

class CustomTabWidget;
class PaintStackedWidget;

namespace Privileges{
    enum Type{
        Prices_view_access = 1,
        Prices_edit_access = 2,
        NDS_enabling_access = 3,
        NDS_changing_access = 4,
        Reserving_access = 5,
        Blue_screen_access = 6
    };
}

namespace Usergroups{
    enum Group{
        Administrator = 1,
        Senior_manager = 2,
        Manager = 3,
        Bookkeeper = 4,
        Stockman = 5
    };
}

// глобальные переменные, используемые в проекте.
// Их назначение и значения - в constants.cpp
// В последствии все эти переменные будут храниться в QSettings

extern QSqlDatabase base;

extern QString GROUPS_TABLE;
extern QString SUBGROUPS_TABLE;
extern QString TOVMARKS_TABLE;
extern QString GREY_TABLE;

extern int SMALL_FONT_SIZE;
extern int SYSTEM_FONT_SIZE;
extern QString SYSTEM_FONT_FAMILY;

extern int ITEMS_ON_PAGE;

extern int MAX_ITEMS_WITHOUT_WARNING;

extern const int MAX_PARAMS;
extern const int max_reserves;

extern int DEFAULT_COLUMN_WIDTH;

extern QString DEFAULT_WHITE_SORT_COLUMN;
extern Qt::SortOrder DEFAULT_WHITE_SORT_ORDER;

extern QStringList columns_white_table;
extern QStringList columns_grey_table;

extern QMap<QString,QString> columns_white_names;
extern QMap<QString,QString> columns_grey_names;

extern QString ANY_ITEM_TEXT;
extern QString NOPLACE_TEXT;
extern QString NOPAR_TEXT;
extern QString NOYEAR_TEXT;
extern QString NOINFO_TEXT;

extern QString USERNAME;
extern int USER_ID;
extern Usergroups::Group USERGROUP;
extern QString PHOTOS_PATH;
extern QString SETTINGS_PATH;

extern int GLOBAL_MODE;

extern QString DEFAULT_WEIGHT_UNIT;
extern QString DEFAULT_UNIT;

extern QString old_tab_name;

extern bool is_connect;

extern const int top_panel_size;
extern const int bottom_panel_size;

extern bool searcher_show_reserve;
extern bool show_hidden;
extern int global_document_tab;

#endif // CONSTANTS_H

