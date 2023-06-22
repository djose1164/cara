#include "loginmanager.h"
#include "constants.h"

LoginManager::LoginManager(QObject *parent)
    : QObject{parent}
    , rest_{new Rest(this)}
{
    QString base{BusinessLayer::Constants::instance()->base()};
    rest_->base(base);
}

LoginManager::Status LoginManager::status() const
{
    return status_.value();
}

void LoginManager::setStatus(Status status)
{
    status_ = status;
}

QBindable<LoginManager::Status> LoginManager::bindableStatus()
{
    return &status_;
}

void LoginManager::requestLogin(const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty())
        return;

    QJsonObject userObj;
    userObj["username"] = username;
    userObj["password"] = password;

    rest_->post("/", userObj.toVariantMap());
    emit loginRequested();

    connect(rest_, &Rest::received, this, [&](const QVariantMap &resp) {
        qDebug() << resp;
    });
}
