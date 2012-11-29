#include "whiteadddialog.h"
#include "ui_whiteadddialog.h"

WhiteAddDialog::WhiteAddDialog(int gr, QString name, int mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::whiteAddDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Новая деталь");
    this->ui->name_box->setEditable(true);
    this->ui->name_box->setAutoCompletion(true);
    this->ui->name_box->setEnabled(false);
    this->ui->name_label->setEnabled(false);
    HeaderComboBoxDelegate *delegate = new HeaderComboBoxDelegate();
    HeaderModel *headermodel = new HeaderModel();
    this->ui->group_box->setItemDelegate(delegate);
    this->ui->group_box->setModel(headermodel);
    this->ui->id_label->setText("");

    this->dialog_mode = mode;
    params_enabled = false;

    create_unrequired_list();
    disable_params();
    fill_groups_box();
    fill_weight_box();
    fill_unit_box();

    if(gr > 0 && name.length() > 0){
        // выбираем строку в комбобоксе и устанавливаем названия параметров
        int group_index = this->ui->group_box->findText(name);
        this->ui->group_box->setCurrentIndex(group_index);
        change_group(group_index);
    }

    connects();
}

WhiteAddDialog::WhiteAddDialog(int id, int mode, QWidget *parent) : QDialog(parent), ui(new Ui::whiteAddDialog){
    ui->setupUi(this);
    this->setWindowTitle("Редактирование детали");
    this->ui->name_box->setEditable(true);
    this->ui->name_box->setAutoCompletion(true);
    this->ui->name_box->setEnabled(false);
    this->ui->name_label->setEnabled(false);
    HeaderComboBoxDelegate *delegate = new HeaderComboBoxDelegate();
    HeaderModel *headermodel = new HeaderModel();
    this->ui->group_box->setItemDelegate(delegate);
    this->ui->group_box->setModel(headermodel);
    this->dialog_mode = mode;
    create_unrequired_list();
    fill_groups_box();
    fill_weight_box();
    fill_unit_box();
    params_enabled = true;

    params["id"] = QString::number(id);
    this->ui->id_label->setText(QString("id = %1").arg(id));
    fill_current();

    connects();
}

WhiteAddDialog::~WhiteAddDialog()
{
    delete ui;
}

inline void WhiteAddDialog::connects(){
    QObject::connect(this->ui->group_box, SIGNAL(activated(int)), SLOT(change_group(int)));
    QObject::connect(this->ui->photo_dialog_button, SIGNAL(clicked()), SLOT(open_photo_dialog()));
    QObject::connect(this->ui->photo_remove_button, SIGNAL(clicked()), SLOT(clear_photo_path()));
    QObject::connect(this->ui->analog_add_button, SIGNAL(clicked()), SLOT(add_analog_to_list()));
    QObject::connect(this->ui->analog_remove_button, SIGNAL(clicked()), SLOT(remove_analog_from_list()));
    QObject::connect(this->ui->cancel_button, SIGNAL(clicked()), SLOT(reject()));
    QObject::connect(this->ui->apply_button, SIGNAL(clicked()), SLOT(apply()));
    QObject::connect(this->ui->save_button, SIGNAL(clicked()), SLOT(accept()));

}

inline void WhiteAddDialog::create_unrequired_list(){
    unrequired_widgets << this->ui->weight_label
                       << this->ui->weight_line
                       << this->ui->weight_unit_box
                       << this->ui->photo_label
                       << this->ui->photo_line
                       << this->ui->photo_dialog_button
                       << this->ui->photo_remove_button
                       << this->ui->weight_label
                       << this->ui->weight_line
                       << this->ui->weight_unit_box
                       << this->ui->unit_label
                       << this->ui->unit_box
                       << this->ui->notes_label
                       << this->ui->notes_edit
                       << this->ui->analogs_label
                       << this->ui->analogs_box
                       << this->ui->analogs_list
                       << this->ui->analog_add_button
                       << this->ui->analog_remove_button
                       << this->ui->par1_label
                       << this->ui->par1_line
                       << this->ui->par2_label
                       << this->ui->par2_line
                       << this->ui->par3_label
                       << this->ui->par3_line
                       << this->ui->par4_label
                       << this->ui->par4_line
                       << this->ui->par5_label
                       << this->ui->par5_line
                       << this->ui->par6_label
                       << this->ui->par6_line
                       << this->ui->par7_label
                       << this->ui->par7_line
                       << this->ui->par8_label
                       << this->ui->par8_line
                       << this->ui->par9_label
                       << this->ui->par9_line
                       << this->ui->par10_label
                       << this->ui->par10_line
                       << this->ui->par11_label
                       << this->ui->par11_line
                       << this->ui->par12_label
                       << this->ui->par12_line
                       << this->ui->apply_button
                       << this->ui->save_button;
}

