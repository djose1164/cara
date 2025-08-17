#ifndef FILENAMEGENERATOR_H
#define FILENAMEGENERATOR_H

#include <QObject>
#include <QQmlEngine>

namespace Billing
{
class FilenameGenerator : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS
public:
    using QObject::QObject;

    virtual ~FilenameGenerator();

    virtual QString getFilename() const = 0;
};
}

#endif // FILENAMEGENERATOR_H
