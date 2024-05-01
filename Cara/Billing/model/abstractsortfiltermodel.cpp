#include "abstractsortfiltermodel.h"

namespace Model
{

AbstractSortFilterModel::AbstractSortFilterModel(QObject *parent)
    : QSortFilterProxyModel{parent}
    , filterEnabled_{true}
{
}

AbstractSortFilterModel::~AbstractSortFilterModel()
{
}

bool AbstractSortFilterModel::filterEnabled() const
{
    return filterEnabled_;
}

void AbstractSortFilterModel::setFilterEnabled(bool newFilterEnabled)
{
    if (filterEnabled_ == newFilterEnabled)
        return;
    filterEnabled_ = newFilterEnabled;
    emit filterEnabledChanged();

    if (filterEnabled_)
        invalidateFilter();
}

}
