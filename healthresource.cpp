#include "healthresource.h"

#include <QRestReply>
#include <QRestAccessManager>
#include <QJsonDocument>

HealthResource::HealthResource(QObject *parent)
    : AbstractResource{parent}
{}

QString HealthResource::path() const
{
    return path_;
}

void HealthResource::setPath(const QString &newPath)
{
    if (path_ == newPath)
        return;
    path_ = newPath;
    emit pathChanged();
}

void HealthResource::readStatus()
{
    qDebug() << "HealthResource::readStatus: issuing request...";
    auto req{api_->createRequest(path_)};
    qDebug() << "HealthResource::readStatus: timeout: " << req.transferTimeout();


    manager_->get(req, this, [this](QRestReply &reply) {
        qDebug() << "HealthResource::readStatus: handling response";

        if (!reply.isSuccess())
        {
            if (reply.error() == QNetworkReply::OperationCanceledError)
                emit timeout();

            emit errorOccurred(reply.errorString());
            qDebug() << "HealthResource::readStatus: errorCode: " << reply.error()
                     << ": " << reply.errorString() << " " << reply.networkReply()->error();
            return;
        }

        if (const auto optionalJson{reply.readJson()})
            emit statusReady(optionalJson->toJson());
    });
}
