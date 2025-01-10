#ifndef SERVER_H
#define SERVER_H

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

signals:
    void newClientConnected(QTcpSocket *socet, QList<QTcpSocket *> &Sockets);
    void disconnectedClient(qintptr socet, QString IP);
    void sendingMesage(QTcpSocket *socket, const QString &message);
};

#endif // SERVER_H
