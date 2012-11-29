#include "greyadddialog.h"
#include "ui_greyadddialog.h"

GreyAddDialog::GreyAddDialog(int id, int status, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GreyAddDialog)
{
    common_constructor(id, status);
    edited = false;
    ui->remain_label->hide();
}

GreyAddDialog::GreyAddDialog(int id, int place_id, int status, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GreyAddDialog)
{
    common_constructor(id, status);
    edited = false;
    ui->remain_label->hide();

    QSqlQuery query(base);
    QString query_str = QString("SELECT storage, rack, board, box FROM places WHERE id = %1").arg(place_id);
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return;
    }
    query.next();
    QString storage = query.value(0).toString();
    QString rack = query.value(1).toString();
    QString board = query.value(2).toString();
    QString box = query.value(3).toString();
    ui->storage_box->setCurrentIndex(ui->storage_box->findText(storage));
    if(storage.length() > 0){
        ui->rack_box->setEnabled(true);
        fill_racks_box();
        if(rack.length() > 0){
            qDebug() << rack;
            ui->rack_box->setCurrentIndex(ui->rack_box->findText(rack));
            fill_boards_box();
            ui->board_box->setEnabled(true);
            if(board.length() > 0){
                ui->board_box->setCurrentIndex(ui->board_box->findText(board));
                fill_boxes_box();
                ui->box_box->setEnabled(true);
                if(box.length() > 0)
                    ui->box_box->setCurrentIndex(ui->box_box->findText(box));
            }
        }
    }
}

GreyAddDialog::GreyAddDialog(int id,
               int grey_id,
               int old_quantity,
               QString year,
               QString inspection,
               QString pack,
               QString storage,
               QString rack,
               QString board,
               QString box,
               QString category,
               QString add_par_1,
               QString add_par_2,
               int status,
               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GreyAddDialog)
{
    common_constructor(id, status);
    edited = true;
    this->grey_id = grey_id;
    this->old_quantity = old_quantity;
    ui->quantity_spin->setMaximum(old_quantity);
    ui->quantity_spin->setValue(old_quantity);
    connect(ui->quantity_spin, SIGNAL(valueChanged(int)), SLOT(change_remain(int)));
    change_remain(ui->quantity_spin->value());

    // теперь заполняем то что есть
    ui->year_box->setEditText(year);
    ui->inspection_box->setCurrentIndex(ui->inspection_box->findText(inspection));
    ui->pack_text->setText(pack);
    ui->category_box->setCurrentIndex(ui->category_box->findText(category));
    ui->addpar1_box->setEditText(add_par_1);
    ui->addpar2_box->setEditText(add_par_2);
    // складские комбобоксы нужно попутно заполнять
    ui->storage_box->setCurrentIndex(ui->storage_box->findText(storage));
    if(storage.length() > 0){
        ui->rack_box->setEnabled(true);
        fill_racks_box();
        if(rack.length() > 0){
            qDebug() << rack;
            ui->rack_box->setCurrentIndex(ui->rack_box->findText(rack));
            fill_boards_box();
            ui->board_box->setEnabled(true);
            if(board.length() > 0){
                ui->board_box->setCurrentIndex(ui->board_box->findText(board));
                fill_boxes_box();
                ui->box_box->setEnabled(true);
                if(box.length() > 0)
                    ui->box_box->setCurrentIndex(ui->box_box->findText(box));
            }
        }
    }
    params["year"] = year;
    params["inspection"] = inspection;
    params["pack"] = pack;
    params["category"] = category;
    params["add_info"] = add_par_1;
    params["defect"] = add_par_2;
    params["storage"] = storage;
    params["rack"] = rack;
    params["board"] = board;
    params["box"] = box;

    // скрываем количество, т.к. его можно редактировать только через списание или перемещение
    ui->remain_label->show();
    ui->apply_button->hide();
}

GreyAddDialog::~GreyAddDialog()
{
    delete ui;
}

void GreyAddDialog::change_remain(int q){
    ui->remain_label->setText(QString("Ост. %1").arg(old_quantity - q));
}

