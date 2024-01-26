#ifndef SALESPERSONREPOSITORYIMP_H
#define SALESPERSONREPOSITORYIMP_H

#include "salespersonrepository.h"

#include <QObject>
#include <QNetworkAccessManager>

namespace Data {
class SalespersonRepositoryImpl : public QObject, public SalespersonRepository
{
    Q_OBJECT
public:
    explicit SalespersonRepositoryImpl(QObject *parent = nullptr);

    QByteArray buffer() const;

    int salespersonId() const;

public slots:
    void getSalespersonById(int id) override;

    void addAssociateSalesperson(const Entity::Salesperson &salesperson) override;

    void getAssociateSalespersons() override;

    void patch(int id, const QByteArray &payload) override;

    void getCustomers(int id) override;
signals:
    void finished();
    void errorOccurred();
    void fetched();
    void added();
    void patched();

private:
    QNetworkRequest prepareRequest(const QString &endpoint = {}) const;

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;

    QUrl base_;
    QByteArray buffer_;
};
}

#endif // SALESPERSONREPOSITORYIMP_H
