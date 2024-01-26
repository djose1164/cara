#include "utils.h"
#include <QJsonDocument>

namespace Utils
{
std::optional<QJsonObject> byteArrayToJsonObject(const QByteArray &data)
{
    QJsonParseError parseError;
    const auto json = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        qDebug() << "byteArrayToJsonObject: Data not JSON: " << parseError.errorString()
                 << "at" << parseError.offset << data;
        return std::nullopt;
    }
    return json.object();
}
}
