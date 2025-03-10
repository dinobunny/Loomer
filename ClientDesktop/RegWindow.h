#ifndef REGWINDOW_H
#define REGWINDOW_H

#include <QDialog>


class MainWindow;

class ServerConnector;

namespace Ui {
class regwindow;
}

class RegWindow : public QDialog
{
    Q_OBJECT

public:

    QString Style_Sheete();

    explicit RegWindow(QWidget *parent = nullptr);
    ~RegWindow();

private slots:
    void on_pushButton_Log_in_clicked();
    void on_pushButton_Sing_up_clicked();
    void on_pushButton_end_clicked();
    void on_pushButton_return_clicked();

private:
    Ui::regwindow *ui;
    ServerConnector *serverConnector;

    void ShowMainForm();
    void StartUpWindow();

signals:
    void CloseWindow();




};

#endif // REGWINDOW_H
