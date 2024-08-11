#ifndef PRICEHISTORYMODEL_H
#define PRICEHISTORYMODEL_H

#include <QAbstractListModel>
#include <QProperty>
#include <QQmlEngine>
#include "pricehistoryrepository.h"

namespace Product
{

class PriceHistoryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString errorString READ default BINDABLE bindableErrorString FINAL)
    Q_PROPERTY(Product::PriceHistoryRepository *repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)
    QML_ELEMENT
public:
    using ProductId = int;

    enum class RoleNames
    {
        ProductIdRole = Qt::UserRole,
        PriceRole,
        PriceTypeRole,
        StartDateRole,
        ThruDateRole
    };

    Q_ENUM(Product::PriceHistoryRepository::Error)

    explicit PriceHistoryModel(QObject *parent = nullptr);

    QBindable<QString> bindableErrorString();

    PriceHistoryRepository *repository() const;
    void setRepository(PriceHistoryRepository *newRepository);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void addNewPrice(const QVariantMap &newPrice) noexcept;

    void fetchPriceHistoryBy(ProductId productId);

private slots:
    void newPriceAdded(const QByteArray &data);

    void priceHistoryFetched(const QByteArray &data);

private:
    void set(const QJsonArray &priceHistory);
    void append(const QJsonObject &ph);

signals:
    void errorOccurred(PriceHistoryRepository::Error error, const QString &errorString);

    void repositoryChanged();

private:
    PriceHistoryRepository *repository_;
    QProperty<QString> errorString_;
    QList<QJsonObject> data_;
};
}

#endif // PRICEHISTORYMODEL_H
