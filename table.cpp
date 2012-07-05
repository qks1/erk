#include "table.h"
#include "constants.h"
#include "switcher.h"
#include <QtGui>
#include <QtSql>

Table::Table(QWidget *parent) :
    QWidget(parent)
{
    table = new QTableWidget();
    switcher = new Switcher(table);

    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    table->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    items_on_page = ITEMS_ON_PAGE;

    move_switcher();
    layout();

    connects();

}

inline void Table::connects(){
    // если получен сигнал о смене номера страницы, сменить значение current_page и послать сигнал поисковику
    QObject::connect(this->switcher, SIGNAL(page_changed(int)),
                     this, SLOT(change_page(int)));
}

inline void Table::layout(){
    switcher->show();
    QHBoxLayout *lt = new QHBoxLayout();
    lt->addWidget(table);
    this->setLayout(lt);
}

void Table::move_switcher(){
    switcher->setGeometry(table->width()-switcher->width()-table->verticalScrollBar()->width()-5, table->height()-switcher->height()-table->horizontalScrollBar()->height()-5,switcher->width(),switcher->height());

}

void Table::fill_table(QSqlQuery query, QStringList columns){
    table->clear();
    table->setRowCount(0);

    int rows = 0;

    table->setColumnCount(columns.size());
    table->setHorizontalHeaderLabels(columns);
    table->horizontalHeader()->setMovable(true);

    total_pages = query.size() / items_on_page;
    switcher->set_total_pages("из " + QString::number(total_pages));

    QSqlRecord rec = query.record();
    QString val;
    int i = 0, j;
    QTableWidgetItem* tableItem = 0;

    while(query.next()){
        rows++;
        table->setRowCount(rows);
        j = 0;
        foreach(QString tmp, columns){
            val = query.value(rec.indexOf(tmp)).toString();
            tableItem = new QTableWidgetItem(val);
            table->setItem(i,j,tableItem);
            j++;
        }
        i++;
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}


void Table::change_page(int page){
    current_page = page;
    switcher->set_current_page(page);
}

//--------------------------------------------------------------------------//
//--------------------------- СОБЫТИЯ --------------------------------------//
//--------------------------------------------------------------------------//

void Table::resizeEvent(QResizeEvent *){
    // при изменении размеров каталога (будь то изменение размеров окна
    // или перемещение разделителя) необходимо корректировать положение кнопки сброса
    move_switcher();
}

//--------------------------------------------------------------------------//

