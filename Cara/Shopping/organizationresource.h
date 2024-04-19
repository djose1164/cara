#ifndef ORGANIZATIONRESOURCE_H
#define ORGANIZATIONRESOURCE_H

#include "abstractresource.h"
#include <QQmlEngine>
#include <QRestReply>

namespace Shopping
{
class OrganizationResource : public AbstractResource
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString path MEMBER path_ NOTIFY pathChanged FINAL)
public:
    using OrganizationId = int;

    explicit OrganizationResource(QObject *parent = nullptr);

public slots:
    void readAll();
    void readById(OrganizationId id);

private:
    void handleFinished(QRestReply &reply);

signals:
    void pathChanged();
    void errorOccurred(const QString &errorString);
    void timeout();
    void fetched(const QByteArray &data);

private:
    QString path_;
};
}

#endif // ORGANIZATIONRESOURCE_H
