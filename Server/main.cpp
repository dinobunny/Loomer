#include <QCoreApplication>

#include "server.h"
#include "sending.h"
#include "Config.hpp"


int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    Config config{"./config.json"};
    config.Load();

    server s{config.GetSettings()};  // Создаем объект сервера

    Sending sendObj(&s);

    sendObj.start();

    s.setSending(sendObj);

    return a.exec();
}
