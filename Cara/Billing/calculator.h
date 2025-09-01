#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QProperty>
#include <QQmlEngine>

class QAbstractItemModel;
namespace Billing
{

class Calculator : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double total READ total BINDABLE bindableTotal FINAL)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged FINAL REQUIRED)
    QML_ANONYMOUS
public:
    explicit Calculator(QObject *parent = nullptr);
    virtual ~Calculator() override;

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *newModel);

    double total() const;
    QBindable<double> bindableTotal();

    virtual void calculate() = 0;

protected:
    void setTotal(double newTotal);

signals:
    void modelChanged();

private:
    QAbstractItemModel *model_;
    QProperty<double> total_;
};

} // namespace Billing

#endif // CALCULATOR_H