bool WhiteAddDialog::fill_current(){
    QSqlQuery query(base);
    QString query_str = QString("SELECT * FROM trademarks WHERE id = %1 AND status = 1").arg(params["id"]);
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return false;
    }
    if(query.size() != 1){
        error("Ошибка", QString("Не найдена деталь с заданным id: %1").arg(params["id"]));
        return false;
    }
    QSqlRecord rec = query.record();
    query.next();
    params["subgroup_id"] = query.value(rec.indexOf("subgroup_id")).toString();
    this->ui->group_box->setCurrentIndex(this->ui->group_box->findData(params["subgroup_id"].toInt()));
    change_params_labels(params["subgroup_id"].toInt());
    fill_names_box(params["subgroup_id"].toInt());

    params["name"] = query.value(rec.indexOf("name")).toString();
    this->ui->name_box->setEditText(params["name"]);

    params["weight"] = query.value(rec.indexOf("weight")).toString();
    this->ui->weight_line->setText(params["weight"]);

    params["weight_unit_id"] = query.value(rec.indexOf("weight_unit_id")).toString();
    this->ui->weight_unit_box->setCurrentIndex(this->ui->weight_unit_box->findData(params["weight_unit_id"].toInt()));

    params["unit_id"] = query.value(rec.indexOf("unit_id")).toString();
    this->ui->unit_box->setCurrentIndex(this->ui->unit_box->findData(params["unit_id"].toInt()));

    if(query.value(rec.indexOf("photo")).toString().length() > 0)
        params["photo"] = QString("%1%2").arg(PHOTOS_PATH).arg(query.value(rec.indexOf("photo")).toString());
    else params["photo"] = "";
    this->ui->photo_line->setText(params["photo"]);


    params["notes"] = query.value(rec.indexOf("notes")).toString();
    this->ui->notes_edit->setText(params["notes"]);

    params["par1_val"] = query.value(rec.indexOf("par1_val")).toString();
    this->ui->par1_line->setText(params["par1_val"]);
    params["par2_val"] = query.value(rec.indexOf("par2_val")).toString();
    this->ui->par2_line->setText(params["par2_val"]);
    params["par3_val"] = query.value(rec.indexOf("par3_val")).toString();
    this->ui->par3_line->setText(params["par3_val"]);
    params["par4_val"] = query.value(rec.indexOf("par4_val")).toString();
    this->ui->par4_line->setText(params["par4_val"]);
    params["par5_val"] = query.value(rec.indexOf("par5_val")).toString();
    this->ui->par5_line->setText(params["par5_val"]);
    params["par6_val"] = query.value(rec.indexOf("par6_val")).toString();
    this->ui->par6_line->setText(params["par6_val"]);
    params["par7_val"] = query.value(rec.indexOf("par7_val")).toString();
    this->ui->par7_line->setText(params["par7_val"]);
    params["par8_val"] = query.value(rec.indexOf("par8_val")).toString();
    this->ui->par8_line->setText(params["par8_val"]);
    params["par9_val"] = query.value(rec.indexOf("par9_val")).toString();
    this->ui->par9_line->setText(params["par9_val"]);
    params["par10_val"] = query.value(rec.indexOf("par10_val")).toString();
    this->ui->par10_line->setText(params["par10_val"]);
    params["par11_val"] = query.value(rec.indexOf("par11_val")).toString();
    this->ui->par11_line->setText(params["par11_val"]);
    params["par12_val"] = query.value(rec.indexOf("par12_val")).toString();
    this->ui->par12_line->setText(params["par12_val"]);

    // и аналоги сюда же
    this->ui->analogs_list->clear();
    analogs_old.clear();
    query_str = QString("SELECT m.id, t.id as tid, t.name FROM analogs a "
                        "JOIN trademarks m ON a.trademark_id = m.id "
                        "JOIN trademarks t ON a.analog_id = t.id "
                        "WHERE m.id=%1").arg(params["id"]);
    if(!query.exec(query_str)){
        error("Ошибка (Аналоги)", QString("Не удалось выполнить запрос: ").append(query.lastError().text()));
        return false;
    }
    QListWidgetItem *item = 0;
    while(query.next()){
        item = new QListWidgetItem();
        item->setText(query.value(2).toString());
        item->setData(Qt::UserRole, query.value(1).toInt());
        this->ui->analogs_list->addItem(item);

        analogs_old << query.value(2).toString();
    }

    return true;
}

