#ifndef RESTREVIEWREPOSITORY_H
#define RESTREVIEWREPOSITORY_H

#include "reviewrepository.h"

#include <QQmlEngine>

class QNetworkRequestFactory;
class QRestAccessManager;

namespace Review
{

class RestReviewRepository : public ReviewRepository
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit RestReviewRepository(QObject *parent = nullptr);

    // ReviewRepository interface
public:
    void postReview(const Review &review) override;
    void fetchReviewByCustomerIdAndProductId(CustomerId customerId, ProductId productId) override;
    void fetchReview(ReviewId reviewId) override;
    void fetchReviewsByProductId(ProductId productId) override;
    void deleteReview(ReviewId reviewId) override;
    void editReview(const Review &r) override;
    void canCustomerReview(CustomerId customerId, ProductId productId) override;

private:
    std::shared_ptr<QNetworkRequestFactory> api_;
    QRestAccessManager *manager_;
};

}

#endif // RESTREVIEWREPOSITORY_H
