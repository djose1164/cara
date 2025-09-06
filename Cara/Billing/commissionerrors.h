#ifndef COMMISSIONERRORS_H
#define COMMISSIONERRORS_H

#include <QQmlEngine>

namespace Billing
{

class CommissionErrors
{
    Q_GADGET
public:
    enum class Error
    {
        InvalidRequest, // The passed map may be invalid.
        ItemInsertFailed, // Failed to insert CommissionItem.
        CommissionInsertFailed, // Failed to insert Commission.
        CommissionFetchFailed, // Failed to fetch commissions.
    };
};

} // namespace Billing

namespace CommissionErrorsForeign
{

Q_NAMESPACE
QML_FOREIGN_NAMESPACE(Billing::CommissionErrors)
QML_NAMED_ELEMENT(CommissionErrors)

} // namespace CommissionErrorsForeign

#endif // COMMISSIONERRORS_H