inline void GreyAddDialog::common_constructor(int id, int status){
    ui->setupUi(this);
    this->setWindowTitle("Новая запись");
    this->trademark_id = id;
    this->status = status;
    set_name();

    ui->rack_box->setEnabled(false);
    ui->board_box->setEnabled(false);
    ui->box_box->setEnabled(false);

    fill_inspections_box();
    fill_years_box();
    fill_storages_box();
    fill_categories_box();
    fill_addpar1_box();
    fill_addpar2_box();

    connects();

    this->adjustSize();
    this->setFixedSize(this->size());
}

void GreyAddDialog::connects(){
    QObject::connect(ui->storage_box, SIGNAL(activated(int)), SLOT(storage_selected(int)));
    QObject::connect(ui->rack_box, SIGNAL(activated(int)), SLOT(rack_selected(int)));
    QObject::connect(ui->board_box, SIGNAL(activated(int)), SLOT(board_selected(int)));
    QObject::connect(ui->box_box, SIGNAL(activated(int)), SLOT(box_selected(int)));
    QObject::connect(ui->cancel_button, SIGNAL(clicked()), SLOT(reject()));
    QObject::connect(ui->apply_button, SIGNAL(clicked()), SLOT(apply()));
    QObject::connect(ui->save_button, SIGNAL(clicked()), SLOT(accept()));
}

void GreyAddDialog::set_name(){
    QSqlQuery query(base);
    QString query_str = QString("SELECT name FROM trademarks WHERE id = %1").arg(trademark_id);
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return;
    }
    query.next();
    ui->detail_name_label->setText(query.value(0).toString());
}


// заполнение всяческих комбобоксов
// приёмка
void GreyAddDialog::fill_inspections_box(){
    QSqlQuery query(base);
    QString query_str = "SELECT id, insp_name FROM inspections";
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return;
    }
    while(query.next())
        this->ui->inspection_box->addItem(query.value(1).toString(), query.value(0).toInt());
    this->ui->inspection_box->setCurrentIndex(this->ui->inspection_box->findText(""));
}

// год
void GreyAddDialog::fill_years_box(){
    this->ui->year_box->addItem("бг");
    this->ui->year_box->addItem("196*");
    this->ui->year_box->addItem("197*");
    this->ui->year_box->addItem("198*");
    this->ui->year_box->addItem("199*");
    this->ui->year_box->addItem("200*");
    for(int y=1960; y <= QDate::currentDate().year(); y++)
        this->ui->year_box->addItem(QString::number(y));
}

// склад
void GreyAddDialog::fill_storages_box(){
    QSqlQuery query(base);
    QString query_str = "SELECT DISTINCT storage FROM places ORDER BY storage";
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return;
    }
    while(query.next())
        this->ui->storage_box->addItem(query.value(0).toString());
}

// стеллаж
void GreyAddDialog::fill_racks_box(){
    QSqlQuery query(base);
    QString query_str = QString("SELECT DISTINCT rack FROM places WHERE storage = '%1' ORDER BY rack").arg(ui->storage_box->currentText());
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return;
    }
    while(query.next())
        this->ui->rack_box->addItem(query.value(0).toString());
}

// полка
void GreyAddDialog::fill_boards_box(){
    QSqlQuery query(base);
    QString query_str = QString("SELECT DISTINCT board FROM places WHERE storage = '%1' AND rack = '%2' ORDER BY board")
            .arg(ui->storage_box->currentText())
            .arg(ui->rack_box->currentText());
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return;
    }
    while(query.next())
        this->ui->board_box->addItem(query.value(0).toString());
}

// ящик
void GreyAddDialog::fill_boxes_box(){
    QSqlQuery query(base);
    QString query_str = QString("SELECT DISTINCT box FROM places WHERE storage = '%1' AND rack = '%2' AND board = '%3' ORDER BY box")
            .arg(ui->storage_box->currentText())
            .arg(ui->rack_box->currentText())
            .arg(ui->board_box->currentText());
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return;
    }
    while(query.next())
        this->ui->box_box->addItem(query.value(0).toString());
}

