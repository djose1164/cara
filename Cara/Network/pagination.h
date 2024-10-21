#ifndef PAGINATION_H
#define PAGINATION_H

#include <QObject>
#include <QJsonObject>

namespace Network
{

class Pagination
{
    Q_GADGET
public:
    Pagination();

    int page() const;
    void setPage(int newPage);

    int pages() const;
    void setPages(int newPages);

    int perPage() const;
    void setPerPage(int newPerPage);

    int total() const;
    void setTotal(int newTotal);

    bool isValid() const;

    void nextPage();

    QUrlQuery toUrlQuery() const;

    bool canFetchMore() const;

    const QString toString() const;

    static Pagination fromJson(const QJsonObject &json);

private:
    int page_; // The current page.
    int pages_; // The total number of pages.
    int perPage_; // The maximum number of items on a page.
    int total_; // The total number of items across all pages.
};

} // namespace Network

QDebug operator<<(QDebug dbg, const Network::Pagination &pag);

#endif // PAGINATION_H
