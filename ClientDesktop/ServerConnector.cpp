#include "ServerConnector.h"
#include "UserData.h"
#include "config.h"
#include "m_pack.h"

#include <QTcpSocket>
#include <winsock.h>
#include <QTimer>


M_pack m_pack;

extern UserData &userData ;

ServerConnector::ServerConnector(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)) {}

void ServerConnector::ConnectToServer()
{
    Config config;
    config.Read();

    if (!socket) { // Создаём сокет, если его нет
        socket = new QTcpSocket(this);
    }

    socket->connectToHost(Config::settings.server_ip, Config::settings.server_port);

    if (!userData.getSocket()) {  // Проверка перед установкой
        userData.setSocket(socket);
        qDebug() << "Socket made:" << userData.getSocket() << "desk" << socket->socketDescriptor();
    } else {
        qWarning() << "Socet is already exist";
    }

    if (!socket->waitForConnected(5000)) {
        qWarning() << "Не удалось подключиться к серверу:" << socket->errorString();
        return;
    }

    connect(socket, &QTcpSocket::readyRead, this, &ServerConnector::slotReadyRead);

}

void ServerConnector::SendMyData(int status)
{
    QString message = QString("%1,%2,%3")
        .arg(status)
        .arg(userData.name)
        .arg(userData.pass);

        socket->write(QByteArray::fromStdString(m_pack.puck(message)));

    if (!socket->waitForBytesWritten(5000)) {
        qWarning() << "Sending data error:" << socket->errorString();
    }
}

void ServerConnector::slotReadyRead()
{

    UserData& userdata = UserData::getInstance();

    if(userdata.gotlogindata.value() != true){
        M_pack msg_p;
        QString str = msg_p.unpack(socket->readAll());
        QStringList parts = str.split(",");

        qDebug() << str;

        int messType = parts[0].toInt();
        if (messType == LOGIN_SEC) {

            qDebug()<< "Got desk: " << parts[1];

            userdata.desck = parts[1];
        }
        userdata.gotlogindata = true;
    }
}