// категории
void GreyAddDialog::fill_categories_box(){
    QSqlQuery query(base);
    QString query_str = "SELECT id, category_name FROM categories";
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return;
    }
    while(query.next())
        this->ui->category_box->addItem(query.value(1).toString(), query.value(0).toInt());
    ui->category_box->setCurrentIndex(ui->category_box->findText(""));
}

void GreyAddDialog::fill_addpar1_box(){
    QSqlQuery query(base);
    QString query_str = QString("SELECT DISTINCT add_info FROM greytable WHERE trademark_id = %1").arg(trademark_id);
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return;
    }
    while(query.next())
        this->ui->addpar1_box->addItem(query.value(0).toString());
    ui->addpar1_box->setEditText("");
}

void GreyAddDialog::fill_addpar2_box(){
    QSqlQuery query(base);
    QString query_str = QString("SELECT DISTINCT defect FROM greytable WHERE trademark_id = %1").arg(trademark_id);
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return;
    }
    while(query.next())
        this->ui->addpar2_box->addItem(query.value(0).toString());
    ui->addpar2_box->setEditText("");
}


// СЛОТЫ
void GreyAddDialog::storage_selected(int){
    ui->rack_box->clear();
    ui->board_box->clear();
    ui->box_box->clear();
    ui->rack_box->setEnabled(true);
    ui->board_box->setEnabled(false);
    ui->box_box->setEnabled(false);
    fill_racks_box();
}

void GreyAddDialog::rack_selected(int){
    ui->board_box->clear();
    ui->box_box->clear();
    ui->board_box->setEnabled(true);
    ui->box_box->setEnabled(false);
    fill_boards_box();
}

void GreyAddDialog::board_selected(int){
    ui->box_box->clear();
    ui->box_box->setEnabled(true);
    fill_boxes_box();

}

void GreyAddDialog::box_selected(int){

}

int GreyAddDialog::find_place(){
    // находим id места на складе. Если такового нет - предлагаем обратиться к зав.складу с просьбой создать
    QSqlQuery query(base);
    QString query_str = QString("SELECT id FROM places WHERE storage = '%1' AND rack = '%2' AND board = '%3' AND box = '%4'")
            .arg(ui->storage_box->currentText())
            .arg(ui->rack_box->currentText())
            .arg(ui->board_box->currentText())
            .arg(ui->box_box->currentText());
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()), QMessageBox::Ok);
        return -1;
    }
    if(query.size() == 0){
        QMessageBox::warning(this, "Ошибка", "Введённого Вами расположения на складе не существует в базе. Выберите существующее расположение или обратитесь к заведующему складом с просьбой добавить его.", QMessageBox::Ok);
        return -1;
    }
    query.next();
    return query.value(0).toInt();
}

int GreyAddDialog::detail_exists(int place_id){
    /*
        Проверяем, есть ли в базе деталь с такими же параметрами.
        Должны совпадать:
          - тов. марка (естественно)
          - место (ищем в таблице мест id места по складу, стеллажу, полке, ящику)
          - год
          - приёмка
          - норма упаковки
          - категория
          - доп. параметры 1, 2
          - если речь о синем экране - ещё и имя пользователя
    */
    QSqlQuery query(base);
    QString query_str;
    query_str = QString("SELECT id, quantity FROM greytable WHERE trademark_id = %1 "
                        "AND place_id = %2 "
                        "AND year = '%3' "
                        "AND inspection_id = %4 "
                        "AND pack = '%5' "
                        "AND category_id = %6 "
                        "AND add_info = '%7' "
                        "AND defect = '%8' "
                        "AND status = %9")
            .arg(this->trademark_id)
            .arg(place_id)
            .arg(ui->year_box->currentText())
            .arg(ui->inspection_box->itemData(ui->inspection_box->currentIndex()).toInt())
            .arg(ui->pack_text->text())
            .arg(ui->category_box->itemData(ui->category_box->currentIndex()).toInt())
            .arg(ui->addpar1_box->currentText())
            .arg(ui->addpar2_box->currentText())
            .arg(this->status);
    if(this->status == 3)
        query_str += QString(" AND add_user_id = %1").arg(USER_ID);
    //qDebug() << query_str;
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()), QMessageBox::Ok);
        return -1;
    }

    if(query.size() > 0){
        query.next();
        return query.value(0).toInt();
    }
    return -1;
}

