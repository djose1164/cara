#ifndef WASMPRINTER_H
#define WASMPRINTER_H

#include "abstractprinter.h"

namespace Billing {

class WasmPrinter : public AbstractPrinter
{
    Q_OBJECT
public:
    explicit WasmPrinter(QObject *parent = nullptr);

    void print() override;
    void setFilenameGenerator(FilenameGenerator *generator) override;
    void setDocument(AbstractDocument *document) override;

signals:

private:
    FilenameGenerator *filenameGenerator_;
    AbstractDocument *document_;
};

} // namespace Billing

#endif // WASMPRINTER_H
