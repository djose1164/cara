import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs


Item {
    id: loginForm
    Keys.onReturnPressed: Qt.inputMethod.hide()
    Keys.onEnterPressed: Qt.inputMethod.hide()


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

        Label {
            id: usernameLbl
            text: "Usuario"
            Layout.alignment: Qt.AlignCenter
            font.pointSize: 20
            font.bold: true
        }

        TextField {
            id: usernameInput
            Layout.preferredWidth: parent.width/1.5
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            placeholderText: "Ingrese su usario..."
            font.pointSize: 15
        }

        Label {
            id: passwordLbl
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 40
            text: "Contraseña"
            font.pointSize: 20
            font.bold: true

        }

        TextField {
            id: passwordInput
            Layout.preferredWidth: parent.width/1.5
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
            placeholderText: "Ingrese su contrasena..."
            font.pointSize: 15
            echoMode: TextField.Password
            passwordMaskDelay: 600
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
                console.log("busy: ", db.busy())

                if (usernameInput.text !== "" && passwordInput.text !== ""
                        && db.validate(usernameInput.text, passwordInput.text))
                {
                    console.log("Logged in!")
                    mainStackView.replace("qrc:/qml/Home.qml")
                }
                else
                    messageDialog.visible = true
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

    MessageDialog {
        id: messageDialog
        title: "Usario o contrasena incorrecto."
        text: "Displayed if the credentials are not correct."
        visible: false
    }

    Popup {
        id: popupWait
        width: parent.width/1.5
        height: parent.height/2
        anchors.centerIn: Overlay.overlay
        modal: true

        BusyIndicator {
            id: busyIndicator
            anchors.fill: parent
            running: true
        }
    }

    Connections {
        target: db
        function onBusyChanged() {
            if (db.busy())
                popupWait.open()
            //else
            //    popupWait.close()
        }
    }
}
