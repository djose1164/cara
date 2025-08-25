#include "uniquefilenamegenerator.h"

#include <QDateTime>
#include <QStandardPaths>

using namespace Qt::StringLiterals;

namespace
{
constexpr auto prefix{"%1/factura-%2.pdf"_L1};
constexpr auto dateFormat{"ddMMyyyy-hhmmss"_L1};
}

namespace Billing
{
UniqueFilenameGenerator::UniqueFilenameGenerator(QObject *parent)
    : FilenameGenerator(parent)
    , documentsPath_{QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)}
{}

QString UniqueFilenameGenerator::getFilename() const
{
    const QDateTime currentTime{QDateTime::currentDateTime()};
    return prefix.arg(documentsPath_, currentTime.toString(dateFormat));
}
}
