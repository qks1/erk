#include <QMap>
#include <QString>
#include <QStringList>
#include "constants.h"

/*
    А механизм, в целом такой:
    - есть QMap с columns_***_sql - это соответствие псевдонимов столбцов их именованию в БД,
        из этого будет формироваться запрос SELECT
    - есть QMap columns_***_ids - это соответствие столбцов их логическим индексам в результирующей sql-модели.
        В программе логический индекс столбца можно получить простым обращением к его псевдониму,
        например, columns_white_ids["name"]
    - есть QStringList columns_***_table, куда добавляются строки из _sql, в зависимости от привилегий и прочего.
        В процессе его формирования формируется и _ids: заводится счётчик, изначально равный нулю,
        при добавлении столбца в _table этому столбцу в _ids присваивается значение, равное значению счётчика,
        после чего счётчик увеличивается на единицу.
    - есть QMap columns_***_names, содержащий HeaderData, то есть заголовки столбцов в таблице.
        Названия столбцов в нём должны в точности повторять псевдонимы столбцов в sql-запросе (as %columnname%).

*/

inline void set_columns_white_sql(){
    columns_white_sql["id"] = "t.id as id";
    columns_white_sql["photo"] = "t.photo as photo";
    columns_white_sql["yearsdiscounts"] = "t.years_discounts as years_discounts";
    columns_white_sql["reserve"] = "t.reserve_total as reserve_total";
    columns_white_sql["name"] = "t.name as name";
    columns_white_sql["subgroup"] = "s.name as subgroup";
    columns_white_sql["quantity"] = "t.quantity as quantity";
    columns_white_sql["price_ret"] = "t.price_ret as price_ret";
    columns_white_sql["price_whole"] = "t.price_whole as price_whole";
    columns_white_sql["whole_begin"] = "t.whole_begin as whole_begin";
    columns_white_sql["par1"] = "t.par1_val as par1_val";
    columns_white_sql["par2"] = "t.par2_val as par2_val";
    columns_white_sql["par3"] = "t.par3_val as par3_val";
    columns_white_sql["par4"] = "t.par4_val as par4_val";
    columns_white_sql["par5"] = "t.par5_val as par5_val";
    columns_white_sql["par6"] = "t.par6_val as par6_val";
    columns_white_sql["par7"] = "t.par7_val as par7_val";
    columns_white_sql["par8"] = "t.par8_val as par8_val";
    columns_white_sql["par9"] = "t.par9_val as par9_val";
    columns_white_sql["par10"] = "t.par10_val as par10_val";
    columns_white_sql["par11"] = "t.par11_val as par11_val";
    columns_white_sql["par12"] = "t.par12_val as par12_val";
    columns_white_sql["unit"] = "u.unit_name as unit_name";
    columns_white_sql["weight"] = "t.weight as weight";
    columns_white_sql["weightunit"] = "w.weight_unit_name as weight_unit_name";
    columns_white_sql["notes"] = "t.notes as notes";
    columns_white_sql["created"] = "to_char(t.created, 'YYYY-MM-DD HH24:MI:SS') as created";
    columns_white_sql["edited"] = "to_char(t.edited, 'YYYY-MM-DD HH24:MI:SS') as edited";
    columns_white_sql["retailupdate"] = "CASE WHEN t.retail_update IS NULL THEN '1900-01-01' ELSE to_char(t.retail_update, 'YYYY-MM-DD') END as retail_update";
    columns_white_sql["wholeupdate"] = "CASE WHEN t.whole_update IS NULL THEN '1900-01-01' ELSE to_char(t.whole_update, 'YYYY-MM-DD') END as whole_update";
    columns_white_sql["wholebeginupdate"] = "CASE WHEN t.wholebegin_update IS NULL THEN '1900-01-01' ELSE to_char(t.wholebegin_update, 'YYYY-MM-DD') END as begin_update";
}

