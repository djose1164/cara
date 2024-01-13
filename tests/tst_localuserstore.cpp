#include "user/localuserstore.h"
#include "authentication/login.h"
#include "user/service/userserviceimpl.h"
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
    void init();

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

void TestLocalUserStore::init()
{
    Business::Login login{};
    login.setUsername("josevictoriano");
    login.setPassword("2077120");

    Business::LocalUserStore userStore{};
    QSignalSpy spy{&login, SIGNAL(authenticated(void))};
    login.setUserStore(&userStore);

    login.authenticateCredentials();
    QVERIFY(spy.wait());
}

void TestLocalUserStore::test_shouldReturnTrueIfHasUser() const
{
    Business::LocalUserStore userStore{};

    const bool result{userStore.hasUser()};

    QVERIFY2(result, "there's no any user saved in settings.");
}

void TestLocalUserStore::test_shouldReturnFalseIfDoesntHaveUser() const
{
    Service::UserServiceImpl userService{};
    userService.clear();
    Business::LocalUserStore userStore{};

    const bool result{userStore.hasUser()};

    QVERIFY(result == false);
}

QTEST_MAIN(TestLocalUserStore)

#include "tst_localuserstore.moc"
