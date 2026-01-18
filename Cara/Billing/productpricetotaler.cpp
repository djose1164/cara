#include "productpricetotaler.h"
#include "productmodel.h"

namespace Business
{
ProductPriceTotaler::ProductPriceTotaler(QObject *parent)
    : QObject{parent}
    , model_{}
    , total_{}
    , role_{Role::Invalid}
{
}

QAbstractItemModel *ProductPriceTotaler::model() const
{
    return model_;
}

void ProductPriceTotaler::setModel(QAbstractItemModel *newModel)
{
    if (model_)
        model_->disconnect(this);
    if (model_ == newModel)
        return;

    model_ = newModel;

    connect(model_, &ProductInventory::ProductModel::rowsInserted,
            this, &ProductPriceTotaler::calculateTotal);
    connect(model_, &ProductInventory::ProductModel::dataChanged,
            this, &ProductPriceTotaler::calculateTotal);
    emit modelChanged();
}

int ProductPriceTotaler::total() const
{
    return total_;
}

void ProductPriceTotaler::setTotal(int newTotal)
{
    if (total_ == newTotal)
        return;
    total_ = newTotal;
    emit totalChanged();
}

void ProductPriceTotaler::calculateTotal()
{
    if (not model_ or role_ == Role::Invalid)
    {
        qWarning() << "ProductPriceTotaler::calculateTotal: invalid model or role.";
        return;
    }

    int totalPrice{};
    for (int i{}; i < model_->rowCount(); ++i)
    {
        using enum ProductInventory::ProductModel::RoleNames;
        auto selectedRole{role_ == Role::BuyPriceRole ? BuyPriceRole : PriceRole};
        auto idx{model_->index(i, 0)};
        const auto price{model_->data(idx, int(selectedRole)).toInt()};
        const auto qty{model_->data(idx, int(SelectedQtyRole)).toInt()};
        totalPrice += price * qty;
    }
    setTotal(totalPrice);
}

ProductPriceTotaler::Role ProductPriceTotaler::role() const
{
    return role_;
}

void ProductPriceTotaler::setRole(Role newRole)
{
    if (role_ == newRole)
        return;
    role_ = newRole;
    emit roleChanged();
}

void ProductPriceTotaler::classBegin()
{
}

void ProductPriceTotaler::componentComplete()
{

}
}
