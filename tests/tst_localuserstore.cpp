#include "Authentication/data/restuserauthentication.h"
#include "Authentication/entity/credential.h"
#include "Authentication/login.h"
#include "User/data/localuserstore.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

class TestLocalUserStore : public QObject
{
    Q_OBJECT
public:
    explicit TestLocalUserStore(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void init() const;

    void test_shouldReturnTrueIfHasUser() const;
    void test_shouldReturnFalseIfDoesntHaveUser() const;
};



TestLocalUserStore::TestLocalUserStore(QObject *parent)
    : QObject{parent}
{

}

void TestLocalUserStore::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");
}

void TestLocalUserStore::init() const
{
    Authentication::Business::Login login{};
    QSignalSpy spy{&login, &Authentication::Business::Login::authenticated};

    QVERIFY(spy.isValid());

    Authentication::Data::RestUserAuthentication restAuthenticator{};
    Authentication::Entity::Credential credentials{};
    credentials.setUsername("josevictoriano");
    credentials.setPassword("2077120");
    restAuthenticator.setCredentials(credentials);
    login.setAuthenticator(&restAuthenticator);

    login.authenticate();
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}


void TestLocalUserStore::test_shouldReturnTrueIfHasUser() const
{
    User::Data::LocalUserStore userStore{};
    userStore.restore();

    const bool result{userStore.hasUser()};

    QVERIFY2(result, "there's no any user saved in settings.");
}

void TestLocalUserStore::test_shouldReturnFalseIfDoesntHaveUser() const
{
    User::Data::LocalUserStore userStore{};
    userStore.clear();

    const bool result{userStore.hasUser()};

    QVERIFY(result == false);
}

QTEST_MAIN(TestLocalUserStore)

#include "tst_localuserstore.moc"
