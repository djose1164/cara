#include "productcategoryrepository.h"
#include "constants.h"

#include <QQmlEngine>

namespace Catalogue {
ProductCategoryRepository::ProductCategoryRepository(QObject *parent)
    : QObject{parent}
    , manager_{new QNetworkAccessManager(this)}
    , reply_{nullptr}
{

}

void ProductCategoryRepository::fetchAll()
{
    QQmlEngine engine;
    const auto instance{engine.singletonInstance<DataLayer::Constants *>("Cara.Data", "Constants")};
    const QString base{instance->base()};
    const QUrl url{base + "/products/categories/"};

    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    reply_ = manager_->get(request);

    connect(reply_, &QNetworkReply::finished, this, [&] {
        const auto data{reply_->readAll()};
        emit allFetched(data);

        reply_->deleteLater();
    });
}

}
