#include "review.h"
#include <QJsonObject>
#include <QJsonDocument>

using namespace Qt::StringLiterals;

namespace
{
constexpr int invalidId{-1};
constexpr auto reviewField{"review"_L1};
constexpr auto idField{"id"_L1};
constexpr auto productIdField{"product_id"_L1};
constexpr auto customerIdField{"customer_id"_L1};
constexpr auto reviewTextField{"review_text"_L1};
constexpr auto ratingField{"rating"_L1};
constexpr auto createdAtField{"created_at"_L1};
constexpr auto reviewerField{"reviewer"_L1};
}

namespace Review
{

std::optional<Review> Review::fromJson(const QJsonDocument &json)
{
    if (json.isEmpty() || !json.isObject())
        return std::nullopt;

    Review r;
    r.setReviewId(json[idField].toInt());
    r.setCustomerId(json[customerIdField].toInt());
    r.setProductId(json[productIdField].toInt());
    r.setReviewText(json[reviewTextField].toString());
    r.setCreatedAt(QDateTime::fromString(json[createdAtField].toString(), Qt::ISODate));
    r.setRating(json[ratingField].toInt());
    r.setReviewer(json[reviewerField].toString());
    return r;
}

Review::Review()
    : reviewId_{invalidId}
    , productId_{invalidId}
    , customerId_{invalidId}
    , reviewText_{}
    , rating_{}
    , createdAt_{}
    , reviewer_{}
{}

Review::ProductId Review::productId() const
{
    return productId_;
}

void Review::setProductId(ProductId newProductId)
{
    if (productId_ == newProductId)
        return;
    productId_ = newProductId;
}

Review::CustomerId Review::customerId() const
{
    return customerId_;
}

void Review::setCustomerId(CustomerId newCustomerId)
{
    if (customerId_ == newCustomerId)
        return;
    customerId_ = newCustomerId;
}

QString Review::reviewText() const
{
    return reviewText_;
}

void Review::setReviewText(const QString &newReviewText)
{
    if (reviewText_ == newReviewText)
        return;
    reviewText_ = newReviewText;
}

int Review::rating() const
{
    return rating_;
}

void Review::setRating(int newRating)
{
    if (rating_ == newRating)
        return;
    rating_ = newRating;
}

QDateTime Review::createdAt() const
{
    return createdAt_;
}

void Review::setCreatedAt(const QDateTime &newCreatedAt)
{
    if (createdAt_ == newCreatedAt)
        return;
    createdAt_ = newCreatedAt;
}

QJsonDocument Review::toJson() const
{
    QJsonObject obj
    {
        {customerIdField, customerId_},
        {productIdField, productId_},
        {reviewTextField, reviewText_},
        {ratingField, rating_},
    };
    if (reviewId_ > 0)
        obj[idField] = reviewId_;

    return QJsonDocument{obj};
}

Review::ReviewId Review::reviewId() const
{
    return reviewId_;
}

void Review::setReviewId(ReviewId newReviewId)
{
    if (reviewId_ == newReviewId)
        return;
    reviewId_ = newReviewId;
}

QString Review::reviewer() const
{
    return reviewer_;
}

void Review::setReviewer(const QString &newReviewer)
{
    if (reviewer_ == newReviewer)
        return;
    reviewer_ = newReviewer;
}

} // namespace Review
