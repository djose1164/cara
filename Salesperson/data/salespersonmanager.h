#ifndef DATA_SALESPERSONMANAGER_H
#define DATA_SALESPERSONMANAGER_H

#include "../business/manageable.h"
#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

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
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    QString path_;
    const std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // DATA_SALESPERSONMANAGER_H
