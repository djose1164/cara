qt_add_library(StatisticsLib STATIC)

set_source_files_properties(StatisticsResource.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(StatisticsLib
    URI Statistics
    QML_FILES
        Statistics.qml
        ProductStatistics.qml
        CustomerStatistics.qml
        StatisticsAPI.qml
        OrderStatistics.qml
        StatisticsResource.qml
    RESOURCES
        statistics-color.svg
)
