#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include <QGpgME/DataProvider>
#include <gpgme++/data.h>
#include <gpgme++/key.h>
#include <gpgme++/keylistresult.h>

#include "models/gpgkeylistmodel.h"
#include "preferencedialog.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
#ifdef Q_OS_WIN
    settings(new Settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName())),
#else
    settings(new Settings),
#endif
    prefDialog(new PreferenceDialog(settings, this)),
    ctx(GpgME::Context::createForProtocol(GpgME::OpenPGP)),
    model(nullptr)
{
    this->ui->setupUi(this);

    // init settings
    this->load_settings();

    this->model = new GPGKeyListModel(ctx, this);
    this->ui->treeViewPublicKeys->setModel(this->model);
}

MainWindow::~MainWindow()
{

}

void MainWindow::load_settings()
{
    this->ctx->setEngineFileName(this->settings->engineFileName().toUtf8().constData());
    this->ctx->setEngineHomeDirectory(this->settings->engineHomeDirectory().toUtf8().constData());
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionPreference_triggered()
{
    this->prefDialog->load_settings();
    if ( this->prefDialog->exec() == QDialog::Accepted ) {
        this->load_settings();
        this->model->resetEngineInfo();
    }
}

void MainWindow::on_actionVerifyFile_triggered()
{
    QString filepath = this->ui->lineEditVerify->text();
    if ( filepath.isEmpty() || ! QFileInfo(filepath).exists() ) {
        filepath = QFileDialog::getOpenFileName(this, tr("Verify File"), "", tr("Signature File(*.sig *.gpg *.asc)"));
        if ( filepath.isEmpty() ) {
            return;
        }
    }

    this->ui->lineEditVerify->setText(filepath);
    if ( filepath.endsWith(".sig") && QFileInfo(filepath.left(filepath.size()-4)).exists() ) {
        this->verifyDetachedSignature(filepath, filepath.left(filepath.size()-4));
    } else {
        this->verifyOpaqueSignature(filepath);
    }
}

void MainWindow::on_toolButtonVerify_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this, tr("Verify File"), "", tr("Signature File(*.sig *.gpg *.asc)"));
    if ( filepath.isEmpty() ) {
        return;
    }
    this->ui->lineEditVerify->setText(filepath);

    if ( QFileInfo(filepath).exists() ) {
        this->ui->actionVerifyFile->trigger();
    }
}

void MainWindow::verifyDetachedSignature(const QString &filepath, const QString &datapath)
{
    if ( filepath.isEmpty() ) {
        return;
    }

    std::shared_ptr<QIODevice> sfile(new QFile(filepath));
    if ( ! sfile->open(QIODevice::ReadOnly) ) {
        return;
    }
    std::shared_ptr<QIODevice> dfile(new QFile(datapath));
    if ( ! dfile->open(QIODevice::ReadOnly) ) {
        return;
    }

    QGpgME::QIODeviceDataProvider signProvider(sfile);
    QGpgME::QIODeviceDataProvider dataProvider(dfile);
    GpgME::Data sign(&signProvider);
    GpgME::Data data(&dataProvider);
    auto result = this->ctx->verifyDetachedSignature(sign, data);

    this->setVerifyResult(result);
}

void MainWindow::verifyOpaqueSignature(const QString &filepath)
{
    if ( filepath.isEmpty() ) {
        return;
    }

    std::shared_ptr<QIODevice> file(new QFile(filepath));
    if ( ! file->open(QIODevice::ReadOnly) ) {
        return;
    }

    QGpgME::QIODeviceDataProvider provider(file);
    GpgME::Data data(&provider);
    GpgME::Data plain;
    auto result = this->ctx->verifyOpaqueSignature(data, plain);

    this->setVerifyResult(result);
}

void MainWindow::setVerifyResult(const GpgME::VerificationResult &result)
{
    if ( result.numSignatures() == 0 ) {
        // no signature
        this->ui->plainTextEditVerify->setPlainText(this->tr("no signature."));
        return;
    }

    if ( result.error().code() != 0 ) {
        this->ui->plainTextEditVerify->setPlainText(this->tr("Error: %1").arg(result.error().asString()));
        return;
    }

    QString text;
    QTextStream stream(&text);
    for ( const GpgME::Signature &sign : result.signatures() ) {
        QDateTime creationTime = QDateTime::fromTime_t(sign.creationTime());
        QString fingerprint = sign.fingerprint();

        stream << this->tr("Signature made %1 using key ID %2\n").arg(creationTime.toString()).arg(fingerprint.right(8));

        GpgME::Signature::Summary summary = sign.summary();
        GpgME::Key key = sign.key(true, false);
        GpgME::UserID uid = key.userID(0);
        QString issuer;
        if ( ! uid.isNull() ) {
            issuer = uid.id();
        } else {
            this->ctx->startKeyListing(fingerprint.toUtf8().constData());
            GpgME::Error error;
            key = this->ctx->nextKey(error);
            this->ctx->endKeyListing();
            issuer = key.userID(0).id();
        }
        if ( (summary & GpgME::Signature::Valid) && (summary & GpgME::Signature::Green) ) {
            stream << this->tr("Good signature from %1\n").arg(issuer);
        } else {
            stream << this->tr("Bad signature from %1\n").arg(issuer);
        }
    }
    this->ui->plainTextEditVerify->setPlainText(text);
}
