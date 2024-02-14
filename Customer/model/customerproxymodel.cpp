#include "customerproxymodel.h"
#include "customermodel.h"

namespace Model
{
CustomerProxyModel::CustomerProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
    , text_{}
{
    sort(0);
}

QString CustomerProxyModel::text() const
{
    return text_;
}

void CustomerProxyModel::setText(const QString &newText)
{
    if (text_ == newText)
        return;
    text_ = newText;
    emit textChanged();
    invalidateFilter();
}
bool CustomerProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const auto index{sourceModel()->index(source_row, 0, source_parent)};
    const auto fullname{
        index.data(
            int(CustomerModel::RoleNames::FullnameRole)
        )
        .toString()
    };
    return fullname.isEmpty() || fullname.contains(text_, Qt::CaseInsensitive);
}

bool CustomerProxyModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    using enum CustomerModel::RoleNames;
    const QVariant leftData{sourceModel()->data(source_left, int(FullnameRole))};
    const QVariant rightData{sourceModel()->data(source_right, int(FullnameRole))};

    return QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;
}
}
