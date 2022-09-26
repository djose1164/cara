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
                                         })
            }
        }

        Button {
            id: signupBtn
            text: "Registrarse"
            font.pointSize: 15
            font.bold: true
            onClicked:
                mainStackView.replace("qrc:/main/SignUp.qml")
        }
    }

    MessagePopup {
        id: popup
    }

    WorkerScript {
        id: myWorker
        source: "qrc:/js/login.mjs"
    }

    Connections {
        target: myWorker

        function noInternetHandler() {
            popup.text = "Necesitas conectarte a internet.";
        }

        function failedHandler() {
            popup.text = "Usario o contrasena incorrecto."
        }

        function successHandler() {
            settings.username = usernameInput.text
            settings.password = passwordInput.text
            mainStackView.replace("qrc:/main/MainPage.qml", {"currentUser": usernameInput.text})
        }

        property var handlers : {
            "success": successHandler,
            "no_internet": noInternetHandler,
            "credentialsNotValid": failedHandler,
            "notFound": () => popup.text = "Cuenta no registrada.",
        }

        function onMessage(response) {
            const code = response.code;
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
        }
    }
}
