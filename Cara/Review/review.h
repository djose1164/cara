#ifndef REVIEW_H
#define REVIEW_H

//#include <QDateTime>
#include <QQmlEngine>

namespace Review
{

class Review
{
    QML_VALUE_TYPE(review)
    QML_STRUCTURED_VALUE
    Q_GADGET
    Q_PROPERTY(ReviewId reviewId READ reviewId WRITE setReviewId FINAL)
    Q_PROPERTY(int productId READ productId WRITE setProductId FINAL)
    Q_PROPERTY(int customerId READ customerId WRITE setCustomerId FINAL)
    Q_PROPERTY(QString reviewText READ reviewText WRITE setReviewText FINAL)
    Q_PROPERTY(int rating READ rating WRITE setRating FINAL)
    Q_PROPERTY(QDateTime createdAt READ createdAt WRITE setCreatedAt FINAL)
    Q_PROPERTY(QString reviewer READ reviewer WRITE setReviewer FINAL)
public:
    using ProductId = int;
    using CustomerId = int;
    using ReviewId = int;

    static std::optional<Review> fromJson(const QJsonDocument &json);

    Review();

    ProductId productId() const;
    void setProductId(ProductId newProductId);

    CustomerId customerId() const;
    void setCustomerId(CustomerId newCustomerId);

    QString reviewText() const;
    void setReviewText(const QString &newReviewText);

    int rating() const;
    void setRating(int newRating);

    QDateTime createdAt() const;
    void setCreatedAt(const QDateTime &newCreatedAt);

    QJsonDocument toJson() const;

    ReviewId reviewId() const;
    void setReviewId(ReviewId newReviewId);

    QString reviewer() const;
    void setReviewer(const QString &newReviewer);

private:
    ReviewId reviewId_;
    ProductId productId_;
    CustomerId customerId_;
    QString reviewText_;
    int rating_;
    QDateTime createdAt_;
    QString reviewer_;
};

} // namespace Review

#endif // REVIEW_H
