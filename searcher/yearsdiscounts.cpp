#include "yearsdiscounts.h"
#include "ui_yearsdiscounts.h"

YearsDiscounts::YearsDiscounts(int id, QString name, double retail, double whole, QWidget *parent) :
    QDialog(parent)
{
    filled = false;
    trademark_id = id;
    retail_price = retail;
    whole_price = whole;

    this->setWindowTitle("Скидки по годам");
    name_label = new QLabel();
    remember_checkbox = new QCheckBox("Запомнить года");
    up_button = new QPushButton();
    down_button = new QPushButton();
    apply_button = new QPushButton("Записать");
    close_button = new QPushButton("Закрыть");
    // делаем лейбл с названием
    name_label->setText(QString("Наименование: ").append(name));
    QFont tfont;
    tfont.setBold(true);
    tfont.setItalic(true);
    name_label->setFont(tfont);

    apply_retail_checkbox = 0;
    apply_whole_checkbox = 0;

    // украшаем кнопки "вверх" и "вниз"
    int arrow_button_size = 24;
    up_button->setIcon(QPixmap(":/up"));
    down_button->setIcon(QPixmap(":/down"));
    up_button->setFixedSize(arrow_button_size, arrow_button_size);
    down_button->setFixedSize(arrow_button_size, arrow_button_size);

    // сжимаем кнопки "применить" и "закрыть"
    apply_button->setMaximumWidth(apply_button->fontMetrics().width('_') * 16);
    close_button->setMaximumWidth(apply_button->maximumWidth());
    // формируем список годов
    form_years_list();

    // размещщаем всё
    set_layout();

    // заполняем цены, ну и т.д.
    fill_grid();

    // говорим айтемам, что они заполнены
    foreach(YearsDiscountsItem *item, years_items){
        item->set_formed();
    }

    this->adjustSize();
    this->setFixedSize(this->size());

    QObject::connect(this->apply_button, SIGNAL(clicked()), SLOT(accept()));
    QObject::connect(this->close_button, SIGNAL(clicked()), SLOT(reject()));
    QObject::connect(this->up_button, SIGNAL(clicked()), SIGNAL(up_clicked()));
    QObject::connect(this->down_button, SIGNAL(clicked()), SIGNAL(down_clicked()));
    QObject::connect(this->apply_retail_checkbox, SIGNAL(clicked(bool)), SLOT(enable_retail(bool)));
    QObject::connect(this->apply_whole_checkbox, SIGNAL(clicked(bool)), SLOT(enable_whole(bool)));
}

YearsDiscounts::~YearsDiscounts()
{
    //delete ui;
}

void YearsDiscounts::set_layout(){
    // лайаут верхнего уровня, куда входит заголовок, грид с годами и hbox с кнопками
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    // грид с годами
    QGridLayout *years_layout = new QGridLayout();
    // hboxlayout с кнопками, состоящий из трёх маленьких vboxов
    QHBoxLayout *bottom_layout = new QHBoxLayout();
    // первый из vboxов, состоящий из чекбокса "запомнить года" и спейсера, дабы чекбокс был наверху
    QVBoxLayout *remember_layout = new QVBoxLayout();
    // vbox с кнопками "вверх" и "вниз"
    QVBoxLayout *updown_layout = new QVBoxLayout();
    // vbox с кнопками "записать" и "закрыть"
    QVBoxLayout *buttons_layout = new QVBoxLayout();
    // формируем большой-большой грид
    years_layout->setSpacing(0);
    form_grid(years_layout);

    // формируем левый vbox
    remember_layout->addWidget(remember_checkbox);
    remember_layout->addStretch(1);

    // средний vbox
    updown_layout->addWidget(up_button);
    updown_layout->addWidget(down_button);

    // и правый
    buttons_layout->addWidget(apply_button);
    buttons_layout->addWidget(close_button);

    // размещаем три vboxа в ряд
    bottom_layout->addLayout(remember_layout);
    bottom_layout->addLayout(updown_layout);
    bottom_layout->addLayout(buttons_layout);
    bottom_layout->addStretch(1);

    // и теперь всё в главный лайаут
    main_layout->addWidget(name_label);
    main_layout->addLayout(years_layout);
    main_layout->addLayout(bottom_layout);
}

void YearsDiscounts::form_years_list(){
    years_list << "бг" << "60" << "70" << "80";
    for(int y = 1987; y <= QDate::currentDate().year(); y++)
        years_list << QString::number(y);
}

