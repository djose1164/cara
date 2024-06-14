#include "restreviewrepository.h"
#include "Network/networkrequestsingleton.h"
#include <QRestAccessManager>
#include <QRestReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto path{"reviews/"_L1};
constexpr auto idField{"id"_L1};
constexpr auto customerIdField{"customer_id"_L1};
constexpr auto productIdField{"product_id"_L1};
constexpr auto reviewField{"review"_L1};
constexpr auto reviewsField{"reviews"_L1};
constexpr auto canReviewPath{"can_review"_L1};
}

namespace Review
{
RestReviewRepository::RestReviewRepository(QObject *parent)
    : ReviewRepository{parent}
    , api_{Network::NetworkRequestSingleton::instance().api()}
    , qnam_{}
    , manager_{new QRestAccessManager{&qnam_, this}}
{
}

void RestReviewRepository::postReview(const Review &review)
{
    manager_->post(api_->createRequest(path), review.toJson(), this, [&](QRestReply &reply) {
        const auto json{reply.readJson()};
        if (!reply.isSuccess())
        {
            emit errorOccurred(json ? json->toJson() : QByteArray{});
            return;
        }

        if (json && json->isObject() && json->object().contains(reviewField))
        {
            const auto newReview{
                Review::fromJson(
                    QJsonDocument{(*json)[reviewField].toObject()}
                )
            };
            if (newReview)
                emit reviewPosted(*newReview);
            else
                emit errorOccurred("review entity couldn't be made."_ba);
        }
        else
        {
            emit errorOccurred("review field is not present."_ba);
        }
    });
}

void RestReviewRepository::fetchReviewByCustomerIdAndProductId(CustomerId customerId,
                                                               ProductId productId)
{
    const QUrlQuery query
    {
        {customerIdField, QString::number(customerId)},
        {productIdField, QString::number(productId)}
    };
    manager_->get(api_->createRequest(path, query), this, [&](QRestReply &reply){
        if (const auto json{reply.readJson()};
            reply.isSuccess() && json && json->isObject() && json->object().contains(reviewField))
        {
            const auto reviewValue{(*json)[reviewField]};
            if (!reviewValue.isObject())
            {
                emit errorOccurred("review property is not an object."_ba);
                return;
            }

            const auto reviewJson{QJsonDocument(reviewValue.toObject())};
            if (const auto review{Review::fromJson(reviewJson)};
                review)
            {
                emit reviewFetched(*review);
                return;
            }
            else
            {
                emit errorOccurred("review entity couldn't be made."_ba);
                return;
            }
        }
    });
}

void RestReviewRepository::fetchReview(ReviewId reviewId) {}

void RestReviewRepository::fetchReviewsByProductId(ProductId productId)
{
    const QUrlQuery query{ {productIdField, QString::number(productId)} };

    manager_->get(api_->createRequest(path, query), this, [&](QRestReply &reply) {
        if (const auto json{reply.readJson()};
            json && json->isObject() && json->object().contains(reviewsField))
        {
            if (!(*json)[reviewsField].isArray())
            {
                emit errorOccurred("reviews property is not an array."_ba);
                return;
            }

            const auto reviewsArr = (*json)[reviewsField].toArray();
            QList<Review> reviewsList{reviewsArr.size()};

            std::transform(
                reviewsArr.begin(),
                reviewsArr.end(),
                reviewsList.begin(),
                [](const auto &jsonValue) {
                    return Review::fromJson(QJsonDocument{jsonValue.toObject()})
                        .value_or(Review{});
                }
            );

            emit reviewsFetched(reviewsList);
        }
    });
}

void RestReviewRepository::deleteReview(ReviewId reviewId)
{
    manager_->deleteResource(
        api_->createRequest(path + QString::number(reviewId)),
        this,
        [this](QRestReply &reply)
        {
            if (reply.isSuccess())
                emit reviewDeleted();
            else
                emit errorOccurred(reply.readBody());
        }
    );
}

void RestReviewRepository::editReview(const Review &r)
{
    manager_->patch(
        api_->createRequest(path + QString::number(r.reviewId())),
        r.toJson(),
        this,
        [this](QRestReply &reply) {
            if (reply.isSuccess())
                emit reviewEdited();
            else
                emit errorOccurred(reply.readBody());
        }
    );
}

void RestReviewRepository::canCustomerReview(CustomerId customerId, ProductId productId)
{
    const QUrlQuery query
    {
        {customerIdField, QString::number(customerId)},
        {productIdField, QString::number(productId)}
    };

    manager_->get(api_->createRequest(path + canReviewPath, query), this, [this](QRestReply &reply) {
        if (reply.isSuccess())
            emit customerAbleToReview();
        else
            emit customerDisableToReview();
    });
}
}
