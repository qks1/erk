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

// режимы фильтра по годам в сером экране
#define ONE_YEAR_MODE 700
#define MULTI_YEAR_MODE 701

// режимы
#define SEARCHER_WHITE_MODE 800
#define SEARCHER_GREY_MODE 801

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
#define COLUMN_WHITE_ID "t.id as id"
#define COLUMN_WHITE_PHOTO "t.photo as photo"
#define COLUMN_WHITE_YEARSDISCOUNTS "t.years_discounts as years_discounts"
#define COLUMN_WHITE_NAME "t.name as name"
#define COLUMN_WHITE_SUBGROUP "s.name as subgroup"
#define COLUMN_WHITE_QUANTITY "t.quantity as quantity"
#define COLUMN_WHITE_RETAILPRICE "t.price_ret as price_ret"
#define COLUMN_WHITE_WHOLEPRICE "t.price_whole as price_whole"
#define COLUMN_WHITE_WHOLEBEGIN "t.whole_begin as whole_begin"
#define COLUMN_WHITE_WHOLEUNIT "t.whole_begin_unit as whole_begin_unit"
#define COLUMN_WHITE_PAR1 "t.par1_val as par1_val"
#define COLUMN_WHITE_PAR2 "t.par2_val as par2_val"
#define COLUMN_WHITE_PAR3 "t.par3_val as par3_val"
#define COLUMN_WHITE_PAR4 "t.par4_val as par4_val"
#define COLUMN_WHITE_PAR5 "t.par5_val as par5_val"
#define COLUMN_WHITE_PAR6 "t.par6_val as par6_val"
#define COLUMN_WHITE_PAR7 "t.par7_val as par7_val"
#define COLUMN_WHITE_PAR8 "t.par8_val as par8_val"
#define COLUMN_WHITE_PAR9 "t.par9_val as par9_val"
#define COLUMN_WHITE_PAR10 "t.par10_val as par10_val"
#define COLUMN_WHITE_PAR11 "t.par11_val as par11_val"
#define COLUMN_WHITE_PAR12 "t.par12_val as par12_val"
#define COLUMN_WHITE_UNIT "u.unit_name as unit_name"
#define COLUMN_WHITE_WEIGHT "t.weight as weight"
#define COLUMN_WHITE_WEIGHTUNIT "w.weight_unit_name as weight_unit_name"
#define COLUMN_WHITE_NOTES "t.notes as notes"
#define COLUMN_WHITE_CREATED "to_char(t.created, 'YYYY-MM-DD HH24:MI:SS') as created"
#define COLUMN_WHITE_EDITED "to_char(t.edited, 'YYYY-MM-DD HH24:MI:SS') as edited"
#define COLUMN_WHITE_RETAILUPDATE "CASE WHEN t.retail_update IS NULL THEN '1900-01-01' ELSE to_char(t.retail_update, 'YYYY-MM-DD') END as retail_update"
#define COLUMN_WHITE_WHOLEUPDATE "CASE WHEN t.whole_update IS NULL THEN '1900-01-01' ELSE to_char(t.whole_update, 'YYYY-MM-DD') END as whole_update"
#define COLUMN_WHITE_WHOLEBEGINUPDATE "CASE WHEN t.wholebegin_update IS NULL THEN '1900-01-01' ELSE to_char(t.wholebegin_update, 'YYYY-MM-DD') END as begin_update"

// ПОИСКОВИК, СЕРЫЙ ЭКРАН
#define COLUMN_GREY_ID "g.id as id"
#define COLUMN_GREY_NAME "t.name as name"
#define COLUMN_GREY_QUANTITY "g.quantity as quantity"
#define COLUMN_GREY_YEAR "g.year as year"
#define COLUMN_GREY_RETAILPRICE "g.price_ret as price_ret"
#define COLUMN_GREY_STORAGE "p.storage as storage"
#define COLUMN_GREY_RACK "p.rack as rack"
#define COLUMN_GREY_BOARD "p.board as board"
#define COLUMN_GREY_BOX "p.box as box"
#define COLUMN_GREY_INSPECTION "i.insp_name as insp_name"
#define COLUMN_GREY_ADDINFO "g.add_info as add_info"
#define COLUMN_GREY_DEFECT "g.defect as defect"
#define COLUMN_GREY_CATEGORY "c.category_name as category_name";

class CustomTabWidget;

namespace Privileges{
    enum Type{
        Prices_view_access = 1,
        Prices_edit_access = 2,
        NDS_enabling_access = 3,
        NDS_changing_access = 4
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

extern int DEFAULT_COLUMN_WIDTH;

extern QString DEFAULT_WHITE_SORT_COLUMN;
extern Qt::SortOrder DEFAULT_WHITE_SORT_ORDER;

extern QStringList WHITE_TABLE_COLUMNS;
extern QStringList GREY_TABLE_COLUMNS;

extern QMap<QString,QString> WHITE_COLUMNS_NAMES;
extern QMap<QString,QString> GREY_COLUMNS_NAMES;

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

extern bool is_connect;
extern QStackedWidget *mwidget;
extern CustomTabWidget *search_tabs;

#endif // CONSTANTS_H
