#include "localuserstore.h"
#include "login.h"
#include "data/restcustomerrepository.h"
#include <QTest>
#include <QSignalSpy>
#include <QJsonDocument>
#include <QJsonObject>

class TestRestCustomerRepository : public QObject
{
    Q_OBJECT
public:
    explicit TestRestCustomerRepository(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void init();

    void test_shouldReturnCustomersByTheirSalespersonId() const;
    void test_shouldEmitErrorOccurredIfSalespersonidIsNotValid() const;
};



TestRestCustomerRepository::TestRestCustomerRepository(QObject *parent)
    : QObject{parent}
{

}

void TestRestCustomerRepository::initTestCase()
{
    QCoreApplication::setOrganizationDomain("cara.com");
    QCoreApplication::setOrganizationName("Cara");
    QCoreApplication::setApplicationName("CaraTesting");
}

void TestRestCustomerRepository::init()
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

void TestRestCustomerRepository::test_shouldReturnCustomersByTheirSalespersonId() const
{
    Data::RestCustomerRepository repository{};
    QSignalSpy spy{&repository, &Data::RestCustomerRepository::fetched};

    QVERIFY(spy.isValid());

    repository.readBySalespersonId(1);

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);

    const QString errorString{repository.errorString()};

    QVERIFY2(errorString.isEmpty(), QString("An error has occurred: %1").arg(errorString).toUtf8());
}

void TestRestCustomerRepository::test_shouldEmitErrorOccurredIfSalespersonidIsNotValid() const
{
    Data::RestCustomerRepository repository{};
    QSignalSpy spy{&repository, SIGNAL(errorOccurred(void))};

    QVERIFY(spy.isValid());

    repository.readBySalespersonId(0);

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);

    const QString errorString{repository.errorString()};

    QVERIFY(errorString.isEmpty() == false);
}

QTEST_MAIN(TestRestCustomerRepository)

#include "tst_restcustomerrepository.moc"
