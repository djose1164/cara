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
                Qt.inputMethod.close()
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
            placeholderText: "Ingrese su usario..."
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

        MyTextField {
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
        modal: true
        focus: true
        anchors.centerIn: parent
        ColumnLayout {
            anchors.fill: parent
            Label {
                text: "Usario o contrasena incorrecto."
            }
            Button {
                text: "Ok"
                onClicked: popup.close()
            }
        }

    }

    WorkerScript {
        id: myWorker
        source: "qrc:/js/database.mjs"

        onMessage: (messageObject) => {
                       console.log("onMessage: busy: " + messageObject.busy)
                       console.log("property: onMessage: busy: " + busy)
                       busyIndicator.close()
                       if (messageObject.code === "success")
                       mainStackView.replace("qrc:/qml/Home.qml")
                       else
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
            Qt.inputMethod.close()
    }}
}
