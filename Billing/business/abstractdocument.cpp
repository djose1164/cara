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
}
