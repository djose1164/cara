#include "internetstatus.h"
#include <QNetworkReply>

InternetStatus::InternetStatus(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{}
    , status_{Status::NoStatus}
{
    connect(this, &InternetStatus::statusChanged, this, &InternetStatus::onStatusChanged);
}

void InternetStatus::checkInternetConnection()
{
    qDebug() << "InternetStatus::checkInternetConnection: checking internet conection...";

    setStatus(Status::InterentAvailable);
    return;
    QNetworkRequest req{QUrl("https://www.google.com/")};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    reply_ = manager_->get(req);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        if (reply_->error() == QNetworkReply::NoError)
            setStatus(Status::InterentAvailable);
        else if (reply_->error() == QNetworkReply::ConnectionRefusedError)
            setStatus(Status::InternetNotAvailable);
        else
            setStatus(Status::Unknown);

        reply_->deleteLater();
    });
}

void InternetStatus::onStatusChanged()
{
    if (status_ == Status::InternetNotAvailable)
        setStatusString("No hay conexión a internet disponible.");
    else if (status_ == Status::InterentAvailable)
        setStatusString("Hay conexión a internet disponible.");
    else
        setStatusString("El estado de la conexión a internet es desconocido.");
}

QString InternetStatus::statusString() const
{
    return statusString_;
}

void InternetStatus::setStatusString(const QString &newStatusString)
{
    if (statusString_ == newStatusString)
        return;
    statusString_ = newStatusString;
    emit statusStringChanged();
}

InternetStatus::Status InternetStatus::status() const
{
    return status_;
}

void InternetStatus::setStatus(const Status &newStatus)
{
    if (status_ == newStatus)
        return;
    status_ = newStatus;
    emit statusChanged();
}
