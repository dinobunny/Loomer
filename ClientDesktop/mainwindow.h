#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidgetItem>
#include <QMainWindow>
#include <QTcpSocket>
#include <QVector>
#include <QJsonObject>
#include <QMainWindow>

#include "enums.h"
#include "getpath.h"




QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(const QString& configPath, QWidget *parent = nullptr);
    ~MainWindow();
    QString Get_Path(Directorys file, Files directory);
    void SendToServer(const QString &str);

    private slots:
        void slotReadyRead();
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void onConnected();
    void onError(QAbstractSocket::SocketError error);
    void onDisconnected();
    void setupConnection();


private:
    void extracted();
    void Socket_print();
    void Socket_delete(QString socket_to_delete);
    void Read_Config(QTcpSocket *socket);
    QString Style_Sheete();


    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    QString MySocket;
    QString Interlocutor {};
    QVector<QString> Sockets;

    bool Close_Window_stat = false;

protected:
    void closeEvent(QCloseEvent *event) override;

};


#endif // MAINWINDOW_H
