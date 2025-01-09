#include "sending.h"
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QMutex>
#include <windows.h>


QMutex mutex1;

Sending::Sending(server* srv, QObject* parent)
    : QThread(parent), m_server(srv){
    Sending::Sending_Identifier();
}

void Sending::Sending_Identifier() {
    qDebug()<<"ID";
    QTimer* timer = new QTimer(this);
    timer->setInterval(3000);

    connect(timer, &QTimer::timeout, this, [this, &timer]() {
        mutex1.lock();
        // qDebug() << "cheacking connection ...";

        for(int i = 0; i < Sockets.size(); i ++){

        }

        mutex1.unlock();

    });

    timer->start();

}
void Sending::Get_New_Client(QTcpSocket* socket, QList<QTcpSocket*> Sockets_reciverd) {
    qDebug() << "Socket from server:" << socket->socketDescriptor();

    Sockets = Sockets_reciverd;


    // Отправляем идентификатор новому клиенту
    QString MyIdentifier = QString("02, %1, %2") //my_identifier
                               .arg(socket->peerAddress().toString())
                               .arg(QString::number(socket->socketDescriptor()));
    sendToSocket(socket, MyIdentifier);

    qDebug() << "Sockets size :" << Sockets.size();

    for (int i = 0; i < Sockets.size(); ++i) {
        QTcpSocket* otherSocket = Sockets[i];

        // Пропускаем отключенные сокеты
        if (otherSocket->state() != QAbstractSocket::ConnectedState) {
            qDebug() << "Skipping disconnected socket:" << otherSocket;
            continue;
        }

        // Пропускаем сам себя
        if (socket->socketDescriptor() == otherSocket->socketDescriptor()) {
            continue;
        }

        // Отправляем новому клиенту информацию о других сокетах
        QString identifier1 = QString("03, %1, %2") //the_identifier
                                  .arg(otherSocket->peerAddress().toString())
                                  .arg(QString::number(otherSocket->socketDescriptor()));
        sendToSocket(socket, identifier1);

        // Отправляем другим сокетам информацию о новом клиенте
        QString identifier2 = QString("03, %1, %2") //the_identifier
                                  .arg(socket->peerAddress().toString())
                                  .arg(QString::number(socket->socketDescriptor()));
        sendToSocket(otherSocket, identifier2);
    }

    qDebug() << "DONE";
}

void Sending::Get_Disconnected_Client(qintptr socket, QString IP){
    qDebug()<<" Dicsonnected socket" << socket;
    qDebug()<<" Dicsonnected IP" << IP;


    mutex1.lock();
    for(int i = 0; i < Sockets.size(); i++){
        if(Sockets[i]->state() != QAbstractSocket::ConnectedState ){
            Sockets.removeAt(i);
        }
    }

    for(int i = 0; i < Sockets.size(); i++){

        QString identifier3 = QString("05, %1, %2") //the_identifier
                                  .arg(IP)
                                  .arg(socket);
        sendToSocket(Sockets[i], identifier3);
    }
    mutex1.unlock();
}

void Sending::sendToSocket(QTcpSocket* socket, const QString& message) {
    if (socket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Socket not connected:" << socket->socketDescriptor();
        return;
    }

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << message;

    socket->write(data);

    if (!socket->waitForBytesWritten(5000)) {  // Timeout для предотвращения вечного ожидания
        qDebug() << "Error waiting for bytes to be written:" << socket->errorString();
    }

    QThread::msleep(120);
}
