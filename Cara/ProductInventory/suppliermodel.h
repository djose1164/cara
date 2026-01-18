#ifndef SUPPLIERMODEL_H
#define SUPPLIERMODEL_H

#include "abstractsupplierrepository.h"
#include <QAbstractListModel>
#include <QQmlEngine>

namespace ProductInventory
{

class SupplierModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ProductInventory::AbstractSupplierRepository *repository
               READ repository
               WRITE setRepository
               NOTIFY repositoryChanged FINAL)
    QML_ELEMENT
public:
    enum class RoleNames
    {
        SupplierIdRole = Qt::UserRole + 1,
        NameRole,
        UniqueCodeRole
    };

    explicit SupplierModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    AbstractSupplierRepository *repository() const;
    void setRepository(AbstractSupplierRepository *newRepository);

public slots:
    void fetchSuppliers();

signals:
    void repositoryChanged();

private slots:
    void reset(const QList<Supplier> &suppliers);

private:
    QList<Supplier> data_;
    QPointer<AbstractSupplierRepository> repository_;
};

} // namespace ProductInventory

#endif // SUPPLIERMODEL_H
