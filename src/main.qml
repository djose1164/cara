import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/users"
import "qrc:/errors"

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

    Connections {
        target: TheCurrentUser

        function onServerUnavailable(response) {
            const message = "Nuestros servidores se encuentran actualmente bajo "
                          + "mantenimiento. Puedes ver como va en [Twiterr](https://twitter.com/carardoficial).";
            mainStackView.replace("qrc:/errors/ErrorPage.qml", {message});
        }
    }

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }
}
