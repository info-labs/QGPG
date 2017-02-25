#include "mainwindow.h"
#include <QApplication>

#include <QDir>

#include <gpgme++/global.h>
#include <gpgme.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("InfoLabsJP");
    QCoreApplication::setOrganizationDomain("info-labs.jp");
    QCoreApplication::setApplicationName("QGPG");

#ifdef Q_OS_WIN
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("GnuPG");
    gpgme_set_global_flag("w32-inst-dir", dir.path().toUtf8().constData());
#endif
    GpgME::initializeLibrary();


    MainWindow w;
    w.show();

    return a.exec();
}
