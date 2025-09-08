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
    , errorString_{}
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
            if (reply.error() == QNetworkReply::ConnectionRefusedError)
            {
                setErrorString(u"No se ha podido establecer una conexión con el servidor. "
                               "Intenta de nuevo más tarde."_s);
                emit errorOccurred(errorString());
                emit connectionRefused(errorString());
            }
            else if (reply.httpStatus() == HttpStatus::UpgradeRequired)
            {
                setErrorString("Necesitas actualizar Cara a la ultima version."_L1);
                emit errorOccurred(errorString());
                emit upgradeRequired(errorString());
            }
            else
            {
                if (optionalJson && optionalJson->isObject()
                    && optionalJson->object().contains(messageField))
                {
                    setErrorString((*optionalJson)[messageField].toString());
                    emit errorOccurred(errorString());
                }
                else
                {
                    const QString errorMessage{reply.errorString().isEmpty()
                                                   ? reply.networkReply()->errorString()
                                                   : reply.errorString()};
                    setErrorString(errorMessage);
                    emit errorOccurred(errorMessage);
                }
            }
            return;
        }

        if (!optionalJson)
            return;
        emit statusReady(optionalJson->toJson());
        if (optionalJson->isObject() and optionalJson->object().contains("status"_L1))
        {
            const QString healthStatus{optionalJson->object()["status"_L1].toString()};
            qDebug() << "healthStatus: " << optionalJson->object();
            if (healthStatus == "OK"_L1)
                emit serverHealthy();
            else
            {
                setErrorString("Nuestros servidores se encuentran actualmente bajo "
                               "mantenimiento. Puedes ver como va en "
                               "<a href=\"https://twitter.com/carardoficial\">Twiterr</a>."_L1);
                emit errorOccurred(errorString());
                emit serverMaintenance(errorString());
            }
        }
    });
}

QString HealthResource::errorString() const
{
    return errorString_;
}

void HealthResource::setErrorString(const QString &newErrorString)
{
    errorString_ = newErrorString;
}

QBindable<QString> HealthResource::bindableErrorString() const
{
    return &errorString_;
}
