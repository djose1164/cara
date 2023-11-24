#ifndef SALESPERSONCONTROLLER_H
#define SALESPERSONCONTROLLER_H

#include "data/salespersonrepositoryimp.h"
#include "salesperson.h"
#include <QObject>
#include <QQmlEngine>

namespace Controller
{
class SalespersonController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit SalespersonController(QObject *parent = nullptr);

public slots:
    void getSalespersonById(int id);

signals:
    void fetched(const Entity::Salesperson &salesperson);

private:
    Data::SalespersonRepositoryImpl *repository_;
};
}

#endif // SALESPERSONCONTROLLER_H
