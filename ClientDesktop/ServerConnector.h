#ifndef SERVERCONNECTOR_H
#define SERVERCONNECTOR_H

#include "UserData.h"
#include <QString>
#include <QObject>
#include <QTcpSocket>

class ServerConnector : public QObject
{
    Q_OBJECT  // Убираем точку с запятой

public:
    explicit ServerConnector(QObject* parent = nullptr);

    void ConnectToServer();
    void SendMyData(int status);

private:
    QTcpSocket *socket;


signals:
    void socketConnected();         // Сигнал о подключении

private slots:
    void slotReadyRead();           // Слот для чтения данных из сокета

};

#endif // SERVERCONNECTOR_H
