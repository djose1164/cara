add_library(ControlsLib STATIC)
qt_add_qml_module(ControlsLib
    URI Cara.Controls
    QML_FILES
    RESOURCES
        selected-circle.png
        after-selected-circle.png
        before-selected-circle.png
        QML_FILES LinearProgressIndicator.qml
        QML_FILES CircleIndicator.qml
    DEPENDENCIES
        QtQuick QtQuick.Controls
)