void YearsDiscounts::form_grid(QGridLayout *years_layout){
    int cnt = years_list.count();
    int columncnt = cnt%3 == 0 ? cnt/3 : cnt/3 + 1;
    cnt = 0;
    //YearsDiscountsItem *item = 0;
    // первый и второй столбец заполняем по числу columncnt
    // cnt - счётчег
    for(; cnt < columncnt; cnt++){
        YearsDiscountsItem *item = new YearsDiscountsItem(retail_price, whole_price);
        item->set_year(years_list.at(cnt));
        years_layout->addWidget(item, cnt+1, 0);
        years_items << item;
    }
    for(; cnt < columncnt*2; cnt++){
        YearsDiscountsItem *item = new YearsDiscountsItem(retail_price, whole_price);
        item->set_year(years_list.at(cnt));
        years_layout->addWidget(item, cnt-columncnt+1, 1);
        years_items << item;
    }

    // третий - пока не кончатся айтемы в years_list
    for(; cnt < years_list.count(); cnt++){
        YearsDiscountsItem *item = new YearsDiscountsItem(retail_price, whole_price);
        item->set_year(years_list.at(cnt));
        years_layout->addWidget(item, cnt-columncnt*2+1, 2);
        years_items << item;
    }
    add_header(0, years_layout);
    add_header(1, years_layout);
    add_header(2, years_layout);
}

void YearsDiscounts::fill_grid(){
    // запросом получим массив скидок для данного id
    QSqlQuery query(base);
    QString query_str = QString("SELECT id, year, percents, manual, retail_price, whole_price "
                                "FROM years_discounts WHERE trademark_id = %1 ORDER BY year").arg(trademark_id);
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()));
        return;
    }
    QSqlRecord rec = query.record();

    // теперь пройдёмся по результатам запроса
    // для каждого года найдём индекс в years_list
    // он равнозначен индексу в years_items
    // соответственно, выберем виджет из years_items и поправим в нём значения
    QString cur_year = "";
    int year_index = 0;
    double cur_percents = 0;
    bool cur_manual = false;
    while(query.next()){
        cur_year = query.value(rec.indexOf("year")).toString();
        year_index = years_list.indexOf(cur_year);
        if(year_index < 0)
            continue;
        YearsDiscountsItem *item = years_items.at(year_index);
        // поменять надо plusminus, percents, retail, whole и manual
        cur_manual = query.value(rec.indexOf("manual")).toBool();
        item->set_manual(cur_manual);
        if(!cur_manual){
            cur_percents = query.value(rec.indexOf("percents")).toInt();
            item->set_plusminus(cur_percents >= 0);
            item->set_percents(abs(cur_percents));
        }
        item->set_retail(query.value(rec.indexOf("retail_price")).toDouble());
        item->set_whole(query.value(rec.indexOf("whole_price")).toDouble());
    }

}

void YearsDiscounts::add_header(int column, QGridLayout *years_layout){
    QWidget *stupid_widget = new QWidget();
    QLabel *year = new QLabel("Год", stupid_widget);
    QLabel *percents = new QLabel("%", stupid_widget);
    QLabel *retail = new QLabel("Розн.", stupid_widget);
    QLabel *whole = new QLabel("Опт.", stupid_widget);
    QLabel *manual = new QLabel("Вр.", stupid_widget);
    year->adjustSize();
    retail->adjustSize();
    whole->adjustSize();

    if(column == 0){
        apply_retail_checkbox = new QCheckBox(stupid_widget);
        apply_whole_checkbox = new QCheckBox(stupid_widget);
        apply_retail_checkbox->setChecked(true);
        apply_whole_checkbox->setChecked(true);
    }

    stupid_widget->setFixedWidth(years_items.at(0)->width());
    stupid_widget->setFixedHeight(year->height());

    year->move(years_items.at(0)->year_label_x(), 0);
    percents->move(years_items.at(0)->percents_spinbox_x(), 0);
    retail->move(years_items.at(0)->retail_label_x(), 0);
    whole->move(years_items.at(0)->whole_label_x(), 0);
    manual->move(years_items.at(0)->manual_checkbox_x(), 0);
    if(column == 0){
        apply_retail_checkbox->move(retail->x() + retail->width() + 2, 0);
        apply_whole_checkbox->move(whole->x() + whole->width() + 2, 0);
    }

    years_layout->addWidget(stupid_widget, 0, column);
}

