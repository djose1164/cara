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
    : UserEntity{}
    , hasPreviousLogin_{} //TODO: Does Qt's engine free the memory later?
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
        emit loggingIn();

        QVariantMap user{resp["user"].toMap()};
        setUsername(user["username"].toString());
        setUserType(static_cast<UserTypes>(user["user_type_id"].toInt()));

        QVariantMap personInfo{user["person_info"].toMap()};
        setForename(personInfo["forename"].toString());
        setSurname(personInfo["surname"].toString());
        setCustomerId(personInfo["customer_id"].toInt());

        emit loggedIn();
    });

    connect(this, &CurrentUser::loggedIn, this, [&] {
        save();
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
    return accessToken_.value();
}

void CurrentUser::setAccessToken(const QString &token)
{
    accessToken_ = token;
}

void CurrentUser::logOut()
{
    emit loggingOut();

    clear();
    rest_.accessToken("");
    hasPreviousLogin_ = false;

    emit loggedOut();
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
    emit loggingIn();

    settings_.setValue("Credentials/refreshToken", "Bearer " + user["refresh_token"].toString());
    const QString token{"Bearer " + user["access_token"].toString()};
    rest_.accessToken(token);

    const auto userObj{user["user"].toMap()};

    const auto personInfo = userObj["person_info"].toMap();
    setForename(personInfo["forename"].toString());
    setSurname(personInfo["surname"].toString());
    setCustomerId(personInfo["customer_id"].toInt());

    setEmail(userObj["email"].toString());
    setUserType(static_cast<UserTypes>(userObj["user_type_id"].toInt()));
    setUserId(userObj["id"].toInt());
    qDebug() << "userType: " << userType() << "\n" << "forename: " << forename()
                  << "\nuserId: " << userId();

    emit restReady(token);
    emit loggedIn();
}

void CurrentUser::save()
{
    settings_.beginGroup("Contact");
    settings_.setValue("forename", forename());
    settings_.setValue("surname", surname());
    settings_.setValue("email", email());
    settings_.endGroup();

    settings_.beginGroup("Credentials")    ;
    settings_.setValue("username", username());
    settings_.setValue("userTypeId", int(userType()));
    settings_.endGroup();
}

bool CurrentUser::existPreviousLogin()
{
    const auto userTypeId{settings_.value("Credentials/userTypeId").toInt()};
    setUserType(static_cast<UserTypes>(userTypeId));

    const QString username{settings_.value("Credentials/username").toString()};
    setForename(username);

    const QString forename{settings_.value("Contact/forename").toString()};
    setForename(forename);

    const QString surname{settings_.value("Contact/surname").toString()};
    setSurname(surname);

    const QString email{settings_.value("Contact/email").toString()};
    setEmail(email);

    const auto refreshToken{settings_.value("Credentials/refreshToken").toString()};
    return !refreshToken.isEmpty();
}
