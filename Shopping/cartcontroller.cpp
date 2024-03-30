#include "cartcontroller.h"
#include "networkrequestsingleton.h"
#include <QSettings>

using namespace Qt::StringLiterals;

static constexpr auto path{"shopping"_L1};

namespace Shopping {
CartController::CartController(QObject *parent)
    : QObject{parent}
    , api_{Utils::NetworkRequestSingleton::instance().api()}
    , manager_{std::make_shared<QRestAccessManager>(new QNetworkAccessManager{this})}
    , order_{new EntityLayer::OrderEntity(this)}
    , model_{new Model::OrderModel(this)}
    , totalToPay_{}
{
    // Keep totalToPay updated in the view
    connect(model_, &Model::OrderModel::dataChanged, this, [this](const QModelIndex &, const QModelIndex &, const QList<int> &roles) {
        if (roles.indexOf(int(Model::OrderModel::RoleNames::TotalRole)) != -1)
            emit totalToPayChanged();
    });
}

EntityLayer::OrderEntity *CartController::order() const
{
    return order_;
}

Model::OrderModel *CartController::model() const
{
    return model_;
}

bool CartController::isProductInCart(const EntityLayer::Product &product) const
{
    // TODO: This' being repetive. Consider to create a find method in model_.
    const auto predicate = [&](EntityLayer::DetailsEntity *const d) -> bool {
        return product == d->product();
    };

    return [&]() -> bool {
        // TODO: Consider to add a custom `foreach` for traverse model.
        for (int i{}; i < model_->count(); ++i)
            if (predicate(model_->get(i)))
                return true;
        return false;
    }();
}

int CartController::totalToPay() const
{
    int sum{};
    for (int i{}; i < model_->count(); ++i)
        sum += model_->get(i)->total();

    return sum;
}

void CartController::setOrder(EntityLayer::OrderEntity * const order)
{
    if (order_ != order)
    {
        order_ = order;
        emit orderChanged();
    }
}

void CartController::setModel(Model::OrderModel * const model)
{
    if (model_ != model)
    {
        model_ = model;
        emit modelChanged();
    }
}

void CartController::addtoCart(const EntityLayer::Product &product)
{
    auto detail{new EntityLayer::DetailsEntity(model_)};
    detail->setProduct(product);

    model_->append(detail);
}

void CartController::removeFromCart(const int index)
{
    model_->remove(index);
}

void CartController::checkout(const Entity::Customer &buyer)
{
    qDebug() << "Checking out...";

    prepareOrder(buyer);

    const auto order{
        QJsonDocument::fromJson(order_->toJson())
            .toVariant()
            .toMap()
    };

    qDebug() << "CartController::checkout: " << order;

    manager_->post(api_->createRequest(path), order, this, [this](QRestReply &reply) {
        if  (reply.isSuccess())
        {
            emit checkoutSucceeded();
            return;
        }

        // We failed.
        if (const auto resp{reply.readJson()})
            emit checkoutFailed(resp->object().toVariantMap());
        else
            emit checkoutFailed(QVariantMap{});
    });
}

void CartController::prepareOrder(const Entity::Customer &buyer)
{
    // TODO: For later, the payment process with credit/debit card must taken
    // into account.
    order_->setCustomer(buyer);

    order_->setDetails(model_->toJson());
}
}
