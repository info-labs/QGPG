#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QDialog>
#include <QSharedPointer>

class Settings;

namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferenceDialog(const QSharedPointer<Settings> &settings, QWidget *parent = nullptr);
    ~PreferenceDialog();

    void load_settings();
    void save_settings();

private slots:
    void on_accepted();
    void on_toolButtonFileName_clicked();
    void on_toolButtonHomeDir_clicked();

private:
    QSharedPointer<Ui::PreferenceDialog> ui;
    QSharedPointer<Settings> settings;
};

#endif // PREFERENCEDIALOG_H
