#include "settings.h"

#include <QCoreApplication>
#include <QDir>

Settings::Settings()
{

}

QString Settings::engineFileName() const
{
#ifdef Q_OS_WIN
    QDir dir(QCoreApplication::applicationDirPath());
    return this->value(ENGINE_FILENAME, dir.absoluteFilePath("GnuPG/gpg2.exe").toUtf8().constData()).toString();
#else
    return this->value(ENGINE_FILENAME, "/usr/bin/gpg2").toString();
#endif
}

QString Settings::engineHomeDirectory() const
{
    QDir homeDir(QDir::homePath());
    QDir appDir(QCoreApplication::applicationDirPath());
#ifdef Q_OS_WIN
    appDir.cdUp();
#endif
    homeDir.cd(".gpg");
    appDir.cd(".gpg");
    return this->value(ENGINE_HOMEDIR, appDir.exists() ? appDir.path() : homeDir.path()).toString();
}

void Settings::setEngineFileName(const QString &filename)
{
    this->setValue(ENGINE_FILENAME, filename);
}

void Settings::setEngineHomeDirectory(const QString &homedir)
{
    this->setValue(ENGINE_HOMEDIR, homedir);
}

const QString Settings::ENGINE_FILENAME = "EngineFileName";
const QString Settings::ENGINE_HOMEDIR = "EngineHomeDir";
