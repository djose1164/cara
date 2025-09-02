#include <QSignalSpy>
#include <QTest>
#include "loginhelper.h"
#include "ordercommentrepository.h"
#include <dotenv.h>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto contentTemplate{"Hello we're testing! %1"_L1};
}

class TestOrderCommentRepository : public QObject
{
    Q_OBJECT
public:
    TestOrderCommentRepository(QObject *parent = nullptr);

private slots:
    void initTestCase();

    void test_shouldEmitCommentsFetchedIfOrderIdIsValid() const;
    void test_shouldEmitCommentAddedIfCommentWasCreated() const;
};



TestOrderCommentRepository::TestOrderCommentRepository(QObject *parent)
    : QObject(parent)
{}

void TestOrderCommentRepository::initTestCase()
{
    LoginHelper::login();
}

void TestOrderCommentRepository::test_shouldEmitCommentsFetchedIfOrderIdIsValid() const
{
    constexpr int orderId{100};
    const std::unique_ptr<User::CommentRepository> repository{
        std::make_unique<User::OrderCommentRepository>()};

    QSignalSpy spy{repository.get(), &User::CommentRepository::commentsFetched};
    QVERIFY(spy.isValid());

    repository->fetchCommentsByOrderId(orderId);
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

void TestOrderCommentRepository::test_shouldEmitCommentAddedIfCommentWasCreated() const
{
    constexpr int orderId{100};
    const auto content{contentTemplate.arg(QDateTime::currentDateTime().toString(Qt::ISODate))};
    const auto userId{QString::fromStdString(dotenv::getenv("USER_ID_TEST"))};
    User::Comment c;
    c.setUserId(userId.toInt());
    c.setContent(content);
    QVERIFY(c.isValid());

    const std::unique_ptr<User::CommentRepository> repository{
        std::make_unique<User::OrderCommentRepository>()};

    QSignalSpy spy{repository.get(), &User::CommentRepository::commentAdded};
    QVERIFY(spy.isValid());

    repository->addCommentToOrder(orderId, c);
    QVERIFY(spy.wait());

    QCOMPARE(spy.count(), 1);
}

QTEST_MAIN(TestOrderCommentRepository)

#include "tst_ordercommentrepository.moc"
