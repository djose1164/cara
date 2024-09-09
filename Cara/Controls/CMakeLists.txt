find_package(Qt6 REQUIRED COMPONENTS Gui)

add_library(ControlsLib STATIC)
qt_add_qml_module(ControlsLib
    URI Cara.Controls
    RESOURCES
        after-selected-circle.png
        before-selected-circle.png
        circle.png
        dashed-line.png
        selected-circle.png
        straight-line.png
    QML_FILES
        CircleIndicator.qml
        DateField.qml
        LinearProgressIndicator.qml
        NumberTextField.qml
        TrashButton.qml
    SOURCES
        datevalidator.h datevalidator.cpp
    DEPENDENCIES
        QtQuick QtQuick.Controls
)

target_link_libraries(ControlsLib PRIVATE Qt6::Gui)
