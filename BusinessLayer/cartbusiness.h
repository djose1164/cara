#ifndef CARTBUSINESS_H
#define CARTBUSINESS_H

#include "rest.h"

#include <QObject>
#include <QtQml>

namespace BusinessLayer {

class CartBusiness : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit CartBusiness(QObject *parent = nullptr);

public slots:
    void checkout(const QList<QVariantMap> &content);

signals:

private:
    Rest rest_;
};
}

#endif // CARTBUSINESS_H
