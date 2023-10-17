#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>
#include <QtQml>

namespace DataLayer {
class Constants : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString base READ base WRITE setBase NOTIFY baseChanged)
    Q_PROPERTY(int appWidth READ appWidth CONSTANT)
    Q_PROPERTY(int appHeight READ appHeight CONSTANT)

public:
    Constants();

    void setBase(const QString &base);
    QString base() const;

    int appWidth() const;
    int appHeight() const;

signals:
    void baseChanged();
    void ready();

private:
    const QString testingBase_;
    const QString remoteBase_;
    QString base_;

    int appWidth_;
    int appHeight_;
};
}

#endif // CONSTANTS_H
