#ifndef COSTCALCULATOR_H
#define COSTCALCULATOR_H

#include <QAbstractItemModel>
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
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged FINAL REQUIRED)
    Q_PROPERTY(int total READ total WRITE setTotal NOTIFY totalChanged FINAL)
public:
    explicit CostCalculator(QObject *parent = nullptr);

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *newModel);

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
    QAbstractItemModel *model_;
    int total_;
};
}

#endif // COSTCALCULATOR_H
