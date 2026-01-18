#include "Replication/bootstrapreplicationservice.h"
#include "sqlitedatabase.h"
#include <QJsonObject>
#include <QSignalSpy>
#include <QSqlQuery>
#include <QTest>

using namespace Replication;
using namespace Qt::StringLiterals;

class TestBootstrapReplicationService : public QObject
{
    Q_OBJECT

public:
    TestBootstrapReplicationService();
    ~TestBootstrapReplicationService();

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();

    void test_replicationForUser() const;
};

TestBootstrapReplicationService::TestBootstrapReplicationService()
{
}

TestBootstrapReplicationService::~TestBootstrapReplicationService()
{
}

void TestBootstrapReplicationService::initTestCase()
{
    // code to be executed before the first test function
    ProductInventory::SQLiteDatabase::initDatabase();

    QSqlQuery query;
    query.exec("delete from user where admin_id = 10");
}

void TestBootstrapReplicationService::init()
{
    // code to be executed before each test function
}

void TestBootstrapReplicationService::cleanupTestCase()
{
    // code to be executed after the last test function
}

void TestBootstrapReplicationService::cleanup()
{
    // code to be executed after each test function
}


void TestBootstrapReplicationService::test_replicationForUser() const
{
    const auto replicationService{std::make_unique<BootstrapReplicationService>()};
    QSignalSpy spy{replicationService.get(), &BootstrapReplicationService::finished};
    QJsonObject payload{{"username"_L1, "TestBootstrapReplicationService"_L1},
                        {"forename"_L1, "Hola"_L1},
                        {"surname"_L1, "Adios"_L1},
                        {"adminId"_L1, 10}};

    QVERIFY(spy.isValid());
    replicationService->replicateForUser(payload.toVariantMap());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestBootstrapReplicationService)

#include "tst_bootstrapreplicationservice.moc"
