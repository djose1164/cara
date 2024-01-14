#include "user/entity/usercredential.h"
#include <QtTest>
#include <QSignalSpy>

// add necessary includes here

class TestUserCredential : public QObject
{
    Q_OBJECT

public:
    TestUserCredential();
    ~TestUserCredential();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void test_shouldReturnTrueIfCredentailsAreValid() const;
    void test_shouldReturnFalseIfPasswordIsEmpty() const;
    void test_shouldReturnFalseIfUsernameIsEmpty() const;
    void test_shouldReturnFalseIfPasswordAndUsernameAreEmpty() const;
};

TestUserCredential::TestUserCredential() {}

TestUserCredential::~TestUserCredential() {}

void TestUserCredential::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");
}

void TestUserCredential::cleanupTestCase()
{

}

void TestUserCredential::test_shouldReturnTrueIfCredentailsAreValid() const
{
    User::Entity::UserCredential credentials{};
    credentials.setUsername("cara");
    credentials.setPassword("1234");

    QVERIFY(credentials.isValid());
}

void TestUserCredential::test_shouldReturnFalseIfPasswordIsEmpty() const
{
    User::Entity::UserCredential credentials{};
    credentials.setUsername("cara");
    credentials.setPassword("");

    QVERIFY(credentials.isValid() == false);
}

void TestUserCredential::test_shouldReturnFalseIfUsernameIsEmpty() const
{
    User::Entity::UserCredential credentials{};
    credentials.setUsername("");
    credentials.setPassword("1234");

    QVERIFY(credentials.isValid() == false);
}

void TestUserCredential::test_shouldReturnFalseIfPasswordAndUsernameAreEmpty() const
{
    User::Entity::UserCredential credentials{};
    credentials.setUsername("");
    credentials.setPassword("");

    QVERIFY(credentials.isValid() == false);
}

QTEST_MAIN(TestUserCredential)

#include "tst_usercredential.moc"
