#ifndef PRODUCTENTITY_H
#define PRODUCTENTITY_H

#include "jsonexportable.h"
#include <QObject>
#include <QtQml>

namespace EntityLayer {
class ProductEntity : public QObject, public JsonExportable
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description MEMBER description_ NOTIFY descriptionChanged)
    Q_PROPERTY(int price READ price WRITE setPrice NOTIFY priceChanged)
    Q_PROPERTY(int productId READ productId WRITE setProductId NOTIFY productIdChanged)
    Q_PROPERTY(QString imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)

public:
    explicit ProductEntity(QObject *parent = nullptr);

    static ProductEntity *fromMap(const QVariantMap &map);

    bool operator==(ProductEntity *const rhs);
    bool operator!=(ProductEntity *const rhs);

    QString name() const;
    QString description() const;
    QString imageUrl() const;

    void setPrice(int price);
    int price() const;
    QBindable<int> bindablePrice() const;

    int productId() const;

    Q_INVOKABLE QByteArray toJson() const override;

signals:
    void nameChanged();
    void descriptionChanged();
    void priceChanged();
    void productIdChanged();
    void imageUrlChanged();

public slots:
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setImageUrl(const QString &url);

    void setProductId(int productId);

private:
    QString name_;
    QString description_;
    QProperty<int> price_;
    int productId_;

    QString imageUrl_;
};
}

#endif // PRODUCTENTITY_H
