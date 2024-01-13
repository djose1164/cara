#include "app.h"

using BusinessLayer::App;

App *App::instance_{nullptr};

App *App::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)

    Q_ASSERT(instance_ == nullptr);

    instance_ = new App;
    return instance_;
}

App *App::instance()
{
    Q_ASSERT(instance_);

    return instance_;
}

QString App::accessToken() const
{
    return accessToken_;
}

void App::setAccessToken(const QString &accessToken)
{
    accessToken_ = accessToken;
}

QBindable<QString> App::bindableAccessToken() const
{
    return &accessToken_;
}

ModelLayer::BillModel *App::billModel() const
{
    return billModel_.value();
}

void App::setBillModel(ModelLayer::BillModel * const billModel)
{
    billModel_ = billModel;
}

QBindable<ModelLayer::BillModel> App::bindableBillModel() const
{
    return &billModel_;
}

App::App()
    : QObject{}
    , rest_{new Rest(this)}
{
}
