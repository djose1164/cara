#include "Authentication/service/localuserauthentication.h"
#include "keychainimpl.h"
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
    Authentication::KeyChainImpl keychain;
    QSignalSpy spy{&keychain, &Authentication::KeyChainImpl::keyRestored};
    QString token{};
    connect(&keychain,
            &Authentication::KeyChainImpl::keyRestored,
            this,
            [&](QString, const QString &value) { token = value; });

    QVERIFY(spy.isValid());
    QVERIFY(spy.wait());

    QVERIFY(!token.isEmpty());

    Authentication::Service::LocalUserAuthentication ur{};
    ur.setRefreshToken(token);

    QSignalSpy authSpy{&ur, &Authentication::Service::LocalUserAuthentication::succeeded};

    QVERIFY(authSpy.isValid());

    ur.authenticate();

    QVERIFY(authSpy.wait());

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
