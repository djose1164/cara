#include "Replication/userreplicator.h"
#include "sqlitedatabase.h"
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>
#include <QSqlQuery>

using namespace Replication;
using namespace Qt::StringLiterals;

class TestUserReplicator : public QObject
{
    Q_OBJECT

public:
    TestUserReplicator();
    ~TestUserReplicator();

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();
    void test_apply();
    void test_entityName();
};

TestUserReplicator::TestUserReplicator()
{
}

TestUserReplicator::~TestUserReplicator()
{
}

void TestUserReplicator::initTestCase()
{
    // code to be executed before the first test function
    ProductInventory::SQLiteDatabase::initDatabase();
}

void TestUserReplicator::init()
{
    // code to be executed before each test function
}

void TestUserReplicator::cleanupTestCase()
{
    // code to be executed after the last test function
}

void TestUserReplicator::cleanup()
{
    // code to be executed after each test function
}

void TestUserReplicator::test_apply()
{
    std::unique_ptr<EntityReplicator> replicator = std::make_unique<UserReplicator>();
    QSignalSpy spy{replicator.get(), &EntityReplicator::finished};
    QJsonObject payload{{"username"_L1, "hola"_L1},
                        {"forename"_L1, "Hola"_L1},
                        {"surname"_L1, "Adios"_L1},
                        {"adminId"_L1, 5}};

    QVERIFY(spy.isValid());
    replicator->apply(payload);

    QCOMPARE(spy.count(), 1);
}

void TestUserReplicator::test_entityName()
{
    std::unique_ptr<EntityReplicator> replicator = std::make_unique<UserReplicator>();

    QCOMPARE(replicator->entityName(), "user"_L1);
}

QTEST_MAIN(TestUserReplicator)

#include "tst_userreplicator.moc"
