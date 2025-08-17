#include "pdforderprinter.h"
#include <QStandardPaths>
#include <QTextDocument>
#include "abstractdocument.h"
#include "abstractprinter.h"
#include "filenamegenerator.h"
#include "generalprinter.h"
#if !defined(Q_OS_WASM)
#include <QtConcurrent>
#else
#include "wasmprinter.h"
#endif


using namespace Qt::StringLiterals;

namespace Billing
{
PdfOrderPrinter::PdfOrderPrinter(QObject *parent)
    : QObject{parent}
    , status_{Status::Idle}
    , document_{}
    , filenameGenerator_{nullptr}
    , printer_{std::make_unique<QPrinter>(QPrinter::HighResolution)}
    , orderPrinter_{createPlatformPrinter(this)}

{
    printer_->setPageSize(QPageSize::A4);

    connect(orderPrinter_, &AbstractPrinter::orderPrinted,
            this, &PdfOrderPrinter::orderPrinted);
}

void PdfOrderPrinter::print()
{
    if (filenameGenerator_ == nullptr)
    {
        qCritical() << "PdfOrderPrinter::print: filenameGenerator_ cant be null.";
        return;
    }

    orderPrinter_->setFilenameGenerator(filenameGenerator_);
    orderPrinter_->setDocument(document_);
    orderPrinter_->print();

    //auto result{QtConcurrent::run([&] { printBill(); })};
}

void PdfOrderPrinter::printBill()
{
    using enum Status;

    const QString filename{filenameGenerator_->getFilename()};
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

AbstractPrinter *PdfOrderPrinter::createPlatformPrinter(QObject *parent)
{
#ifdef Q_OS_WASM
    return new WasmPrinter{parent};
#else
    return new GeneralPrinter{parent};
#endif
}

FilenameGenerator *PdfOrderPrinter::filenameGenerator() const
{
    return filenameGenerator_;
}

void PdfOrderPrinter::setFilenameGenerator(FilenameGenerator *newFilenameGenerator)
{
    if (filenameGenerator_ == newFilenameGenerator)
        return;
    filenameGenerator_ = newFilenameGenerator;
    emit filenameGeneratorChanged();
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
