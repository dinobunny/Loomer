#include <QCoreApplication>
#include "server.h"
#include "sending.h"




int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    server server;  // Создаем объект сервера

    Sending sendObj(&server);

    sendObj.start();

    server.setSending(sendObj);


    return a.exec();
}
