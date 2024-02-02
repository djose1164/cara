#ifndef RESTORDERREPOSITORY_H
#define RESTORDERREPOSITORY_H

#include "business/orderrepository.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Data
{
class RestOrderRepository : public Business::OrderRepository
{
    Q_OBJECT
public:
    explicit RestOrderRepository(QObject *parent = nullptr);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

public slots:
    void add(const QVariantMap &payload) override;

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    const QString base_;
    QString errorString_;
};
}

#endif // RESTORDERREPOSITORY_H
