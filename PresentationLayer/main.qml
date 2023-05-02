import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtWebSockets
import "qrc:/errors"
import "qrc:/singleton"

ApplicationWindow {
    id: app
    width: Constants.appWidth
    height: Constants.appHeight
    visible: true
    title: "Cara"

    property string iconSize: "24x24"

    StackView {
        id: mainStackView
        anchors.fill: parent
        initialItem: "qrc:/main/MainPage.qml"
    }

    WebSocket {
        active: true
        url: "ws://127.0.0.1:5000/echo"
        onBinaryMessageReceived: msg => print(`msg: ${msg}`)
        onTextMessageReceived: txt => print(`text: ${txt}`)
        onErrorStringChanged: print("webSocket's status: " + status + `: ${errorString}`)
        onStatusChanged: if (status === WebSocket.Open)sendBinaryMessage('{"message": "holalala"}')
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
