#include <QtTest>

#include "authentication/data/restuserauthentication.h"
#include "user/entity/usercredential.h"

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
    void test_shouldReturnTrueIfBaseContainsApiUsersLogin() const;

private:
};

TestRestUserAuthentication::TestRestUserAuthentication() {}

TestRestUserAuthentication::~TestRestUserAuthentication() {}

void TestRestUserAuthentication::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");
}

void TestRestUserAuthentication::init()
{

}

void TestRestUserAuthentication::cleanup()
{

}

void TestRestUserAuthentication::test_shouldEmitSucceededUponSuccessfulAuthentication() const
{
    User::Entity::UserCredential credentials;
    credentials.setUsername("cara");
    credentials.setPassword("1234");
    Data::RestUserAuthentication auth{credentials};
    QSignalSpy spy{&auth, SIGNAL(succeeded(void))};

    QVERIFY(spy.isValid());

    auth.authenticate();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestRestUserAuthentication::test_shouldEmitFailedUponUnsuccessfulAuthentication() const
{
    User::Entity::UserCredential credentials;
    credentials.setUsername("cara");
    credentials.setPassword("12346");
    Data::RestUserAuthentication auth{credentials};
    QSignalSpy spy{&auth, SIGNAL(failed(void))};

    QVERIFY(spy.isValid());

    auth.authenticate();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestRestUserAuthentication::test_shouldReturnTrueIfBaseContainsApiUsersLogin() const
{
    User::Entity::UserCredential credentials{};
    Data::RestUserAuthentication auth{credentials};

    const bool result{auth.base().contains("/api/users/login")};

    QCOMPARE(result, true);
}

QTEST_MAIN(TestRestUserAuthentication)

#include "tst_restuserauthentication.moc"
