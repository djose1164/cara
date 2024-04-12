#include "customerresource.h"
#include "httpstatuscodes.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QRestAccessManager.h>
#include <QRestReply>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto resourceIdentification = "/%1"_L1;
constexpr auto summaryPath{"/summary"_L1};
constexpr auto userKey{"user_id"_L1};
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

    manager_->get(req, this, [&](QRestReply &reply) {
        if (!reply.isSuccess())
        {
            setErrorString(reply.errorString());
            emit errorOccurred();
            if (reply.httpStatus() == HttpStatus::NotFound)
            {
                setErrorString("El recurso solicitado no existe."_L1);
                emit contentNotFound();
            }
            return;
        }

        setData(reply.readBody());

        emit fetched(data());
    });
}

void CustomerResource::readByUserId(UserId id)
{
    QUrlQuery query{};
    query.addQueryItem(userKey, QString::number(id));
    const auto req{api_->createRequest(path_ + "/"_L1, std::move(query))};

    manager_->get(req, this, [&](QRestReply &reply) {
        if (!reply.isSuccess())
        {
            setErrorString(reply.errorString());
            emit errorOccurred();
            if (reply.httpStatus() == HttpStatus::NotFound)
            {
                setErrorString("El recurso solicitado no existe."_L1);
                emit contentNotFound();
            }
            return;
        }

        setData(reply.readBody());

        emit fetched(data());
    });
}

void CustomerResource::readSummary(CustomerId id)
{
    const auto resourceId{resourceIdentification.arg(QString::number(id))};
    const auto req{api_->createRequest(path_ + resourceId + summaryPath)};

    manager_->get(req, this, [&](QRestReply &reply) {
        if (!reply.isSuccess())
        {
            setErrorString(reply.errorString());
            emit errorOccurred();
            return;
        }

        setData(reply.readBody());
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
