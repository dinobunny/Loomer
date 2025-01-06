#include "sending.h"
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QMutex>
#include <windows.h>

QMutex mutex1;

Sending::Sending(server* srv, QObject* parent)
    : QThread(parent), m_server(srv){
    // Sending::Sending_Identifier();
}

void Sending::Sending_Identifier() {
    qDebug()<<"ID";
    QTimer* timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this]() {
        QMutexLocker locker(&mutex1);
        QList<QTcpSocket*> sockets = m_server->getArray(); // Локальная копия

        // for(auto soc_to_ckek : sockets){
        //     qDebug() <<"Socket" <<  soc_to_ckek->socketDescriptor() << Qt::endl;
        // }

        locker.unlock();

        QList<QTcpSocket*> disconnectedSockets;

        // Поиск отключённых сокетов
        for (QTcpSocket* socket : sockets) {
            if (socket->state() != QAbstractSocket::ConnectedState) {
                disconnectedSockets.append(socket);

                // Уведомление других клиентов
                QByteArray dataToRemove;
                QDataStream outToRemove(&dataToRemove, QIODevice::WriteOnly);
                outToRemove.setVersion(QDataStream::Qt_6_0);

                quintptr descriptorToDelete = socket->socketDescriptor();
                qDebug()<<"desk :"<<descriptorToDelete;
                QString identifierToRemove = QString("tOrEmUvE %1, %2")
                                                 .arg(socket->peerAddress().toString())
                                                 .arg(QString::number(descriptorToDelete));

                outToRemove << identifierToRemove;

                for (QTcpSocket* socketTarget : sockets) {
                    if (socket != socketTarget) {
                        socketTarget->write(dataToRemove);
                    }
                }
            }
        }

        // Удаление отключённых сокетов
        for (QTcpSocket* socket : disconnectedSockets) {
            locker.relock();
            m_server->getArray().removeOne(socket); // Удаляем из оригинального списка
            locker.unlock();

            socket->deleteLater();
            qDebug() << "Client disconnected, removing socket:" << socket->peerAddress().toString();
        }

        // Отправка идентификаторов активным сокетам
        for (QTcpSocket* socketTarget : sockets) {
            if (socketTarget->state() == QAbstractSocket::ConnectedState) {
                for (QTcpSocket* socketToSend : sockets) {
                    QByteArray data;
                    QDataStream out(&data, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_6_0);

                    quintptr descriptorToSend = socketToSend->socketDescriptor();
                    QString identifier = (socketToSend == socketTarget)
                                             ? QString("mYthEinDeNtIfIcAtOr %1, %2")
                                                   .arg(socketToSend->peerAddress().toString())
                                                   .arg(QString::number(descriptorToSend))
                                             : QString("thEinDeNtIfIcAtOr %1, %2")
                                                   .arg(socketToSend->peerAddress().toString())
                                                   .arg(QString::number(descriptorToSend));

                    out << identifier;
                    socketTarget->write(data);
                }
            }
        }
    });

    timer->start(1000);
}

void Sending::Get_New_Client(QTcpSocket* socket, QList<QTcpSocket*> Sockets_reciverd) {
    qDebug() << "Socket from server:" << socket->socketDescriptor();

    Sockets = Sockets_reciverd;

    for (auto i : Sockets) {
        qDebug() << i << " ";
    }

    // Отправляем идентификатор новому клиенту
    QString MyIdentifier = QString("mYthEinDeNtIfIcAtOr %1, %2")
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
        QString identifier1 = QString("thEinDeNtIfIcAtOr %1, %2")
                                  .arg(otherSocket->peerAddress().toString())
                                  .arg(QString::number(otherSocket->socketDescriptor()));
        sendToSocket(socket, identifier1);

        // Отправляем другим сокетам информацию о новом клиенте
        QString identifier2 = QString("thEinDeNtIfIcAtOr %1, %2")
                                  .arg(socket->peerAddress().toString())
                                  .arg(QString::number(socket->socketDescriptor()));
        sendToSocket(otherSocket, identifier2);
    }

    qDebug() << "DONE";
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
    if (!socket->waitForBytesWritten()) {
        qDebug() << "Error waiting for bytes to be written:" << socket->errorString();
    } else {
        qDebug() << "Message sent to socket:" << socket->socketDescriptor() << message;
    }
}
