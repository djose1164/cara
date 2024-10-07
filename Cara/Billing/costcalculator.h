#ifndef COSTCALCULATOR_H
#define COSTCALCULATOR_H

#include "ProductInventory/productmodel.h"
#include <QObject>
#include <QQmlEngine>
#include <QQmlParserStatus>

namespace Business
{
class CostCalculator : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    QML_ELEMENT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(ProductInventory::ProductModel *model READ model WRITE setModel NOTIFY modelChanged FINAL REQUIRED)
    Q_PROPERTY(int total READ total WRITE setTotal NOTIFY totalChanged FINAL)
public:
    explicit CostCalculator(QObject *parent = nullptr);

    ProductInventory::ProductModel *model() const;
    void setModel(ProductInventory::ProductModel *newModel);

    int total() const;
    void setTotal(int newTotal);

    // QQmlParserStatus interface
    void classBegin() override;
    void componentComplete() override;

private slots:
    void calculateTotal();

signals:
    void modelChanged();

    void totalChanged();

private:
    ProductInventory::ProductModel *model_;
    int total_;
};
}

#endif // COSTCALCULATOR_H
