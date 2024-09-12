#include "pdforderprinter.h"
#include <QStandardPaths>
#include <QTextDocument>
#include <QtConcurrent>
#include "abstractdocument.h"
#include "uniquefilenamegenerator.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto htmlPath{":/qt/qml/Billing/order-model.html"_L1};
constexpr auto stylePath{":/qt/qml/Billing/style.css"_L1};
}

namespace Billing
{
PdfOrderPrinter::PdfOrderPrinter(QObject *parent)
    : QObject{parent}
    , path_{QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)}
    , status_{Status::Idle}
    , document_{}
    , filenameGenerator_{std::make_unique<UniqueFilenameGenerator>()}
    , printer_{std::make_unique<QPrinter>(QPrinter::HighResolution)}
{
    printer_->setPageSize(QPageSize::A4);
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
    auto result{QtConcurrent::run([&] { printBill(); })};
}

void PdfOrderPrinter::printBill()
{
    using enum Status;

    const QString filename{path_ + filenameGenerator_->getFilename()};
    printer_->setOutputFileName(filename);

    const QSizeF pageSize{printer_->pageRect(QPrinter::Point).size()};
    if (!document_)
    {
        qCritical() << "PdfOrderPrinter::printToPdf: Not document set.";
        setStatus(Error);
        return;
    }

    document_->setPageSize(pageSize);

    setStatus(Printing);
    document_->print(printer_.get());
    setStatus(Idle);

    Q_ASSERT(this);
    emit orderPrinted(filename);
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
