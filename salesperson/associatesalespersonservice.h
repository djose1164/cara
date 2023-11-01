#ifndef ASSOCIATESALESPERSONSERVICE_H
#define ASSOCIATESALESPERSONSERVICE_H

#include "data/associatesalespersonrepository.h"
#include <QObject>
#include <QQmlEngine>

namespace Salesperson {
class AssociateSalespersonService : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged FINAL)

public:
    explicit AssociateSalespersonService(QObject *parent = nullptr);

    QByteArray data() const;
    void setData(const QByteArray &newData);

public slots:
    void fetchAllAssociateSalesperson();
    void addAssociateSalesperson(const QVariantMap &data);
    void fetchAssociateSalespersonById(int id);

signals:
    void errorOccurred();
    void fetched();
    void added();

    void dataChanged();

private:
    Data::AssociateSalespersonRepository *salespersonRepository_;
    QByteArray data_;
};
}
#endif // ASSOCIATESALESPERSONSERVICE_H
