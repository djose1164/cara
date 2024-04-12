#ifndef REVIEWMODEL_H
#define REVIEWMODEL_H

#include "review.h"
#include "reviewrepository.h"
#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

namespace Review
{

class ReviewModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(ReviewRepository *repository READ repository NOTIFY repositoryChanged FINAL)
public:
    enum class RoleNames
    {
        ProductIdRole = Qt::UserRole,
        CustomerIdRole,
        ReviewTextRole,
        RatingRole,
        CreatedAtRole,
        ReviewerRole
    };
    explicit ReviewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    ReviewRepository *repository() const;
    void setRepository(ReviewRepository *newRepository);

public slots:
    void fetchReviewsByProductId(ReviewRepository::ProductId productId);

private slots:
    void reviewsFetched(const QList<Review>& reviews);

signals:
    void repositoryChanged();

private:
    ReviewRepository *repository_;
    QList<Review> data_;
};

} // namespace Review

#endif // REVIEWMODEL_H
