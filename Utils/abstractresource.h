#ifndef ABSTRACTRESOURCE_H
#define ABSTRACTRESOURCE_H

#include <QNetworkRequestFactory>
#include <QObject>
#include <QQmlEngine>

class QRestAccessManager;
// TODO: For some weird reason the class can't be registered when inside a namespace.
class AbstractResource : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    using Manager = std::shared_ptr<QRestAccessManager>;
    using Api = std::shared_ptr<QNetworkRequestFactory>;

    explicit AbstractResource(QObject *parent = nullptr);
    virtual ~AbstractResource() = default;

    void setManager(const Manager &newManager);
    void setApi(const Api &newApi);

protected:
    Manager manager_;
    Api api_;
};

#endif // ABSTRACTRESOURCE_H
