#include "Authentication/data/restuserauthentication.h"
#include "Authentication/login.h"
#include "Salesperson/salespersoncontroller.h"
#include "data/localuserstore.h"
#include "loginhelper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>

class TestSalespersonController : public QObject
{
    Q_OBJECT
public:
    explicit TestSalespersonController(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void test_shouldGetSalespersonById() const;
    void test_shouldGetSalespeopleByAdminId() const;
};



TestSalespersonController::TestSalespersonController(QObject *parent)
    : QObject{parent}
{

}

void TestSalespersonController::initTestCase()
{
    LoginHelper::login();
}

void TestSalespersonController::test_shouldGetSalespersonById() const
{
    Salesperson::SalespersonController spController{};
    spController.setSalespersonId(1);
    QSignalSpy spy{&spController, SIGNAL(fetched(void))};

    QVERIFY(spy.isValid());

    spController.readSalesperson();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
    QVERIFY(spController.data().isEmpty() == false);

    const auto obj{
        QJsonDocument::fromJson(spController.data())
            .object()
    };

    QVERIFY(obj.contains("salesperson"));

    const auto doc{QJsonDocument(obj["salesperson"].toObject())};
    const auto sp{Entity::Salesperson::fromJson(doc.toJson())};

    QVERIFY(sp.isValid());
}

void TestSalespersonController::test_shouldGetSalespeopleByAdminId() const
{
    Salesperson::SalespersonController spController{};
    spController.setAdminId(5);
    QSignalSpy spy{&spController, SIGNAL(fetched(void))};

    QVERIFY(spy.isValid());

    spController.readSalespeople();

    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
    QVERIFY(spController.data().isEmpty() == false);

    const auto obj{
        QJsonDocument::fromJson(spController.data())
            .object()
    };

    QVERIFY(obj.contains("salespersons"));

    const bool result{obj["salespersons"].isArray()};

    QVERIFY(result);
}

QTEST_MAIN(TestSalespersonController)

#include "tst_salespersoncontroller.moc"
