#include "salespersoncontroller.h"
#include <QJsonDocument>
#include <QJsonObject>

namespace Controller
{
SalespersonController::SalespersonController(QObject *parent)
    : QObject{parent}
    , repository_{new Data::SalespersonRepositoryImpl(this)}
{}

void SalespersonController::getSalespersonById(int id)
{
    Q_ASSERT(id > 0);
    qDebug() << "Fetching salesperson with id " << id;
    repository_->getSalespersonById(id);
    connect(repository_, &Data::SalespersonRepositoryImpl::fetched, this, [&, id]{
        Q_ASSERT(repository_->buffer().contains("salesperson"));
        const auto resp{QJsonDocument::fromJson(repository_->buffer()).object()};
        const auto sp{Entity::Salesperson::fromJson(
            QJsonDocument(resp["salesperson"].toObject()).toJson()
        )};

        qDebug() << "Fetched salesperson with id " << id;
        emit fetched(sp);
    });
}
}
