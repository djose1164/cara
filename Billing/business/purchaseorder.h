#ifndef PURCHASEORDER_H
#define PURCHASEORDER_H

#include "abstractresource.h"
#include <QQmlEngine>

namespace Billing
{
class PurchaseOrder : public AbstractResource
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString path MEMBER path_ FINAL REQUIRED)
public:
    explicit PurchaseOrder(QObject *parent = nullptr);

public slots:
    void placeOrder(const QVariantMap &data);
    void fetchOrders();

signals:
    void orderPlaced(const QByteArray &data);
    void ordersFetched(const QByteArray &data);
    void errorOccurred(const QString &errorString);

private:
    QString path_;
};
}

#endif // PURCHASEORDER_H
