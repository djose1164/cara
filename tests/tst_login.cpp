#include "Authentication/data/restuserauthentication.h"
#include "Authentication/data/tokenstore.h"
#include "Authentication/entity/credential.h"
#include "Authentication/login.h"
#include "Authentication/service/localuserauthentication.h"
#include "User/data/localuserstore.h"
#include "LoginHelper.h"
#include <QSettings>
#include <QSignalSpy>
#include <QtTest>

using namespace Qt::StringLiterals;

class TestLogin : public QObject
{
    Q_OBJECT

public:
    TestLogin();
    ~TestLogin();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_shouldEmitAuthenticatedOnceUsingValidCredentials() const;
    void test_shouldEmitAuthenticatedOnceUponRefreshingToken() const;

    void test_shouldEmitErrorOccurredOnceIfCredentialsAreInvalid() const;
};

TestLogin::TestLogin() {}

TestLogin::~TestLogin() {}

void TestLogin::initTestCase()
{
    LoginHelper::login();
}

void TestLogin::cleanupTestCase()
{

}


void TestLogin::test_shouldEmitAuthenticatedOnceUsingValidCredentials() const
{
    auto login = std::make_unique<Authentication::Business::Login>();
    Authentication::Data::RestUserAuthentication auth{};
    Authentication::Entity::Credential c{};
    c.setUsername("cara");
    c.setPassword("1234");
    auth.setCredentials(c);
    login->setAuthenticator(&auth);

    QSignalSpy spy{login.get(), &Authentication::Business::Login::authenticated};
    QVERIFY(spy.isValid());

    login->authenticate();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestLogin::test_shouldEmitAuthenticatedOnceUponRefreshingToken() const
{
    auto login = std::make_unique<Authentication::Business::Login>();
    Authentication::Service::LocalUserAuthentication auth{};
    const auto token{QSettings{}.value("Credentials/refreshToken"_L1).toString()};

    QVERIFY(!token.isEmpty());

    auth.setRefreshToken(token);
    login->setAuthenticator(&auth);

    QSignalSpy spy{login.get(), &Authentication::Business::Login::authenticated};

    login->authenticate();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}


void TestLogin::test_shouldEmitErrorOccurredOnceIfCredentialsAreInvalid() const
{
    auto login = std::make_unique<Authentication::Business::Login>();
    Authentication::Data::RestUserAuthentication auth{};
    Authentication::Entity::Credential c{};
    c.setUsername("cara");
    c.setPassword("12349");
    auth.setCredentials(c);
    login->setAuthenticator(&auth);

    QSignalSpy spy{login.get(), &Authentication::Business::Login::errorOccurred};

    QVERIFY(spy.isValid());

    login->authenticate();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestLogin)

#include "tst_login.moc"
