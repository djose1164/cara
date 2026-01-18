#ifndef COMMISSIONREPOSITORY_H
#define COMMISSIONREPOSITORY_H

#include "commission.h"
#include <QObject>
#include <QQmlEngine>
#include <QRestReply>

namespace Billing
{

class CommissionRepository : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    // QML_ELEMENT
    // QML_UNCREATABLE("This is an abstract class.")
public:
    using CommissionId = int;
    using SellerId = int;

    enum class Error
    {
        FetchFailure,
        AddFailure,
        PatchFailure,
        MappingFailure,
        ItemInsertFailed,
        CommissionInsertFailed
    };

    explicit CommissionRepository(QObject* parent = nullptr) : QObject(parent) {}

    virtual ~CommissionRepository() = default;

    virtual void fetchCommissionsBySellerId(SellerId sellerId) = 0;

    virtual void fetchCommissions() = 0;

    virtual void addCommission(const Commission &commission) = 0;

    virtual void patchCommission(const Commission &commission, std::function<void()> callback = nullptr) = 0;

    virtual bool supportsServerSideCalculation() const = 0;

signals:
    void fetched(const QList<Billing::Commission> &commissionList);

    void errorOccurred(CommissionRepository::Error error, const QString &errorString);

    void added(const Billing::Commission &commission);
};

} // namespace Billing

#endif // COMMISSIONREPOSITORY_H
