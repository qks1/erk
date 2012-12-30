#include "reserveslist.h"

ReservesList::ReservesList(int id, QString name, QWidget *parent) :
    QDialog(parent)
{
    this->setWindowTitle("Резерв");
    this->setAttribute(Qt::WA_DeleteOnClose);
    buttons_lt = new QHBoxLayout();
    escape_button = create_panel_button(this, buttons_lt, "Закрыть окно", ":back", SLOT(accept()));
    buttons_lt->addStretch(1);
    int f = qApp->font().pointSize();
    settings = get_settings();
    columns = settings->value(QString("%1/RESERVES_LIST").arg(USERNAME), QVariant()).value<ColumnsStruct>();
    QList<int> lst;
    lst << f*12 << f*6 << f*6 << f*8 << f*12;
    docs_list = new BaseTableView("RESERVES_LIST", false, &columns, lst, this);
    reserves_list = new QListWidget(this);
    docs_list->verticalHeader()->hide();

    bar = new QStatusBar(this);
    bar->setSizeGripEnabled(false);
    QLabel *name_label = new QLabel(name, this);
    bar->addPermanentWidget(name_label, 1);

    QVBoxLayout *vlt = new QVBoxLayout(this);
    vlt->addLayout(buttons_lt);
    vlt->addWidget(bar);
    vlt->addWidget(docs_list);
    vlt->addWidget(reserves_list);
    docs_list->setFixedHeight(f*16);
    reserves_list->setFixedHeight(f*8);
    this->setFixedWidth(f*48);
    this->setLayout(vlt);

    QString query_str = QString("SELECT dt.name, d.docnum, dd.quantity, d.docdate, u.login as user "
                        " FROM docs_details dd JOIN docslist d ON d.id = dd.doc_id "
                        " JOIN doctypes dt ON d.doctype_id = dt.id "
                        " JOIN users u ON d.user_id = u.id "
                        " WHERE dd.grey_id IN (SELECT id FROM greytable WHERE trademark_id = %1)")
            .arg(id);

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query_str, base);
    if(model->lastError().isValid()){
        QMessageBox::information(this, "Ошибка", "Не удалось выполнить запрос: " + model->lastError().text() + "reserves_list_1", QMessageBox::Ok);
        return;
    }
    docs_list->setModel(model);
    docs_list->set_filled();
    docs_list->restore_state();

    QMap<QString, QString> columns_names;
    columns_names["name"] = "Документ";
    columns_names["docnum"] = "№";
    columns_names["quantity"] = "Кол-во";
    columns_names["docdate"] = "Дата";
    columns_names["user"] = "Автор";

    for(int i = 0; i < model->columnCount(); i++){
        if(columns_names[model->headerData(i, Qt::Horizontal).toString()] != "")
            model->setHeaderData(i, Qt::Horizontal, columns_names[model->headerData(i, Qt::Horizontal).toString()]);
    }

    QSqlQuery query(base);
    query_str = QString("SELECT r.quantity, rd.doc_num, u.login "
            " FROM reserve r "
            " JOIN reserve_docs rd ON rd.id = r.doc_id "
            " JOIN users u ON u.id = rd.user_id "
            " WHERE r.grey_id IN (SELECT id FROM greytable WHERE trademark_id = %1)")
            .arg(id);
    if(!query.exec(query_str)){
        QMessageBox::information(this, "Ошибка", "Не удалось выполнить запрос: " + query.lastError().text() + "reserves_list_2", QMessageBox::Ok);
        return;
    }
    QListWidgetItem *it;
    while(query.next()){
        it = new QListWidgetItem(reserves_list);
        it->setText(QString("В резерве у %1 (заявка %2): %3 шт.").arg(query.value(2).toString()).arg(query.value(1).toInt()).arg(query.value(0).toInt()));
    }

    int sum = 0;
    query_str = QString("SELECT SUM(quantity) FROM docs_details WHERE grey_id IN (SELECT id FROM greytable WHERE trademark_id = %1)").arg(id);
    if(!query.exec(query_str)){
        QMessageBox::information(this, "Ошибка", "Не удалось выполнить запрос: " + query.lastError().text() + "reserves_list_3", QMessageBox::Ok);
        return;
    }
    query.next();
    sum += query.value(0).toInt();
    query_str = QString("SELECT SUM(quantity) FROM reserve WHERE grey_id IN (SELECT id FROM greytable WHERE trademark_id = %1)").arg(id);
    if(!query.exec(query_str)){
        QMessageBox::information(this, "Ошибка", "Не удалось выполнить запрос: " + query.lastError().text() + "reserves_list_4", QMessageBox::Ok);
        return;
    }
    query.next();
    sum += query.value(0).toInt();

    QLabel *sum_label = new QLabel(this);
    sum_label->setText(QString("Всего в резерве: %1 шт.").arg(sum));
    bar->addPermanentWidget(sum_label);



}

ReservesList::~ReservesList(){
    settings->setValue(QString("%1/%2").arg(USERNAME).arg("RESERVES_LIST"), QVariant::fromValue<ColumnsStruct>(columns));
    delete settings;
}
