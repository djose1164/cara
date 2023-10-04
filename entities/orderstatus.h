#ifndef ORDERSTATUS_H
#define ORDERSTATUS_H

#include <QObject>
#include <QQmlEngine>
#include <QProperty>

class OrderStatus : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString statusString READ statusString WRITE setStatusString NOTIFY statusStringChanged)
    Q_PROPERTY(int statusId READ statusId WRITE setStatusId NOTIFY statusIdChanged)

public:
    enum class Status {
        Pending = 1,
        Processing,
        Sent,
        Delivered,
        Canceled,
        OnHold,
        Any
    };
    Q_ENUM(Status)

    explicit OrderStatus(QObject *parent = nullptr);

    static OrderStatus *fromMap(const QVariantMap &map);

    int statusId() const;
    void setStatusId(int statusId);
    QBindable<int> bindableStatusId() const;

    QString statusString() const;
    void setStatusString(const QString &status);
    QBindable<QString> bindableStatusString() const;

signals:
    void statusIdChanged();
    void statusStringChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(OrderStatus, int, statusId_, &OrderStatus::statusIdChanged)
    Q_OBJECT_BINDABLE_PROPERTY(OrderStatus, QString, statusString_, &OrderStatus::statusStringChanged)
};

#endif // ORDERSTATUS_H
