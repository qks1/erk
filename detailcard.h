#ifndef DETAILCARD_H
#define DETAILCARD_H

#include <QtGui>
#include <QtSql>
#include "helpers.h"
#include "constants.h"
#include "mytablemodel.h"

class DetailCard : public QWidget
{
    Q_OBJECT
public:
    explicit DetailCard(int id, QWidget *parent = 0);
    int exec();

private:
    int id;
    QWidget *parent;
    

signals:
    
public slots:
    
};

#endif // DETAILCARD_H