bool GreyAddDialog::add_quantity(int id){
    QSqlQuery query(base);
    QString query_str = QString("UPDATE greytable SET quantity = quantity + %1 WHERE id = %2 RETURNING quantity")
            .arg(ui->quantity_spin->value())
            .arg(id);
    //qDebug() << query_str;
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()), QMessageBox::Ok);
        return false;
    }
    query.next();
    // выводим сообщение об успешном завершении операции
    QMessageBox::information(this, "Добавлено", QString("Количество суммировано с id %1. Общее количество: %2")
                             .arg(id)
                             .arg(query.value(0).toInt()),
                             QMessageBox::Ok);
    return true;
}


bool GreyAddDialog::save_new(bool transaction){
    /*
      Проверка на корректность.
      - количество должно быть больше нуля;
      - файл с фотографией (если он указан) должен существовать.
    */
    // потом

    int place_id = find_place();
    if(place_id < 0)
        return false;

    int existing_id = detail_exists(place_id);
    // если такая деталь уже есть!
    if(existing_id > 0){
        // добавляем кол-во к этой записи
        if(!add_quantity(existing_id))
            return false;
        return true;
    }
    // если нет - нужно создать новую запись
    if(transaction){
        if(!base.transaction()){
            error("Ошибка", QString("Невозможно начать транзакцию\n").append(base.lastError().text()));
            return false;
        }
    }

    QSqlQuery query(base);
    QString query_str;

    params.clear();
    params["trademark_id"] = QString::number(trademark_id);
    params["quantity"] = QString::number(ui->quantity_spin->value());
    params["place_id"] = QString::number(place_id);
    params["year"] = ui->year_box->currentText();
    params["inspection_id"] = QString::number(ui->inspection_box->itemData(ui->inspection_box->currentIndex()).toInt());
    params["pack"] = ui->pack_text->text();
    params["category_id"] = QString::number(ui->category_box->itemData(ui->category_box->currentIndex()).toInt());
    params["add_info"] = ui->addpar1_box->currentText();
    params["defect"] = ui->addpar2_box->currentText();
    params["reserve_total"] = QString::number(0);
    params["status"] = QString::number(this->status);
    params["add_user_id"] = QString::number(USER_ID);

    QString params_names = QStringList(params.keys()).join(", ");
    QString params_values = QStringList(params.values()).join("', '");
    query_str = QString("INSERT INTO greytable (%1) VALUES ('%2') RETURNING id").arg(params_names).arg(params_values);
    qDebug() << query_str;
    if(!query.exec(query_str)){
        error("Ошибка", QString("Ошибка при сохранении детали.\n").append(query.lastError().text()));
        query.finish();
        if(transaction) base.rollback();
        return false;
    }
    success_text += "Деталь добавлена в базу.";
    query.next();
    int last_id = query.value(0).toInt();
    if(last_id > 0){
       success_text += QString(" Номер (id): %1\n").arg(last_id);
       params["id"] = QString::number(last_id);
    }
    else{
        error("Ошибка", QString("Некорректное добавление записи в базу (id = %1)").arg(last_id));
        query.finish();
        if(transaction) base.rollback();
        return false;
    }
    emit set_place_id(place_id);

    if(transaction) base.commit();
    //QMessageBox::information(this, "", success_text, QMessageBox::Ok);
    return true;
}

void GreyAddDialog::fill_current(){

}

