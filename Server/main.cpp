#include <QCoreApplication>

#include "server.h"
#include "sending.h"
#include "Config.hpp"


int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);
  
    Config config{"./config.json"};
    config.Load();

    server server{config.GetSettings()};

    Sending sendObj(&server);

    sendObj.start();

    server.setSending(sendObj);

    return a.exec();
}
