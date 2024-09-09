#ifndef SALEMODEL_H
#define SALEMODEL_H

#include <QAbstractListModel>
#include <QQmlEngine>
#include "Network/pagination.h"
#include "sale.h"

namespace Network
{
class Pagination;
}
namespace Billing
{

class SaleRepository;

class SaleModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(SaleRepository *repository READ repository WRITE setRepository NOTIFY repositoryChanged FINAL)
    QML_ELEMENT
public:
    enum class RoleNames
    {
        SaleIdRole = Qt::UserRole,
        OrderIdRole,
        CustomerIdRole,
        SaleItemsRole,
        TotalAmountRole,
        CreatedAtRole
    };
    Q_ENUM(RoleNames)

    explicit SaleModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QHash<int, QByteArray> roleNames() const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    static void validateSale(const QVariantMap &sale);

    SaleRepository *repository() const;
    void setRepository(SaleRepository *newRepository);

    Network::Pagination pagination() const;
    void setPagination(const Network::Pagination &newPagination);

public slots:
    void append(const QVariantMap &sale);
    void fetchSales();

private slots:
    void salesFetched(const QList<Sale> &sales, const Network::Pagination &pagination);
    void saleAdded(const QJsonObject &json);

signals:
    void errorOccurred(const QString &errorString);

    void repositoryChanged();

private:
    QList<Sale> data_;
    SaleRepository *repository_;
    int totalPages_;
    Network::Pagination pagination_;
    bool isFetching_;
};

} // namespace Billing

#endif // SALEMODEL_H
