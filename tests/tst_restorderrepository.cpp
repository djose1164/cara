#include "data/restorderrepository.h"
#include "localuserstore.h"
#include "login.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::StringLiterals;

class TestRestOrderRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestRestOrderRepository(QObject *parent = nullptr);

    QByteArray readJsonFile(const QLatin1StringView path) const;

private slots:
    void initTestCase();
    void init();

    void test_shouldEmitResourceAddedIfUsingGoodOrder() const;
    void test_shouldEmitErrorOccurredIfUsingBadOrder() const;

private:
    QJsonObject goodOrder_;
    QJsonObject badOrder_;
};



TestRestOrderRepository::TestRestOrderRepository(QObject *parent)
    : QObject{parent}
{

}

QByteArray TestRestOrderRepository::readJsonFile(const QLatin1StringView path) const
{
    QFile file{path};

    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "couldn't open file.";
        return {};
    };

    const auto jsonArrayByte{file.readAll()};
    file.close();

    return jsonArrayByte;
}

void TestRestOrderRepository::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");

    Business::Login login{};
    login.setUsername("josevictoriano");
    login.setPassword("2077120");

    Business::LocalUserStore userStore{};
    QSignalSpy spy{&login, SIGNAL(authenticated(void))};
    login.setUserStore(&userStore);

    login.authenticateCredentials();
    QVERIFY(spy.wait());
}

void TestRestOrderRepository::init()
{

}

void TestRestOrderRepository::test_shouldEmitResourceAddedIfUsingGoodOrder() const
{
    Data::RestOrderRepository repository{};
    QSignalSpy spy{&repository, &Data::RestOrderRepository::resourceAdded};

    const auto jsonDoc{QJsonDocument::fromJson(readJsonFile(":/good_order.json"_L1))};

    QVERIFY(jsonDoc.isObject());

    const auto jsonObject{jsonDoc.object()};

    QVERIFY(jsonObject.isEmpty() == false);

    repository.add(jsonObject.toVariantMap());
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestRestOrderRepository::test_shouldEmitErrorOccurredIfUsingBadOrder() const
{
    Data::RestOrderRepository repository{};
    QSignalSpy spy{&repository, &Data::RestOrderRepository::errorOccurred};

    const auto jsonDoc{QJsonDocument::fromJson(readJsonFile(":/bad_order.json"_L1))};

    QVERIFY(jsonDoc.isObject());

    const auto jsonObject{jsonDoc.object()};

    QVERIFY(jsonObject.isEmpty() == false);

    repository.add(jsonObject.toVariantMap());
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestRestOrderRepository)

#include "tst_restorderrepository.moc"
