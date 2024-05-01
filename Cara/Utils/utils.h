#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>

class QRestReply;
namespace Utils
{
std::optional<QJsonObject> byteArrayToJsonObject(const QByteArray &data);

consteval bool runningOnAndroid()
{
#ifdef Q_OS_ANDROID
    return true;
#else
    return false;
#endif
}

bool isOperationCanceledError(const QRestReply &reply);
}
#endif // UTILS_H
