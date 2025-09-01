#include "profitcalculator.h"

using namespace Qt::StringLiterals;

namespace Billing
{

ProfitCalculator::ProfitCalculator(QObject *parent)
    : Calculator{parent}
    , percentage_{1}
    , grossTotal_{}
{
    connect(this, &ProfitCalculator::percentageChanged, this, &ProfitCalculator::calculate);
}

void ProfitCalculator::calculate()
{
    if (!model() or model()->rowCount() < 1 or percentage_ < 1)
        return;
    const double calcPercentage{percentage_ / 100.};
    double calcTotal{};
    for (int i{}; i < model()->rowCount(); ++i)
    {
        using enum ProductInventory::ProductModel::RoleNames;
        const QModelIndex idx = model()->index(i, 0);
        const double sellPrice = model()->data(idx, int(PriceRole)).toDouble();
        const double buyPrice = model()->data(idx, int(BuyPriceRole)).toDouble();
        const int qty = model()->data(idx, int(SelectedQtyRole)).toInt();

        calcTotal += ((sellPrice - buyPrice) * qty);
    }

    setGrossTotal(calcTotal);
    setTotal(grossTotal_ * calcPercentage);
}

double ProfitCalculator::grossTotal() const
{
    return grossTotal_;
}

void ProfitCalculator::setGrossTotal(double newGrossTotal)
{
    grossTotal_ = newGrossTotal;
}

QBindable<double> ProfitCalculator::bindableGrossTotal()
{
    return &grossTotal_;
}

int ProfitCalculator::percentage() const
{
    return percentage_;
}

void ProfitCalculator::setPercentage(int newPercentage)
{
    if (percentage_ == newPercentage)
        return;
    percentage_ = newPercentage;
    emit percentageChanged();
}

} // namespace Billing
