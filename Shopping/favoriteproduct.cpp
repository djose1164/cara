#include "favoriteproduct.h"
#include "favorite.h"
#include <QRestAccessManager>
#include <QRestReply>
#include <QJsonDocument>

#define checkReply(reply)                            \
    do {                                             \
        if (!(reply).isSuccess())                    \
        {                                            \
            emit errorOccurred(reply.errorString()); \
            return;                                  \
        }                                            \
    } while (false)

using namespace Qt::StringLiterals;

static constexpr auto resourceId{"/%1"_L1};

namespace Shopping
{
FavoriteProduct::FavoriteProduct(QObject *parent)
    : AbstractResource{parent}
{}

QString FavoriteProduct::path() const
{
    return path_;
}

void FavoriteProduct::setPath(const QString &newPath)
{
    if (path_ == newPath)
        return;
    path_ = newPath;
    emit pathChanged();
}

void FavoriteProduct::add(const Favorite &favorite)
{
    if (!favorite.valid())
    {
        emit errorOccurred("Favorite item is invalid"_L1);
        return;
    }

    manager_->post(api_->createRequest(path_), favorite.toJson(),
                   this, &FavoriteProduct::handleFinished);
}

void FavoriteProduct::remove(const Favorite &favorite)
{
    if (!favorite.valid())
    {
        emit errorOccurred("Favorite item is invalid"_L1);
        return;
    }

    QString tmp{path_ + resourceId.arg(QString::number(favorite.favoriteId()))};
    auto req{api_->createRequest(std::move(tmp))};
    manager_->deleteResource(std::move(req), this, [this](QRestReply &reply) {
        checkReply(reply);

        emit resourceDeleted();
    });
}

void FavoriteProduct::readByCustomerId(UserId id)
{
    Q_UNUSED(id);
    manager_->get(
        api_->createRequest(path_),
        this, [this](QRestReply &reply) {
            checkReply(reply);

            if (const auto doc{reply.readJson()})
                emit resourceFetched(doc->toJson());
        }
    );
}

void FavoriteProduct::handleFinished(QRestReply &reply)
{
    if (!reply.isSuccess())
    {
        emit errorOccurred(reply.errorString());
        return;
    }

    if (const auto json{reply.readJson()})
        emit resourceCreated(json->toJson());
}
}
