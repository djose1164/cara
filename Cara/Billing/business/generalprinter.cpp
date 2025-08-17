#include "generalprinter.h"
#include "abstractdocument.h"
#include "filenamegenerator.h"

#include <QPrinter>


namespace Billing {

GeneralPrinter::GeneralPrinter(QObject *parent)
    : AbstractPrinter{parent}
    , filenameGenerator_{}
{

}

void GeneralPrinter::print()
{
    qDebug() << "GeneralPrinter::print: Printing any other platform than WASM.";


    //NOTE: filenameGenerator_ should use strategy algorithm, to generate random
    // names following certain strategies.
    const QString filename{filenameGenerator_->getFilename()};
    QPrinter printer{QPrinter::HighResolution};
    printer.setOutputFileName(filename);

    const QSizeF pageSize{printer.pageRect(QPrinter::Point).size()};
    if (!document_)
    {
        qCritical() << "PdfOrderPrinter::printToPdf: Not document set.";
        return;
    }

    document_->setPageSize(pageSize);

    document_->print(&printer);


    emit orderPrinted(filename);
}

void GeneralPrinter::setFilenameGenerator(FilenameGenerator *generator)
{
    filenameGenerator_ = generator;
}

void GeneralPrinter::setDocument(AbstractDocument *document)
{
    document_ = document;
}

} // namespace Billing
