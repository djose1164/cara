import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCore

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
        initialItem: loginSettings.value("isLoggedin") ? "qrc:/main/MainPage.qml" : "qrc:/login/Login.qml"
    }

    Settings {
        id: loginSettings
        category: "Credentials"
    }
}
