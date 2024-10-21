#ifndef VALIDATIONERROR_H
#define VALIDATIONERROR_H

#include <QAnyStringView>
#include <QString>
#include <stdexcept>

class ValidationError : public std::runtime_error
{
public:
    explicit ValidationError(QAnyStringView msg)
        : std::runtime_error{msg.toString().toStdString()}
    {
    }
};

#endif // VALIDATIONERROR_H
