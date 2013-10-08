#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void on_EdUrl_textChanged(const QString &str);
    void on_btGo_clicked();

    void on_btBack_clicked();

    void on_EdUrl_returnPressed();

private:
    void loadUrl(const QString & url);

    Ui::MainWindow *ui;
    QString url;
};

#endif // MAINWINDOW_H