inline void set_columns_grey_sql(){
    columns_grey_sql["id"] = "g.id as id";
    columns_grey_sql["name"] = "t.name as name";
    columns_grey_sql["quantity"] = "g.quantity as quantity";
    columns_grey_sql["reserve"] = "g.reserve_total as reserve";
    columns_grey_sql["year"] = "g.year as year";
    columns_grey_sql["price_ret"] = "CASE WHEN manual_price = true THEN g.price_ret ELSE grey_price(g.trademark_id, g.year, true) END as price_ret";
    columns_grey_sql["price_whole"] = "CASE WHEN manual_price = true THEN g.price_whole ELSE grey_price(g.trademark_id, g.year, false) END as price_whole";
    columns_grey_sql["whole_begin"] = "t.whole_begin as whole_begin";
    columns_grey_sql["storage"] = "p.storage as storage";
    columns_grey_sql["rack"] = "p.rack as rack";
    columns_grey_sql["board"] = "p.board as board";
    columns_grey_sql["box"] = "p.box as box";
    columns_grey_sql["pack"] = "g.pack as pack";
    columns_grey_sql["inspection_id"] = "g.inspection_id as inspection_id";
    columns_grey_sql["inspection"] = "i.insp_name as insp_name";
    columns_grey_sql["addinfo"] = "g.add_info as add_info";
    columns_grey_sql["defect"] = "g.defect as defect";
    columns_grey_sql["category"] = "c.category_name as category_name";
    columns_grey_sql["trademark_id"] = "g.trademark_id as trademark_id";
    columns_grey_sql["category_id"] = "g.category_id as category_id";
}

inline void set_columns_white(bool prices_access){
    int c = 0;
    columns_white_table << columns_white_sql["id"];
    columns_white_ids["id"] = c++;
    columns_white_table << columns_white_sql["photo"];
    columns_white_ids["photo"] = c++;
    columns_white_table << columns_white_sql["reserve"];
    columns_white_ids["reserve"] = c++;
    if(prices_access){
        columns_white_table << columns_white_sql["yearsdiscounts"];
        columns_white_ids["yearsdiscounts"] = c++;
    }
    columns_white_table << columns_white_sql["name"];
    columns_white_ids["name"] = c++;
    columns_white_table << columns_white_sql["subgroup"];
    columns_white_ids["subgroup"] = c++;
    columns_white_table << columns_white_sql["quantity"];
    columns_white_ids["quantity"] = c++;
    if(prices_access){
        columns_white_table << columns_white_sql["price_ret"];
        columns_white_ids["price_ret"] = c++;
        columns_white_table << columns_white_sql["price_whole"];
        columns_white_ids["price_whole"] = c++;
        columns_white_table << columns_white_sql["whole_begin"];
        columns_white_ids["whole_begin"] = c++;
    }
    columns_white_table << columns_white_sql["par1"];
    columns_white_ids["par1"] = c++;
    columns_white_table << columns_white_sql["par2"];
    columns_white_ids["par2"] = c++;
    columns_white_table << columns_white_sql["par3"];
    columns_white_ids["par3"] = c++;
    columns_white_table << columns_white_sql["par4"];
    columns_white_ids["par4"] = c++;
    columns_white_table << columns_white_sql["par5"];
    columns_white_ids["par5"] = c++;
    columns_white_table << columns_white_sql["par6"];
    columns_white_ids["par6"] = c++;
    columns_white_table << columns_white_sql["par7"];
    columns_white_ids["par7"] = c++;
    columns_white_table << columns_white_sql["par8"];
    columns_white_ids["par8"] = c++;
    columns_white_table << columns_white_sql["par9"];
    columns_white_ids["par9"] = c++;
    columns_white_table << columns_white_sql["par10"];
    columns_white_ids["par10"] = c++;
    columns_white_table << columns_white_sql["par11"];
    columns_white_ids["par11"] = c++;
    columns_white_table << columns_white_sql["par12"];
    columns_white_ids["par12"] = c++;
    columns_white_table << columns_white_sql["unit"];
    columns_white_ids["unit"] = c++;
    columns_white_table << columns_white_sql["weight"];
    columns_white_ids["weight"] = c++;
    columns_white_table << columns_white_sql["weightunit"];
    columns_white_ids["weightunit"] = c++;
    columns_white_table << columns_white_sql["notes"];
    columns_white_ids["notes"] = c++;
    columns_white_table << columns_white_sql["created"];
    columns_white_ids["created"] = c++;
    columns_white_table << columns_white_sql["edited"];
    columns_white_ids["edited"] = c++;
    if(prices_access){
        columns_white_table << columns_white_sql["retailupdate"];
        columns_white_ids["retailupdate"] = c++;
        columns_white_table << columns_white_sql["wholeupdate"];
        columns_white_ids["wholeupdate"] = c++;
        columns_white_table << columns_white_sql["wholebeginupdate"];
        columns_white_ids["wholebeginupdate"] = c++;
    }
}