bool WhiteAddDialog::fill_weight_box(){
    QSqlQuery query(base);
    QString str_select = "SELECT id, weight_unit_name FROM weight_units";
    if(!query.exec(str_select)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(str_select));
        return false;
    }
    while(query.next())
        this->ui->weight_unit_box->addItem(query.value(1).toString(), query.value(0).toInt());
    this->ui->weight_unit_box->setCurrentIndex(this->ui->weight_unit_box->findText(DEFAULT_WEIGHT_UNIT));
    return true;
}

bool WhiteAddDialog::fill_unit_box(){
    QSqlQuery query(base);
    QString str_select = "SELECT id, unit_name FROM units";
    if(!query.exec(str_select)){
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(str_select));
        return false;
    }
    while(query.next())
        this->ui->unit_box->addItem(query.value(1).toString(), query.value(0).toInt());
    this->ui->unit_box->setCurrentIndex(this->ui->unit_box->findText(DEFAULT_UNIT));
    return true;
}

bool WhiteAddDialog::fill_groups_box(){
    // выбираем список групп из БД
    QSqlQuery query(base);
    QString str_select = "SELECT id, name FROM " + GROUPS_TABLE + " ORDER BY name";

    // если не удаётся выполнить запрос, возвращаем ошибку
    if(!query.exec(str_select)){
        error("Ошибка", QString("Не удалось получить список групп:\n").append(str_select));
        return false;
    }

    QString group_name;                              // название группы - для отображения в каталоге
    int group_num;                                   // номер группы - для использования в фильтре

    // начинаем разбирать результат запроса
    QSqlRecord rec = query.record();
    while(query.next()){
        group_name = query.value(rec.indexOf("name")).toString();
        group_num = query.value(rec.indexOf("id")).toInt();
        // добавляем группу в комбобокс
        this->ui->group_box->addItem(group_name, -1);

        // ДОБАВЛЯЕМ ПОДГРУППУ
        if(!fill_subgroup(group_num))
            return false;
    }
    //this->ui->group_box->setEditText("Выберите группу");
    this->ui->group_box->setCurrentIndex(-1);
    return true;
}

bool WhiteAddDialog::fill_subgroup(int id){
    QSqlQuery query(base);
    QString str_select = "SELECT id, name FROM " + SUBGROUPS_TABLE + " WHERE group_id=" + QString::number(id);

    // если не удаётся выполнить запрос, возвращаем ошибку
    if(!query.exec(str_select)){
        error("Ошибка", QString("Не удалось получить список подгрупп:\n").append(str_select));
        return false;
    }

    QString subgroup_name;               // отображаемый текст
    int subgroup_num;                    // номер подгруппы для передачи в фильтр

    // начинаем разбирать результат запроса
    QSqlRecord rec = query.record();
    while(query.next()){
        subgroup_name = query.value(rec.indexOf("name")).toString();
        subgroup_num = query.value(rec.indexOf("id")).toInt();
        // добавляем подгруппу в комбобокс
        this->ui->group_box->addItem(subgroup_name, subgroup_num);
    }
    return true;
}

