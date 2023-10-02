#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>
#include <QtQml>

namespace BusinessLayer {
class Constants : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString base READ base WRITE setBase NOTIFY baseChanged)
    Q_PROPERTY(int appWidth READ appWidth CONSTANT)
    Q_PROPERTY(int appHeight READ appHeight CONSTANT)

public:
    void setBase(const QString &base);
    QString base() const;

    int appWidth() const;
    int appHeight() const;

    static Constants *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine);
    static Constants *instance();

signals:
    void baseChanged();

private:
    Constants();

    const QString localBase_;
    const QString remoteBase_;
    QString base_;
    int appWidth_;
    int appHeight_;
    static Constants *instance_;
};
}

#endif // CONSTANTS_H
