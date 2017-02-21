#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "models/gpgkeylistmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    ctx(GpgME::Context::createForProtocol(GpgME::OpenPGP)),
    model(new GPGKeyListModel(ctx))
{
    this->ui->setupUi(this);

    this->ui->treeViewPublicKeys->setModel(this->model);
}

MainWindow::~MainWindow()
{

}
