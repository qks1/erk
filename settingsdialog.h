#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>
#include "constants.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0, Qt::WindowFlags = 0);

private:
    QLineEdit *base_name;
    QLineEdit *base_host;
    QLineEdit *base_user;
    QLineEdit *base_pass;
    QLineEdit *photo_path;
    QPushButton *path_button;
    QFontComboBox *system_font_list;
    QSpinBox *system_font_sizes;
    QFontComboBox *print_font_list;
    QSpinBox *print_font_sizes;
    QSettings *settings;


signals:

public slots:

private slots:
    void photo_slot();
    void accept();
    //void reject();

};

#endif // SETTINGSDIALOG_H
