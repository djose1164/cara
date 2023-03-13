
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
    , timer_{std::make_unique<QTimer>(this)}
{
    timer_->start(840'000);
    connect(timer_.get(), &QTimer::timeout, this, &Rest::refreshAccessToken);
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
        reply_ = manager_.post(req, doc);
        break;
    }
    case Verb::PATCH: {
        auto doc{QJsonDocument::fromVariant(obj).toJson(QJsonDocument::Compact)};
        reply_ = manager_.sendCustomRequest(req, "PATCH", doc);
        break;
    }
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
    qInfo() << "Rest::patch";
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

    reply_->deleteLater();
    emit failed(*buffer_);

    if (error == QNetworkReply::AuthenticationRequiredError)
        emit unauthorized();
    else if (error == QNetworkReply::ConnectionRefusedError)
        emit serviceUnavailable();
}

void Rest::refreshAccessToken()
{
    if (refreshToken_.isEmpty())
        return;

    qsizetype idx{};
    if (base_.indexOf("/api") == -1)
        return;

    QString str{"/api"};
    idx = base_.indexOf(str);
    auto base{base_.sliced(0, idx + str.length())};
    const auto url{base + "/users/refresh/"};

    QNetworkRequest req{QUrl{url}};
    qDebug() << "Rest: url " << url;

    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", refreshToken_.toUtf8());
    auto reply{manager_.get(req)};
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const auto data{reply->readAll()};
        QVariantMap token{QJsonDocument::fromJson(data).toVariant().toMap()};
        accessToken("Bearer " + token["access_token"].toString());

        reply->deleteLater();
    });
}
