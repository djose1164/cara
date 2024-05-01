#include "document.h"

#include <QTextDocument>

namespace Billing
{
Document::Document(QObject *parent)
    : Billing::AbstractDocument{parent}
{}

void Document::print(QPagedPaintDevice *printer)
{
    QTextDocument doc{};
    doc.setDefaultStyleSheet(style_);
    doc.setHtml(html_);
    doc.setPageSize(pageSize());
    doc.print(printer);
}

QString Document::html() const
{
    return html_;
}

void Document::setHtml(const QString &newHtml)
{
    if (html_ == newHtml)
        return;
    html_ = newHtml;
    emit htmlChanged();
}

QString Document::style() const
{
    return style_;
}

void Document::setStyle(const QString &newStyle)
{
    if (style_ == newStyle)
        return;
    style_ = newStyle;
    emit styleChanged();
}
}
