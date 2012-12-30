#include "r_classes.h"

////////////////////////////////////////////////////////////////////////////

AddRString::AddRString(int type, QString old_string, QWidget *parent) : QDialog(parent){
    this->setWindowTitle("Добавить");

    company_label = new QLabel("Фирма", this);
    manager_label = new QLabel("Менеджер", this);
    courier1_label = new QLabel("Курьер1", this);
    courier2_label = new QLabel("Курьер2", this);
    courier3_label = new QLabel("Курьер3", this);
    info_label = new QLabel("Комментарий", this);

    company_box = new RComboBox(this);
    manager_box = new RComboBox(this);
    courier1_box = new RComboBox(this);
    courier2_box = new RComboBox(this);
    courier3_box = new RComboBox(this);
    company_completer = new AdvancedCompleter(this);
    manager_completer = new AdvancedCompleter(this);
    courier1_completer = new AdvancedCompleter(this);
    courier2_completer = new AdvancedCompleter(this);
    courier3_completer = new AdvancedCompleter(this);
    company_box->setCompleter(company_completer);
    manager_box->setCompleter(manager_completer);
    courier1_box->setCompleter(courier1_completer);
    courier2_box->setCompleter(courier2_completer);
    courier3_box->setCompleter(courier3_completer);
    company_box->set_timeout(100);
    company_box->set_min_chars(1);
    manager_box->set_timeout(100);
    manager_box->set_min_chars(1);
    courier1_box->set_timeout(100);
    courier1_box->set_min_chars(1);
    courier2_box->set_timeout(100);
    courier2_box->set_min_chars(1);
    courier3_box->set_timeout(100);
    courier3_box->set_min_chars(1);
    info_edit = new QLineEdit(this);
    couriers_boxes << courier1_box << courier2_box << courier3_box;
    couriers_completers << courier1_completer << courier2_completer << courier3_completer;

    ok_button = new QPushButton("OK", this);
    cancel_button = new QPushButton("Отмена", this);
    form_button = new QPushButton(this);
    del_button = new QPushButton(this);

    form_button->setIcon(QIcon(QPixmap(":/down")));
    del_button->setIcon(QIcon(QPixmap(":/no")));
    form_button->setToolTip("Сформировать строку");
    del_button->setToolTip("Удалить строку из базы");


    old_text = new QTextEdit(this);
    editing = (old_string.length() == 0 ? false : true);
    this->old_string = old_string;
    company_id = -1;
    manager_id = -1;
    couriers_ids[0] = -1;
    couriers_ids[1] = -1;
    couriers_ids[2] = -1;
    set_layout();
    this->type = type;
    if(type == 1)
        decompose();
    else{
        fill_companies();
        fill_managers();
        fill_couriers();
    }
    if(old_string.length() > 0)
        old_text->setText(old_string);


    connect(company_box, SIGNAL(currentIndexChanged(int)), SLOT(company_changed(int)));
    connect(company_box, SIGNAL(editTextChanged(QString)), SLOT(company_text_changed(QString)));
    connect(ok_button, SIGNAL(clicked()), SLOT(accept()));
    connect(cancel_button, SIGNAL(clicked()), SLOT(reject()));
    connect(this->form_button, SIGNAL(clicked()), SLOT(form_string_slot()));
    connect(this->del_button, SIGNAL(clicked()), SLOT(del_string_slot()));
}

