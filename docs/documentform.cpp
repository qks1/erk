#include "documentform.h"
#include "ui_documentform.h"

DocumentForm::DocumentForm(QStackedWidget *stack,
                           QStack<QWidget*> prev_wgts,
                           int doctype,
                           int provider_id,
                           QString provider,
                           int contragent_id,
                           QString contragent,
                           int consignee_id,
                           QString consignee,
                           QString notes,
                           QString r_notes,
                           bool print_r,
                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentForm)
{
    ui->setupUi(this);
    this->stack = stack;
    this->prev_wgts = prev_wgts;

    // сохраняем
    this->doctype = doctype;
    this->old_provider_id = provider_id;
    this->provider_id = provider_id;
    this->old_contragent_id = contragent_id;
    this->contragent_id = contragent_id;
    this->old_consignee_id = consignee_id;
    this->consignee_id = consignee_id;
    this->old_notes = notes;
    this->notes = notes;
    this->old_r_notes = r_notes;
    this->r_notes = r_notes;

    r_completer = new AdvancedCompleter(ui->r_combobox);
    r_model = new TwoColorsSqlModel();
    //ui->r_combobox->setModel(r_model);
    ui->r_combobox->setCompleter(r_completer);

    fill_r_box();

    // устанавливаем в интерфейс
    ui->provider_edit->setText(provider);
    ui->contragent_edit->setText(contragent);
    ui->cons_edit->setText(consignee);
    ui->note_edit->setText(notes);
    ui->r_combobox->setEditText(r_notes);
    ui->print_checkbox->setChecked(print_r);

    //ui->right_frame->setFixedWidth(qApp->font().pointSize() * 24);
    double f = qApp->font().pointSize();
    ui->weight_label->adjustSize();

    connects();
    r_notes_changed(ui->r_combobox->currentText());
    qDebug() << ui->r_combobox->lineEdit()->palette().color(QPalette::Text).toRgb();
}

DocumentForm::~DocumentForm()
{
    delete ui;
}

inline void DocumentForm::connects(){
    connect(ui->provider_edit, SIGNAL(double_clicked()), SLOT(select_provider()));
    connect(ui->contragent_edit, SIGNAL(double_clicked()), SLOT(select_contragent()));
    connect(ui->cons_edit, SIGNAL(double_clicked()), SLOT(select_consignee()));
    connect(ui->provider_button, SIGNAL(clicked()), SLOT(delete_provider()));
    connect(ui->contragent_button, SIGNAL(clicked()), SLOT(delete_contragent()));
    connect(ui->cons_button, SIGNAL(clicked()), SLOT(delete_consignee()));
    connect(ui->note_edit, SIGNAL(textChanged(QString)), SLOT(notes_changed(QString)));
    connect(ui->r_combobox, SIGNAL(editTextChanged(QString)), SLOT(r_notes_changed(QString)));
    connect(ui->add_rstring_button, SIGNAL(clicked()), SLOT(add_rstring()));
}

QString DocumentForm::r_string(){
    return ui->r_combobox->currentText();
}

void DocumentForm::select_provider(){
    prev_wgts.push(stack->currentWidget());
    ContragentsList *providers_list = new ContragentsList(true, 2, this);
    providers_list->show();
    connect(providers_list, SIGNAL(selected(int, QString)), SLOT(set_provider(int, QString)));
    stack->addWidget(providers_list);
    stack->setCurrentWidget(providers_list);
    connect(providers_list, SIGNAL(delete_me()), SLOT(close_current()));
}

void DocumentForm::select_contragent(){
    prev_wgts.push(stack->currentWidget());
    ContragentsList *contragents_list = new ContragentsList(true, 1, this);
    contragents_list->show();
    connect(contragents_list, SIGNAL(selected(int, QString)), SLOT(set_contragent(int, QString)));
    stack->addWidget(contragents_list);
    stack->setCurrentWidget(contragents_list);
    connect(contragents_list, SIGNAL(delete_me()), SLOT(close_current()));
}

void DocumentForm::select_consignee(){
    prev_wgts.push(stack->currentWidget());
    ContragentsList *consignees_list = new ContragentsList(true, 1, this);
    consignees_list->show();
    connect(consignees_list, SIGNAL(selected(int, QString)), SLOT(set_consignee(int, QString)));
    stack->addWidget(consignees_list);
    stack->setCurrentWidget(consignees_list);
    connect(consignees_list, SIGNAL(delete_me()), SLOT(close_current()));
}

void DocumentForm::close_current(){
    stack->removeWidget(stack->currentWidget());
    stack->setCurrentWidget(prev_wgts.last());

    prev_wgts.pop();
}

void DocumentForm::set_provider(int id, QString name){
    ui->provider_edit->setText(name);
    this->provider_id = id;
    emit something_changed();
}

void DocumentForm::set_contragent(int id, QString name){
    ui->contragent_edit->setText(name);
    this->contragent_id = id;
    emit something_changed();
}

void DocumentForm::set_consignee(int id, QString name){
    ui->cons_edit->setText(name);
    this->consignee_id = id;
    emit something_changed();
}

