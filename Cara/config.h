#ifndef CONFIG_H_IN
#define CONFIG_H_IN

#define CARA_VERSION "0.19.0"

#include <QAnyStringView>

consteval QAnyStringView getCaraVersion()
{
    return "0.19.0";
}

#endif // CONFIG_H_IN
