#include "reviewmodel.h"
#include "restreviewrepository.h"

using namespace Qt::StringLiterals;

namespace Review
{

ReviewModel::ReviewModel(QObject *parent)
    : QAbstractListModel{parent}
    , repository_{new RestReviewRepository{this}}
    , data_{}
{
    connect(repository_, &ReviewRepository::reviewsFetched,
            this, &ReviewModel::reviewsFetched);
}

int ReviewModel::rowCount(const QModelIndex &parent) const
{
    return data_.count();
}

QVariant ReviewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant{};

    const int row{index.row()};
    switch (static_cast<RoleNames>(role))
    {
        using enum RoleNames;
    case CustomerIdRole:
        return data_.at(row).customerId();
    case ProductIdRole:
        return data_.at(row).productId();
    case ReviewTextRole:
        return data_.at(row).reviewText();
    case RatingRole:
        return data_.at(row).rating();
    case CreatedAtRole:
        return data_.at(row).createdAt();
    case ReviewerRole:
        return data_.at(row).reviewer();
    default:
        return QVariant{};
    }
}

QHash<int, QByteArray> ReviewModel::roleNames() const
{
    using enum RoleNames;
    static QHash<int, QByteArray> hash
    {
        {int(ProductIdRole), "productId"_ba},
        {int(CustomerIdRole), "customerId"_ba},
        {int(ReviewTextRole), "reviewText"_ba},
        {int(RatingRole), "rating"_ba},
        {int(CreatedAtRole), "createdAt"_ba},
        {int(ReviewerRole), "reviewer"_ba}
    };
    return hash;
}

ReviewRepository *ReviewModel::repository() const
{
    return repository_;
}

void ReviewModel::setRepository(ReviewRepository *newRepository)
{
    if (repository_ == newRepository)
        return;
    repository_ = newRepository;
    emit repositoryChanged();
}

void ReviewModel::fetchReviewsByProductId(ReviewRepository::ProductId productId)
{
    repository_->fetchReviewsByProductId(productId);
}

void ReviewModel::reviewsFetched(const QList<Review> &reviews)
{
    beginResetModel();
    data_ = reviews;
    endResetModel();
}

} // namespace Review
