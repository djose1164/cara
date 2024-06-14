#include "tokenstore.h"
#include "Network/networkrequestsingleton.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

using namespace Qt::StringLiterals;

namespace Authentication::Data
{
TokenStore::TokenStore(QObject *parent)
    : Store::IStore(parent)
    , refresh_{}
    , access_{}
{}

bool TokenStore::save(const QByteArray &data)
{
    qDebug() << "TokenStore::save: saving...";
    constexpr auto refresh_token{"refresh_token"_L1};
    constexpr auto access_token{"access_token"_L1};
    if (!data.contains(refresh_token) && !data.contains(access_token))
    {
        qWarning() << "TokenStore::save: none token to save";
        return false;
    }

    QSettings settings;
    settings.beginGroup("Credentials"_L1);

    const auto obj{QJsonDocument::fromJson(data).object()};
    if (const auto refreshValue{obj[refresh_token]};
        refreshValue.isString())
    {
        setRefresh(refreshValue.toString());
        settings.setValue("refreshToken", refresh());
    }

    if (const auto accessValue{obj[access_token]};
        accessValue.isString())
    {
        setAccess(accessValue.toString());
        settings.setValue("accessToken"_L1, access_);
    }
    settings.endGroup();

    // TODO: I think this should be deleted.
    Q_ASSERT(!access_.isEmpty());
    const auto api{Network::NetworkRequestSingleton::instance().api()};
    api->setBearerToken(access_.toUtf8());
    return true;
}

void TokenStore::restore()
{
    QSettings settings;
    settings.beginGroup("Credentials"_L1);
    setAccess(settings.value("accessToken"_L1).toString());
    setRefresh(settings.value("refreshToken"_L1).toString());
    settings.endGroup();
}

void TokenStore::clear()
{
    QSettings settings;
    settings.remove("Credentials");

    access_.clear();
    emit accessChanged();
    refresh().clear();
}


QString TokenStore::refresh() const
{
    return refresh_;
}

void TokenStore::setRefresh(const QString &newRefresh)
{
    refresh_ = newRefresh;
}

QBindable<QString> TokenStore::bindableRefreshToken() const
{
    return &refresh_;
}

QString TokenStore::access() const
{
    return access_;
}

void TokenStore::setAccess(const QString &newAccess)
{
    if (access_ == newAccess)
        return;
    access_ = newAccess;
    emit accessChanged();
}
}
