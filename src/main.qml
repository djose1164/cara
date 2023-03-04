import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"

ApplicationWindow {
    id: app
    width: 360
    height: 640
    visible: true
    title: "Cara"

    property string iconSize: "24x24"

    StackView {
        id: mainStackView
        anchors.fill: parent
        initialItem: TheCurrentUser.previousLogin ? "qrc:/main/MainPage.qml"
                                   : "qrc:/login/Login.qml"
    }

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }
}
