import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id:login
    anchors.fill: parent

    Label {
        id:loginLbl
        anchors.top:  parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Iniciar Sesion"
        font.pointSize: parent.width * .09
        font.bold: true
    }

    Item {
        id: loginInfo
        width: parent.width
        anchors.top: loginLbl.bottom
        anchors.topMargin: 100

        Label {
            id: usernameLbl
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Usuario"
            font.pointSize: 15
            font.bold: true

        }


        TextField {
            id: usernameInput
            width: parent.width/2
            anchors.top: usernameLbl.bottom
            anchors.topMargin: 25
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Ingrese su usario..."
        }

        Label {
            id: passwordLbl
            anchors.top: usernameLbl.bottom
            anchors.topMargin: 150
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Contraseña"
            font.pointSize: 15
            font.bold: true

        }


        TextField {
            id: passwordInput
            width: parent.width/2
            anchors.top: passwordLbl.bottom
            anchors.topMargin: 25
            anchors.horizontalCenter: parent.horizontalCenter
            placeholderText: "Ingrese su usario..."
        }
    }

    Button {
        id: loginBtn
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Iniciar Sesion"
        font.pointSize: 15
        font.bold: true
    }
}
