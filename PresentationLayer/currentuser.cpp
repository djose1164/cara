#include "currentuser.h"
#include "constants.h"

using EntityLayer::UserEntity;
using UserTypes = EntityLayer::UserEntity::UserTypes;

CurrentUser *CurrentUser::instance_{nullptr};

CurrentUser::CurrentUser()
    : hasPreviousLogin_{} //TODO: Does Qt's engine free the memory later?
    , settings_{}
{
    hasPreviousLogin_ = existPreviousLogin();
    setup();

    connect(this, &CurrentUser::previousLoginFound, this, [] {
        qDebug("onPreviousLoginFound make request");
    });

    connect(this, &CurrentUser::restReady, this, [this] {
        rest_.get("/");
    });

    connect(&rest_, &Rest::succeeded, this, [this](const QVariantMap &resp) {
        QVariantMap user{resp["user"].toMap()};
        setUsername(user["username"].toString());
        setUserType(static_cast<UserTypes>(user["user_type_id"].toInt()));

        QVariantMap personInfo{user["person_info"].toMap()};
        setForename(personInfo["forename"].toString());
        setSurname(personInfo["surname"].toString());
        setCustomerId(personInfo["customer_id"].toInt());
        const QString type{QVariant::fromValue(userType()).toString()};
        qDebug() << "user_type_id " << type << "\ncustomerId " << customerId()
                 << "\nusername " << username();

        emit userEntityReady();
    });
}

void CurrentUser::setup()
{
    if (hasPreviousLogin_)
    {
        const auto refreshToken{settings_.value("Credentials/refreshToken").toString()};
        initRest(refreshToken);
        emit previousLoginFound();
    }
    else
        emit noPreviousLoginFound();

    emit completed();
}

CurrentUser *CurrentUser::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)

    Q_ASSERT(instance_ == nullptr);

    instance_ = new CurrentUser;
    return instance_;
}

CurrentUser *CurrentUser::instance()
{
    Q_ASSERT(instance_);

    return instance_;
}

void CurrentUser::initRest(const QString &refreshToken)
{
    const QString restBase{BusinessLayer::Constants::instance()->base()};
    rest_.base(restBase + "/users");
    rest_.refreshToken(refreshToken);
    rest_.refreshAccessToken();

    connect(&rest_, &Rest::accessTokenChanged, this, [this] {
        emit restReady();
    });
}

bool CurrentUser::existPreviousLogin()
{
    const auto refreshToken{settings_.value("Credentials/refreshToken").toString()};
    return !refreshToken.isEmpty();
}
