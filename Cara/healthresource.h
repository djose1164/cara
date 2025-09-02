#ifndef HEALTHRESOURCE_H
#define HEALTHRESOURCE_H

#include "common/abstractresource.h"
#include <QQmlEngine>

class HealthResource : public AbstractResource
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
public:
    explicit HealthResource(QObject *parent = nullptr);

    QString path() const;
    void setPath(const QString &newPath);

public slots:
    void readStatus();

signals:
    void pathChanged();

    void timeout();

    void statusReady(const QByteArray &data);

    void errorOccurred(const QString &errorString);

    void connectionRefused();

    void upgradeRequired();

private:
    QString path_;
};

#endif // HEALTHRESOURCE_H