inline void AddRString::set_layout(){
    int f = qApp->font().pointSize();

    company_box->setFixedWidth(f*32);
    manager_box->setFixedWidth(f*32);
    couriers_boxes.at(0)->setFixedWidth(f*32);
    couriers_boxes.at(1)->setFixedWidth(f*32);
    couriers_boxes.at(2)->setFixedWidth(f*32);
    info_edit->setFixedWidth(f*32);

    form_button->setFixedSize(f*2.5, f*2.5);
    del_button->setFixedSize(form_button->size());
    old_text->setMaximumHeight(f*8);
    //old_text->adjustSize();

    QGridLayout *glt = new QGridLayout();
    glt->addWidget(company_label, 0, 0);
    glt->addWidget(manager_label, 1, 0);
    glt->addWidget(courier1_label, 2, 0);
    glt->addWidget(courier2_label, 3, 0);
    glt->addWidget(courier3_label, 4, 0);
    glt->addWidget(info_label, 5, 0);

    glt->addWidget(company_box, 0, 1);
    glt->addWidget(manager_box, 1, 1);
    glt->addWidget(couriers_boxes.at(0), 2, 1);
    glt->addWidget(couriers_boxes.at(1), 3, 1);
    glt->addWidget(couriers_boxes.at(2), 4, 1);
    glt->addWidget(info_edit, 5, 1);

    QHBoxLayout *hlt = new QHBoxLayout();
    hlt->addWidget(ok_button);
    hlt->addWidget(cancel_button);

    QVBoxLayout *small_vlt = new QVBoxLayout();
    small_vlt->addWidget(form_button);
    small_vlt->addWidget(del_button);

    QHBoxLayout *bottom_hlt = new QHBoxLayout();
    bottom_hlt->addWidget(old_text);
    bottom_hlt->addLayout(small_vlt);

    QVBoxLayout *vlt = new QVBoxLayout(this);
    vlt->addLayout(glt);
    vlt->addLayout(hlt);
    vlt->addLayout(bottom_hlt);

    this->setLayout(vlt);
}

void AddRString::decompose(){
    QSqlQuery query(base);
    QString query_str = QString("SELECT id, company, manager, courier1, courier2, courier3, notes FROM r_strings WHERE string = '%1'").arg(old_string);
    if(!query.exec(query_str)){
        qDebug() << "Невозможно выполнить запрос: " << query.lastError().text();
        return;
    }
    query.next();

    fill_companies();
    company_id = -1;
    if(query.value(1).isValid()){
        company_box->setEditText(query.value(1).toString());
        if(company_box->findText(query.value(1).toString()) >= 0)
            company_id = company_box->itemData(company_box->findText(query.value(1).toString())).toInt();
    }
    if(manager_box->count() == 0) fill_managers(company_id);
    if(query.value(2).isValid()){
        manager_box->setEditText(query.value(2).toString());
        if(manager_box->findText(query.value(2).toString()) >= 0)
            manager_id = manager_box->itemData(manager_box->findText(query.value(2).toString())).toInt();
    }
    if(couriers_boxes.at(0)->count() == 0) fill_couriers(company_id);
    if(query.value(3).isValid()){
        couriers_boxes[0]->setEditText(query.value(3).toString());
        if(couriers_boxes[0]->findText(query.value(3).toString()) >= 0)
            couriers_ids[0] = couriers_boxes[0]->itemData(couriers_boxes[0]->findText(query.value(3).toString())).toInt();
    }
    if(query.value(4).isValid()){
        couriers_boxes[1]->setEditText(query.value(4).toString());
        if(couriers_boxes[1]->findText(query.value(4).toString()) >= 0)
            couriers_ids[1] = couriers_boxes[1]->itemData(couriers_boxes[1]->findText(query.value(4).toString())).toInt();
    }
    if(query.value(5).isValid()){
        couriers_boxes[2]->setEditText(query.value(5).toString());
        if(couriers_boxes[2]->findText(query.value(5).toString()) >= 0)
            couriers_ids[2] = couriers_boxes[2]->itemData(couriers_boxes[2]->findText(query.value(5).toString())).toInt();
    }
    if(query.value(6).isValid()){
        info_edit->setText(query.value(6).toString());
    }
}

void AddRString::fill_companies(){
    company_box->clear();
    QSqlQuery query(base);
    //QString query_str = "SELECT id, name, city, phone, info, pattern FROM r_companies ORDER BY name";
    QString query_str = "SELECT id, name, pattern FROM r_companies ORDER BY name";
    if(!query.exec(query_str)){
        qDebug() << "Невозможно выполнить запрос: " << query.lastError().text();
        return;
    }
    QString item_str = QString();
    QStringList completer_strings, completer_data;
    company_box->addItem("");
    while(query.next()){
        item_str = query.value(1).toString();
        company_box->addItem(item_str, query.value(0).toInt());
        completer_strings << item_str;
        completer_data << query.value(2).toString();
    }
    company_completer->set_strings_list(completer_strings);
    company_completer->set_data_list(completer_data);
}

