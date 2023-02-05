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

    property string source
    property string iconSize: "24x24"

    StackView {
        id: mainStackView
        anchors.fill: parent
        initialItem: loginSettings.havePreviousLogin ? "qrc:/main/MainPage.qml" : "qrc:/login/Login.qml"
    }

    Settings {
        id: loginSettings
        category: "Credentials"
        property string username
        property string password
        //readonly property bool havePreviousLogin: value("username") && value("password");
        Component.onCompleted: {setValue("username", ""); setValue("password", "")}
    }
}
