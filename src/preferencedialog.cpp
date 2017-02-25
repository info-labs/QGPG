#include "preferencedialog.h"
#include "ui_preferencedialog.h"

#include <QFileDialog>

#include "settings.h"

PreferenceDialog::PreferenceDialog(const QSharedPointer<Settings> &settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog),
    settings(settings)
{
    ui->setupUi(this);

    connect(this, &QDialog::accepted, this, &PreferenceDialog::on_accepted);

    // load settings
    this->load_settings();
}

PreferenceDialog::~PreferenceDialog()
{

}

void PreferenceDialog::load_settings()
{
    this->ui->lineEditFileName->setText(this->settings->engineFileName());
    this->ui->lineEditHomeDir->setText(this->settings->engineHomeDirectory());
}

void PreferenceDialog::save_settings()
{
    this->settings->setEngineFileName(this->ui->lineEditFileName->text());
    this->settings->setEngineHomeDirectory(this->ui->lineEditHomeDir->text());
}

void PreferenceDialog::on_accepted()
{
    this->save_settings();
}

void PreferenceDialog::on_toolButtonFileName_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,
                                                    tr("Engine"),
                                                    "",
#ifdef Q_OS_WIN
                                                    tr("GPG(gpg.exe gpg2.exe)")
#else
                                                    tr("GPG(gpg gpg2)")
#endif
                                                    );
    if ( filepath.isEmpty() ) {
        return;
    }
    this->ui->lineEditFileName->setText(filepath);
}

void PreferenceDialog::on_toolButtonHomeDir_clicked()
{
    QString dirpath = QFileDialog::getExistingDirectory(this, tr("Home directory"), "");
    if ( dirpath.isEmpty() ) {
        return;
    }
    this->ui->lineEditHomeDir->setText(dirpath);
}