void WhiteAddDialog::change_group(int index){
    // получаем номер группы из данных, хранящихся в этом элементе
    int group_id = this->ui->group_box->itemData(index).toInt();
    if(group_id > 0 && group_id < ENLARGER){
        change_params_labels(group_id);
        fill_names_box(group_id);
    }
}

void WhiteAddDialog::fill_names_box(int group_id){
    // ДАЛЕЕ: ЗАПОЛНЯЕМ БОКС С НАЗВАНИЯМИ
    this->ui->name_label->setEnabled(true);
    this->ui->name_box->setEnabled(true);
    this->ui->name_box->clear();
    this->details_names.clear();
    this->ui->analogs_list->clear();
    QSqlQuery query(base);
    QString query_str = "SELECT id, name FROM " + TOVMARKS_TABLE + " WHERE subgroup_id = " + QString::number(group_id);
    if(!query.exec(query_str))
        error("Ошибка", QString("Не удалось выполнить запрос: ").append(query_str));
    else{
        while(query.next()){
            DetailSmall ds = {query.value(0).toInt(), query.value(1).toString()};
            details_names << ds;
            this->ui->name_box->addItem(ds.name);
        }
        if(this->dialog_mode == DIALOG_WHITE_NEW)
            this->ui->name_box->setEditText("");
        else
            this->ui->name_box->setEditText(params["name"]);
    }
    // теперь можно заполнить остальные, необязательные параметры
    QObject::connect(this->ui->name_box, SIGNAL(editTextChanged(QString)), SLOT(enable_params(QString)));
}

void WhiteAddDialog::open_photo_dialog(){
    QString photo_name = QFileDialog::getOpenFileName(this, "Выберите файл", QString(), "Изображения (*.jpg *.jpeg *.gif *.bmp *.png)");
    if(photo_name.length() > 0)
        this->ui->photo_line->setText(photo_name);
}

void WhiteAddDialog::clear_photo_path(){
    this->ui->photo_line->clear();
}

void WhiteAddDialog::add_analog_to_list(){
    int index = this->ui->analogs_box->currentIndex();
    QString analog_name = this->ui->analogs_box->itemText(index);
    int analog_id = this->ui->analogs_box->itemData(index).toInt();
    if(this->ui->analogs_list->findItems(analog_name, 0).count() == 0){
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(analog_name);
        item->setData(Qt::UserRole, analog_id);
        this->ui->analogs_list->addItem(item);
    }
}

void WhiteAddDialog::remove_analog_from_list(){
    this->ui->analogs_list->removeItemWidget(this->ui->analogs_list->takeItem(this->ui->analogs_list->currentRow()));
}

void WhiteAddDialog::enable_params(QString str){
    if(str.length() == 0)
        disable_params();
    else{
        if(!params_enabled){
            QWidget *w;
            foreach(w, unrequired_widgets)
                w->setEnabled(true);
            params_enabled = true;
        }
        this->ui->analogs_box->clear();
        for(int i = 0; i < details_names.count(); i++)
            if(details_names[i].name != str)
                this->ui->analogs_box->addItem(details_names[i].name, details_names[i].id);
        this->ui->analogs_box->setEditText("");
    }
}

void WhiteAddDialog::disable_params(){
    QWidget *w;
    foreach(w, unrequired_widgets)
        w->setEnabled(false);
    this->ui->analogs_box->clear();
    this->ui->analogs_box->setEditText("");
    this->ui->analogs_list->clear();
    params_enabled = false;
}

