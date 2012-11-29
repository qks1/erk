#include "paintstackedwidget.h"

PaintStackedWidget::PaintStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
}

void PaintStackedWidget::paintEvent(QPaintEvent *e){
    QStackedWidget::paintEvent(e);
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.drawLine(0,0,width(),height());
}
