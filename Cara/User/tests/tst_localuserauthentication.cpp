#include "Authentication/data/tokenstore.h"
#include "Authentication/login.h"
#include "Authentication/service/localuserauthentication.h"
#include "loginhelper.h"
#include <QSignalSpy>
#include <QtTest>

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
    LoginHelper::login();
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
    QSignalSpy spy{&ur, &Authentication::Service::LocalUserAuthentication::errorOccurred};

    QVERIFY(spy.isValid());

    ur.setRefreshToken("invalid token");
    ur.authenticate();

    QVERIFY(spy.wait());

    const bool res{ur.isAuthenticated()};

    QVERIFY(res == false);
}

QTEST_MAIN(TestLocalUserAuthentication)

#include "tst_localuserauthentication.moc"
