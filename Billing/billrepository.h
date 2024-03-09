#ifndef BILLREPOSITORY_H
#define BILLREPOSITORY_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlEngine>
#include "Utils/abstractresource.h"
#include "ibillrepository.h"

namespace DataLayer {
class BillRepository : public QObject, public IBillRepository
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit BillRepository(QObject *parent = nullptr);

public slots:
    void fetchByCustomerId(int customerId) override;

    void fetchById(int orderId) override;

    void fetchByAdminId(int adminId) override;

signals:
    void errorOccurred(const QByteArray &data);

    /**
     * @brief Emited when fetching many bills.
     * @param data
     */
    void billsFetched(const QByteArray &data);

    /**
     * @brief Emitted when fetching only one bill.
     * @param data
     */
    void billFetched(const QByteArray &data);
private:
    void fetchBy(const QString &param, int id);

    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    AbstractResource::Api api_;
};
}

#endif // BILLREPOSITORY_H
