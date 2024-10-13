#ifndef PROFITCALCULATOR_H
#define PROFITCALCULATOR_H

#include <QQmlEngine>
#include "ProductInventory/productmodel.h"
#include "calculator.h"

namespace EntityLayer
{
class Product;
}
namespace Catalogue
{
class CatalogueRepository;
}

namespace Billing
{

class ProfitCalculator : public Calculator
{
    Q_OBJECT
    Q_PROPERTY(int percentage READ percentage WRITE setPercentage NOTIFY percentageChanged FINAL)
    Q_PROPERTY(double grossTotal READ grossTotal BINDABLE bindableGrossTotal FINAL)
    QML_ELEMENT
public:
    explicit ProfitCalculator(QObject *parent = nullptr);

    int percentage() const;
    void setPercentage(int newPercentage);

    double grossTotal() const;
    void setGrossTotal(double newGrossTotal);
    QBindable<double> bindableGrossTotal();

    void calculate() override;

signals:
    void percentageChanged();

    void selectorModelChanged();

private:
    int percentage_;
    QPropertyNotifier supplierIdHandler_;
    QProperty<double> grossTotal_;
};

} // namespace Billing

#endif // PROFITCALCULATOR_H
