#ifndef HEALTHRESOURCE_H
#define HEALTHRESOURCE_H

#include "common/abstractresource.h"
#include <QProperty>
#include <QQmlEngine>

class HealthResource : public AbstractResource
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
    Q_PROPERTY(QString errorString BINDABLE bindableErrorString FINAL)
public:
    explicit HealthResource(QObject *parent = nullptr);

    QString path() const;
    void setPath(const QString &newPath);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);
    QBindable<QString> bindableErrorString() const;

public slots:
    void readStatus();

signals:
    void pathChanged();

    void timeout();

    void statusReady(const QByteArray &data);

    void errorOccurred(const QString &errorString);

    void connectionRefused(const QString &msg);


    void upgradeRequired(const QString &msg);
    void serverHealthy();
    void serverMaintenance(const QString &msg);

private:
    QString path_;
    QProperty<QString> errorString_;
};

#endif // HEALTHRESOURCE_H
