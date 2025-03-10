#include "m_pack.h"
#include <msgpack.hpp>


QString M_pack::unpack(QByteArray rawData) {

    msgpack::object_handle oh = msgpack::unpack(rawData.constData(), rawData.size());
    msgpack::object obj = oh.get();
    QString data = QString::fromStdString(obj.as<std::string>());
    return data;
}

std::string M_pack::puck(QString rawData){

    std::string msg = rawData.toStdString();
    msgpack::sbuffer buffer;
    msgpack::pack(buffer, msg);
    return std::string(buffer.data(), buffer.size());
}


