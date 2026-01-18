#ifndef PRODUCTPRICETOTALER_H
#define PRODUCTPRICETOTALER_H

#include <QAbstractItemModel>
#include <QObject>
#include <QQmlEngine>
#include <QQmlParserStatus>

namespace Business
{
class ProductPriceTotaler : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged FINAL REQUIRED)
    Q_PROPERTY(ProductPriceTotaler::Role role READ role WRITE setRole NOTIFY roleChanged FINAL REQUIRED)
    Q_PROPERTY(int total READ total NOTIFY totalChanged FINAL)
    QML_ELEMENT
public:
    enum class Role
    {
        BuyPriceRole,
        SellPriceRole,
        Invalid
    };
    Q_ENUM(Role)

    explicit ProductPriceTotaler(QObject *parent = nullptr);

    QAbstractItemModel *model() const;
    void setModel(QAbstractItemModel *newModel);

    int total() const;
    void setTotal(int newTotal);

    // QQmlParserStatus interface
    void classBegin() override;
    void componentComplete() override;

    Role role() const;
    void setRole(Role newRole);

private slots:
    void calculateTotal();

signals:
    void modelChanged();

    void totalChanged();

    void roleChanged();

private:
    QPointer<QAbstractItemModel> model_;
    int total_;
    Role role_;
};
}

#endif // PRODUCTPRICETOTALER_H
