#include "restcustomerrepository.h"
#include "../user/localuserstore.h"
#include <QSettings>
#include <QNetworkReply>

namespace Data
{
RestCustomerRepository::RestCustomerRepository(QObject *parent)
    : Business::CustomerRepository{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , data_{}
    , errorString_{}
{}

QByteArray RestCustomerRepository::data() const
{
    return data_;
}

void RestCustomerRepository::setData(const QByteArray &newData)
{
    if (data_ == newData)
        return;
    data_ = newData;
    emit dataChanged();
}

void RestCustomerRepository::readBySalespersonId(SalespersonId salespersonId)
{
    const auto req{prepareRequest(QString("/?salesperson_id=%1").arg(salespersonId))};

    reply_ = manager_->get(req);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        setData(reply_->readAll());
        if (reply_->error() != QNetworkReply::NoError)
        {
            setErrorString(reply_->errorString());
            emit errorOccurred();
        }
        else
            emit fetched(data());

        reply_->deleteLater();
    });
}

QNetworkRequest RestCustomerRepository::prepareRequest(const QString &endpoint)
{
    const QUrl url{QSettings().value("base").toString() + "/customers" + endpoint};
    QNetworkRequest req{url};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const QString token{Business::LocalUserStore().user().accessToken()};
    req.setRawHeader("Authorization", token.toUtf8());
    return req;
}

QString RestCustomerRepository::errorString() const
{
    return errorString_;
}

void RestCustomerRepository::setErrorString(const QString &newErrorString)
{
    if (errorString_ == newErrorString)
        return;
    errorString_ = newErrorString;
    emit errorStringChanged();
}
}