void YearsDiscounts::accept(){
    if(!base.transaction()){
        QMessageBox::warning(this, "Ошибка", "Невозможно начать транзакцию", QMessageBox::Ok);
        return;
    }
    // заново выбираем скидки для этого id, ибо вдруг чего изменилось
    QSqlQuery query(base);
    QString query_str = QString("SELECT id, year, percents, manual, retail_price, whole_price "
                                "FROM years_discounts WHERE trademark_id = %1 ORDER BY year").arg(trademark_id);
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()));
        base.rollback();
        return;
    }
    QSqlRecord rec = query.record();

    // запихиваем всё в qmap
    QMap<QString,discount> id_discounts;
    discount current;
    QString cur_year;
    while(query.next()){
        cur_year = query.value(rec.indexOf("year")).toString();
        current.id = query.value(rec.indexOf("id")).toInt();
        current.percents = query.value(rec.indexOf("percents")).toInt();
        current.retail_price = query.value(rec.indexOf("retail_price")).toDouble();
        current.whole_price = query.value(rec.indexOf("whole_price")).toDouble();
        current.manual = query.value(rec.indexOf("manual")).toBool();

        id_discounts[cur_year] = current;
    }
    // ок. теперь проходим по всем years_items
    foreach(YearsDiscountsItem *item, years_items){
        // ежели скидка установлена
        if(item->percents() != 0 || item->retail_price() != this->retail_price || item->whole_price() != this->whole_price){
            //qDebug() << QString("It suuuuucks! Retail = %1, while the base = %2, whole = %3, while the base = %4. And percents = %5!").arg(item->retail_price()).arg(this->retail_price).arg(item->whole_price()).arg(this->whole_price).arg(item->percents());
            // проверяем, есть ли скидка на этот год в базе (т.е. есть ли такой ключ в id_discounts).
            if(id_discounts.count(item->year()) > 0){
                // если есть - проверяем, изменено ли что-нибудь. Если да - делаем UPDATE
                current = id_discounts.value(item->year());
                if(item->percents()*(item->mode() ? -1 : 1) != current.percents ||
                        item->retail_price() != current.retail_price ||
                        item->whole_price() != current.whole_price ||
                        item->manual() != current.manual){
                    QString query_str =  QString("UPDATE years_discounts SET percents = %1, retail_price = %2, whole_price = %3, manual = %4, user_id = %5 "
                                                 "WHERE id = %6")
                                                    .arg(item->manual() ? 0 : item->percents() * (item->mode() ? 1 : -1))
                                                    .arg(item->retail_price())
                                                    .arg(item->whole_price())
                                                    .arg(item->manual() ? "true" : "false")
                                                    .arg(USER_ID)
                                                    .arg(current.id);
                    if(!query.exec(query_str)){
                        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()));
                        base.rollback();
                        return;
                    }
                    qDebug() << QString("UPDATE сделан за год %1").arg(item->year());
                }
            }
            // фух
            else{
                // если скидки на этот год в базе НЕТ, значит, её надо вставить
                QString query_str = QString("INSERT INTO years_discounts (trademark_id, year, percents, retail_price, whole_price, manual, user_id)"
                                            "VALUES (%1, '%2', '%3', %4, %5, %6, %7)")
                        .arg(this->trademark_id)
                        .arg(item->year())
                        .arg(item->manual() ? 0 : item->percents() * (item->mode() ? 1 : -1))
                        .arg(item->retail_price())
                        .arg(item->whole_price())
                        .arg(item->manual() ? "true" : "false")
                        .arg(USER_ID);
                if(!query.exec(query_str)){
                    QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()));
                    base.rollback();
                    return;
                }
                qDebug() << QString("INSERT сделан за год %1").arg(item->year());
            }
        }
        // ашащащшшащашаашшвщыашщ
        // теперь - если скидка НЕ установленаааа
        // тут проще - если в базе за этот год скидка есть, удаляем её, иначе проходим мимо
        else{
            if(id_discounts.count(item->year()) > 0){
                current = id_discounts.value(item->year());
                QString query_str = QString("DELETE FROM years_discounts WHERE id = %1").arg(current.id);

                if(!query.exec(query_str)){
                    QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос.\n%1").arg(query.lastError().text()));
                    base.rollback();
                    return;
                }
                qDebug() << QString("DELETE сделан за год %1").arg(item->year());
            }
        }
    }
    while(!base.commit());
    emit need_reset();
    // перейти на строчку вниз
    emit down_clicked();
}

void YearsDiscounts::renew(int id, QString name, double retail, double whole){
    this->trademark_id = id;
    this->name_label->setText(QString("Наименование: ").append(name));
    this->retail_price = retail;
    this->whole_price = whole;

    foreach(YearsDiscountsItem *item, years_items){
        item->set_unformed();
    }

    // нужно перезаполнить все итемы.
    // если стоит галка "запоминать года" - ничего не выбираем из базы, тупо пересчитываем
    // где стоят проценты - считаем новую цену
    // где стоит цены вручную - сохраняем её
    // иначе - базовая цена

    // в любом случае меняем базовые цены айтема
    foreach(YearsDiscountsItem *item, years_items){
        item->set_base_retail(this->retail_price);
        item->set_base_whole(this->whole_price);
    }
    if(remember_checkbox->isChecked()){
        foreach(YearsDiscountsItem *item, years_items){
            if(!item->manual())
                item->count_prices();   // это поставит базовую цену там, где 0%
            // а где стоит manual - просто ничего не меняем
        }
    }
    else{
        // ставим в каждый айтем базовые цены
        foreach(YearsDiscountsItem *item, years_items){
            item->set_retail(this->retail_price);
            item->set_whole(this->whole_price);
            item->set_percents(0);
            item->set_plusminus(true);
            item->set_manual(false);
        }
        // выбираем из базы, омг
        // может, и проканает
        fill_grid();
    }

    foreach(YearsDiscountsItem *item, years_items){
        item->set_formed();
    }

}

void YearsDiscounts::enable_retail(bool enabled){
    foreach(YearsDiscountsItem *item, years_items){
        item->set_retail_enabled(enabled);
    }
}

void YearsDiscounts::enable_whole(bool enabled){
    foreach(YearsDiscountsItem *item, years_items){
        item->set_whole_enabled(enabled);
    }
}

