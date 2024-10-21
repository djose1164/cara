#include "calculator.h"

#include <QAbstractListModel>

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

QAbstractListModel *Calculator::model() const
{
    return model_;
}

void Calculator::setModel(QAbstractListModel *newModel)
{
    if (model_ == newModel)
        return;
    if (model_)
    {
        disconnect(model_, &QAbstractListModel::dataChanged, this, &Calculator::calculate);
        disconnect(model_, &QAbstractListModel::rowsInserted, this, &Calculator::calculate);
        disconnect(model_, &QAbstractListModel::rowsRemoved, this, &Calculator::calculate);
    }

    model_ = newModel;

    if (model_)
    {
        connect(model_, &QAbstractListModel::dataChanged, this, &Calculator::calculate);
        connect(model_, &QAbstractListModel::rowsInserted, this, &Calculator::calculate);
        connect(model_, &QAbstractListModel::rowsRemoved, this, &Calculator::calculate);
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