void AddRString::fill_managers(int company){
    QString c = manager_box->currentText();
    manager_box->clear();
    QSqlQuery query(base);
    //QString query_str = "SELECT id, name, phone, email, info, pattern FROM r_managers";
    QString query_str = "SELECT id, name, pattern FROM r_managers";
    if(company != 0)
        query_str.append(QString(" WHERE company_id = %1").arg(company));
    query_str += " ORDER BY name";
    if(!query.exec(query_str)){
        qDebug() << "Невозможно выполнить запрос: " << query.lastError().text();
        return;
    }
    QString item_str = QString();
    QStringList completer_strings, completer_data;
    manager_box->addItem("");
    while(query.next()){
        item_str = query.value(1).toString();
        manager_box->addItem(item_str, query.value(0).toInt());
        completer_strings << item_str;
        completer_data << query.value(/*5*/2).toString();
    }
    manager_completer->set_strings_list(completer_strings);
    manager_completer->set_data_list(completer_data);
    manager_box->setEditText(c);
}

void AddRString::fill_couriers(int company){
    for(int i = 0; i < couriers_boxes.count(); i++){
        QString c = couriers_boxes[i]->currentText();
        couriers_boxes[i]->clear();
        QSqlQuery query(base);
        //QString query_str = "SELECT id, name, phone, email, info, pattern FROM r_couriers ";
        QString query_str = "SELECT id, name, pattern FROM r_couriers ";
        if(company != 0)
            query_str.append(QString(" WHERE id IN (SELECT courier_id FROM r_couriers_companies WHERE company_id = %1)").arg(company));
        query_str += " ORDER BY name";
        if(!query.exec(query_str)){
            qDebug() << "Невозможно выполнить запрос: " << query.lastError().text();
            return;
        }
        couriers_boxes[i]->addItem("");
        QString item_str = QString();
        QStringList completer_strings, completer_data;
        while(query.next()){
            item_str = query.value(1).toString();
            couriers_boxes[i]->addItem(item_str, query.value(0).toInt());
            completer_strings << item_str;
            completer_data << query.value(/*5*/2).toString();
        }
        couriers_completers[i]->set_strings_list(completer_strings);
        couriers_completers[i]->set_data_list(completer_data);
        couriers_boxes[i]->setEditText(c);
    }
}

void AddRString::company_changed(int c){
    int c_id = company_box->itemData(c).toInt();
    fill_managers(c_id);
    fill_couriers(c_id);
}

void AddRString::company_text_changed(QString txt){
    if(txt.length() == 0){
        fill_managers();
        fill_couriers();
    }
}

