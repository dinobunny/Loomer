#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QTcpSocket>
#include <QVector>
#include "enums.h"


#include <QJsonObject>


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
    QString Get_Path(Directorys file, Files directory);

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


    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    QString MySocket;
    QString Interlocutor;
    QVector<QString> Sockets;
};



class Config {
private:
    MainWindow *mainWindow;

public:
    Config(){}
    void Read();

    Config(MainWindow *mw) : mainWindow(mw) {}

    struct Settings {
        QJsonObject config_obj;
        qint16 server_port;
        QString server_ip;
    };

    static Settings settings;

};
#endif // MAINWINDOW_H
