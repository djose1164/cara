#ifndef INVENTORYREPOSITORY_H
#define INVENTORYREPOSITORY_H

#include "iinventoryrepository.h"

#include <QQmlEngine>

namespace Data
{
class InventoryRepository : public Business::IInventoryRepository
{
    Q_OBJECT
    Q_PROPERTY(QByteArray data READ data BINDABLE bindableData FINAL)
    QML_ELEMENT
    QML_IMPLEMENTS_INTERFACES(Business::IInventoryRepository)
public:
    explicit InventoryRepository(QObject *parent = nullptr);

    QByteArray data() const;
    void setData(const QByteArray &newData);
    QBindable<QByteArray> bindableData() const;

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

public slots:
    void fetchAll(SalespersonId spId) override;

    void update(const QVariantMap &stocks) override;

    void fetchAnyByProductId(ProductId productId) override;

signals:

private:
    QList<EntityLayer::Product> mapJsonToProducts(const QJsonArray &array) const;

private:
    QNetworkAccessManager *manager_;
    QPointer<QNetworkReply> reply_;
    QProperty<QByteArray> data_;
    QString errorString_;
    QString path_;
    const std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // INVENTORYREPOSITORY_H