inline void set_columns_grey(bool prices_access){
    int c = 0;
    columns_grey_table << columns_grey_sql["id"];
    columns_grey_ids["id"] = c++;
    columns_grey_table << columns_grey_sql["name"];
    columns_grey_ids["name"] = c++;
    columns_grey_table << columns_grey_sql["quantity"];
    columns_grey_ids["quantity"] = c++;
    columns_grey_table << columns_grey_sql["reserve"];
    columns_grey_ids["reserve"] = c++;
    if(prices_access){
        columns_grey_table << columns_grey_sql["price_ret"];
        columns_grey_ids["price_ret"] = c++;
        columns_grey_table << columns_grey_sql["price_whole"];
        columns_grey_ids["price_whole"] = c++;
        columns_grey_table << columns_grey_sql["whole_begin"];
        columns_grey_ids["whole_begin"] = c++;
    }
    columns_grey_table << columns_grey_sql["year"];
    columns_grey_ids["year"] = c++;
    columns_grey_table << columns_grey_sql["storage"];
    columns_grey_ids["storage"] = c++;
    columns_grey_table << columns_grey_sql["rack"];
    columns_grey_ids["rack"] = c++;
    columns_grey_table << columns_grey_sql["board"];
    columns_grey_ids["board"] = c++;
    columns_grey_table << columns_grey_sql["box"];
    columns_grey_ids["box"] = c++;
    columns_grey_table << columns_grey_sql["pack"];
    columns_grey_ids["pack"] = c++;
    columns_grey_table << columns_grey_sql["inspection"];
    columns_grey_ids["inspection"] = c++;
    columns_grey_table << columns_grey_sql["addinfo"];
    columns_grey_ids["addinfo"] = c++;
    columns_grey_table << columns_grey_sql["defect"];
    columns_grey_ids["defect"] = c++;
    columns_grey_table << columns_grey_sql["category"];
    columns_grey_ids["category"] = c++;
    columns_grey_table << columns_grey_sql["trademark_id"];
    columns_grey_ids["trademark_id"] = c++;
    columns_grey_table << columns_grey_sql["inspection_id"];
    columns_grey_ids["inspection_id"] = c++;
    columns_grey_table << columns_grey_sql["category_id"];
    columns_grey_ids["category_id"] = c++;
}

inline void set_columns_white_names(){
    columns_white_names["id"] = "id";
    columns_white_names["name"] = "Имя";
    columns_white_names["subgroup"] = "Группа";
    columns_white_names["quantity"] = "Кол-во";
    columns_white_names["price_ret"] = "Розн. цена";
    columns_white_names["price_whole"] = "Опт. цена";
    columns_white_names["whole_begin"] = "Опт с";
    columns_white_names["par1_val"] = "1";
    columns_white_names["par2_val"] = "2";
    columns_white_names["par3_val"] = "3";
    columns_white_names["par4_val"] = "4";
    columns_white_names["par5_val"] = "5";
    columns_white_names["par6_val"] = "6";
    columns_white_names["par7_val"] = "7";
    columns_white_names["par8_val"] = "8";
    columns_white_names["par9_val"] = "9";
    columns_white_names["par10_val"] = "10";
    columns_white_names["par11_val"] = "11";
    columns_white_names["par12_val"] = "12";
    columns_white_names["photo"] = "Фото";
    columns_white_names["unit_name"] = "Ед.изм.";
    columns_white_names["weight"] = "Вес";
    columns_white_names["notes"] = "Примечание";
    columns_white_names["created"] = "Создан";
    columns_white_names["edited"] = "Изменён";
    columns_white_names["retail_update"] = "Розн. цена изм.";
    columns_white_names["whole_update"] = "Опт. цена изм.";
    columns_white_names["begin_update"] = "Опт с изм.";
    columns_white_names["years_discounts"] = "Г";
    columns_white_names["reserve_total"] = "Р";
}

inline void set_columns_grey_names(){
    columns_grey_names["id"] = "id";
    columns_grey_names["quantity"] = "Кол-во";
    columns_grey_names["reserve"] = "Резерв";
    columns_grey_names["year"] = "Год";
    columns_grey_names["name"] = "Имя";
    columns_grey_names["storage"] = "Склад";
    columns_grey_names["rack"] = "Стеллаж";
    columns_grey_names["board"] = "Полка";
    columns_grey_names["box"] = "Ящик";
    columns_grey_names["insp_name"] = "Приёмка";
    columns_grey_names["add_info"] = "Доп.пар.1";
    columns_grey_names["defect"] = "Доп.пар.2";
    columns_grey_names["category_name"] = "Категория";
    columns_grey_names["price_ret"] = "Розн. цена";
    columns_grey_names["price_whole"] = "Опт. цена";
    columns_grey_names["whole_begin"] = "Опт с";
    columns_grey_names["trademark_id"] = "id в белом";
    columns_grey_names["inspection_id"] = "ыфвыфыв";
    columns_grey_names["category_id"] = "авававава";
    columns_grey_names["pack"] = "Н. уп.";
}
