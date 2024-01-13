qt_add_library(StatisticsLib STATIC)
qt_add_qml_module(StatisticsLib
    URI Statistics
    QML_FILES
        Statistics.qml
        ProductStatistics.qml
        CustomerStatistics.qml
        StatisticsAPI.qml
        CustomerStatisticsDetails.qml
        OrderStatistics.qml
    RESOURCES
        statistics-color.svg
)
