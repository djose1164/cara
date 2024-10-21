#include "costcalculator.h"

namespace Business
{
CostCalculator::CostCalculator(QObject *parent)
    : QObject{parent}
    , model_{}
    , total_{}
{
}

ProductInventory::ProductModel *CostCalculator::model() const
{
    return model_;
}

void CostCalculator::setModel(ProductInventory::ProductModel *newModel)
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
    for (int i{}; i < model_->count(); ++i)
    {
        const auto product{model_->get(i)};
        totalPrice += product.price() * product.selectedQty();
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
