#ifndef JSONEXPORTABLE_H
#define JSONEXPORTABLE_H

#include <QByteArray>

namespace EntityLayer {
class JsonExportable
{
public:
    virtual ~JsonExportable() = default;
    virtual QByteArray toJson() const = 0;
};
}

#endif // JSONEXPORTABLE_H
