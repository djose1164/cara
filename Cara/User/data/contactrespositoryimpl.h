#ifndef CONTACTRESPOSITORYIMPL_H
#define CONTACTRESPOSITORYIMPL_H

#include "contactrepository.h"
#include <QObject>

class QRestAccessManager;
class QNetworkRequestFactory;

namespace Data {
class ContactRespositoryImpl : public QObject, public ContactRepository
{
    Q_OBJECT
public:
    explicit ContactRespositoryImpl(QObject *parent = nullptr);

    void getById(int id, Handler &&h) override;
    void update(const QVariantMap &data, const QUrlQuery &query, Handler &&h) override;

private:
    QRestAccessManager *manager_;
    std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // CONTACTRESPOSITORYIMPL_H
