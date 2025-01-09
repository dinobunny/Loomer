#include "server.h"
#include "sending.h"
#include <QDebug>
#include <QTimer>
#include <QStringBuilder>
#include "qmutex.h"



QList<QTcpSocket*> server::Sockets;
QMutex server::mutex;

server::server() {
    if (this->listen(QHostAddress::Any, 2323)) {
        qDebug() << "Server started on port 2323";
    } else {
        qDebug() << "Error starting server";
    }

}


void server::setSending(Sending& sending) {
    sendingPtr = &sending;
    connect(this, &server::newClientConnected, sendingPtr, &Sending::Get_New_Client);
    connect(this, &server::disconnectedClient, sendingPtr, &Sending::Get_Disconnected_Client);
}


void server::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket* socket = new QTcpSocket(this);
    if (socket->setSocketDescriptor(socketDescriptor)) {
        QString clientIP = socket->peerAddress().toString();
        qDebug() << "Client connected from IP:" << clientIP << "with deck:" << socketDescriptor;

        Sockets.push_back(socket);
        qDebug()<< "Socket :"<<socket->peerPort();

        QString IP = socket->peerAddress().toString();

        connect(socket, &QTcpSocket::readyRead, this, &server::slotsReadyRead);
        connect(socket, &QTcpSocket::disconnected, this, [this, socketDescriptor, IP](){
            qDebug() << "disconnected dest form server" <<socketDescriptor;
            emit disconnectedClient(socketDescriptor, IP);
        });
        qDebug() << "Client connected, socket descriptor:" << socketDescriptor;

        emit newClientConnected(socket, Sockets);

    }
}

void server::slotsReadyRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        QDataStream in(socket);
        in.setVersion(QDataStream::Qt_6_0);

        if (in.status() == QDataStream::Ok) {
            qDebug() << "Reading data...";
            QString str;
            in >> str;

            QString Identifier = str.left(2);
            int messageType = Identifier.toInt();
             qDebug()<<str;
             qDebug()<<messageType;



            if (messageType==04) {
                QString idPart = str.mid(QString(Identifier+",").length()).trimmed();
                qDebug() << "ID part:" << idPart;

                QStringList identificators = idPart.split(",");
                qDebug() << "Identificators:" << identificators << "Size:" << identificators.size();

                if (identificators.size() == 3) {
                    quintptr RESIVER = identificators[0].toLongLong();  // Interlocutor
                    quintptr SENDLER = identificators[1].toLongLong();  // MySocket
                    QString text = identificators[2];                    // Message Text

                    // Output for verification
                    qDebug() << "Sender ID:" << SENDLER;
                    qDebug() << "Receiver ID:" << RESIVER;
                    qDebug() << "Message Text:" << text;

                    connect(this, &server::ComunicationMesage,this, &server::Comunication);
                    emit ComunicationMesage(RESIVER, SENDLER, text);

                    // Emit ComunicationPare if needed
                } else {
                    qDebug() << "Invalid number of identifiers! Found:" << identificators.size();
                }
            }


            else {
            qDebug() << "Received:" << str;
            SendToClient(socket, str);}

        } else {
            qDebug() << "DataStream error";
        }
    }
}


void server::SendToClient(QTcpSocket* clientSocket, const QString& str) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    out << str;

    if (clientSocket->state() == QAbstractSocket::ConnectedState) {
        clientSocket->write(data);
    } else {
        // Обработка закрытых сокетов
        qDebug()<<"Client "<<clientSocket->peerAddress()<<" is DISCONNECT";
        clientSocket->disconnectFromHost();
        Sockets.removeAll(clientSocket);
        // clientSocket->deleteLater();
    }
}




// void server::handleDisconnection() {

//     QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

//     if (!socket) {
//         qDebug() << "Error: Null socket in handleDisconnection";
//         return;
//     }

//     QByteArray dataToRemove;
//     QDataStream outToRemove(&dataToRemove, QIODevice::WriteOnly);
//     outToRemove.setVersion(QDataStream::Qt_6_0);

//     // Створення повідомлення для видалення

//     quintptr deskriptor_to_delete = socket->socketDescriptor();
//     QString identifierToRemove = QString("tOrEmUvE %1, %2")
//                                      .arg(socket->peerAddress().toString())
//                                      .arg(QString::number(deskriptor_to_delete));

//     outToRemove << identifierToRemove;

//     // Відправлення повідомлення всім підключеним клієнтам
//     for (auto socketTarget : Sockets) {
//         if(socket!=socketTarget){
//         socketTarget->write(dataToRemove);
//         qDebug() << "Sent delete command to" << socketTarget->peerAddress().toString();
//         }
//     }

//     if (socket) {
//         qDebug() << "Client disconnected, removing socket";
//         Sockets.removeAll(socket);
//         socket->deleteLater();
//     }
// }

// void server::SendIdentificator() {
//     QByteArray data;
//     QDataStream out(&data, QIODevice::WriteOnly);
//     out.setVersion(QDataStream::Qt_6_0);

//     QList<QTcpSocket*> activeSockets;

//     for (QTcpSocket* socket : Sockets) {
//         if (socket->state() == QAbstractSocket::ConnectedState) {
//             activeSockets.append(socket);
//         } else {
//             socket->deleteLater();
//         }
//     }

//     for (QTcpSocket* socketTarget : activeSockets) {
//         for (QTcpSocket* socketToSend : activeSockets) {
//             quintptr deskriptor_to_send = socketToSend->socketDescriptor();
//             QString identifier = (socketToSend == socketTarget)
//             ? QString("mYthEinDeNtIfIcAtOr %1, %2").arg(socketToSend->peerAddress().toString()).arg(QString::number(deskriptor_to_send))
//             : QString("thEinDeNtIfIcAtOr %1, %2").arg(socketToSend->peerAddress().toString()).arg(QString::number(deskriptor_to_send));

//             QByteArray data;
//             QDataStream out(&data, QIODevice::WriteOnly);
//             out << identifier;


//             if (socketTarget->write(data) == -1) {
//                 qDebug() << "Error writing to socket" << socketTarget->errorString();
//             }
//         }
//     }

//     Sockets = activeSockets;
// }

void server::Comunication(quintptr resiver, quintptr sender, QString text)
{
    qDebug()<<"resiver"<<resiver;
    qDebug()<<"sender"<<sender;
    qDebug()<<"text"<<text;

    QTcpSocket* RESIVER = nullptr; // Указатель на сокет получателя
    QTcpSocket* SENDER = nullptr;  // Указатель на сокет отправителя

    // Предположим, что resiver и sender — это дескрипторы сокетов
    for (QTcpSocket* socket : Sockets) {
        if (socket->socketDescriptor() == resiver) {
            RESIVER = socket; // Присваиваем указатель на сокет
            qDebug()<<"Socet Deskriptor"<<RESIVER->socketDescriptor();
        }
    }

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_0);
    QString  message = QString("04,%1,%2,%3")
                                .arg(resiver)
                                .arg(sender)
                                .arg(text);
    out << message;

    if (RESIVER && RESIVER->state() == QAbstractSocket::ConnectedState) {

        RESIVER->write(data); // Преобразуем QString в QByteArray
    }



}

QList<QTcpSocket*>& server:: getArray() const  {
    QMutexLocker locker(&mutex);

}



void Resive_Identifier(QList<QTcpSocket*> Resiving_Identifier){
    for(auto resive:Resiving_Identifier){
        qDebug()<<"RESIVED " << resive;
        }

}
