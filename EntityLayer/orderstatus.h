#ifndef ORDERSTATUS_H
#define ORDERSTATUS_H

#include <QObject>
#include <QQmlEngine>
#include <QProperty>

class OrderStatus : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString statusString READ statusString WRITE setStatusString)
    Q_PROPERTY(int statusId READ statusId WRITE setStatusId NOTIFY statusIdChanged)

public:
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

private:
    Q_OBJECT_BINDABLE_PROPERTY(OrderStatus, int, statusId_, &OrderStatus::statusIdChanged)
    Q_OBJECT_BINDABLE_PROPERTY(OrderStatus, QString, statusString_)
};

#endif // ORDERSTATUS_H
