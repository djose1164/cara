#ifndef COMMISSIONREPOSITORY_H
#define COMMISSIONREPOSITORY_H

#include <QObject>
#include <QQmlEngine>
#include <QRestReply>

namespace Billing
{

class Commission;

class CommissionRepository : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
    // QML_ELEMENT
    // QML_UNCREATABLE("This is an abstract class.")
public:
    using CommissionId = int;

    enum class Error
    {
        FailedToFetch,
        FailedToAdd,
        FailedToPatch,
        FailedToMapToEntity
    };

    explicit CommissionRepository(QObject *parent = nullptr) {}

    virtual ~CommissionRepository() = default;

    virtual void fetchCommissionsBySalespersonId(int spId) = 0;

    virtual void fetchCommissions() = 0;

    virtual void addCommission(const Commission &commission) = 0;

    virtual void patchCommission(const Commission &commission, std::function<void()> callback = nullptr) = 0;

signals:
    void fetched(const QList<Commission> &commissionList);

    void errorOccuerred(const QString &errorString, Error error);

    void added(const Commission &commission);
};

} // namespace Billing

#endif // COMMISSIONREPOSITORY_H
