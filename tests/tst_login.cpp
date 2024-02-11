#include "Authentication/login.h"
#include "Authentication/data/restuserauthentication.h"
#include "Authentication/service/localuserauthentication.h"
#include "User/data/localuserstore.h"
#include "authentication/entity/credential.h"
#include <QtTest>
#include <QSignalSpy>
#include <QSettings>

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
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");

    Authentication::Entity::Credential c{};
    c.setUsername("cara");
    c.setPassword("1234");

    Authentication::Data::RestUserAuthentication auth{};
    auth.setCredentials(c);

    User::Data::LocalUserStore us{};
    Authentication::Business::Login login{};
    login.setAuthenticator(&auth);
    login.setStore(&us);

    QSignalSpy spy{&login, &Authentication::Business::Login::dataSaved};
    QVERIFY(spy.isValid());

    login.authenticate();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
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

    QSignalSpy spy{login.get(), SIGNAL(errorOccurred(void))};

    QVERIFY(spy.isValid());

    login->authenticate();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestLogin)

#include "tst_login.moc"
