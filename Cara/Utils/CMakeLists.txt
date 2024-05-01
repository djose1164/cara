qt_add_library(UtilsLib STATIC)

set_target_properties(UtilsLib PROPERTIES AUTOMOC ON)

qt_add_qml_module(UtilsLib
    URI Cara.Utils
    VERSION 1.0
    SOURCES
        abstractresource.h abstractresource.cpp
        fileutils.h fileutils.cpp
        httpstatuscodes.h
        internetstatus.h internetstatus.cpp
        notimplementated.h
        qnamsingleton.h qnamsingleton.cpp
        shareplataform.h shareplataform.cpp
        shareutils.h shareutils.cpp
        utils.h utils.cpp
    QML_FILES
        BackButton.qml
        BackPage.qml
        BusyPopup.qml
        BusyStackLayout.qml
        CaraApi.qml
        Card.qml
        CenterBusyIndicator.qml
        CloseButton.qml
        CloseHeader.qml
        DateDelegate.qml
        DatePicker.qml
        DrawerHeader.qml
        Header.qml
        ListViewPage.qml
        Message.qml
        MessagePane.qml
        MoreVertButton.qml
        NumberTextField.qml
        PasswordInput.qml
        ProcessingIndicator.qml
        RedirectGroupBox.qml
        RedirectLabel.qml
        ScrollablePage.qml
        SearchBar.qml
        SearchButton.qml
        SearchField.qml
        SearchItem.qml
        SearchPage.qml
        Star.qml
        StarRating.qml
        TimeoutDialog.qml
        TitleHeader.qml
        PicSwipeView.qml
        EmailField.qml
        SearchHeader.qml
    RESOURCES
        utils.js
        star.png
        star-unfilled.png
        QML_FILES SelectorHeaderItem.qml
)

if(ANDROID)
    target_sources(UtilsLib PRIVATE
        androidshareplataform.h androidshareplataform.cpp
    )
endif()

target_link_libraries(UtilsLib PRIVATE Qt6::Core Qt6::Network)
