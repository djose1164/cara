#ifndef GENERALPRINTER_H
#define GENERALPRINTER_H

#include "abstractprinter.h"


namespace Billing {

class GeneralPrinter : public Billing::AbstractPrinter
{
    Q_OBJECT
public:
    explicit GeneralPrinter(QObject *parent = nullptr);

    void print() override;
    void setFilenameGenerator(FilenameGenerator *generator) override;
    void setDocument(AbstractDocument *document) override;

private:
    FilenameGenerator *filenameGenerator_;
    AbstractDocument *document_;
};

} // namespace Billing

#endif // GENERALPRINTER_H
