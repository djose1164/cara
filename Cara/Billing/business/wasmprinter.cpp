#include "wasmprinter.h"
#include "abstractdocument.h"
#include "filenamegenerator.h"

#include <QBuffer>
#include <QPdfWriter>
#include <emscripten.h>


namespace
{

EM_JS(void, saveFile, (const char *contentPointer, size_t contentLength, const char *filename), {
    // Make the file contents and file name hint accessible to JavaScript: convert
    // the char * to a JavaScript string a create a subarray view into the C heap.
    const filenameHint = UTF8ToString(filename);
    const fileContent = Module.HEAP8.subarray(contentPointer, contentPointer + contentLength);

    const arrayBuffer = fileContent.slice();
    const uint8Array = new Uint8Array(arrayBuffer);
    uint8Array.fill(255);
    const fileblob = new Blob([arrayBuffer]);

    // Create a hidden download link and click it programatically
    const link = document.createElement("a");
    document.body.appendChild(link);
    link.download = filenameHint;
    link.href = window.URL.createObjectURL(fileblob);
    link.style = "display:none";
    link.click();
    document.body.removeChild(link);
})

}

namespace Billing
{

WasmPrinter::WasmPrinter(QObject *parent)
    : AbstractPrinter{parent}
    , filenameGenerator_{}
    , document_{}
{
}

void WasmPrinter::print()
{
    QBuffer buffer{};
    buffer.open(QBuffer::WriteOnly);
    QPdfWriter p{&buffer};
    document_->print(&p);

    const auto rawData{buffer.buffer()};
    auto filename{filenameGenerator_->getFilename()};
    saveFile(rawData.constData(), rawData.length(), filename.toUtf8());

    emit orderPrinted(filename);
}

void WasmPrinter::setFilenameGenerator(FilenameGenerator *generator)
{
    filenameGenerator_ = generator;
}

void WasmPrinter::setDocument(AbstractDocument *document)
{
    document_ = document;
}

} // namespace Billing
