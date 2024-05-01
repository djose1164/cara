#ifndef FILENAMEGENERATOR_H
#define FILENAMEGENERATOR_H

#include <QString>

namespace Billing
{
class FilenameGenerator
{
public:
    virtual ~FilenameGenerator();

    virtual QString getFilename() const = 0;
};
}

#endif // FILENAMEGENERATOR_H
