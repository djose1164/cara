#ifndef UNIQUEFILENAMEGENERATOR_H
#define UNIQUEFILENAMEGENERATOR_H

#include <filenamegenerator.h>

namespace Billing
{
class UniqueFilenameGenerator : public FilenameGenerator
{
public:
    UniqueFilenameGenerator();

    // FilenameGenerator interface
    QString getFilename() const override;
};
}

#endif // UNIQUEFILENAMEGENERATOR_H
