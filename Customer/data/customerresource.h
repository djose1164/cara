#ifndef CUSTOMERRESOURCE_H
#define CUSTOMERRESOURCE_H

#include "abstractresource.h"
#include <QObject>
#include <QQmlEngine>
#include <QNetworkAccessManager>

namespace Data
{
class CustomerResource : public AbstractResource
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged FINAL)
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
    Q_PROPERTY(QString path MEMBER path_ REQUIRED FINAL)
public:
    using SalespersonId = int;

    explicit CustomerResource(QObject *parent = nullptr);

    QByteArray data() const;
    void setData(const QByteArray &newData);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

public slots:
    void readBySalespersonId(SalespersonId salespersonId);

signals:
    void dataChanged();
    void errorStringChanged();
    void errorOccurred();
    void fetched(const QByteArray &data);

    void contentNotFound();

private:
    QByteArray data_;
    QString errorString_;
    QString path_;
};
}

#endif // CUSTOMERRESOURCE_H
