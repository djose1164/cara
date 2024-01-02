#ifndef RESTORDERREPOSITORY_H
#define RESTORDERREPOSITORY_H

#include "orderrepository.h"

namespace Data
{
class RestOrderRepository : public Business::OrderRepository
{
    Q_OBJECT
public:
    explicit RestOrderRepository(QObject *parent = nullptr);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

private:
    QNetworkRequest prepareRequest(const QString &endpoint = "/");

private:
    QRestAccessManager *manager_;
    QRestReply *reply_;
    const QString base_;
    QString errorString_;

    // OrderRepository interface
public slots:
    void add(const QVariantMap &payload) override;
};
}

#endif // RESTORDERREPOSITORY_H
