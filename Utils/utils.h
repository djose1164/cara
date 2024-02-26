#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>

namespace Utils
{
std::optional<QJsonObject> byteArrayToJsonObject(const QByteArray &data);

constexpr bool runningOnAndroid()
{
#ifdef Q_OS_ANDROID
    return true;
#else
    return false;
#endif
}
}
#endif // UTILS_H
