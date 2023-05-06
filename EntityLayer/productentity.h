#ifndef PRODUCTENTITY_H
#define PRODUCTENTITY_H

#include "entity.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer {
class ProductEntity : public QObject, public Entity
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString name MEMBER name_ NOTIFY nameChanged)
    Q_PROPERTY(QString description MEMBER description_ NOTIFY descriptionChanged)
    Q_PROPERTY(int price MEMBER price_ NOTIFY priceChanged)
    Q_PROPERTY(int productId MEMBER productId_ NOTIFY productIdChanged)
    Q_PROPERTY(QString imageUrl MEMBER imageUrl_ NOTIFY imageUrlChanged)
    Q_PROPERTY(int quantity MEMBER quantity_ NOTIFY quantityChanged)

public:
    explicit ProductEntity(QObject *parent = nullptr);

    Q_INVOKABLE QVariantMap toMap() const override;

signals:
    void nameChanged();
    void descriptionChanged();
    void priceChanged();
    void productIdChanged();
    void imageUrlChanged();
    void quantityChanged();

private:
    QString name_;
    QString description_;
    int price_;
    int quantity_;
    int productId_;

    QString imageUrl_;
};
}

#endif // PRODUCTENTITY_H
