#ifndef FAVORITEPRODUCT_H
#define FAVORITEPRODUCT_H

#include <QQmlEngine>
#include <abstractresource.h>

class QRestReply;

namespace Shopping
{
class Favorite;

class FavoriteProduct : public AbstractResource
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
public:
    using UserId = int;

    explicit FavoriteProduct(QObject *parent = nullptr);

    QString path() const;
    void setPath(const QString &newPath);

public slots:
    void add(const Favorite &favorite);

    void remove(const Favorite &favorite);

    void readByCustomerId(UserId id);

signals:
    void pathChanged();

    void errorOccurred(const QString &errorString);

    void resourceCreated(const QByteArray &data = {});

    void resourceDeleted();

    void resourceFetched(const QByteArray &data);

private:
    void handleFinished(QRestReply &reply);

private:
    QString path_;
};
}

#endif // FAVORITEPRODUCT_H
