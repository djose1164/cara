#ifndef ABSTRACTRESOURCE_H
#define ABSTRACTRESOURCE_H

#include <QNetworkRequestFactory>
#include <QObject>
#include <QQmlEngine>
#include <QRestAccessManager>

// TODO: For some weird reason the class can't be registered when inside a namespace.
class AbstractResource : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    explicit AbstractResource(QObject *parent = nullptr);
    virtual ~AbstractResource() = default;

    void setManager(const std::shared_ptr<QRestAccessManager> &newManager);
    void setApi(const std::shared_ptr<QNetworkRequestFactory> &newApi);

protected:
    std::shared_ptr<QRestAccessManager> manager_;
    std::shared_ptr<QNetworkRequestFactory> api_;
};

#endif // ABSTRACTRESOURCE_H
