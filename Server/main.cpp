#include "server.h"
#include "sending.h"
#include "Config.hpp"
#include <QCoreApplication>
#include <QFile>
#include <iostream>
#include <QCommandLineParser> // обробка параметрів через QCommandLineParser  щобв у майбутньому було легше додавати нові опції (наприклад, порт).


static void myLogMessageHandler(const QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString formattedMsg = qFormatLogMessage(type, context, msg) + "\n";

    // Записываем в файл
    QFile logFile("app.log");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        logFile.write(qUtf8Printable(formattedMsg));
    }

    // Выводим в консоль
    fprintf(stderr, "%s", qUtf8Printable(formattedMsg));
    fflush(stderr);
}

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    qInstallMessageHandler(myLogMessageHandler);
    // Install custom message pattern
    qSetMessagePattern("%{time yyyy-MM-dd hh:mm:ss,zzz} [%{type}] [%{line}] [%{file}]: %{message}");

    QCommandLineParser parser;
    parser.setApplicationDescription("Server application");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption configOption("config", "Path to config file", "path");
    parser.addOption(configOption);

    parser.process(a);

    QString configPath = parser.isSet(configOption) ? parser.value(configOption) : "./config_server.json";
    std::cout << "Using config: " << configPath.toStdString() << std::endl;

    Config config{configPath.toStdString()};
    config.Load();


    server server{config.GetSettings()};
    Sending sendObj(&server);

    sendObj.start();
    server.setSending(sendObj);

    return a.exec();
}