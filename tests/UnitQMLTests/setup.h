#ifndef SETUP_H
#define SETUP_H

#include <QObject>

class Setup : public QObject
{
    Q_OBJECT
public:
    explicit Setup(QObject *parent = nullptr);

public slots:
    void applicationAvailable() const;
    void init() const;

signals:
};

#endif // SETUP_H
