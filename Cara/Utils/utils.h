#ifndef UTILS_H
#define UTILS_H

#include <QJsonObject>
#include <QMetaProperty>

class QRestReply;

class QNetworkRequest;
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

template <typename T>
concept HasStaticMetaObject = requires(T)
{
    {T::staticMetaObject} -> std::convertible_to<const QMetaObject *>;
};

template <typename T>
    //requires HasStaticMetaObject<T>
QJsonObject toJson(const void *entity)
{
    QJsonObject obj;
    const QMetaObject *metaObj = &T::staticMetaObject;
    for (int i{metaObj->propertyOffset()}; i < metaObj->propertyCount(); ++i)
    {
        auto &&prop{metaObj->property(i)};
        if (auto&& propValue{prop.readOnGadget(entity)}; prop.isReadable())
        {
            const auto propertyName{QString::fromLatin1(prop.name())};
            obj[propertyName] = QJsonValue::fromVariant(propValue);
        }
    }

    return obj;
}

void setBoundaryToRequest(QAnyStringView boundary, QNetworkRequest &request);

}
#endif // UTILS_H
