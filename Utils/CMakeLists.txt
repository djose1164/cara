qt_add_library(UtilsLib STATIC)

set_target_properties(UtilsLib PROPERTIES AUTOMOC ON)

qt_add_qml_module(UtilsLib
    URI Utils
    VERSION 1.0
    SOURCES
        abstractresource.h abstractresource.cpp
        fileutils.h fileutils.cpp
        internetstatus.h internetstatus.cpp
        networkrequestsingleton.h networkrequestsingleton.cpp
        notimplementated.h
        restservice.h restservice.cpp
        utils.h utils.cpp
    QML_FILES
        BackButton.qml
        BackPage.qml
        BusyStackLayout.qml
        CaraApi.qml
        Card.qml
        CenterBusyIndicator.qml
        CloseButton.qml
        DateDelegate.qml
        DatePicker.qml
        DrawerHeader.qml
        Header.qml
        ImagePicker.qml
        ListViewPage.qml
        Message.qml
        MessagePane.qml
        MoreVertButton.qml
        NumberTextField.qml
        ProcessingIndicator.qml
        RedirectGroupBox.qml
        RedirectLabel.qml
        ScrollablePage.qml
        SearchBar.qml
        SearchButton.qml
        SearchField.qml
        SearchHeaderItem.qml
        SearchPage.qml
        Star.qml
        StarRating.qml
        TitleHeader.qml
    RESOURCES
        utils.js
)
