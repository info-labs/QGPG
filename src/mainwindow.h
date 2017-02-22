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
    void on_actionVerifyFile_triggered();
    void on_toolButtonVerify_clicked();

private:
    void verifyDetachedSignature(const QString &filepath, const QString &datapath);
    void verifyOpaqueSignature(const QString &filepath);
    void setVerifyResult(const GpgME::VerificationResult &result);

    QSharedPointer<Ui::MainWindow> ui;
    QSharedPointer<GpgME::Context> ctx;
    GPGKeyListModel *model;

};

#endif // MAINWINDOW_H
