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
        BackButton.qml
        BackPage.qml
        BusyPopup.qml
        BusyStackLayout.qml
        Card.qml
        CenterBusyIndicator.qml
        CircleIndicator.qml
        CloseButton.qml
        CloseHeader.qml
        DateDelegate.qml
        DateField.qml
        DatePicker.qml
        DrawerHeader.qml
        EmailField.qml
        Header.qml
        LinearProgressIndicator.qml
        ListViewPage.qml
        Message.qml
        MessagePane.qml
        MoreVertButton.qml
        NumberTextField.qml
        PicSwipeView.qml
        ProcessingIndicator.qml
        RedirectGroupBox.qml
        RedirectLabel.qml
        ScrollablePage.qml
        SearchBar.qml
        SearchButton.qml
        SearchField.qml
        SearchHeader.qml
        Star.qml
        StarRating.qml
        TimeoutDialog.qml
        TitleHeader.qml
        TrashButton.qml
    RESOURCES
        star-unfilled.png
        star.png
    SOURCES
        datevalidator.h datevalidator.cpp
    DEPENDENCIES
        QtQuick QtQuick.Controls
        QML_FILES EntitySelector.qml
        QML_FILES EntityCard.qml
)

target_link_libraries(ControlsLib PRIVATE Qt6::Gui)
