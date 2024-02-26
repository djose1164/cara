#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QObject>
#include <QQmlEngine>

namespace Utils
{
class FileUtils : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit FileUtils(QObject *parent = nullptr);

    Q_INVOKABLE bool removeFile(const QString &filename);

public slots:
    void removeMatchingFiles(const QUrl &path, const QString &nameFilter);

signals:
};
}

#endif // FILEUTILS_H
