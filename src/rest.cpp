/**
 * @author: Daniel Victoriano
 * @title: Rest module
 * @brief Handle any request to RESTful APIs.
 * @copyright Cara 2023
 */

#include "rest.h"
#include <QDebug>
#include <QJsonDocument>

Rest::Rest(QObject *parent)
    : QObject{parent}
    , base_{}
    , code_{}
    , message_{}
    , manager_{}
    , reply_{nullptr}
    , buffer_{nullptr}
{

}

Rest::~Rest()
{
}

QString Rest::base() const
{
    return base_;
}

void Rest::base(const QString &base)
{
    if (base == base_)
        return;

    base_ = base;
    emit baseChanged();
}

QString Rest::accessToken() const
{
    return accessToken_;
}

void Rest::accessToken(const QString &token)
{
    if (accessToken_ == token)
        return;

    accessToken_ = token;
    emit accessTokenChanged();
}

QString Rest::refreshToken() const
{
    return refreshToken_;
}

void Rest::refreshToken(const QString &token)
{
    refreshToken_ = token;
}

const QVariantMap &Rest::headers() const
{
    return headers_;
}

void Rest::setHeaders(const QVariantMap &headers)
{
    if (headers == headers_)
        return;

    headers_ = headers;
    emit headersChanged();
}

void Rest::request(Rest::Verb verb, const QString &endpoint, const QVariant &obj)
{
    auto fullpath = base_ + endpoint;
    qInfo() << "Rest: url " << fullpath;

    QNetworkRequest req{QUrl{fullpath}};
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if (!accessToken_.isEmpty())
        req.setRawHeader("Authorization", accessToken_.toUtf8());
    else if (!refreshToken_.isEmpty())
        req.setRawHeader("Authorization", refreshToken_.toUtf8());

    if (!headers_.isEmpty())
    {
        foreach (auto &key, headers_.keys())
            req.setRawHeader(key.toUtf8(), headers()[key].toByteArray());
    }

    switch (verb)
    {
    case Verb::GET:
        reply_ = manager_.get(req);
        break;

    case Verb::POST: {
        auto doc{QJsonDocument::fromVariant(obj).toJson(QJsonDocument::Compact)};
        qDebug() << "doc: " << doc.toStdString();
        reply_ = manager_.post(req, doc);
        break;
    }
    case Verb::PATCH:
        break;

    case Verb::PUT:
        break;
    }

    connect(reply_, &QNetworkReply::requestSent, this, &Rest::sent);
    connect(reply_, &QNetworkReply::finished, this,  &Rest::replyFinished);
    connect(reply_, &QNetworkReply::errorOccurred, this, &Rest::errorOccurred);
}

void Rest::get(const QString &endpoint)
{
    request(Verb::GET, endpoint, QVariant());
}

void Rest::post(const QString &endpoint, const QVariantMap &obj)
{
    request(Verb::POST, endpoint, obj);
}

void Rest::put(const QString &endpoint, const QVariantMap &obj)
{
    request(Verb::PUT, endpoint, obj);
}

void Rest::patch(const QString &endpoint, const QVariantMap &obj)
{
    request(Verb::PATCH, endpoint, obj);
}

void Rest::replyFinished()
{
    auto reply{reply_->readAll()};
    const auto doc{QJsonDocument::fromJson(reply)};
    buffer_ = std::make_unique<QVariantMap>(doc.toVariant().toMap());
    emit received(*buffer_);

    if (reply_->error() == QNetworkReply::NoError)
        emit succeeded(*buffer_);

    reply_->deleteLater();
}

void Rest::errorOccurred(QNetworkReply::NetworkError error)
{
    qWarning() << "Rest: " << reply_->errorString();
    buffer_ = std::make_unique<QVariantMap>();
    buffer_->insert("code", error);
    buffer_->insert("message", reply_->errorString());

    emit failed(*buffer_);

    if (error == QNetworkReply::AuthenticationRequiredError)
        emit unauthorized();

    reply_->deleteLater();
}
