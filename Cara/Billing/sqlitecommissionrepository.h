#ifndef SQLITECOMMISSIONREPOSITORY_H
#define SQLITECOMMISSIONREPOSITORY_H

#include "commissionrepository.h"

namespace Billing
{

class CommissionItem;

class SQLiteCommissionRepository : public Billing::CommissionRepository
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    explicit SQLiteCommissionRepository(QObject *parent = nullptr);

    void fetchCommissionsBySellerId(int spId) override;

    void fetchCommissions() override;

    void addCommission(const Commission &commission) override;

    void patchCommission(const Commission &commission, std::function<void()> callback = nullptr) override;

    bool supportsServerSideCalculation() const override;

private:
    /**
     * @brief insertCommission inserts a new commission into SQLite db.
     * @param commission Must be a valid commission.
     * @return true on success, false on failure.
     */
    bool insertCommission(const Commission &commission);

    /**
     * @brief insertCommissionItem inserts a new CommissionItem into SQLite db.
     * @param ci Must be a valid CommissionItem.
     * @return true on success, false on failure.
     */
    bool insertCommissionItem(const CommissionItem &ci);

    /**
     * @brief fetchCommissionItems Fetch all the items associated to a commission.
     * @param commissionId The commission unique code
     * @return A list of commission's items.
     */
    std::optional<QList<CommissionItem>> fetchCommissionItems(const QUuid &commissionId) const;
};

} // namespace Billing

#endif // SQLITECOMMISSIONREPOSITORY_H