void DocumentForm::delete_provider(){
    if(QMessageBox::question(this, "Удалить?", "Удалить поставщика?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;
    ui->provider_edit->clear();
    this->provider_id = 0;
    emit something_changed();
}

void DocumentForm::delete_contragent(){
    if(QMessageBox::question(this, "Удалить?", "Удалить покупателя?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;
    ui->contragent_edit->clear();
    this->contragent_id = 0;
    emit something_changed();
}

void DocumentForm::delete_consignee(){
    if(QMessageBox::question(this, "Удалить?", "Удалить грузополучателя?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;
    ui->cons_edit->clear();
    this->consignee_id = 0;
    emit something_changed();
}

void DocumentForm::notes_changed(QString s){
    notes = s;
    emit something_changed();
}

void DocumentForm::r_notes_changed(QString s){
    /*// тут не так всё просто
    // ищем строку s в базе, и только если нашлась - меняем r_id и r_notes
    // но если она пустая - ставим значение 0
    if(s.length() == 0){
        r_id = 0;
        r_notes = s;
        emit something_changed();
        return;
    }
    QSqlQuery query(base);
    QString query_str = QString("SELECT id FROM r_strings WHERE string = '%1'").arg(s);
    if(!query.exec(query_str)){
        qDebug() << "Не удалось выполнить запрос: " << query.lastError().text();
        return;
    }
    if(query.size() != 0){
        query.next();
        r_id = query.value(0).toInt();
        r_notes = s;
    }*/
    // тэкс
    // ищем текст в заполненном комбобоксе.
    // если его нет - красим надпись синим.
    // если есть - красим тем цветом, каким он покрашен в боксе.
    int index = ui->r_combobox->findText(s);
    QPalette palette(ui->r_combobox->lineEdit()->palette());
    if(index < 0)
        palette.setColor(QPalette::Text, Qt::blue);
    else{
        QColor c = r_model->data(r_model->index(index, 2)).toInt() == 2 ? Qt::blue : Qt::black;
        palette.setColor(QPalette::Text, c);
    }
    ui->r_combobox->lineEdit()->setPalette(palette);
    emit something_changed();
}

void DocumentForm::fill_r_box(){
    QString txt = ui->r_combobox->currentText();
    ui->r_combobox->clear();
    QString query_str = "SELECT string, pattern, type FROM r_strings ORDER BY string";
    r_model->setQuery(query_str, base);
    ui->r_combobox->setModel(r_model);
    QStringList strings, patterns;
    for(int i = 0; i < r_model->rowCount(); i++){
        //ui->r_combobox->addItem(r_model->data(r_model->index(i, 0)).toString(), r_model->data(r_model->index(i, 2)).toInt());
        strings << r_model->data(r_model->index(i, 0)).toString();
        patterns << r_model->data(r_model->index(i, 1)).toString();
    }
    r_completer->set_strings_list(strings);
    r_completer->set_data_list(patterns);
    ui->r_combobox->setEditText(txt);
    r_notes_changed(txt);
    //ui->who_combobox->setModel(query);
}

void DocumentForm::add_rstring(){
    QString current_r_text = ui->r_combobox->currentText().length() > 0 ? ui->r_combobox->currentText() : QString();
    int index = ui->r_combobox->findText(current_r_text);

    //AddRString *rs = new AddRString(index >= 0 ? ui->r_combobox->itemData(index).toInt(): 2, current_r_text, this);
    AddRString *rs = new AddRString(index >= 0 ? r_model->data(r_model->index(index, 2)).toInt(): 2, current_r_text, this);
    connect(rs, SIGNAL(string_signal(QString)), SLOT(add_to_rbox(QString)));
    rs->exec();
}

void DocumentForm::add_to_rbox(QString str){
    /*QSqlQuery query(base);
    // проверяем, а вдруХ такая есть уже
    if(!query.exec(QString("SELECT * FROM r_strings WHERE string = '%1'").arg(str))){
        qDebug() << query.lastError().text();
        return;
    }
    if(query.size() > 0){
        ui->r_combobox->setCurrentIndex(ui->r_combobox->findText(str));
        return;
    }
    if(!query.exec(QString("INSERT INTO r_strings(string, pattern, type) VALUES ('%1', '%2', 1) RETURNING id").arg(str).arg(create_pattern(str)))){
        qDebug() << query.lastError().text();
        return;
    }*/
    fill_r_box();
    ui->r_combobox->setEditText(str);
    r_notes_changed(str);
}



TwoColorsSqlModel::TwoColorsSqlModel(QWidget *parent) : QSqlQueryModel(parent){

}

QVariant TwoColorsSqlModel::data(const QModelIndex &item, int role) const{
    if(item.column() == 0 && role == Qt::TextColorRole){
        if(QSqlQueryModel::data(QSqlQueryModel::index(item.row(), 2), Qt::DisplayRole).toInt() == 2)
            return Qt::blue;
    }

    return QSqlQueryModel::data(item, role);
}

