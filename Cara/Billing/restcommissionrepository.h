#ifndef RESTCOMMISSIONREPOSITORY_H
#define RESTCOMMISSIONREPOSITORY_H

#include <QNetworkReply>
#include <QQmlEngine>
#include "commissionrepository.h"

class QRestAccessManager;
class QNetworkRequestFactory;
namespace Billing
{


class RestCommissionRepository : public CommissionRepository
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    explicit RestCommissionRepository(QObject *parent = nullptr);

    void fetchCommissionsBySellerId(int spId) override;

    void fetchCommissions() override;

    void addCommission(const Commission &commission) override;

    void patchCommission(const Commission &commission, std::function<void()> callback = nullptr) override;

    bool supportsServerSideCalculation() const override;

private:
    std::unique_ptr<QRestAccessManager> manager_;
    std::shared_ptr<QNetworkRequestFactory> api_;
    QString path_;
};

} // namespace Billing

#endif // RESTCOMMISSIONREPOSITORY_H
