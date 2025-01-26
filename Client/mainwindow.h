#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QTcpSocket>
#include <QVector>
#include "enums.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void slotReadyRead();
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();

    void on_listWidget_itemClicked(QListWidgetItem *item);


private:
    void SendToServer(const QString &str);
    void Socket_print();
    void Socket_delete(QString socket_to_delete);
    void Read_Config(QTcpSocket *socket);
    QString Style_Sheete();
    QString Get_Path(Directorys file, Files directory);


    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    QString MySocket;
    QString Interlocutor;
    QVector<QString> Sockets;
};
#endif // MAINWINDOW_H
