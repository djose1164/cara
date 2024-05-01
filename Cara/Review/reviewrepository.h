#ifndef REVIEWREPOSITORY_H
#define REVIEWREPOSITORY_H

#include "review.h"

#include <QQmlEngine>

namespace Review
{
class ReviewRepository : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    using CustomerId = int;
    using ProductId = int;
    using ReviewId = int;

    using QObject::QObject;

    virtual ~ReviewRepository() = default;

    virtual void postReview(const Review &review) = 0;

    virtual void fetchReviewByCustomerIdAndProductId(CustomerId customerId, ProductId productId) = 0;

    virtual void fetchReview(ReviewId reviewId) = 0;

    virtual void fetchReviewsByProductId(ProductId productId) = 0;

    virtual void deleteReview(ReviewId reviewId) = 0;

    virtual void editReview(const Review &r) = 0;

    virtual void canCustomerReview(CustomerId customerId, ProductId productId) = 0;

signals:
    void reviewPosted(const Review &review);

    void reviewFetched(const Review &review);

    void reviewsFetched(const QList<Review> &review);

    void reviewDeleted();

    void reviewEdited();

    void errorOccurred(const QByteArray &resp);

    void customerAbleToReview();

    void customerDisableToReview();
};
}

#endif // REVIEWREPOSITORY_H
