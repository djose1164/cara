#include "calculator.h"

#include <QAbstractItemModel>

namespace Billing
{

Calculator::Calculator(QObject *parent)
    : QObject{parent}
    , model_{}
    , total_{}
{
}

Calculator::~Calculator()
{
}

QAbstractItemModel *Calculator::model() const
{
    return model_;
}

void Calculator::setModel(QAbstractItemModel *newModel)
{
    if (model_ == newModel)
        return;
    if (model_)
    {
        disconnect(model_, &QAbstractItemModel::dataChanged, this, &Calculator::calculate);
        disconnect(model_, &QAbstractItemModel::rowsInserted, this, &Calculator::calculate);
        disconnect(model_, &QAbstractItemModel::rowsRemoved, this, &Calculator::calculate);
    }

    model_ = newModel;

    if (model_)
    {
        connect(model_, &QAbstractItemModel::dataChanged, this, &Calculator::calculate);
        connect(model_, &QAbstractItemModel::rowsInserted, this, &Calculator::calculate);
        connect(model_, &QAbstractItemModel::rowsRemoved, this, &Calculator::calculate);
    }
    emit modelChanged();
}

double Calculator::total() const
{
    return total_.value();
}

void Calculator::setTotal(double newTotal)
{
    total_ = newTotal;
}

QBindable<double> Calculator::bindableTotal()
{
    return &total_;
}

} // namespace Billing
