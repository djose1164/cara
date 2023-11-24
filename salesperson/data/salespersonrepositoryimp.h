#ifndef ASSOCIATESALESPERSONREPOSITORYIMP_H
#define ASSOCIATESALESPERSONREPOSITORYIMP_H

#include "associatesalespersonrepository.h"

#include <QObject>
#include <QNetworkAccessManager>

namespace Data {
class AssociateSalespersonRepositoryImpl : public QObject, public AssociateSalespersonRepository
{
    Q_OBJECT
public:
    explicit AssociateSalespersonRepositoryImpl(QObject *parent = nullptr);

public slots:
    void getAssociateSalespersonById(int id) override;

    void addAssociateSalesperson(const Entity::Salesperson &salesperson) override ;

    void getAssociateSalespersons() override;

signals:
    void finished(const QByteArray &data);
    void errorOccurred();
    void fetched();
    void added();
private:
    QNetworkRequest prepareRequest() const;

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;

    QUrl base_;
};
}

#endif // ASSOCIATESALESPERSONREPOSITORYIMP_H
