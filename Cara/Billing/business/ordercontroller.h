#ifndef ORDERCONTROLLER_H
#define ORDERCONTROLLER_H

#include "orderrepository.h"
#include <QObject>
#include <QQmlEngine>
#include <QJsonObject>

namespace Business
{
class OrderController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
    Q_PROPERTY(Error error READ error WRITE setError NOTIFY errorChanged FINAL)
public:
    enum class Error
    {
        NoError,
        ValidationError
    };
    Q_ENUM(Error)

    explicit OrderController(QObject *parent = nullptr);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

    Error error() const;
    void setError(Error newError);

    Q_INVOKABLE bool validateOrder(const QVariantMap &order);

public slots:
    void createOrder(const QVariantMap &order);

signals:
    void orderCreated(const QJsonObject &json);

    void errorOccurred();

    void errorStringChanged();

    void errorChanged();

private:
    OrderRepository *repository_;
    QString errorString_;
    Error error_;
};
}

#endif // ORDERCONTROLLER_H
