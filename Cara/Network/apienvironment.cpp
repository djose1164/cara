#include "apienvironment.h"

Api::Api(QObject *parent)
    : QObject{parent}
{

}

QString Api::uploadsPath() const
{
    auto &&path{getApiPath().toString()};
    return path.replace("/api"_L1, "/uploads"_L1) + "/"_L1;;
}
