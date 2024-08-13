#include "utils.h"
#include <QJsonDocument>
#include <QRestReply>

using namespace Qt::StringLiterals;

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

void setBoundaryToRequest(QAnyStringView boundary, QNetworkRequest &request)
{
    const auto headerValue{"multipart/form-data; boundary=\"%1\""_L1.arg(boundary.toString())};
    request.setHeader(QNetworkRequest::ContentTypeHeader, headerValue);
}

}
