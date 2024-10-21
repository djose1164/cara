#include "pagination.h"

#include <QUrlQuery>

using namespace Qt::StringLiterals;

namespace Network
{

Pagination::Pagination()
    : page_{}
    , pages_{}
    , perPage_{}
    , total_{}
{
}

int Pagination::page() const
{
    return page_;
}

void Pagination::setPage(int newPage)
{
    page_ = newPage;
}

int Pagination::pages() const
{
    return pages_;
}

void Pagination::setPages(int newPages)
{
    pages_ = newPages;
}

int Pagination::perPage() const
{
    return perPage_;
}

void Pagination::setPerPage(int newPerPage)
{
    perPage_ = newPerPage;
}

int Pagination::total() const
{
    return total_;
}

void Pagination::setTotal(int newTotal)
{
    total_ = newTotal;
}

bool Pagination::isValid() const
{
    return total_ > 0 and perPage_ > 0 and page_ > 0;
}

void Pagination::nextPage()
{
    ++page_;
}

QUrlQuery Pagination::toUrlQuery() const
{
    QUrlQuery query;
    query.addQueryItem("page"_L1, QString::number(page_));
    query.addQueryItem("per_page"_L1, QString::number(perPage_));
    return query;
}

bool Pagination::canFetchMore() const
{
    return pages_ > page_;
}

const QString Pagination::toString() const
{
    return "Pagination(total=%1, perPage=%2, page=%3, pages=%4)"_L1
        .arg(QString::number(total_),
             QString::number(perPage_),
             QString::number(page_),
             QString::number(pages_));
}

Pagination Pagination::fromJson(const QJsonObject &json)
{
    Pagination pagination;
    pagination.setPage(json["page"_L1].toInt());
    pagination.setPages(json["pages"_L1].toInt());
    pagination.setPerPage(json["per_page"_L1].toInt());
    pagination.setTotal(json["total"_L1].toInt());
    return pagination;
}

} // namespace Network

QDebug operator<<(QDebug dbg, const Network::Pagination &pag)
{
    dbg << pag.toString();
    return dbg;
}
