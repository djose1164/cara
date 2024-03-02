#ifndef CUSTOMERRESOURCE_H
#define CUSTOMERRESOURCE_H

#include "abstractresource.h"
#include "customerrepository.h"
#include <QObject>
#include <QQmlEngine>
#include <QNetworkAccessManager>

namespace Data
{
class CustomerResource
    : public AbstractResource, public Business::CustomerRepository
{
    Q_OBJECT
    QML_ELEMENT
    Q_INTERFACES(Business::CustomerRepository)
    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL REQUIRED)
public:
    using SalespersonId = int;
    using CustomerId = int;

    explicit CustomerResource(QObject *parent = nullptr);

    QByteArray data() const;
    void setData(const QByteArray &newData);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

    QString path() const;
    void setPath(const QString &newPath);

public slots:
    void readBySalespersonId(SalespersonId salespersonId) override final;
    void readSummary(CustomerId id);

signals:
    void dataChanged();
    void errorStringChanged();
    void errorOccurred();
    void fetched(const QByteArray &data);

    void contentNotFound();

    void pathChanged();

private:
    QByteArray data_;
    QString errorString_;
    QString path_;
};
}

#endif // CUSTOMERRESOURCE_H
