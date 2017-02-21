#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSharedPointer>

#include <QMainWindow>

#include <gpgme++/context.h>

namespace Ui {
class MainWindow;
}

class GPGKeyListModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionAboutQt_triggered();

private:
    QSharedPointer<Ui::MainWindow> ui;
    QSharedPointer<GpgME::Context> ctx;
    GPGKeyListModel *model;

};

#endif // MAINWINDOW_H
