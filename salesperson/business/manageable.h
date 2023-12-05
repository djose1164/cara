#ifndef MANAGEABLE_H
#define MANAGEABLE_H

#include <QObject>

namespace Business
{
class Manageable : public QObject
{
    Q_OBJECT
public:
    explicit Manageable(QObject *parent = nullptr);

    virtual void read(int id) = 0;
    virtual void readAll(int adminId) = 0;

    virtual void add(const QByteArray &data) = 0;

    virtual void edit(int id, const QByteArray &data) = 0;

    virtual void remove(int id) = 0;

    QByteArray buffer() const;
    void setBuffer(const QByteArray &newBuffer);

    QString errorString() const;
    void setErrorString(const QString &newErrorString);

    QString base() const;

signals:
    void added();
    void edited();
    void fetched();

    void errorOccurred();

private:
    QByteArray buffer_;
    QString errorString_;
    const QString base_;
};
}

#endif // MANAGEABLE_H
