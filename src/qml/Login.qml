import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: loginRoot
    anchors.fill: parent
    Label {
        id:loginLbl
        anchors.top:  parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Iniciar Sesion"
        font.pointSize: 20
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
            Layout.preferredHeight: this.font.pointSize * 2
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: -10
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
            Layout.preferredHeight: this.font.pointSize * 2
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: -10
            placeholderText: "Ingrese su contrasena..."
            font.pointSize: 15
            echoMode: TextField.Password
        }
    }

    RowLayout
    {
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
                if (usernameInput.text !== "" && passwordInput.text !== "")
                    mainStackView.replace("qrc:/qml/Home.qml")
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
}
