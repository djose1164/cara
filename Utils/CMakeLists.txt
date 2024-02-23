qt_add_library(UtilsLib STATIC)

set_target_properties(UtilsLib PROPERTIES AUTOMOC ON)

qt_add_qml_module(UtilsLib
    URI Utils
    VERSION 1.0
    SOURCES
        abstractresource.h abstractresource.cpp
        internetstatus.h internetstatus.cpp
        networkrequestsingleton.h networkrequestsingleton.cpp
        notimplementated.h
        restservice.h restservice.cpp
        utils.h utils.cpp
    QML_FILES
        Card.qml
        DateDelegate.qml
        DatePicker.qml
        DrawerHeader.qml
        ListViewPage.qml
        MessagePane.qml
        ProcessingIndicator.qml
        RedirectGroupBox.qml
        RedirectLabel.qml
        ScrollablePage.qml
        SearchBar.qml
        SearchButton.qml
        SearchField.qml
        SearchHeaderItem.qml
        SearchPage.qml
)
