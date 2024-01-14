#ifndef INVENTORYREPOSITORY_H
#define INVENTORYREPOSITORY_H

#include "iinventoryrepository.h"

#include <QQmlEngine>

namespace Data
{
class InventoryRepository : public Business::IInventoryRepository
{
    Q_OBJECT
    QML_ELEMENT
    QML_IMPLEMENTS_INTERFACES(Business::IInventoryRepository)
public:
    explicit InventoryRepository(QObject *parent = nullptr);

    QByteArray data() const;
    void setData(const QByteArray &newData);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

public slots:
    void fetchAll(SalespersonId spId) override;

    void update(const QVariantMap &stocks) override;

signals:

private:
    QNetworkRequest prepareRequest(const QString &endpoint);

private:
    QNetworkAccessManager *manager_;
    QPointer<QNetworkReply> reply_;
    QByteArray data_;
    QString errorString_;
};
}

#endif // INVENTORYREPOSITORY_H
