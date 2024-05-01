#ifndef ANDROIDSHAREPLATAFORM_H
#define ANDROIDSHAREPLATAFORM_H

#include "shareplataform.h"

namespace Utils
{
class AndroidSharePlataform : public SharePlataform
{
public:
    AndroidSharePlataform();

    void share(const QUrl &path) const override;
};
}

#endif // ANDROIDSHAREPLATAFORM_H
