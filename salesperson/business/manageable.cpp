#include "manageable.h"
#include <QSettings>

namespace Business
{
Manageable::Manageable(QObject *parent)
    : QObject{parent}
    , base_{QSettings()
                .value("base")
                .toString() + "/salespersons"}
{
}

QByteArray Manageable::buffer() const
{
    return buffer_;
}

void Manageable::setBuffer(const QByteArray &newBuffer)
{
    buffer_ = newBuffer;
}

QString Manageable::errorString() const
{
    return errorString_;
}

void Manageable::setErrorString(const QString &newErrorString)
{
    errorString_ = newErrorString;
}

QString Manageable::base() const
{
    return base_;
}
}