void WhiteAddDialog::change_params_labels(int gr){
    QSqlQuery q(base);
    QString query = "SELECT * FROM " + SUBGROUPS_TABLE + " WHERE id=" + QString::number(gr);
    if(!q.exec(query)){
        error("Ошибка", QString("Не удалось выполнить запрос:\n").append(query));
        // params в этом случае остаётся пустым, и названия столбцов просто не заполнятся
    }
    else{
        QSqlRecord rec = q.record();
        q.next();
        this->ui->par1_label->setText(q.value(rec.indexOf("par1")).toString());
        this->ui->par2_label->setText(q.value(rec.indexOf("par2")).toString());
        this->ui->par3_label->setText(q.value(rec.indexOf("par3")).toString());
        this->ui->par4_label->setText(q.value(rec.indexOf("par4")).toString());
        this->ui->par5_label->setText(q.value(rec.indexOf("par5")).toString());
        this->ui->par6_label->setText(q.value(rec.indexOf("par6")).toString());
        this->ui->par7_label->setText(q.value(rec.indexOf("par7")).toString());
        this->ui->par8_label->setText(q.value(rec.indexOf("par8")).toString());
        this->ui->par9_label->setText(q.value(rec.indexOf("par9")).toString());
        this->ui->par10_label->setText(q.value(rec.indexOf("par10")).toString());
        this->ui->par11_label->setText(q.value(rec.indexOf("par11")).toString());
        this->ui->par12_label->setText(q.value(rec.indexOf("par12")).toString());
    }
}

bool WhiteAddDialog::valid_name(){
    bool alright = true;
    // кавычек в названии быть не должно!
    if(this->ui->name_box->currentText().indexOf("'") >= 0 || this->ui->name_box->currentText().indexOf("\"") >= 0){
        valid_error_text += "Недопустимые символы (кавычки) в названии!\n";
        alright = false;
    }
    // проверяем, существует ли деталь с таким именем
    QSqlQuery query(QString("SELECT id FROM trademarks WHERE name = '%1' AND status = 1").arg(this->ui->name_box->currentText()), base);
    if(query.size() > 0){
        valid_error_text += "Деталь с таким наименованием уже существует.\n";
        alright = false;
    }

    return alright;
}

bool WhiteAddDialog::valid_weight(){
    bool alright = true;
    // проверяем, корректно ли введён вес
    QRegExp re("[0-9]+([,.][0-9]+)?");
    if(this->ui->weight_line->text().length() > 0 && (!re.exactMatch(this->ui->weight_line->text()))){
        valid_error_text += "Вес должен быть целым числом или десятичной дробью.\n";
        alright = false;
    }

    return alright;
}

bool WhiteAddDialog::valid_photo(){
    bool alright = true;
    // проверяем, существует ли файл с фотографией
    if(this->ui->photo_line->text().length() > 0){
        if(!QFile::exists(this->ui->photo_line->text())){
            valid_error_text += "Файла с фотографией не существует.\n";
            alright = false;
        }
    }

    return alright;
}

bool WhiteAddDialog::valid(){
    valid_error_text = "";
    bool alright = true;
    if(this->dialog_mode == DIALOG_WHITE_NEW)
        return (valid_name() & valid_weight() & valid_photo());
    else if(this->dialog_mode == DIALOG_WHITE_EDIT){
        if(params["name"] != this->ui->name_box->currentText())
            alright = alright & valid_name();
        if(params["weight"] != this->ui->weight_line->text())
            alright = alright & valid_weight();
        if(params["photo"] != this->ui->photo_line->text())
            alright = alright & valid_photo();
    }

    return alright;
}

