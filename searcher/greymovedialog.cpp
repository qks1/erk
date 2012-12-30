#include "greymovedialog.h"
#include "ui_greymovedialog.h"

GreyMoveDialog::GreyMoveDialog(int id,
                               int status,
                               int grey_id,
                               int quantity,
                               QString name,
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
                               QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GreyMoveDialog)
{
    ui->setupUi(this);

    this->id = id;
    this->status = status;
    ui->name_label->setText(name);
    this->grey_id = grey_id;
    this->quantity = quantity;
    ui->quantity_spin->setValue(0);
    ui->quantity_spin->setMaximum(quantity);
    connect(ui->quantity_spin, SIGNAL(valueChanged(int)), SLOT(change_remain(int)));
    change_remain(ui->quantity_spin->value());

    this->storage = storage;
    this->rack = rack;
    this->board = board;
    this->box = box;
    this->year = year;
    this->pack = pack;
    this->inspection = inspection;
    this->category = category;
    this->addpar1 = add_par_1;
    this->addpar2 = add_par_2;

    // заполняем складские комбобоксы.
    // т.к. это ни разу не DRY, потом переписать
    fill_boxes();

    connect(ui->save_button, SIGNAL(clicked()), SLOT(apply()));
    connect(ui->cancel_button, SIGNAL(clicked()), SLOT(reject()));



}

GreyMoveDialog::~GreyMoveDialog()
{
    delete ui;
}

void GreyMoveDialog::fill_boxes(){
    fill_storages_box();
    ui->storage_box->setCurrentIndex(ui->storage_box->findText(storage));
    if(storage.length() > 0){
        ui->rack_box->setEnabled(true);
        fill_racks_box();
        if(rack.length() > 0){
            //qDebug() << rack;
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

// склад
void GreyMoveDialog::fill_storages_box(){
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
void GreyMoveDialog::fill_racks_box(){
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
void GreyMoveDialog::fill_boards_box(){
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
void GreyMoveDialog::fill_boxes_box(){
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

void GreyMoveDialog::change_remain(int q){
    ui->remain_label->setText(QString("Ост. %1").arg(quantity - q));
}

void GreyMoveDialog::apply(){
    // проверяем, изменено ли что-то и не нулевое ли количество.
    // В этом случае просто выходим.
    if(ui->storage_box->currentText() != this->storage
            || ui->rack_box->currentText() != this->rack
            || ui->board_box->currentText() != this->board
            || ui->box_box->currentText() != this->box
            || ui->quantity_spin->value() == 0){
        QDialog::reject();
        return;
    }

    // проверяем, есть ли в базе деталь с такими параметрами и расположением
    // (для этого мы и передавали кучу параметров в конструкторе)
    // Если есть - добавляем кол-во туда и вычитаем отсюда,
    // если нет - изменяем расположение текущей (если кол-во переносится полностью)
    // или создаём новую деталь с такими же параметрами и новым расположением.


    QDialog::accept();
}