bool GreyAddDialog::save_edited(){
    /*int place_id = find_place();
    if(place_id < 0)
        return false;

    int existing_id = detail_exists(place_id);
    // если такая деталь уже есть!
    if(existing_id > 0){
        // добавляем кол-во к этой записи
        if(!add_quantity(existing_id))
            return false;
        return true;
    }
*/
    QSqlQuery query(base);
    QString query_str;
    int place_id;

    // подготавливаем список изменённых параметров
    QMap<QString, QString> changed_params;
    if(params["year"] != ui->year_box->currentText())
        changed_params["year"] = ui->year_box->currentText();
    if(params["inspection"] != ui->inspection_box->currentText())
        changed_params["inspection_id"] = QString::number(ui->inspection_box->itemData(ui->inspection_box->currentIndex()).toInt());
    if(params["pack"] != ui->pack_text->text())
        changed_params["pack"] = ui->pack_text->text();
    if(params["category"] != ui->category_box->currentText())
        changed_params["category_id"] = QString::number(ui->category_box->itemData(ui->category_box->currentIndex()).toInt());
    if(params["add_info"] != ui->addpar1_box->currentText())
        changed_params["add_info"] = ui->addpar1_box->currentText();
    if(params["defect"] != ui->addpar2_box->currentText())
        changed_params["defect"] = ui->addpar2_box->currentText();

    // изменение места отслеживаем отдельно, чтобы знать, имеет ли место быть перемещение
    bool move = false;
    if(params["storage"] != ui->storage_box->currentText()
            || params["rack"] != ui->rack_box->currentText()
            || params["board"] != ui->board_box->currentText()
            || params["box"] != ui->box_box->currentText()){
        move = true;
    }
    if(move){
        place_id = find_place();
        changed_params["place_id"] = QString::number(place_id);
    }

    // если изменённых параметров так и не нашлось, то всё ок
    if(changed_params.count() == 0)
        return true;


    if(!base.transaction()){
        error("Ошибка", QString("Невозможно начать транзакцию\n").append(base.lastError().text()));
        return false;
    }
    // если перемещается часть кол-ва, нужно создать новую запись, а от существующей отнять кол-во
    if(ui->quantity_spin->value() != old_quantity){
        if(!create_new_grey()){
            base.rollback();
            return false;
        }
        if(move){
            // ЗАПИСАТЬ ПЕРЕМЕЩЕНИЕ В ИСТОРИЮ!
        }
    }
    else{
        // делаем UPDATE
        query_str = "UPDATE greytable SET ";
        QMapIterator<QString, QString> it(changed_params);
        while(it.hasNext()){
            it.next();
            query_str += QString("%1 = '%2'").arg(it.key()).arg(it.value());
            if(it.hasNext()) query_str += ", ";
        }
        query_str += QString(" WHERE id = %1").arg(grey_id);
        if(!query.exec(query_str)){
            error("Ошибка", QString("Ошибка при сохранении детали.\n").append(query.lastError().text()));
            query.finish();
            base.rollback();
            return false;
        }
    }

    base.commit();
    return true;
}

bool GreyAddDialog::create_new_grey(){
    if(!save_new(false))
        return false;
    QSqlQuery query(base);
    // а от существующего id отнять кол-во
    QString query_str = QString("UPDATE greytable SET quantity = quantity - %1 WHERE id = %2").arg(ui->quantity_spin->value()).arg(grey_id);
    if(!query.exec(query_str)){
        error("Ошибка", QString("Ошибка при сохранении детали.\n").append(query.lastError().text()));
        return false;
    }
    return true;
}

bool GreyAddDialog::apply(){
    if(ui->quantity_spin->value() <= 0){
        QMessageBox::warning(this, "Ошибка", "Введите количество", QMessageBox::Ok);
        return false;
    }
    if(!edited){
        if(save_new()){
            //edited = true;
            fill_current();
            emit need_refresh();
            return true;
        }
    }
    else{
        if(save_edited()){
            fill_current();
            emit need_refresh();
            return true;
        }
    }
    return false;
}

void GreyAddDialog::accept(){
    if(apply())
        QDialog::accept();
}
