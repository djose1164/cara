#include "selectionproxymodel.h"
#include "productmodel.h"

#include <QJsonArray>

using namespace Qt::StringLiterals;

namespace ProductInventory
{

SelectionProxyModel::SelectionProxyModel(QObject *parent) noexcept
    : QSortFilterProxyModel{parent}
    , selectionModel_{}
{
}

QItemSelectionModel *SelectionProxyModel::selectionModel() const noexcept
{
    return selectionModel_;
}

void SelectionProxyModel::setSelectionModel(QItemSelectionModel *newSelectionModel) noexcept
{
    if (selectionModel_ == newSelectionModel)
        return;

    if (selectionModel_)
        selectionModel_->disconnect(this);

    beginFilterChange();
    selectionModel_ = newSelectionModel;

    if (selectionModel_)
    {
        connect(selectionModel_,
                &QItemSelectionModel::selectionChanged,
                this,
                &SelectionProxyModel::invalidateRowsFilter);
        connect(selectionModel_,
                &QItemSelectionModel::selectionChanged,
                this,
                &SelectionProxyModel::countChanged);
    }
    invalidateRowsFilter();

    emit selectionModelChanged();
}

int SelectionProxyModel::count() const noexcept
{
    return sourceModel() ? selectionModel_->selectedIndexes().count() : 0;
}

QJsonArray SelectionProxyModel::toJsonArray() const
{
    const auto accesor = [this](const QModelIndex &productIdx, int role) {
        return sourceModel()->data(productIdx, role);
    };

    QJsonArray array;
    std::ranges::for_each(selectionModel_->selectedIndexes(), [&](const auto &productIdx) {
        using enum ProductModel::RoleNames;
        QJsonObject json;
        json["product_id"_L1] = accesor(productIdx, int(ProductIdRole)).toInt();
        json["quantity"_L1] = accesor(productIdx, int(SelectedQtyRole)).toInt();
        array.append(std::move(json));
    });
    return array;
}

bool SelectionProxyModel::filterAcceptsRow(int source_row,
                                                             const QModelIndex &source_parent) const
{
    if (not selectionModel_ || not sourceModel())
        return false;

    auto currentIndex{sourceModel()->index(source_row, 0, source_parent)};
    return selectionModel_->isSelected(currentIndex);
}

} // namespace ProductInventory