bool WhiteAddDialog::save_new(){
    success_text = "";

    /* Проверка на корректность введённых данных. Имеются следующие ограничений:
        - наименование не должно дублировать уже имеющееся в базе, а также не должно содержать кавычек;
        - вес должен быть целым числом или десятичной дробью (в качестве разделителя допускаются точка и запятая);
        - файл с фотографией должен существовать;
    */
    if(!valid()){
        error("Ошибка", valid_error_text);
        return false;
    }

    // превращаем имя в паттерн путём убирания пробелов и перевода всё в нижний регистр
    QString pattern = this->ui->name_box->currentText().remove(QRegExp("[^a-zA-Zа-яА-Я\\d]")).toUpper();

    // подготавливаем и выполняем запрос
    if(!base.transaction()){
        error("Ошибка", QString("Невозможно начать транзакцию\n").append(base.lastError().text()));
        return false;
    }

    params["name"] = this->ui->name_box->currentText();
    params["pattern"] = pattern;
    params["subgroup_id"] = this->ui->group_box->itemData(this->ui->group_box->currentIndex()).toString();
    params["par1_val"] = this->ui->par1_line->text();
    params["par2_val"] = this->ui->par2_line->text();
    params["par3_val"] = this->ui->par3_line->text();
    params["par4_val"] = this->ui->par4_line->text();
    params["par5_val"] = this->ui->par5_line->text();
    params["par6_val"] = this->ui->par6_line->text();
    params["par7_val"] = this->ui->par7_line->text();
    params["par8_val"] = this->ui->par8_line->text();
    params["par9_val"] = this->ui->par9_line->text();
    params["par10_val"] = this->ui->par10_line->text();
    params["par11_val"] = this->ui->par11_line->text();
    params["par12_val"] = this->ui->par12_line->text();
    params["unit_id"] = this->ui->unit_box->itemData(this->ui->unit_box->currentIndex()).toString();
    params["weight_unit_id"] = this->ui->weight_unit_box->itemData(this->ui->weight_unit_box->currentIndex()).toString();
    params["notes"] = this->ui->notes_edit->toPlainText();
    params["status"] = "1";
    params["years_discounts"] = "0";
    params["reserve_total"] = "0";

    if(this->ui->weight_line->text().length() > 0)
        params["weight"] = this->ui->weight_line->text().replace(",", ".");

    QString params_names = QStringList(params.keys()).join(", ");
    QString params_values = QStringList(params.values()).join("', '");
    QString query_str = QString("INSERT INTO trademarks (%1) VALUES ('%2') RETURNING id").arg(params_names).arg(params_values);

    QSqlQuery query(base);
    if(!query.exec(query_str)){
        error("Ошибка", QString("Ошибка при сохранении детали.\n").append(query.lastError().text()));
        query.finish();
        base.rollback();
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
        base.rollback();
        return false;
    }
    //qDebug() << query_str;
    if(!base.commit()){
        error("Ошибка", QString("Не удалось завершить транзакцию\n").append(base.lastError().text()));
        return false;
    }

    params["photo"] = this->ui->photo_line->text();
    if(!proceed_photo())
        success_text += "Фото не добавлено.\n";

    // осталось добавить аналоги в таблицу 'analogs'
    if(!add_analogs())
        success_text += "Аналоги добавить не удалось.\n";

    QMessageBox::information(this, "", success_text, QMessageBox::Ok);
    return true;
}

bool WhiteAddDialog::proceed_photo(){
    QSqlQuery query(base);
    // на основе полученного id переименовываем фотографию и копируем её в системную папку
    if(params["photo"].length() > 0){
        QString newname = QString("%1%2.%3").arg(PHOTOS_PATH).arg(params["id"]).arg(QFileInfo(params["photo"]).completeSuffix());
        bool exists = false;

        // если файл существует, переименовываем его, чтобы потом в случае фейла при копировании восстановить
        QDir dir(PHOTOS_PATH);
        QStringList f(QString("%1.*").arg(params["id"]));
        QStringList l = dir.entryList(f);
        QString oldname;
        if(l.count() > 0){
            exists = true;
            oldname = QString("%1%2").arg(PHOTOS_PATH).arg(l.at(0));
            if(QFile::rename(oldname, QString("%1.old").arg(oldname))){
                for(int i = 1; i < l.count(); i++)
                    QFile::remove(QString("%1%2").arg(PHOTOS_PATH).arg(l.at(i)));
            }
        }
        if(QFile::copy(params["photo"], newname)){
            if(exists) QFile::remove(oldname.append(".old"));
        }
        else{
            success_text += QString("Не удалось скопировать файл по адресу: %1\n").arg(newname);
            if(exists) QFile::rename(oldname.append(".old"), oldname);
            return false;
        }

        // добавляем поле "фото" в базу
        if(!query.exec(QString("UPDATE trademarks SET photo = '%1' WHERE id = %2").arg(QFileInfo(newname).fileName()).arg(params["id"]))){
            success_text += QString("Ошибка при добавлении поля \"фото\".\n").append(query.lastError().text());
            return false;
        }
    }

    return true;
}

