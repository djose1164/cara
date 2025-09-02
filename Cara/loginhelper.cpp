#include "loginhelper.h"
#include "Authentication/data/restuserauthentication.h"
#include "Authentication/data/tokenstore.h"
#include "Authentication/entity/credential.h"
#include "Authentication/login.h"
#include "User/data/localuserstore.h"
#include "dotenv.h"
#include <QCoreApplication>
#include <QSignalSpy>

void LoginHelper::login()
{
    setAppConf();
    loginWithoutAppConf();
}

void LoginHelper::loginWithoutAppConf()
{
    dotenv::init();

    Authentication::Entity::Credential c{};
    c.setUsername(QString::fromStdString(dotenv::getenv("USER_TEST")));
    c.setPassword(QString::fromStdString(dotenv::getenv("PASSWORD_TEST")));

    Authentication::Data::RestUserAuthentication auth{};
    auth.setCredentials(c);

    User::Data::LocalUserStore us{};
    Authentication::Business::Login login{};
    login.setAuthenticator(&auth);
    Authentication::Data::TokenStore ts{};
    QObject::connect(&login, &Authentication::Business::Login::authenticated,
                     &ts, &Authentication::Data::TokenStore::save);

    User::Data::LocalUserStore lus{};
    QObject::connect(&login, &Authentication::Business::Login::authenticated,
                     &lus, &User::Data::LocalUserStore::save);

    QSignalSpy spy{&login, &Authentication::Business::Login::authenticated};
    QVERIFY(spy.isValid());

    login.authenticate();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void LoginHelper::setAppConf()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");
}

LoginHelper::LoginHelper() {}
