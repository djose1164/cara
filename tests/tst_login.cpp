#include <QtTest>
#include <QSignalSpy>
#include "../authentication/login.h"
#include "localuserstore.h"

// add necessary includes here

class TestLogin : public QObject
{
    Q_OBJECT

public:
    TestLogin();
    ~TestLogin();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();

    void test_shouldEmitAuthenticatedOnceUsingValidCredentials() const;
    void test_shouldSwitchToLoginCredentialsModeUponAuthenticateCredentials() const;
    void test_shouldSwitchToRefreshTokenModeUponAuthenticateRefreshToken() const;
    void test_shouldEmitErrorOccurredOnceIfCredentialsAreInvalid() const;
};

TestLogin::TestLogin() {}

TestLogin::~TestLogin() {}

void TestLogin::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");
}

void TestLogin::cleanupTestCase()
{

}

void TestLogin::init()
{
    Business::Login login{};
    Business::LocalUserStore userStore{};
    login.setUserStore(&userStore);
    login.setUsername("cara");
    login.setPassword("1234");

    QSignalSpy spy(&login, SIGNAL(authenticated(void)));

    QVERIFY(spy.isValid());

    login.authenticateCredentials();
    QVERIFY(spy.wait());
}

void TestLogin::test_shouldEmitAuthenticatedOnceUsingValidCredentials() const
{
    Business::Login login{};
    login.setUsername("cara");
    login.setPassword("1234");

    QSignalSpy spy(&login, SIGNAL(authenticated(void)));

    QVERIFY(spy.isValid());

    login.authenticateCredentials();
    QVERIFY(spy.wait());
    // Check what IP address is being in used

    QCOMPARE(spy.count(), 1);
}

void TestLogin::test_shouldSwitchToLoginCredentialsModeUponAuthenticateCredentials() const
{
    auto login = std::make_unique<Business::Login>();
    login->setUsername("cara");
    login->setPassword("1234");

    login->authenticateCredentials();

    using AuthenticationMode = Business::Login::AuthenticationMode;
    QCOMPARE(login->mode(), AuthenticationMode::LoginCredentials);
}

void TestLogin::test_shouldSwitchToRefreshTokenModeUponAuthenticateRefreshToken() const
{
    auto login = std::make_unique<Business::Login>();

    login->authenticateRefreshToken();

    using AuthenticationMode = Business::Login::AuthenticationMode;
    QCOMPARE(login->mode(), AuthenticationMode::RefreshToken);
}

void TestLogin::test_shouldEmitErrorOccurredOnceIfCredentialsAreInvalid() const
{
    Business::Login login{};
    login.setUsername("");
    login.setPassword("123");
    QSignalSpy spy{&login, SIGNAL(errorOccurred(void))};

    QVERIFY(spy.isValid());

    login.authenticateCredentials();

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestLogin)

#include "tst_login.moc"
