#include "pdforderprinter.h"
#include <QPrinter>
#include <QStandardPaths>
#include <QTextDocument>
#include <QtConcurrent>
#include "abstractdocument.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto htmlPath{":/qt/qml/Billing/order-model.html"_L1};
constexpr auto stylePath{":/qt/qml/Billing/style.css"_L1};
constexpr auto prefix{"/factura-%1.pdf"_L1};
}

namespace Billing
{
PdfOrderPrinter::PdfOrderPrinter(QObject *parent)
    : QObject{parent}
    , path_{QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)}
    , status_{Status::Idle}
    , document_{}
{
}

QString PdfOrderPrinter::path() const
{
    return path_;
}

void PdfOrderPrinter::setPath(const QString &newPath)
{
    if (path_ == newPath)
        return;
    path_ = newPath;
    emit pathChanged();
}

void PdfOrderPrinter::print()
{
    auto result{QtConcurrent::run([&] { printToPdf(); })};
}

void PdfOrderPrinter::printToPdf()
{
    using enum Status;
    setStatus(Printing);

    const QString filename{getUniqueFilename()};
    const auto printer{preparePrinter(filename)};

    const QSizeF pageSize{printer->pageRect(QPrinter::Point).size()};
    const auto doc{prepareDocument(pageSize)};

    doc->setHtml(document_->getFormattedOrder());
    doc->print(printer.get());

    setStatus(Idle);

    emit orderPrinted(QUrl::fromLocalFile(filename));
}

std::unique_ptr<QPrinter> PdfOrderPrinter::preparePrinter(QAnyStringView filename) const
{
    auto printer{std::make_unique<QPrinter>(QPrinter::HighResolution)};
    printer->setPageSize(QPageSize::A4);

    qDebug() << "PdfOrderPrinter::preparePrinter: " << filename;
    printer->setOutputFileName(filename.toString());

    if (!printer->isValid())
        return nullptr;

    return printer;
}

std::unique_ptr<QTextDocument> PdfOrderPrinter::prepareDocument(const QSizeF &pageSize) const
{
    auto doc{std::make_unique<QTextDocument>()};
    doc->setDefaultStyleSheet(document_->style());
    doc->setPageSize(pageSize);

    return doc;
}

QString PdfOrderPrinter::getUniqueFilename() const
{
    constexpr auto format{"ddMMyyyy-hhmmss"_L1};
    QDateTime currentTime{QDateTime::currentDateTime()};

    return path_ + prefix.arg(currentTime.toString(format));
}

AbstractDocument *PdfOrderPrinter::document() const
{
    return document_;
}

void PdfOrderPrinter::setDocument(AbstractDocument *newDocument)
{
    if (document_ == newDocument)
        return;
    document_ = newDocument;
    emit documentChanged();
}

PdfOrderPrinter::Status PdfOrderPrinter::status() const
{
    return status_;
}

void PdfOrderPrinter::setStatus(const Status &newStatus)
{
    if (status_ == newStatus)
        return;
    status_ = newStatus;
    emit statusChanged();
}
}
