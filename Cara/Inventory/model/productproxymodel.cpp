#include "productproxymodel.h"
#include "productmodel.h"

namespace Model
{
ProductProxyModel::ProductProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
    , productName_{}
{}

QString ProductProxyModel::productName() const
{
    return productName_;
}

void ProductProxyModel::setProductName(const QString &newProductName)
{
    if (productName_ == newProductName)
        return;
    productName_ = newProductName;
    emit productNameChanged();
    invalidateFilter();
}

bool ProductProxyModel::filterAcceptsRow(int source_row,
                                         const QModelIndex &source_parent) const
{
    const QModelIndex index{sourceModel()->index(source_row, 0, source_parent)};
    const QString name{
        index.data(int(ProductModel::RoleNames::ProductNameRole)).toString()
    };
    return productName_.isEmpty()
           || name.contains(productName_, Qt::CaseInsensitive);
}
}
