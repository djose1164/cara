#include "pricehistorymodel.h"

#include <QRestReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto path{"products/%1/price_history"_L1};
constexpr auto priceHistoryField{"price_history"_L1};
constexpr auto startDateField{"start_date"_L1};
constexpr auto thruDateField{"thru_date"_L1};

/**
 * @brief dateTimeWithTZ Converts a datetime ISO string to a QDateTime obect.
 * @param dateString A valid datetime represented in ISO
 * @return A QDateTime obect
 */
QDateTime dateTimeWithTZ(QStringView dateString)
{
    auto &&date{QDateTime::fromString(dateString, Qt::ISODate)};
    date.setTimeZone(QTimeZone{QTimeZone::UTC});
    return date;
}

QString priceTypeToReadableString(QStringView priceType)
{
    constexpr auto templateStr{"Precio de %1"_L1};
    if (priceType == "sell"_L1)
        return templateStr.arg("Venta"_L1);
    else if (priceType == "buy"_L1)
        return templateStr.arg("Compra"_L1);
    return "Error: Unkwon type: %1"_L1.arg(priceType);
}
}

namespace Product
{

PriceHistoryModel::PriceHistoryModel(QObject *parent)
    : QAbstractListModel{parent}
    , repository_{}
    , errorString_{}

{
}

QBindable<QString> PriceHistoryModel::bindableErrorString()
{
    return &errorString_;
}

void PriceHistoryModel::addNewPrice(const QVariantMap &newPrice) noexcept
{
    if (const int price{newPrice["price"].toInt()}; price <= 0)
    {
        using enum PriceHistoryRepository::Error;
        errorString_ = "Invalid price: %1."_L1.arg(QString::number(price));
        emit errorOccurred(FailedToAddNewPrice, errorString_);
        return;
    }

    repository_->addNewPrice(newPrice);
}

void PriceHistoryModel::fetchPriceHistoryBy(ProductId productId)
{
    if (productId < 0)
    {
        return;
    }

    repository_->fetchPriceHistoryBy(productId);
}

void PriceHistoryModel::newPriceAdded(const QByteArray &data)
{
    qDebug() << "PriceHistoryModel::newPriceAdded: " << QJsonDocument::fromJson(data);
    QJsonObject &oldLatest{data_.last()};
    QJsonObject &&newLatest{QJsonDocument::fromJson(data).object()};

    oldLatest[thruDateField] = newLatest[startDateField].toString();

    const QModelIndex idx{index(data_.count() - 1)};
    emit dataChanged(idx, idx, {int(RoleNames::ThruDateRole)});

    append(newLatest);
}

void PriceHistoryModel::priceHistoryFetched(const QByteArray &data)
{
    if (const auto json{QJsonDocument::fromJson(data)}; json.isArray() && json.array().count() > 0)
    {
        set(json.array());
    }
    else
    {
        errorString_ = "PriceHistoryModel::priceHistoryFetched: The fecthed data is not an array or is an empty array."_L1;
        emit errorOccurred(PriceHistoryRepository::Error::FailedToFetchPriceHistory, errorString_);
    }
}

void PriceHistoryModel::set(const QJsonArray &priceHistory)
{
    beginResetModel();
    data_.clear();
    for (const auto &entity : priceHistory)
    {
        data_.append(entity.toObject());
    }
    endResetModel();
}

void PriceHistoryModel::append(const QJsonObject &ph)
{
    const auto count{data_.count()};
    beginInsertRows(QModelIndex{}, count, count);
    data_.append(ph);
    endInsertRows();
}

PriceHistoryRepository *PriceHistoryModel::repository() const
{
    return repository_;
}

void PriceHistoryModel::setRepository(PriceHistoryRepository *newRepository)
{
    if (repository_ == newRepository)
        return;

    if (repository_)
        repository_->disconnect(this);

    repository_ = newRepository;

    if (repository_)
    {
        connect(repository_,
                &PriceHistoryRepository::errorOccurred,
                this,
                &PriceHistoryModel::errorOccurred);
        connect(repository_,
                &PriceHistoryRepository::added,
                this,
                &PriceHistoryModel::newPriceAdded);
        connect(repository_,
                &PriceHistoryRepository::fetched,
                this,
                &PriceHistoryModel::priceHistoryFetched);
    }

    emit repositoryChanged();
}

int PriceHistoryModel::rowCount(const QModelIndex &parent) const
{
    return data_.count();
}

QVariant PriceHistoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const auto &entity{data_.at(index.row())};
    switch (static_cast<RoleNames>(role))
    {
    case RoleNames::ProductIdRole:
        return entity["product_id"_L1].toInt();
    case RoleNames::PriceRole:
        return entity["price"_L1].toString().toDouble();
    case RoleNames::PriceTypeRole:
        Q_ASSERT(entity.contains("price_type"_L1));
        return priceTypeToReadableString(entity["price_type"_L1].toString());
    case RoleNames::StartDateRole:
        return dateTimeWithTZ(entity["start_date"_L1].toString());
    case RoleNames::ThruDateRole:
        return dateTimeWithTZ(entity["thru_date"_L1].toString());
    }
}

QHash<int, QByteArray> PriceHistoryModel::roleNames() const
{
    using enum RoleNames;
    return {
        {int(ProductIdRole), "productId"_ba},
        {int(PriceRole), "price"_ba},
        {int(PriceTypeRole), "priceType"_ba},
        {int(StartDateRole), "startDate"_ba},
        {int(ThruDateRole), "thruDate"_ba}
    };
}

}

