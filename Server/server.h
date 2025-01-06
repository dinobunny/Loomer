#ifndef SERVER_H
#define SERVER_H

#include "qmutex.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QByteArray>
#include <QDebug>

class Sending;

class server : public QTcpServer {
    Q_OBJECT

public:
    server();
    QList<QTcpSocket*>& getArray() const;  // Объявление метода

    void setSending(Sending& sending);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void slotsReadyRead();
    // void handleDisconnection();
   // void SendIdentificator();
    void Comunication(quintptr RESEVER, quintptr SENDER, QString text );

private:
    void ConectionHub();
    void SendToClient(QTcpSocket *clientSocket, const QString& str);
    void SendTOClientData(QVector<QTcpSocket *> identificators);
    static QList<QTcpSocket*> Sockets;
    static QMutex mutex;
    QByteArray Data;

     Sending* sendingPtr = nullptr;

signals:
    void ComunicationPare(QVector<QTcpSocket*> identificators);
    void ComunicationMesage(quintptr RESEVER, quintptr SENDER, QString text );
//  void newConnection(QTcpSocket* socket);

    void newClientConnected(QTcpSocket* socet, QList<QTcpSocket*> Sockets);

public slots:
     // void Resive_Identifier(QList<QTcpSocket*> resiving_identifier);

};

#endif // SERVER_H
