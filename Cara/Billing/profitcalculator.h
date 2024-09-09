#ifndef PROFITCALCULATOR_H
#define PROFITCALCULATOR_H

#include <QQmlEngine>
#include "calculator.h"

namespace EntityLayer
{
class Product;
}
namespace Catalogue
{
class CatalogueRepository;
}
namespace Model
{
class ProductModel;
}
namespace Billing
{

class ProfitCalculator : public Calculator
{
    Q_OBJECT
    Q_PROPERTY(int supplierId READ supplierId WRITE setSupplierId BINDABLE bindableSupplierId FINAL)
    Q_PROPERTY(int percentage READ percentage WRITE setPercentage NOTIFY percentageChanged FINAL)
    Q_PROPERTY(Model::ProductModel *selectorModel READ selectorModel WRITE setSelectorModel NOTIFY selectorModelChanged FINAL)
    Q_PROPERTY(double grossTotal READ grossTotal BINDABLE bindableGrossTotal FINAL)
    QML_ELEMENT
public:
    explicit ProfitCalculator(QObject *parent = nullptr);
    ~ProfitCalculator() override;

    // Calculator interface
    int supplierId() const;
    void setSupplierId(int newSupplierId);
    QBindable<int> bindableSupplierId();

    int percentage() const;
    void setPercentage(int newPercentage);

    QList<EntityLayer::Product *> productList() const;
    void setProductList(const QList<EntityLayer::Product *> &newProductList);

    Model::ProductModel *selectorModel() const;
    void setSelectorModel(Model::ProductModel *newSelectorModel);

    double grossTotal() const;
    void setGrossTotal(double newGrossTotal);
    QBindable<double> bindableGrossTotal();

    void calculate() override;

private slots:
    void updateProductList();
    void takeBuyPriceFromProductList(int posInModel);
    void setBuyPriceInSelectorModel();

signals:
    void percentageChanged();

    void selectorModelChanged();

private:
    std::optional<EntityLayer::Product *> findByProductId(int productId);
    void removeByProductId(int productId);
    std::optional<double> getBuyPriceByProductId(int productId);

    Catalogue::CatalogueRepository *catalogueRepository_;
    QProperty<int> supplierId_;
    int percentage_;
    QList<EntityLayer::Product *> productList_;
    QPropertyNotifier supplierIdHandler_;
    Model::ProductModel *selectorModel_;
    QProperty<double> grossTotal_;
};

} // namespace Billing

#endif // PROFITCALCULATOR_H
