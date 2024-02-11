qt_add_library(UtilsLib STATIC)

set_target_properties(UtilsLib PROPERTIES AUTOMOC ON)

qt_add_qml_module(UtilsLib
    URI Utils
    VERSION 1.0
    SOURCES
        internetstatus.h internetstatus.cpp
        utils.h utils.cpp
        abstractresource.h abstractresource.cpp
        restservice.h restservice.cpp
        networkrequestsingleton.h networkrequestsingleton.cpp
        notimplementated.h
    QML_FILES
        ListViewPage.qml
        DrawerHeader.qml
        RedirectLabel.qml
        ScrollablePage.qml
        DatePicker.qml
        DateDelegate.qml
        RedirectGroupBox.qml
        Card.qml
        SearchHeaderItem.qml
        SearchBar.qml
        SearchButton.qml
        SearchField.qml
        SearchPage.qml
)
