#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QFile>

#include "Config.hpp"

Config::Config(std::string_view aFilePath)
    : filePath{aFilePath}
{

}

void Config::Load()
{
    QFile file(filePath.c_str());
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal() << "Error open config";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    // Парсим JSON
    QJsonDocument config_json = QJsonDocument::fromJson(data);

    if (config_json.isNull()) {
        qFatal() << "Error work with config";
        return;
    }

    settings.config_obj = config_json.object();

    QString server_channel_string = settings.config_obj.value("Settings").toObject().value("host-adres").toString();
    settings.server_port = settings.config_obj.value("Settings").toObject().value("server-port").toInt();


    if(server_channel_string == "Any")
    {
        settings.server_channel = QHostAddress::Any;
    }
}

const Config::Settings &Config::GetSettings() const
{
    return settings;
}
