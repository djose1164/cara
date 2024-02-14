#ifndef RESTSERVICE_H
#define RESTSERVICE_H

#include "abstractresource.h"

#include <QNetworkRequestFactory>
#include <QObject>
#include <QQmlEngine>

namespace Utils
{
class RestService : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    QML_ELEMENT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged FINAL)
    Q_PROPERTY(QRestAccessManager *manager READ manager CONSTANT FINAL)
    Q_PROPERTY(QQmlListProperty<AbstractResource> resources READ resources CONSTANT FINAL)
    Q_CLASSINFO("DefaultProperty", "resources")

public:
    explicit RestService(QObject *parent = nullptr);

    QUrl url() const;
    void setUrl(const QUrl &newUrl);

    QRestAccessManager *manager() const;
    QNetworkRequestFactory *api() const;

    QQmlListProperty<AbstractResource> resources();

    void classBegin() override;
    void componentComplete() override;

    void append(AbstractResource *resource);

signals:
    void urlChanged();

private:
    void setupResource(AbstractResource &r);

private:
    QUrl url_;
    QList<AbstractResource *> resources_;
    std::shared_ptr<QRestAccessManager> manager_;
    std::shared_ptr<QNetworkRequestFactory> api_;
};
}

#endif // RESTSERVICE_H