void AddRString::accept(){
    QSqlQuery query(base);
    QString query_str;
    // если чего-то нет в базе - добавляем
    // фирма
    if(company_box->currentText().length() == 0)
        company_id = -1;
    else if(company_box->findText(company_box->currentText()) < 0){
        query_str = QString("INSERT INTO r_companies(name, pattern) VALUES ('%1', '%2') RETURNING id")
                .arg(company_box->currentText())
                .arg(create_pattern(company_box->currentText()));
        if(!query.exec(query_str)){
            qDebug() << "Не удалось добавить фирму. " << query.lastError().text();
            return;
        }
        query.next();
        company_id = query.value(0).toInt();
    }
    else company_id = company_box->itemData(company_box->findText(company_box->currentText())).toInt();
    // манагер
    if(manager_box->currentText().length() == 0)
        manager_id = -1;
    else if(manager_box->findText(manager_box->currentText()) < 0){
        QString columns = "name, pattern";
        QString values = QString("'%1', '%2'")
                .arg(manager_box->currentText())
                .arg(create_pattern(manager_box->currentText(), ","));
        if(company_id >= 0){
            columns.append(", company_id");
            values.append(QString(", %1").arg(company_id));
        }

        query_str = QString("INSERT INTO r_managers(%1) VALUES (%2)")
                .arg(columns)
                .arg(values);
        if(!query.exec(query_str)){
            qDebug() << "Не удалось добавить манагера. " << query.lastError().text();
            return;
        }
    }
    else manager_id = manager_box->itemData(manager_box->findText(manager_box->currentText())).toInt();
    // курьер
    for(int i = 0; i < couriers_boxes.count(); i++){
        couriers_ids[i] = -1;
        if(couriers_boxes.at(i)->currentText().length() == 0)
            couriers_ids[i] = -1;
        else if(couriers_boxes.at(i)->findText(couriers_boxes.at(i)->currentText()) < 0){
            query_str = QString("INSERT INTO r_couriers(name, pattern) VALUES ('%1', '%2') RETURNING id")
                    .arg(couriers_boxes.at(i)->currentText())
                    .arg(create_pattern(couriers_boxes.at(i)->currentText(), ","));
            if(!query.exec(query_str)){
                qDebug() << "Не удалось добавить курьера. " << query.lastError().text();
                return;
            }
            query.next();
            couriers_ids[i] = query.value(0).toInt();
        }
        if(company_id >= 0 && couriers_ids[i] > 0){
            query_str = QString("INSERT INTO r_couriers_companies(courier_id, company_id) "
                                " VALUES (%1, %2)").arg(couriers_ids[i]).arg(company_id);
            if(!query.exec(query_str)){
                qDebug() << "Не удалось добавить. " << query.lastError().text();
                return;
            }
        }
        else couriers_ids[i] = couriers_boxes.at(i)->itemData(couriers_boxes.at(i)->findText(couriers_boxes.at(i)->currentText())).toInt();
    }

    // если это редактирование:
    // тип 1 - сравниваем строку в old_text и новую формализованную. Если разные - предлагаем заменить.
    // тип 2 - заменяем старую строку на новую формализованную
    // если не редактирование:
    // если есть в формальных боксах - формируем и записываем.
    // если только в old_text - предлагаем записать с кодом 2
    if(editing){
        if(!form_string()){
            QMessageBox::information(this, "Ошибка", "Введите формализованную строку.", QMessageBox::Ok);
            return;
        }
        if(type == 1){
            if(new_string != old_string){
                if(QMessageBox::question(this, "Заменить?", QString("Записать новую строку \"%1\"?").arg(new_string), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
                    return;
                else add_to_rbox(new_string, true);
            }
            else
                QMessageBox::information(this, "Ничего не изменено", "Ничего не изменено.", QMessageBox::Ok);
        }
        else{
            if(QMessageBox::question(this, "Заменить?", QString("Формализованная строка: \"%1\". Заменить?").arg(new_string), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
                return;
            else add_to_rbox(new_string, false);
        }
    }
    else{
        if(form_string())
            add_to_rbox(new_string, true);
        else{
            if(QMessageBox::question(this, "Записать?", QString("Записать неформализованную строку?"), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
                return;
            else add_to_rbox(old_text->toPlainText(), true);
        }
    }

    QDialog::accept();
}

bool AddRString::form_string(){
    if(company_box->currentText().length() == 0
            && manager_box->currentText().length() == 0
            && couriers_boxes.at(1)->currentText().length() == 0){
        return false;
    }

    // формируем строку
    new_string = QString();
    QString answer = QString();
    if(company_box->currentText().length() != 0)
        answer += company_box->currentText();
    if(manager_box->currentText().length() != 0){
        if(answer.length() != 0) answer += ", ";
        answer += manager_box->currentText();
    }
    if(couriers_boxes.at(0)->currentText().length() != 0){
        if(answer.length() != 0) answer += ", в Митино ";
        answer += couriers_boxes.at(0)->currentText();
    }
    if(couriers_boxes.at(1)->currentText().length() != 0){
        if(answer.length() != 0) answer += ", ";
        answer += couriers_boxes.at(1)->currentText();
    }
    if(couriers_boxes.at(2)->currentText().length() != 0){
        if(answer.length() != 0) answer += ", ";
        answer += couriers_boxes.at(2)->currentText();
    }

    if(info_edit->text().length() > 0)
        answer += QString(". %1").arg(info_edit->text());
    if(answer.length() <= 0)
        return false;

    new_string = answer;
    return true;

}

void AddRString::form_string_slot(){
    if(form_string())
        old_text->setText(new_string);
}

void AddRString::add_to_rbox(QString str, bool creation){
    QSqlQuery query(base);
    // проверяем, а вдруХ такая есть уже
    if(!query.exec(QString("SELECT * FROM r_strings WHERE string = '%1' AND type = 1").arg(str))){
        qDebug() << query.lastError().text();
        return;
    }
    if(query.size() > 0){
        // если это editing, то предыдущую строку из базы надо удалить
        if(editing)
        {
            QString query_str = QString("DELETE FROM r_strings WHERE string = '%1'").arg(old_string);
            if(!query.exec(query_str)){
                qDebug() << "Не удалось выполнить запрос 1: " << query.lastError().text();
                return;
            }
            emit string_signal(str);
            QDialog::accept();
        }
        emit string_signal(str);
        return;
    }

    // если editing - делаем update, в т.ч. ставим тип 1
    if(editing && !creation){
        QString params = QString("string = '%1', pattern = '%2', type = 1").arg(str).arg(create_pattern(str));
        if(company_id > 0)
            params.append(QString(", company = '%1'").arg(company_box->currentText()));
        else params.append(", company = NULL");

        if(manager_id > 0){
            params.append(QString(", manager = '%2'").arg(manager_box->currentText()));
        }
        else params.append(", manager = NULL");
        if(couriers_ids[0] > 0){
            params.append(QString(", courier1 = '%3'").arg(couriers_boxes[0]->currentText()));
        }
        else params.append(", courier1 = NULL");
        if(couriers_ids[1] > 0){
            params.append(QString(", courier2 = '%4'").arg(couriers_boxes[1]->currentText()));
        }
        else params.append(", courier2 = NULL");
        if(couriers_ids[2] > 0){
            params.append(QString(", courier3 = '%5'").arg(couriers_boxes[2]->currentText()));
        }
        else params.append(", courier3 = NULL");
        if(info_edit->text().length() > 0)
            params.append(QString(", notes = '%1'").arg(info_edit->text()));
        else params.append(", notes = ''");

        QString query_str = QString("UPDATE r_strings SET %1 WHERE string = '%2'")
                .arg(params).arg(old_string);
        qDebug() << query_str;
        if(!query.exec(query_str)){
            qDebug() << "Не удалось выполнить запрос 2: " << query.lastError().text();
            return;
        }
        emit string_signal(str);
        return;
    }
    // если не editing
    int ttype = new_string.length() == 0 ? 2 : 1;
    QString columns = "string, pattern, type";
    QString values = QString("'%1', '%2', %3").arg(str).arg(create_pattern(str)).arg(ttype);
    if(company_id > 0){
        columns.append(", company");
        values.append(QString(", '%1'").arg(company_box->currentText()));
    }
    if(manager_id > 0){
        columns.append(", manager");
        values.append(QString(", '%1'").arg(manager_box->currentText()));
    }
    if(couriers_ids[0] > 0){
        columns.append(", courier1");
        values.append(QString(", '%1'").arg(couriers_boxes[0]->currentText()));
    }
    if(couriers_ids[1] > 0){
        columns.append(", courier2");
        values.append(QString(", '%1'").arg(couriers_boxes[1]->currentText()));
    }
    if(couriers_ids[2] > 0){
        columns.append(", courier3");
        values.append(QString(", '%1'").arg(couriers_boxes[2]->currentText()));
    }
    if(info_edit->text().length() > 0){
        columns.append(", notes");
        values.append(QString(", '%1'").arg(info_edit->text()));
    }
    qDebug() << QString("INSERT INTO r_strings(%1) VALUES (%2) RETURNING id").arg(columns).arg(values);
    if(!query.exec(QString("INSERT INTO r_strings(%1) VALUES (%2) RETURNING id").arg(columns).arg(values))){
        qDebug() << query.lastError().text();
        return;
    }
    emit string_signal(str);
}

void AddRString::del_string_slot(){
    if(!editing)
        return;
    if(QMessageBox::question(this, "Удалить?", QString("Удалить из базы строку \"%1\"?").arg(old_string), QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;
    QSqlQuery query(base);
    if(!query.exec(QString("DELETE FROM r_strings WHERE string = '%1'").arg(old_string))){
        qDebug() << query.lastError().text();
        return;
    }
    emit string_signal(QString());
    QDialog::reject();
}
