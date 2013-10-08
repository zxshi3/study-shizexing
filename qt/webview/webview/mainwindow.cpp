#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    url = "http://scout.me";

    url = "http://www.google.com";
    //loadUrl(url);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadUrl(const QString & url)
{
    QString tmpUrl;
    if (!url.startsWith("http", Qt::CaseInsensitive)) {
        tmpUrl = QString("http://") + url;
    } else {
        tmpUrl = url;
    }
    qWarning("loading : %s", tmpUrl.toStdString().c_str());
    ui->webView->setUrl(QUrl(tmpUrl));
    //ui->webView->load((QUrl(tmpUrl)));
}

void MainWindow::on_EdUrl_textChanged(const QString &str)
{
    url = str;
}

void MainWindow::on_btGo_clicked()
{
    loadUrl(url);
}

void MainWindow::on_btBack_clicked()
{
    ui->webView->back();
}

void MainWindow::on_EdUrl_returnPressed()
{
    loadUrl(url);
}
