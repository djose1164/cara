#include "healthresource.h"
#include "Utils/httpstatuscodes.h"
#include "config.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRestAccessManager>
#include <QRestReply>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto versionLocator{"/%1"_L1};
constexpr auto messageField{"message"_L1};
}

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
    auto req{api_->createRequest(path_ + versionLocator.arg(getCaraVersion().toString()))};

    manager_->get(req, this, [this](QRestReply &reply) {
        qDebug() << "HealthResource::readStatus: handling response";
        if (reply.error() == QNetworkReply::OperationCanceledError)
        {
            emit timeout();
            return;
        }

        const auto optionalJson{reply.readJson()};
        if (!reply.isSuccess())
        {
            const QString errorMessage{
                reply.errorString().isEmpty() ? reply.networkReply()->errorString()
                                              : reply.errorString()
            };

            if (reply.error() == QNetworkReply::ConnectionRefusedError)
                emit connectionRefused();
            else if (reply.httpStatus() == HttpStatus::UpgradeRequired)
                emit upgradeRequired();
            else
            {
                if (optionalJson && optionalJson->isObject()
                    && optionalJson->object().contains(messageField))
                    emit errorOccurred((*optionalJson)[messageField].toString());
                else
                    emit errorOccurred(errorMessage);
            }

            return;
        }

        if (optionalJson)
            emit statusReady(optionalJson->toJson());
    });
}
