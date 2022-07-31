import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: loginForm

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
                busyIndicator.open()
                if (usernameInput.text !== "" && passwordInput.text !== "")
                    myWorker.sendMessage({
                                             "username": usernameInput.text,
                                             "password": passwordInput.text,
                                             "busy": true,
                                             "type": "login"
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

    MessagePopup {
        id: popup
    }

    Worker {
        id: myWorker
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
            mainStackView.replace("qrc:/qml/Home.qml", {"currentUser": usernameInput.text})
        }

        property var handlers : {
            "success": successHandler,
            "no_internet": noInternetHandler,
            "credentialsNotValid": failedHandler,
        }

        function onResultReady(code) {
            print(code)
            popup.text = "Envianos un correo reportando este bug!"
            busyIndicator.close()
            handlers[code]();
            if (code !== "success")
                popup.open();
        }
    }

    BusyPopup {
        id: busyIndicator
    }

    TapHandler {
        onTapped: {
            usernameInput.focus = false
            passwordInput.focus = false
            Qt.inputMethod.hide()
        }}
}
