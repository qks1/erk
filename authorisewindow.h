#ifndef AUTHORISEWINDOW_H
#define AUTHORISEWINDOW_H

#include <QDialog>
#include <QtGui>
#include <QtSql>
#include "constants.h"
#include "helpers.h"
#include "mainwindow.h"

namespace Ui {
class AuthoriseWindow;
}

class AuthoriseWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit AuthoriseWindow(bool success, QWidget *parent = 0);
    ~AuthoriseWindow();
    
private:
    Ui::AuthoriseWindow *ui;

    void fill_users_box();
    void create_mainwindow();
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *);

signals:
    void close_signal();
    void success_signal();

private slots:
    void enter_slot();
    void close_slot();
    void settings_slot();
    void set_focus_slot(int n = 0);
};

class AuthSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AuthSettingsDialog(QWidget *parent = 0, Qt::WindowFlags = 0);
    ~AuthSettingsDialog();

private:
    QLineEdit *base_name;
    QLineEdit *base_host;
    QLineEdit *base_user;
    QLineEdit *base_pass;
    QSettings *settings;
    bool need_restart;
    QString old_basename;
    QString old_basehost;
    QString old_baseuser;
    QString old_basepass;

signals:

public slots:

private slots:
    void accept();
    void reject();

};


#endif // AUTHORISEWINDOW_H
