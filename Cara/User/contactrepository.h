#ifndef CONTACTREPOSITORY_H
#define CONTACTREPOSITORY_H

#include <QObject>

class QRestReply;
class QUrlQuery;

class ContactRepository
{
public:
    using Handler = std::function<void(QRestReply &reply)>;

    virtual ~ContactRepository();

    virtual void getById(int id, Handler &&h) = 0;

    virtual void update(const QVariantMap &data, const QUrlQuery &query, Handler &&h) = 0;
};

#endif // CONTACTREPOSITORY_H
