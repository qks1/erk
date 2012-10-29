#ifndef GREYADDDIALOG_H
#define GREYADDDIALOG_H

#include <QDialog>

namespace Ui {
class GreyAddDialog;
}

class GreyAddDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GreyAddDialog(QWidget *parent = 0);
    ~GreyAddDialog();
    
private:
    Ui::GreyAddDialog *ui;
};

#endif // GREYADDDIALOG_H
