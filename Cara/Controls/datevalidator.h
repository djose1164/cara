#ifndef DATEVALIDATOR_H
#define DATEVALIDATOR_H

#include <QQmlEngine>
#include <QValidator>

namespace Cara {

class DateValidator : public QValidator
{
    Q_OBJECT
    Q_PROPERTY(QDate from READ from WRITE setFrom NOTIFY fromChanged FINAL)
    Q_PROPERTY(QDate to READ to WRITE setTo NOTIFY toChanged FINAL)
    QML_ELEMENT
public:
    DateValidator(QObject *parent = nullptr);

    State validate(QString &input, int &pos) const override;

    QDate from() const;
    void setFrom(const QDate &newFrom);

    QDate to() const;
    void setTo(const QDate &newTo);

    void setRange(const QDate &newFrom, const QDate &newTo);

signals:
    void fromChanged();
    void toChanged();

private:
    QDate from_;
    QDate to_;
};

} // namespace Cara

#endif // DATEVALIDATOR_H
