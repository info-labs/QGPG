#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>


class Settings : public QSettings
{
    Q_OBJECT
public:
    Settings();

    QString engineFileName() const;
    QString engineHomeDirectory() const;

    void setEngineFileName(const QString &filename);
    void setEngineHomeDirectory(const QString &homedir);

    static const QString ENGINE_FILENAME;
    static const QString ENGINE_HOMEDIR;

};

#endif // SETTINGS_H
