#include "utils.h"
#include <QJsonDocument>
#include <QRestReply>

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

bool isOperationCanceledError(const QRestReply &reply)
{
    return reply.error() == QNetworkReply::OperationCanceledError;
}

}
