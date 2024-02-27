#ifndef SHAREPLATAFORM_H
#define SHAREPLATAFORM_H

#include <QUrl>

namespace Utils
{
class SharePlataform
{
public:
    virtual ~SharePlataform();

    virtual void share(const QUrl &path) const = 0;
};
}

#endif // SHAREPLATAFORM_H
