#ifndef CONFIG_H_IN
#define CONFIG_H_IN

#define CARA_VERSION "0.21.0"

#include <QString>

constexpr auto CARA_BUILD_TIME{QLatin1StringView("27/08/2025")};

consteval QAnyStringView getCaraVersion()
{
    return "0.21.0";
}

#endif // CONFIG_H_IN
