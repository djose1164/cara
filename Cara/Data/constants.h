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

    Q_PROPERTY(QString base READ base WRITE setBase NOTIFY baseChanged FINAL)
    Q_PROPERTY(int appWidth READ appWidth CONSTANT FINAL)
    Q_PROPERTY(int appHeight READ appHeight CONSTANT FINAL)
    Q_PROPERTY(QString uploadsPath READ uploadsPath CONSTANT FINAL)

public:
    static QUrl getBase();

    Constants(QObject *parent = nullptr);

    void setBase(const QString &base);
    QString base() const;

    int appWidth() const;
    int appHeight() const;

    void setUploadsPath();
    QString uploadsPath() const;

signals:
    void baseChanged();
    void ready();

private:
    const QString testingBase_;
    const QString remoteTesting_;
    const QString production_;
    QString base_;
    QString uploadsPath_;

    int appWidth_;
    int appHeight_;
};
}

#endif // CONSTANTS_H
