#include "fileutils.h"
#include "utils.h"

#include <QDir>
#include <QFile>

namespace Utils
{
FileUtils::FileUtils(QObject *parent)
    : QObject{parent}
{}

bool FileUtils::removeFile(const QString &filename)
{
    QFile f{filename};
    return f.remove();
}

void FileUtils::removeMatchingFiles(const QUrl &path, const QString &nameFilter)
{
    QDir dir{Utils::runningOnAndroid() ? path.toString() : path.toLocalFile()};
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList{} << nameFilter);

    for (const QString &f : dir.entryList())
        dir.remove(f);
}
}
