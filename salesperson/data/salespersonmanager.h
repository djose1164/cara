#ifndef DATA_SALESPERSONMANAGER_H
#define DATA_SALESPERSONMANAGER_H

#include "business/manageable.h"
#include <QNetworkAccessManager>

namespace Data
{
class SalespersonManager : public Business::Manageable
{
    Q_OBJECT
public:
    SalespersonManager(QObject *parent = nullptr);


    // Manageable interface
public:
    void read(int id) override;
    void readAll(int adminId) override;

    void add(const QByteArray &data) override;
    void edit(int id, const QByteArray &data) override;
    void remove(int id) override;

private:
    QNetworkRequest prepareRequest(const QString &endpoint = {});

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
};
}

#endif // DATA_SALESPERSONMANAGER_H
