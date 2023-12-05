#ifndef SALESPERSONCONTROLLER_H
#define SALESPERSONCONTROLLER_H

#include "business/manageable.h"
#include <QObject>
#include <QQmlEngine>

namespace Controller
{
class SalespersonController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int adminId READ adminId WRITE setAdminId NOTIFY adminIdChanged FINAL REQUIRED)
    Q_PROPERTY(int salespersonId READ salespersonId WRITE setSalespersonId NOTIFY salespersonIdChanged FINAL)
    Q_PROPERTY(QByteArray buffer READ buffer WRITE setBuffer NOTIFY bufferChanged FINAL)
public:
    explicit SalespersonController(QObject *parent = nullptr);

    int adminId() const;
    void setAdminId(int newAdminId);

    QByteArray buffer() const;
    void setBuffer(const QByteArray &newBuffer);

    int salespersonId() const;
    void setSalespersonId(int newSalespersonId);

public slots:
    void readAll();
    void read(int salespersonId);

    void update();
signals:
    void adminIdChanged();
    void fetched();
    void updated();

    void bufferChanged();

    void salespersonIdChanged();

private:
    Business::Manageable *manageable_;
    int adminId_;
    int salespersonId_;
    QByteArray buffer_;
};
}

#endif // SALESPERSONCONTROLLER_H
