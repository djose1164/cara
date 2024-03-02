#ifndef ORDERRESOURCE_H
#define ORDERRESOURCE_H

#include <QObject>
#include <QQmlEngine>
#include "abstractresource.h"

namespace Billing
{
class OrderResource : public AbstractResource
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
public:
    using OrderId = int;

    explicit OrderResource(QObject *parent = nullptr);

    QString path() const;
    void setPath(const QString &newPath);

public slots:
    void getOrderById(OrderId orderId);

signals:
    void pathChanged();

    void errorOccurred(const QString &erroString);

    void fetched(const QByteArray &order);

private:
    QString path_;
};
}

#endif // ORDERRESOURCE_H
