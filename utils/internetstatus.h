#ifndef INTERNETSTATUS_H
#define INTERNETSTATUS_H

#include <QObject>
#include <QQmlEngine>
#include <QNetworkAccessManager>


class InternetStatus : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(Status status READ status NOTIFY statusChanged FINAL)
    Q_PROPERTY(QString statusString READ statusString NOTIFY statusStringChanged FINAL)

public:
    enum class Status
    {
        InterentAvailable,
        InternetNotAvailable,
        Unknown,
        NoStatus
    };
    Q_ENUM(Status);

    explicit InternetStatus(QObject *parent = nullptr);

    Status status() const;
    void setStatus(const Status &newStatus);

    QString statusString() const;
    void setStatusString(const QString &newStatusString);

public slots:
    void checkInternetConnection();

private slots:
    //void on_statusChanged();
    void onStatusChanged();

signals:
    void statusChanged();

    void statusStringChanged();

private:
    QNetworkAccessManager *manager_;
    QNetworkReply *reply_;

    Status status_;
    QString statusString_;
};

#endif // INTERNETSTATUS_H