bool WhiteAddDialog::add_analogs(){
    if(!base.transaction()){
        error("Ошибка", QString("Невозможно начать транзакцию\n").append(base.lastError().text()));
        return false;
    }
    QSqlQuery query(base);
    QString query_str;
    // удаляем текущие аналоги для данного id
    query_str = QString("DELETE FROM analogs WHERE trademark_id = %1").arg(params["id"]);
    if(!query.exec(query_str)){
        error("Ошибка", QString("Не удалось удалить записи из таблицы analogs:\n").append(query.lastError().text()));
        base.rollback();
        return false;
    }

    // вставляем новые
    for(int i = 0; i < this->ui->analogs_list->count(); i++){
        int analog_id = this->ui->analogs_list->item(i)->data(Qt::UserRole).toInt();
        query_str = QString("SELECT * FROM analogs WHERE trademark_id = %1 AND analog_id = %2").arg(params["id"]).arg(analog_id);
        if(!query.exec(query_str)){
            error("Ошибка", QString("Не удалось выполнить запрос к таблице analogs:\n").append(query.lastError().text()));
            base.rollback();
            return false;
        }
        query_str = QString("INSERT INTO analogs(trademark_id, analog_id) VALUES (%1, %2)").arg(params["id"]).arg(analog_id);
        if(!query.exec(query_str)){
            error("Ошибка", QString("Не удалось вставить запись в таблицу analogs:\n").append(query.lastError().text()));
            base.rollback();
            return false;
        }
    }
    if(!base.commit()){
        error("Ошибка", QString("Не удалось завершить транзакцию\n").append(base.lastError().text()));
        return false;
    }

    return true;
}

