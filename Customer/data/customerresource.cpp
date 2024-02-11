#include "customerresource.h"
#include <QSettings>
#include <QNetworkReply>
#include <QRestReply>
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceIdentification = "/%1"_L1;
}

namespace Data
{
CustomerResource::CustomerResource(QObject *parent)
    : AbstractResource{parent}
    , Business::CustomerRepository{}
    , data_{}
    , errorString_{}
    , path_{}
{}

QByteArray CustomerResource::data() const
{
    return data_;
}

void CustomerResource::setData(const QByteArray &newData)
{
    if (data_ == newData)
        return;
    data_ = newData;
    emit dataChanged();
}

void CustomerResource::readBySalespersonId(SalespersonId salespersonId)
{
    const auto req{api_->createRequest(path_
        + QString("/?salesperson_id=%1").arg(salespersonId))};

    manager_->get(req, this, [&](QRestReply *reply) {
        if (!reply->isSuccess())
        {
            setErrorString(reply->errorString());
            emit errorOccurred();
            if (reply->httpStatus() == 404)
            {
                setErrorString("El recurso solicitado no existe.");
                emit contentNotFound();
            }
            return;
        }
        setData(reply->body());

        emit fetched(data());
    });
}

QString CustomerResource::path() const
{
    return path_;
}

void CustomerResource::setPath(const QString &newPath)
{
    if (path_ == newPath)
        return;
    path_ = newPath;
    emit pathChanged();
}

QString CustomerResource::errorString() const
{
    return errorString_;
}

void CustomerResource::setErrorString(const QString &newErrorString)
{
    if (errorString_ == newErrorString)
        return;
    errorString_ = newErrorString;
    emit errorStringChanged();
}
}
