#include "config.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>


void Config::Read() {
    QFile file("config_client.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Error open config";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    // Парсим JSON
    QJsonDocument config_json = QJsonDocument::fromJson(data);

    if (config_json.isNull()) {
        qDebug() << "Error worck with config";
        return;
    }

    QJsonObject config_obj = config_json.object();

    Config::settings.server_ip =
        config_obj.value("Settings").toObject().value("server-ip").toString();
    Config::settings.server_port =
        config_obj.value("Settings").toObject().value("server-port").toInt();

}