bool WhiteAddDialog::save_edited(){
    if(!valid()){
        error("Ошибка", valid_error_text);
        return false;
    }
    success_text = "";

    // подготавливаем и выполняем запрос
    if(!base.transaction()){
        error("Ошибка", QString("Невозможно начать транзакцию\n").append(base.lastError().text()));
        return false;
    }
    QSqlQuery query(base);


    QMap<QString, QString> changed_params;
    if(params["name"] != this->ui->name_box->currentText()){
        // превращаем имя в паттерн путём убирания пробелов и перевода всё в нижний регистр
        QString pattern = this->ui->name_box->currentText().remove(QRegExp("[^a-zA-Zа-яА-Я\\d]")).toUpper();
        params["name"] = this->ui->name_box->currentText();
        changed_params["name"] = this->ui->name_box->currentText();
        params["pattern"] = pattern;
        changed_params["pattern"] = pattern;
    }
    if(params["subgroup_id"] != this->ui->group_box->itemData(this->ui->group_box->currentIndex()).toString()){
        params["subgroup_id"] = this->ui->group_box->itemData(this->ui->group_box->currentIndex()).toString();
        changed_params["subgroup_id"] = this->ui->group_box->itemData(this->ui->group_box->currentIndex()).toString();
    }
    if(params["par1_val"] != this->ui->par1_line->text())
        changed_params["par1_val"] = params["par1_val"] = this->ui->par1_line->text();
    if(params["par2_val"] != this->ui->par2_line->text())
        changed_params["par2_val"] = params["par2_val"] = this->ui->par2_line->text();
    if(params["par3_val"] != this->ui->par3_line->text())
        changed_params["par3_val"] = params["par3_val"] = this->ui->par3_line->text();
    if(params["par4_val"] != this->ui->par4_line->text())
        changed_params["par4_val"] = params["par4_val"] = this->ui->par4_line->text();
    if(params["par5_val"] != this->ui->par5_line->text())
        changed_params["par5_val"] = params["par5_val"] = this->ui->par5_line->text();
    if(params["par6_val"] != this->ui->par6_line->text())
        changed_params["par6_val"] = params["par6_val"] = this->ui->par6_line->text();
    if(params["par7_val"] != this->ui->par7_line->text())
        changed_params["par7_val"] = params["par7_val"] = this->ui->par7_line->text();
    if(params["par8_val"] != this->ui->par8_line->text())
        changed_params["par8_val"] = params["par8_val"] = this->ui->par8_line->text();
    if(params["par9_val"] != this->ui->par9_line->text())
        changed_params["par9_val"] = params["par9_val"] = this->ui->par9_line->text();
    if(params["par10_val"] != this->ui->par10_line->text())
        changed_params["par10_val"] = params["par10_val"] = this->ui->par10_line->text();
    if(params["par11_val"] != this->ui->par11_line->text())
        changed_params["par11_val"] = params["par11_val"] = this->ui->par11_line->text();
    if(params["par12_val"] != this->ui->par12_line->text())
        changed_params["par12_val"] = params["par12_val"] = this->ui->par12_line->text();

    if(params["unit_id"] != this->ui->unit_box->itemData(this->ui->unit_box->currentIndex()).toString())
        changed_params["unit_id"] = params["unit_id"] = this->ui->unit_box->itemData(this->ui->unit_box->currentIndex()).toString();
    if(params["weight_unit_id"] != this->ui->weight_unit_box->itemData(this->ui->weight_unit_box->currentIndex()).toString())
        changed_params["weight_unit_id"] = params["weight_unit_id"] = this->ui->weight_unit_box->itemData(this->ui->weight_unit_box->currentIndex()).toString();
    if(params["notes"] != this->ui->notes_edit->toPlainText())
        changed_params["notes"] = params["notes"] = this->ui->notes_edit->toPlainText();

    if(this->ui->weight_line->text().length() > 0 && params["weight"] != this->ui->weight_line->text().replace(",", "."))
        changed_params["weight"] = params["weight"] = this->ui->weight_line->text().replace(",", ".");

    if(changed_params.size() > 0){
        QString query_str = "UPDATE trademarks SET ";
        QMapIterator<QString, QString> it(changed_params);
        while(it.hasNext()){
            it.next();
            query_str += QString("%1 = '%2'").arg(it.key()).arg(it.value());
            if(it.hasNext()) query_str += ", ";
        }
        query_str += QString(" WHERE id = %1").arg(params["id"]);
        //qDebug() << query_str;
        if(!query.exec(query_str)){
            error("Ошибка", QString("Ошибка при сохранении детали.\n").append(query.lastError().text()));
            query.finish();
            base.rollback();
            return false;
        }
        success_text += "Данные обновлены.\n";
    }    
    if(!base.commit()){
        error("Ошибка", QString("Не удалось завершить транзакцию\n").append(base.lastError().text()));
        return false;
    }


    // обрабатываем фото
    if(params["photo"] != this->ui->photo_line->text()){
        params["photo"] = this->ui->photo_line->text();
        success_text += (proceed_photo() ? "Фото обновлено.\n" : "Фото не обновлено.\n");
    }

    // осталось обработать аналоги
    // ищем каждый в analogs_old, и если список отличается - сохраняем его заново
    bool changed = false;
    if(analogs_old.count() != this->ui->analogs_list->count())
        changed = true;
    else{
        for(int i = 0; i < this->ui->analogs_list->count(); i++){
            if(analogs_old.indexOf(this->ui->analogs_list->item(i)->text()) < 0){
                changed = true;
                break;
            }
        }
    }
    if(changed)
        success_text += (add_analogs() ? "Аналоги изменены.\n" : "Аналоги изменить не удалось.\n");

    if(success_text.length() > 0)
        QMessageBox::information(this, "", success_text, QMessageBox::Ok);
    return true;
}

bool WhiteAddDialog::apply(){
    if(this->dialog_mode == DIALOG_WHITE_NEW){
        if(save_new()){
            this->dialog_mode = DIALOG_WHITE_EDIT;
            fill_current();
            return true;
        }
    }
    else if(this->dialog_mode == DIALOG_WHITE_EDIT)
        if(save_edited()){
            fill_current();
            return true;
        }
    return false;
}

void WhiteAddDialog::accept(){
    if(apply())
        QDialog::accept();
}
