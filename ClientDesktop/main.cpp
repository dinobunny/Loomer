#include <QApplication>
#include "mainwindow.h"
#include "RegWindow.h"
#include "userdata.h"
#include <QCommandLineParser>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Client application");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption configOption("config", "Path to config file", "path");
    parser.addOption(configOption);

    parser.process(app);

    QString configPath = parser.isSet(configOption) ? parser.value(configOption) : "./config_client.json";
    qDebug() << "Using config: " << configPath.toStdString().c_str();

    RegWindow* regwindow = new RegWindow(configPath);
    regwindow->show();

    int regwindresult = app.exec();

    if (regwindresult == 0) {
        delete regwindow;
        MainWindow mainwindow(configPath);
        mainwindow.show();
        return app.exec();
    }

    delete regwindow;
    return regwindresult;
}
