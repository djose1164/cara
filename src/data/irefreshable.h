#ifndef IREFRESHABLE_H
#define IREFRESHABLE_H

namespace DataLayer {
class IRefreshable
{
public:
    virtual ~IRefreshable();

    virtual void refresh() = 0;
};
}

#endif // IREFRESHABLE_H
