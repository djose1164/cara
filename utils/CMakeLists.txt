qt_add_library(UtilsLib STATIC)

set_target_properties(UtilsLib PROPERTIES AUTOMOC ON)

qt_add_qml_module(UtilsLib
    URI Utils
    VERSION 1.0
    SOURCES
        internetstatus.h internetstatus.cpp
        rest.h rest.cpp
    QML_FILES
        ListViewPage.qml
        DrawerHeader.qml
        RedirectLabel.qml
        ScrollablePage.qml
        DatePicker.qml
        DateDelegate.qml
        RedirectGroupBox.qml
        SOURCES cararestapi.h cararestapi.cpp
)

