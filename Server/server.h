#ifndef SERVER_H
#define SERVER_H

#include "qjsonobject.h"
#include "qmutex.h"
#include <QList>
#include <QTcpServer>
#include <QTcpSocket>

class Sending;

class server : public QTcpServer {
    Q_OBJECT

public:
    server();
    void setSending(Sending &sending);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void slotsReadyRead();

private:
    static QList<QTcpSocket *> Sockets;
    static QMutex mutex;

    Sending *sendingPtr = nullptr;
    qint16 server_port;
    QHostAddress::SpecialAddress addressEnum;

signals:
    void newClientConnected(QTcpSocket *socet, QList<QTcpSocket *> &Sockets);
    void disconnectedClient(qintptr socet, QString IP);
    void sendingMesage(QTcpSocket *socket, const QString &message);
};


class Config {
public:
    Config(){}
    void Read();

    struct Settings {
         QJsonObject config_obj;
         qint16 server_port;
         QHostAddress::SpecialAddress server_channel;
    };

    static Settings settings;

};

#endif // SERVER_H
