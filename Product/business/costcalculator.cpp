#include "costcalculator.h"

namespace Business
{
CostCalculator::CostCalculator(QObject *parent)
    : QObject{parent}
    , model_{}
    , total_{}
{
    connect(this, &CostCalculator::modelChanged, this, [&] {
        qDebug() << "CostCalculator::CostCalculator: connecting dataChanged with calculateTotal";
        if (model_ == nullptr)
            return;

        connect(model_, &Model::ProductModel::countChanged,
                this, &CostCalculator::calculateTotal);
        connect(model_, &Model::ProductModel::dataChanged,
                this, &CostCalculator::calculateTotal);
    });
}

Model::ProductModel *CostCalculator::model() const
{
    return model_;
}

void CostCalculator::setModel(Model::ProductModel *newModel)
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
        const auto product{
            qobject_cast<EntityLayer::Product *>(model_->get(i))
        };
        totalPrice += product->price() * product->selectedQty();
    }
    setTotal(totalPrice);
}
}
