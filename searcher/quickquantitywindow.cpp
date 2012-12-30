#include "quickquantitywindow.h"

QuickQuantityWindow::QuickQuantityWindow(int trademark_id, QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("Кол-во по году");
    this->trademark_id = trademark_id;
    category = new QCheckBox("С кат.");
    category->setCheckState(Qt::Unchecked);
    table = new QTableWidget();
    label = new QLabel();

    // малость украшаем таблицу
    table->verticalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->horizontalHeader()->setHighlightSections(false);

    // считаем кол-во без категории и с категорией
    int with_cat = count_quantity(1);
    // чекбокс активен только если есть с категорией
    category->setEnabled(with_cat > 0);

    // сверху будет лейбл с кол-вом
    // под ним будет чекбокс
    // снизу - таблица
    QVBoxLayout *vlt = new QVBoxLayout(this);
    vlt->addWidget(label);
    vlt->addWidget(category);
    vlt->addWidget(table);
    this->setLayout(vlt);

    fill_table(false);

    QObject::connect(this->category, SIGNAL(clicked(bool)), SLOT(fill_table(bool)));
}

int QuickQuantityWindow::count_quantity(int category_mode){
    // 0 - без категории, 1 - с категорией, 2 - всего
    QString cat_str = "";
    int result = 0;
    if(category_mode == 0)
        cat_str = " AND category_id=(SELECT id from categories where category_name  = '')";
    else if(category_mode == 1)
        cat_str = " AND category_id IN (SELECT id from categories where category_name  != '')";
    else
        cat_str = "";
    QSqlQuery query(base);
    QString query_str = QString("SELECT sum(quantity) FROM greytable "
                                "WHERE trademark_id=%1 %2")
            .arg(this->trademark_id).arg(cat_str);
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос: %1").arg(query.lastError().text()), QMessageBox::Ok);
        return 0;
    }
    if(query.size() < 1)
        result = 0;
    else{
        query.next();
        result = query.value(0).toInt();
    }
    return result;
}

void QuickQuantityWindow::fill_table(bool category){
    // если category = true, выбираем все детали
    // если false - только без категории
    QString query_str = category ?
                QString("SELECT g.year, sum(g.quantity), c.category_name "
                        "FROM greytable g "
                        "JOIN categories c ON g.category_id=c.id "
                        "WHERE g.trademark_id = %1 "
                        "GROUP BY g.year, c.category_name").arg(this->trademark_id) :
                QString("SELECT year, sum(quantity) "
                        "FROM greytable "
                        "WHERE (category_id = (SELECT id FROM categories WHERE category_name = '') "
                        "OR category_id = NULL) "
                        "AND trademark_id = %1 "
                        "GROUP BY year").arg(this->trademark_id);
    QSqlQuery query(base);
    if(!query.exec(query_str)){
        QMessageBox::warning(this, "Ошибка", QString("Не удалось выполнить запрос: %1").arg(query.lastError().text()), QMessageBox::Ok);
        return;
    }

    // подготавливаем таблицу
    table->clear();
    table->setColumnCount(category ? 3 : 2);
    table->setRowCount(query.size());
    set_header_labels();
    set_label(count_quantity(category ? 2 : 0));

    // Типа разбираем.
    int i = 0;
    while(query.next()){
        table->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
        table->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
        if(category)
            table->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
        i++;
    }

    // меняем размеры
    resize_all(category);
}

void QuickQuantityWindow::set_header_labels(){
    QStringList labels;
    labels << "Год" << "Кол-во" << "Категория";
    table->setHorizontalHeaderLabels(labels);
}

void QuickQuantityWindow::set_label(int quantity){
    label->setText(QString("Общее кол-во: %1").arg(quantity));
    label->adjustSize();
}

void QuickQuantityWindow::resize_all(bool cat){
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    //int wdth = table->columnWidth(0) + table->columnWidth(1);
    //if(category) wdth += table->columnWidth(2);

    //this->setFixedWidth(wdth);
    this->label->adjustSize();
    this->category->adjustSize();
    this->adjustSize();
    this->setFixedSize(this->size());

}
