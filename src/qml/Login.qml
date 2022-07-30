import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: loginForm

    property bool busy: false

    Label {
        id:loginLbl
        anchors.top:  parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Iniciar Sesion"
        font.pointSize: parent.width * .1
        font.bold: true
    }

    ColumnLayout {
        id: loginInfo
        width: parent.width
        anchors.top: loginLbl.bottom
        anchors.topMargin: 100
        spacing: 20

        component MyTextField: TextField {
            onEditingFinished: {
                focus = false
                Qt.inputMethod.hide()
            }
        }

        Label {
            id: usernameLbl
            text: "Usuario"
            Layout.alignment: Qt.AlignCenter
            font.pointSize: 20
            font.bold: true
        }

        MyTextField {
            id: usernameInput
            Layout.preferredWidth: parent.width/1.5
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            placeholderText: "Usuario"
            inputMethodHints: Qt.ImhLowercaseOnly
            font.pointSize: 15
            onEditingFinished: Qt.inputMethod.hide()
        }

        Label {
            id: passwordLbl
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 40
            text: "Contraseña"
            font.pointSize: 20
            font.bold: true
        }

        PasswordInput {
            id: passwordInput
            Layout.preferredWidth: parent.width/1.5
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            font.pointSize: 15
        }
    }

    RowLayout {
        id: buttonsRowLayout
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 15

        Button {
            id: loginBtn
            text: "Iniciar Sesion"
            font.pointSize: 15
            font.bold: true
            onClicked: {
                console.log("Button clicked")
                busyIndicator.open()
                if (usernameInput.text !== "" && passwordInput.text !== "")
                    myWorker.sendMessage({
                                             "username": usernameInput.text,
                                             "password": passwordInput.text,
                                             "busy": true
                                         })
            }
        }

        Button {
            id: signupBtn
            text: "Registrarse"
            font.pointSize: 15
            font.bold: true
            onClicked:
                mainStackView.replace("qrc:/qml/SignUp.qml")
        }
    }

    Popup {
        id: popup
        width: parent.width - 20
        anchors.centerIn: parent
        focus: true
        modal: true

        property alias text: popupLbl.text

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            Label {
                id: popupLbl
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
                font.pointSize: 20
            }

            Button {
                text: "Ok"
                Layout.alignment: Qt.AlignHCenter
                onClicked: popup.close()
            }
        }

    }

    WorkerScript {
        id: myWorker
        source: "qrc:/js/database.mjs"

        signal resultReady(code: string)

        onMessage: (msgObject) => resultReady(msgObject.code)
    }

    Connections {
        target: myWorker

        function noInternetHandler() {
            popup.text = "Necesitas conectarte a internet.";
            print("In no internet handler: popu.text: " + popup.text)
        }

        function failedHandler() {
            popup.text = "Usario o contrasena incorrecto."
        }

        function successHandler() {
            mainStackView.replace("qrc:/qml/Home.qml")
        }

        property var errorHandlers : {
            "success": successHandler,
            "no_internet": noInternetHandler,
            "failed": failedHandler,
        }

        function onResultReady(code) {
            print(code)
            busyIndicator.close()
            let func = errorHandlers[code];
            func();
            popup.open()
        }
    }

    Popup {
        id: busyIndicator
        width: parent.width * .7
        height: parent.height/2
        anchors.centerIn: parent
        modal: true
        focus: true
        background: Rectangle {
            color: "transparent"
        }

        BusyIndicator {
            anchors.fill: parent
        }
    }
    TapHandler {
        onTapped: {
            usernameInput.focus = false
            passwordInput.focus = false
            Qt.inputMethod.hide()
        }}
}
