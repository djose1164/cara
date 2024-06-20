#include "datevalidator.h"

using namespace Qt::StringLiterals;

namespace
{
constexpr auto dateFormat{"dd/MM/yyyy"_L1};
}

namespace Cara {

DateValidator::DateValidator(QObject *parent)
    : QValidator{parent}
    , from_{}
    , to_{}
{
}

QValidator::State DateValidator::validate(QString &input, int &pos) const
{
    if (input.isEmpty())
        return QValidator::Invalid;

    const auto date{QDate::fromString(input, dateFormat)};

    return date.isValid() and date >= from_ and date <= to_
               ? QValidator::Acceptable : QValidator::Intermediate;
}

QDate DateValidator::to() const
{
    return to_;
}

void DateValidator::setTo(const QDate &newTo)
{
    setRange(from(), newTo);
}

void DateValidator::setRange(const QDate &newFrom, const QDate &newTo)
{
    bool rangedChanged{};
    if (from_ != newFrom)
    {
        from_ = newFrom;
        rangedChanged = true;
        emit fromChanged();
    }

    if (to_ != newTo)
    {
        to_ = newTo;
        rangedChanged = true;
        emit toChanged();
    }

    if (rangedChanged)
        emit changed();
}

QDate DateValidator::from() const
{
    return from_;
}

void DateValidator::setFrom(const QDate &newFrom)
{
    setRange(newFrom, to());
}

} // namespace Cara
