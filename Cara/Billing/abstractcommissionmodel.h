#ifndef ABSTRACTCOMMISSIONMODEL_H
#define ABSTRACTCOMMISSIONMODEL_H

#include <QVariantMap>

class QAbstractItemModel;
namespace Billing
{
class AbstractCommissionModel
{
public:
    using SellerId = int;

    enum class Error {
        InvalidRequest, // The passed map may be invalid.
        ItemInsertFailed, // Failed to insert CommissionItem.
        CommissionInsertFailed, // Failed to insert Commission.
        CommissionFetchFailed, // Failed to fetch commissions.
    };

    virtual ~AbstractCommissionModel() = default;
    virtual void append(const QVariantMap &map) = 0;
    virtual void fetchCommissions() = 0;
    virtual void fetchCommissionsBySellerId(SellerId sellerId) = 0;
    virtual QAbstractItemModel *itemModel() = 0;
    virtual void scheduleDelete() = 0;
};
}

#endif // ABSTRACTCOMMISSIONMODEL_H
