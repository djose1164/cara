#ifndef REVIEWCONTROLLER_H
#define REVIEWCONTROLLER_H

#include "reviewrepository.h"
#include <QObject>
#include <QQmlEngine>

namespace Review
{
class ReviewController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged FINAL)
public:
    using ReviewId = ReviewRepository::ReviewId;
    using CustomerId = ReviewRepository::CustomerId;
    using ProductId = ReviewRepository::ProductId;

    explicit ReviewController(QObject *parent = nullptr);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

public slots:
    void postReview(const Review &review);

    void editReview(const Review &r);

    void fetchReviewByCustomerIdAndProductId(CustomerId customerId, ProductId productId);

    void fetchReview(ReviewId reviewId);

    void fetchReviewsByProductId(ProductId productId);

    void deleteReview(ReviewId reviewId);

    void canCustomerReview(CustomerId customerId, ProductId productId);

signals:
    void errorStringChanged();

    void errorOccurred(const QString &errorString);

    void reviewPosted(const Review &review);

    void reviewFetched(const Review &review);

    void reviewsFetched(const QList<Review> &review);

    void reviewDeleted();

    void reviewEdited();

    void reviewDisabled();

    void customerAbleToReview();

    void customerDisableToReview();

private:
    bool validateReview(const QVariantMap &review);

private:
    ReviewRepository *repository_;
    QString errorString_;
};
}

#endif // REVIEWCONTROLLER_H
