#ifndef UNIQUEFILENAMEGENERATOR_H
#define UNIQUEFILENAMEGENERATOR_H

#include "filenamegenerator.h"

namespace Billing
{
class UniqueFilenameGenerator : public FilenameGenerator
{
    Q_OBJECT
    QML_ELEMENT
public:
    UniqueFilenameGenerator(QObject *parent = nullptr);

    // FilenameGenerator interface
    QString getFilename() const override;

private:
    const QString documentsPath_;
};
}

#endif // UNIQUEFILENAMEGENERATOR_H
