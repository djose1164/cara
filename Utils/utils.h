#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>

namespace Utils
{
std::optional<QJsonObject> byteArrayToJsonObject(const QByteArray &data);
}
#endif // UTILS_H
