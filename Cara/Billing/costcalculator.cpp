#include "costcalculator.h"
#include "productmodel.h"

namespace Business
{
CostCalculator::CostCalculator(QObject *parent)
    : QObject{parent}
    , model_{}
    , total_{}
{
}

QAbstractItemModel *CostCalculator::model() const
{
    return model_;
}

void CostCalculator::setModel(QAbstractItemModel *newModel)
{
    if (model_ == newModel)
        return;
    model_ = newModel;
    emit modelChanged();
}

int CostCalculator::total() const
{
    return total_;
}

void CostCalculator::setTotal(int newTotal)
{
    if (total_ == newTotal)
        return;
    total_ = newTotal;
    emit totalChanged();
}

void CostCalculator::calculateTotal()
{
    int totalPrice{};
    for (int i{}; i < model_->rowCount(); ++i)
    {
        using enum ProductInventory::ProductModel::RoleNames;
        auto idx{model_->index(i, 0)};
        const auto price{model_->data(idx, int(PriceRole)).toInt()};
        const auto qty{model_->data(idx, int(SelectedQtyRole)).toInt()};
        totalPrice += price * qty;
    }
    setTotal(totalPrice);
}

void CostCalculator::classBegin()
{
}

void CostCalculator::componentComplete()
{
    connect(model_, &ProductInventory::ProductModel::rowsInserted,
            this, &CostCalculator::calculateTotal);
    connect(model_, &ProductInventory::ProductModel::dataChanged,
            this, &CostCalculator::calculateTotal);
}
}
