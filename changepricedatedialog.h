#ifndef CHANGEPRICEDATEDIALOG_H
#define CHANGEPRICEDATEDIALOG_H

#include <QDialog>
#include <QSettings>
#include "helpers.h"
#include "constants.h"
#include <QAbstractSpinBox>

namespace Ui {
class ChangePriceDateDialog;
}

class ChangePriceDateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ChangePriceDateDialog(QWidget *parent = 0);
    ~ChangePriceDateDialog();
    
private:
    Ui::ChangePriceDateDialog *ui;
    void set_default_date();

private slots:
    void accept();
};

#endif // CHANGEPRICEDATEDIALOG_H
