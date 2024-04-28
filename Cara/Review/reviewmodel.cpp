#include "reviewmodel.h"
#include "restreviewrepository.h"

using namespace Qt::StringLiterals;

namespace Review
{

ReviewModel::ReviewModel(QObject *parent)
    : QAbstractListModel{parent}
    , repository_{new RestReviewRepository{this}}
    , data_{}
    , status_{Status::Empty}
{
    connect(repository_, &ReviewRepository::reviewsFetched,
            this, &ReviewModel::reviewsFetched);
    connect(repository_, &ReviewRepository::errorOccurred,
            this, [this] { setStatus(Status::Error); });
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
    case ReviewIdRole:
        return data_.at(row).reviewId();
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
        {int(ReviewerRole), "reviewer"_ba},
        {int(ReviewIdRole), "reviewId"_ba}
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
    setStatus(Status::Fetching);
    repository_->fetchReviewsByProductId(productId);
}

void ReviewModel::remove(int index)
{
    if (index < 0 || index >= data_.count())
        return;

    beginRemoveRows(QModelIndex{}, index, index);
    data_.removeAt(index);
    endRemoveRows();
}

void ReviewModel::reviewsFetched(const QList<Review> &reviews)
{
    beginResetModel();
    data_ = reviews;
    endResetModel();

    setStatus(Status::Fetched);
}

ReviewModel::Status ReviewModel::status() const
{
    return status_;
}

void ReviewModel::setStatus(Status newStatus)
{
    if (status_ == newStatus)
        return;
    status_ = newStatus;
    emit statusChanged();
}

} // namespace Review
