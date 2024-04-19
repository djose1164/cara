#include "reviewcontroller.h"
#include "restreviewrepository.h"

namespace Review
{

ReviewController::ReviewController(QObject *parent)
    : QObject{parent}
    , repository_{new RestReviewRepository{this}}
    , errorString_{}
{
    connect(repository_, &ReviewRepository::reviewPosted,
            this, &ReviewController::reviewPosted);
    connect(repository_, &ReviewRepository::reviewFetched,
            this, &ReviewController::reviewFetched);
    connect(repository_, &ReviewRepository::errorOccurred,
            this, &ReviewController::errorOccurred);
    connect(repository_, &ReviewRepository::customerDisableToReview,
            this, &ReviewController::customerDisableToReview);
    connect(repository_, &ReviewRepository::customerAbleToReview,
            this, &ReviewController::customerAbleToReview);
    connect(repository_, &ReviewRepository::reviewDeleted,
            this, &ReviewController::reviewDeleted);
    connect(repository_, &ReviewRepository::reviewEdited,
            this, &ReviewController::reviewEdited);
}

QString ReviewController::errorString() const
{
    return errorString_;
}

void ReviewController::setErrorString(const QString &newErrorString)
{
    if (errorString_ == newErrorString)
        return;
    errorString_ = newErrorString;
    emit errorStringChanged();
}

void ReviewController::postReview(const Review &review)
{
    if (review.customerId() > 0 && review.productId() > 0 && !review.reviewText().isEmpty())
        repository_->postReview(review);
    else
        emit errorOccurred(QStringLiteral("Invalid review."));
}

void ReviewController::editReview(const Review &r)
{
    if (r.reviewId() > 0)
        repository_->editReview(r);
    else
        emit errorOccurred(QStringLiteral("reviewId has to be greater than zero."));
}

void ReviewController::fetchReviewByCustomerIdAndProductId(CustomerId customerId,
                                                           ProductId productId)
{
    repository_->fetchReviewByCustomerIdAndProductId(customerId, productId);
}

void ReviewController::fetchReview(ReviewId reviewId)
{

}

void ReviewController::fetchReviewsByProductId(ProductId productId)
{
    repository_->fetchReviewsByProductId(productId);
}

void ReviewController::deleteReview(ReviewId reviewId)
{
    repository_->deleteReview(reviewId);
}

void ReviewController::canCustomerReview(CustomerId customerId, ProductId productId)
{
    repository_->canCustomerReview(customerId, productId);
}

}
