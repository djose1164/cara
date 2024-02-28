#include "fileutils.h"

#include <QDir>
#include <QFile>

using namespace Qt::StringLiterals;

namespace
{
constexpr int datetimeOffset{14};
constexpr auto dateFormat{"ddMMyyyy-hhmmss"_L1};
}
namespace Utils
{
FileUtils::FileUtils(QObject *parent)
    : QObject{parent}
{}

bool FileUtils::removeFile(const QUrl &filename)
{
    QFile f{filename.toLocalFile()};
    return f.remove();
}

void FileUtils::removeMatchingFiles(const QUrl &path, const QString &nameFilter)
{
    QDir dir{path.toLocalFile()};
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList{} << nameFilter);

    for (const QString &f : dir.entryList())
        if (!dir.remove(f))
            qWarning() << "FileUtils::removeMatchingFiles: Failed to remove: " << f;
}

void FileUtils::removeOldestFiles(int upTo, const QUrl &path, const QString &nameFilter)
{
    QDir dir{path.toLocalFile()};
    dir.setFilter(QDir::Files);
    dir.setSorting(QDir::Time);
    dir.setNameFilters(QStringList{} << nameFilter);

    const auto entryList{dir.entryList()};
    for (auto i{entryList.count() - 1}; i >= upTo; --i)
    {
        const QString &f{entryList.at(i)};
        if (!dir.remove(f))
            qWarning() << "FileUtils::removeOldestFiles: Failed to remove: " << f;
    }
}
}
