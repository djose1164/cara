#ifndef SALESPERSONREPOSITORYIMP_H
#define SALESPERSONREPOSITORYIMP_H

#include "salespersonrepository.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

namespace Data {
class SalespersonRepositoryImpl : public QObject, public SalespersonRepository
{
    Q_OBJECT
public:
    explicit SalespersonRepositoryImpl(QObject *parent = nullptr);

    QByteArray buffer() const;

    int salespersonId() const;

    QString path() const;
    void setPath(const QString &newPath);

public slots:
    void getSalespersonById(int id) override;

    void addAssociateSalesperson(const Entity::Salesperson &salesperson) override;

    void getAssociateSalespersons() override;

    void patch(int id, const QByteArray &payload) override;

    void getCustomers(int salespersonId) override;

private slots:
    void requestFinished();

signals:
    void finished();
    void errorOccurred();
    void fetched();
    void added();
    void patched();

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;

    QString path_;
    QByteArray buffer_;
    const std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // SALESPERSONREPOSITORYIMP_H
