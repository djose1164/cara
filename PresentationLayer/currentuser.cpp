#include "currentuser.h"
#include "constants.h"

#define createIfNull()          \
do {                            \
    if (!instance_)             \
    instance_ = new CurrentUser;\
} while(0)                      \

using EntityLayer::UserEntity;
using UserTypes = EntityLayer::UserEntity::UserTypes;

CurrentUser *CurrentUser::instance_{nullptr};

CurrentUser::CurrentUser()
    : QObject{}
    , hasPreviousLogin_{} //TODO: Does Qt's engine free the memory later?
    , settings_{}
    , user_{nullptr}
{
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

    createIfNull();

    return instance_;
}

CurrentUser *CurrentUser::instance()
{  
    createIfNull();

    return instance_;
}

QString CurrentUser::accessToken() const
{
    return accessToken_;
}

void CurrentUser::setAccessToken(const QString &token)
{
    accessToken_ = token;
}

QBindable<QString> CurrentUser::bindableAccessToken()
{
    return &accessToken_;
}

UserEntity *CurrentUser::user() const
{
    return user_;
}

void CurrentUser::setUser(EntityLayer::UserEntity * const user)
{
    user_ = user;
    user_->setParent(this);
}

void CurrentUser::initRest(const QString &refreshToken)
{
    const QString restBase{BusinessLayer::Constants::instance()->base()};
    rest_.base(restBase + "/users");
    rest_.refreshToken(refreshToken);
    rest_.refreshAccessToken();

    connect(&rest_, &Rest::accessTokenChanged, this, [this] {
        emit restReady(rest_.accessToken());
    });
}

void CurrentUser::fillUserInfo(const QVariantMap &user)
{
    settings_.setValue("Credentials/refreshToken", "Bearer " + user["refresh_token"].toString());

    const QString token{"Bearer " + user["access_token"].toString()};
    setAccessToken(token);

    setUser(UserEntity::fromMap(user));

    save();
}

void CurrentUser::save()
{
    settings_.beginGroup("Contact");
    settings_.setValue("forename", user_->forename());
    settings_.setValue("surname", user_->surname());
    settings_.setValue("email", user_->email());
    settings_.endGroup();

    settings_.beginGroup("Credentials")    ;
    settings_.setValue("username", user_->username());
    settings_.setValue("userTypeId", int(user_->userType()));
    settings_.endGroup();
}

void CurrentUser::clear()
{
    user_->clear();
}

bool CurrentUser::existPreviousLogin()
{
    const auto refreshToken{settings_.value("Credentials/refreshToken").toString()};
    if (refreshToken.isEmpty())
        return false;

    return true;
}

UserTypes CurrentUser::userType() const
{
    return user_->userType();
}
