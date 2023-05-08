#ifndef PRODUCTENTITY_H
#define PRODUCTENTITY_H

#include <QObject>
#include <QtQml>

namespace EntityLayer {
class ProductEntity : public QObject
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

    QString name() const;
    QString description() const;
    QString imageUrl() const;

    int price() const;
    int productId() const;

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

    void setPrice(int price);
    void setProductId(int productId);

private:
    QString name_;
    QString description_;
    int price_;
    int productId_;

    QString imageUrl_;
};
}

#endif // PRODUCTENTITY_H
