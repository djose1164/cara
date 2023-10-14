import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import "qrc:/errors"
import "qrc:/singleton"
import "qrc:/utils"
import models
import core
import data

ApplicationWindow {
    id: app
    width: Constants.appWidth
    height: Constants.appHeight
    visible: true
    title: "Cara"

    property string iconSize: "24x24"
    property alias appStack: mainStackView

    Component.onCompleted: {
        caraApi.checkStatus();
    }

    StackView {
        id: mainStackView
        anchors.fill: parent

        initialItem: Item {
            BusyIndicator { anchors.centerIn: parent }
        }
    }

    /*WebSocket {
        active: true
        url: "ws://cara-rd.live/echo"
        onBinaryMessageReceived: msg => print(`msg: ${msg}`)
        onTextMessageReceived: txt => print(`text: ${txt}`)
        onErrorStringChanged: print("webSocket's status: " + status + `: ${errorString}`)
        onStatusChanged: if (status === WebSocket.Open)sendBinaryMessage(`{"app_version": "${Qt.application.version}"}`)
    }*/

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: Qt.quit()
    }

    CaraApi {
        id: caraApi

        endpoint: "/health"
        onServiceUnavailable: {
            print("Holalalala")
            const message = "Nuestros servidores se encuentran actualmente bajo "
                          + "mantenimiento. Puedes ver como va en [Twiterr](https://twitter.com/carardoficial).";
            mainStackView.replace("qrc:/errors/ErrorPage.qml", {message});
        }
        onSucceeded: {
            loginSetup();
            mainStackView.replace("qrc:/main/MainPage.qml");
        }

        function checkStatus() {
            get("/");
        }
    }

    Connections {
        property int counter
        target: LoginManager

        function onLoggedIn(resp) {
            print("onLoggedIn: counter: ", ++counter)
            CurrentUser.fillUserInfo(resp);
            TheCurrentUser.accessToken = "Bearer " + resp.access_token;
        }

        function onLoggedInWithToken(resp) {
            CurrentUser.userFromSettings();
            TheCurrentUser.accessToken = "Bearer " + resp.access_token;

            print("onLoggedInWithToken: isAdmin", CurrentUser.isAdmin);
        }

        function onLoggedOut() {
            CurrentUser.clear();
        }
    }


    function loginSetup() {
        if (LoginManager.hasPreviousLogin()) {
            LoginManager.requestLoginFromSettings();
        }
    }
}
