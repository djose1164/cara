#ifndef RESTCUSTOMERREPOSITORY_H
#define RESTCUSTOMERREPOSITORY_H

#include "customerrepository.h"
#include <QObject>
#include <QQmlEngine>
#include <QNetworkAccessManager>

namespace Data
{
class RestCustomerRepository : public Business::CustomerRepository
{
    Q_OBJECT
    QML_ELEMENT
    QML_IMPLEMENTS_INTERFACES(Business::CustomerRepository)
    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
public:
    explicit RestCustomerRepository(QObject *parent = nullptr);

    QByteArray data() const;
    void setData(const QByteArray &newData);

    // CustomerRepository interface
    void readBySalespersonId(SalespersonId salespersonId) override;

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

signals:
    void dataChanged();
    void errorStringChanged();

private:
    QNetworkRequest prepareRequest(const QString &endpoint);

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;
    QByteArray data_;
    QString errorString_;
};
}

#endif // RESTCUSTOMERREPOSITORY_H
