#include "UserData.h"


void UserData::setSocket(QTcpSocket* socket) {
    qDebug() << "Setting socket:" << socket;
    UserSocket = socket;
    emit socketSet(socket);  // Оповещение, что сокет установлен
}
