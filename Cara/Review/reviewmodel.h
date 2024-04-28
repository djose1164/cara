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
    Q_PROPERTY(Status status READ status WRITE setStatus NOTIFY statusChanged FINAL)
    Q_PROPERTY(ReviewRepository *repository READ repository NOTIFY repositoryChanged FINAL)
    QML_ELEMENT
public:
    enum class RoleNames
    {
        ProductIdRole = Qt::UserRole,
        CustomerIdRole,
        ReviewTextRole,
        RatingRole,
        CreatedAtRole,
        ReviewerRole,
        ReviewIdRole
    };

    enum class Status
    {
        Fetching,
        Fetched,
        Empty,
        Error
    };
    Q_ENUM(Status)

    explicit ReviewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    ReviewRepository *repository() const;
    void setRepository(ReviewRepository *newRepository);

    Status status() const;
    void setStatus(Status newStatus);

public slots:
    void fetchReviewsByProductId(ReviewRepository::ProductId productId);
    void remove(int index);

private slots:
    void reviewsFetched(const QList<Review>& reviews);

signals:
    void repositoryChanged();

    void statusChanged();

private:
    ReviewRepository *repository_;
    QList<Review> data_;
    Status status_;
};

} // namespace Review

#endif // REVIEWMODEL_H
