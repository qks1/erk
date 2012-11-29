#ifndef PAINTSTACKEDWIDGET_H
#define PAINTSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QPainter>

class PaintStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit PaintStackedWidget(QWidget *parent = 0);
    
private:
    void paintEvent(QPaintEvent *);

    
};

#endif // PAINTSTACKEDWIDGET_H
