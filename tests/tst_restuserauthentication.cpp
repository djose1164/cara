#include "Authentication/data/restuserauthentication.h"
#include "Authentication/login.h"
#include "Authentication/entity/credential.h"
#include "loginhelper.h"
#include <QtTest>

class TestRestUserAuthentication : public QObject
{
    Q_OBJECT

public:
    TestRestUserAuthentication();
    ~TestRestUserAuthentication();

private slots:
    void initTestCase();

    void init();
    void cleanup();

    void test_shouldEmitSucceededUponSuccessfulAuthentication() const;
    void test_shouldEmitFailedUponUnsuccessfulAuthentication() const;
};

TestRestUserAuthentication::TestRestUserAuthentication() {}

TestRestUserAuthentication::~TestRestUserAuthentication() {}

void TestRestUserAuthentication::initTestCase()
{
    LoginHelper::setAppConf();
}

void TestRestUserAuthentication::init()
{

}

void TestRestUserAuthentication::cleanup()
{

}

void TestRestUserAuthentication::test_shouldEmitSucceededUponSuccessfulAuthentication() const
{
    Authentication::Business::Login login{};
    Authentication::Entity::Credential credentials;
    credentials.setUsername("cara");
    credentials.setPassword("1234");
    Authentication::Data::RestUserAuthentication auth{};
    auth.setCredentials(credentials);
    login.setAuthenticator(&auth);

    QSignalSpy spy{&auth, SIGNAL(succeeded(const QByteArray &))};

    QVERIFY(spy.isValid());

    auth.authenticate();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestRestUserAuthentication::test_shouldEmitFailedUponUnsuccessfulAuthentication() const
{
    Authentication::Entity::Credential credentials;
    credentials.setUsername("cara");
    credentials.setPassword("12346");
    Authentication::Data::RestUserAuthentication auth{};
    auth.setCredentials(credentials);
    QSignalSpy spy{&auth, &Authentication::Data::RestUserAuthentication::errorOccurred};

    QVERIFY(spy.isValid());

    auth.authenticate();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestRestUserAuthentication)

#include "tst_restuserauthentication.moc"
