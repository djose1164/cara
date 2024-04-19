#include "Authentication/data/restuserauthentication.h"
#include "Authentication/data/tokenstore.h"
#include "Authentication/entity/credential.h"
#include "Authentication/login.h"
#include "Authentication/service/localuserauthentication.h"
#include "User/data/localuserstore.h"
#include <QtTest>
#include <QSignalSpy>

using namespace Qt::StringLiterals;

class TestLocalUserAuthentication : public QObject
{
    Q_OBJECT

public:
    TestLocalUserAuthentication();
    ~TestLocalUserAuthentication();

private slots:
    void initTestCase() const;

    void test_shouldReturnTrueIfAuthenticationSuccess() const;
    void test_shouldReturnFalseIfAuthenticationFails() const;
};


TestLocalUserAuthentication::TestLocalUserAuthentication() {}

TestLocalUserAuthentication::~TestLocalUserAuthentication() {}

void TestLocalUserAuthentication::initTestCase() const
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");

    auto login = std::make_unique<Authentication::Business::Login>();
    Authentication::Data::RestUserAuthentication auth{};
    Authentication::Entity::Credential c{};
    c.setUsername("cara");
    c.setPassword("1234");
    auth.setCredentials(c);
    login->setAuthenticator(&auth);
    User::Data::LocalUserStore lus{};
    login->setStore(&lus);

    QSignalSpy spy{login.get(), &Authentication::Business::Login::authenticated};
    QVERIFY(spy.isValid());

    login->authenticate();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestLocalUserAuthentication::test_shouldReturnTrueIfAuthenticationSuccess() const
{
    Authentication::Data::TokenStore ts{};
    ts.restore();
    const QString token{ts.refresh()};

    QVERIFY(!token.isEmpty());

    Authentication::Service::LocalUserAuthentication ur{};
    ur.setRefreshToken(token);

    QSignalSpy spy{&ur, &Authentication::Service::LocalUserAuthentication::succeeded};

    QVERIFY(spy.isValid());

    ur.authenticate();

    QVERIFY(spy.wait());

    const bool res{ur.isAuthenticated()};

    QVERIFY(res);
}

void TestLocalUserAuthentication::test_shouldReturnFalseIfAuthenticationFails() const
{
    Authentication::Service::LocalUserAuthentication ur{};
    QSignalSpy spy{&ur, &Authentication::Service::LocalUserAuthentication::finished};

    QVERIFY(spy.isValid());

    ur.setRefreshToken("invalid token");
    ur.authenticate();

    QVERIFY(spy.wait());

    const bool res{ur.isAuthenticated()};

    QVERIFY(res == false);
}

QTEST_MAIN(TestLocalUserAuthentication)

#include "tst_localuserauthentication.moc"
