#include "cartbusiness.h"

using namespace BusinessLayer;

CartBusiness::CartBusiness(QObject *parent)
    : QObject{parent}
    , rest_{}
{
    rest_.base("http://127.0.0.1:5000/api/orders");
    rest_.refreshAccessToken();
}

void CartBusiness::checkout(const QList<QVariantMap> &content)
{
    qDebug() << "Checking out...";

    foreach (auto product, content)
        qDebug() << product;

    rest_.post("/", QVariantMap{});
}
