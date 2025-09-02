#include "User/data/localuserstore.h"
#include "loginhelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>

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
    LoginHelper::setAppConf();
}

void TestLocalUserStore::init() const
{
    LoginHelper::loginWithoutAppConf();
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
