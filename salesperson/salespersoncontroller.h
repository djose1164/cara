#ifndef SALESPERSONCONTROLLER_H
#define SALESPERSONCONTROLLER_H

#include "business/manageable.h"
#include "salesperson.h"
#include <QObject>
#include <QQmlEngine>

namespace Controller
{
/**
 * @brief The SalespersonController class handles, verifies & validates the
 * requests made by the clients related to Salesperson module.
 */
class SalespersonController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int adminId READ adminId WRITE setAdminId NOTIFY adminIdChanged FINAL REQUIRED)
    Q_PROPERTY(int salespersonId READ salespersonId WRITE setSalespersonId NOTIFY salespersonIdChanged FINAL)
    Q_PROPERTY(QByteArray buffer READ buffer WRITE setBuffer NOTIFY bufferChanged FINAL)
public:
    using SalespersonId = Entity::Salesperson::SalespersonId;

    explicit SalespersonController(QObject *parent = nullptr);

    int adminId() const;
    void setAdminId(SalespersonId newAdminId);

    QByteArray buffer() const;
    void setBuffer(const QByteArray &newBuffer);

    int salespersonId() const;
    void setSalespersonId(SalespersonId newSalespersonId);

public slots:
    /**
     * @brief readSalespeople Reads the associated salespeople of adminId_. Emit
     * fetched when done; errorOccured otherwise
     */
    void readSalespeople();

    /**
     * @brief readSalesperson Reads the salesperson having salespersonId_. Emit
     * fetched when done; errorOccured otherwise
     */
    void readSalesperson();

    void update();
signals:
    void adminIdChanged();
    void fetched();
    void updated();
    void errorOccurred();

    void bufferChanged();

    void salespersonIdChanged();

private:
    Business::Manageable *manageable_;
    SalespersonId adminId_;
    SalespersonId salespersonId_;
    QByteArray buffer_;
};
}

#endif // SALESPERSONCONTROLLER_H
