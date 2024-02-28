#include "abstractdocument.h"

namespace Billing
{
AbstractDocument::AbstractDocument(QObject *parent)
    : QObject{parent}
{
}

AbstractDocument::~AbstractDocument()
{
}

QSizeF AbstractDocument::pageSize() const
{
    return pageSize_;
}

void AbstractDocument::setPageSize(const QSizeF &newPageSize)
{
    pageSize_ = newPageSize;
}
}
