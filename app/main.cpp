#include "mainwindow.h"
#include <QApplication>

#include <gpgme++/global.h>

int main(int argc, char *argv[])
{
    GpgME::initializeLibrary();

    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("InfoLabsJP");
    QCoreApplication::setOrganizationDomain("info-labs.jp");
    QCoreApplication::setApplicationName("QGPG");

    MainWindow w;
    w.show();

    return a.exec();
}
