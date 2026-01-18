#include "Replication/abstractreplicationengine.h"
#include "Replication/replicationengine.h"
#include "Replication/userreplicator.h"
#include "sqlitedatabase.h"
#include <QJsonObject>
#include <QSignalSpy>
#include <QSqlQuery>
#include <QTest>

using namespace Replication;
using namespace Qt::StringLiterals;

class TestReplicationEngine : public QObject
{
    Q_OBJECT

public:
    TestReplicationEngine();
    ~TestReplicationEngine();

private slots:
    void initTestCase();
    void init();
    void cleanupTestCase();
    void cleanup();
    void test_run();
    void test_registerReplicator();

private:
    std::unique_ptr<AbstractReplicationEngine> engine_;
};

TestReplicationEngine::TestReplicationEngine()
{
}

TestReplicationEngine::~TestReplicationEngine()
{
}

void TestReplicationEngine::initTestCase()
{
    // code to be executed before the first test function
    ProductInventory::SQLiteDatabase::initDatabase();
    QSqlQuery query;
    query.exec("delete from user where admin_id = 10");
}

void TestReplicationEngine::init()
{
    // code to be executed before each test function
    engine_ = std::make_unique<ReplicationEngine>();
}

void TestReplicationEngine::cleanupTestCase()
{
    // code to be executed after the last test function
}

void TestReplicationEngine::cleanup()
{
    // code to be executed after each test function
}

void TestReplicationEngine::test_run()
{
    QSignalSpy spy{engine_.get(), &AbstractReplicationEngine::finished};
    ReplicationMessageList messages;
    QJsonObject payload{{"username"_L1, "holaaaa"_L1},
                        {"forename"_L1, "Hola"_L1},
                        {"surname"_L1, "Adios"_L1},
                        {"adminId"_L1, 10}};
    ReplicationMessage msg = {.entity = "user"_L1, .payload = payload};
    messages.append(msg);

    QVERIFY(spy.isValid());
    engine_->registerReplicator(new UserReplicator);
    engine_->run(messages);

    QCOMPARE(spy.count(), 1);
}

void TestReplicationEngine::test_registerReplicator()
{
    const auto userReplicator{new UserReplicator};

    engine_->registerReplicator(userReplicator);

    QVERIFY2(engine_->getReplicatorByEntity(userReplicator->entityName()),
             "The replicator wasn't registered.");
}

QTEST_MAIN(TestReplicationEngine)

#include "tst_replicationengine.moc"
