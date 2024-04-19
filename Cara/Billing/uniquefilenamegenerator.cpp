#include "uniquefilenamegenerator.h"

#include <QDateTime>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto prefix{"/factura-%1.pdf"_L1};
constexpr auto dateFormat{"ddMMyyyy-hhmmss"_L1};
}

namespace Billing
{
UniqueFilenameGenerator::UniqueFilenameGenerator() {}

QString UniqueFilenameGenerator::getFilename() const
{
    const QDateTime currentTime{QDateTime::currentDateTime()};
    return prefix.arg(currentTime.toString(dateFormat));
}
}
