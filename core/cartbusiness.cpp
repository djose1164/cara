#include "cartbusiness.h"
#include "billentity.h"
#include "constants.h"

namespace BusinessLayer {
CartBusiness::CartBusiness(QObject *parent)
    : QObject{parent}
    , rest_{new Rest(this)}
    , user_{new EntityLayer::UserEntity(this)}
    , order_{new EntityLayer::OrderEntity(this)}
    , model_{new EntityLayer::OrderModel(this)}
    , totalToPay_{}
{
    const QString base{Constants::instance()->base()};
    rest_->base(base + "/orders");

    QSettings settings{};
    const auto token{settings.value("Credentials/refreshToken").toString()};

    rest_->refreshToken(token);
    rest_->refreshAccessToken();

    // Keep totalToPay updated in the view
    connect(model_, &EntityLayer::OrderModel::dataChanged, this, [this](const QModelIndex &, const QModelIndex &, const QList<int> &roles) {
        if (roles.indexOf(int(EntityLayer::OrderModel::RoleNames::TotalRole)) != -1)
            emit totalToPayChanged();
    });
}

EntityLayer::UserEntity *CartBusiness::user() const
{
    return user_;
}

EntityLayer::OrderEntity *CartBusiness::order() const
{
    return order_;
}

ModelLayer::OrderModel *CartBusiness::model() const
{
    return model_;
}

bool CartBusiness::isProductInCart(EntityLayer::Product * const product)
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

int CartBusiness::totalToPay() const
{
    int sum{};
    for (int i{}; i < model_->count(); ++i)
        sum += model_->get(i)->total();

    return sum;
}

void CartBusiness::setUser(EntityLayer::UserEntity *user)
{
    if (user_ != user)
    {
        user_ = user;
        emit userChanged();
    }
}

void CartBusiness::setOrder(EntityLayer::OrderEntity * const order)
{
    if (order_ != order)
    {
        order_ = order;
        emit orderChanged();
    }
}

void CartBusiness::setModel(ModelLayer::OrderModel * const model)
{
    if (model_ != model)
    {
        model_ = model;
        emit modelChanged();
    }
}

void CartBusiness::addtoCart(EntityLayer::Product *const product)
{
    auto detail{new EntityLayer::DetailsEntity(model_)};
    detail->setProduct(product);

    model_->append(detail);
}

void CartBusiness::removeFromCart(EntityLayer::DetailsEntity * const detail)
{
    const auto predicate = [&](EntityLayer::DetailsEntity *const d) -> bool {
        return detail->product() == d->product();
    };

    model_->remove([&]() -> int {
        int idx{-1};
        for (int i{}; i < model_->count(); ++i)
            if (predicate(model_->get(i)))
                idx = i;
        return idx;
    }());
}

void CartBusiness::checkout()
{
    qDebug() << "Checking out...";

    prepareOrder();

    const auto order{
        QJsonDocument::fromJson(order_->toJson())
            .toVariant()
            .toMap()
    };

    rest_->post("/", order);

    connect(rest_, &Rest::succeeded, this, [&](){
        emit checkoutSucceeded();
    });

    connect(rest_, &Rest::failed, this, [this](const QVariantMap &resp) {
        emit checkoutFailed(resp);
    });
}

void CartBusiness::prepareOrder()
{
    // TODO: For later, the payment process with credit/debit card must taken
    // into account.
    const auto user{order_->user()};
    user->setUserId(user_->userId());
    user->setCustomerId(user_->customerId());

    order_->setDetails(model_->toJson());
}
}
