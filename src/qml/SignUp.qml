import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: signUp
    Keys.onEnterPressed: Qt.inputMethod.hide()
    Keys.onReturnPressed: Qt.inputMethod.hide()

    Label {
        id: signUpLbl
        anchors.top: parent.top
        anchors.topMargin: 30
        text: "Registrarse"
        font.pointSize: 20
        font.bold: true
    }

    GridLayout {
        id: completeNameGL
        width: parent.width
        anchors.top: signUpLbl.bottom
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter
        columns: 2
        rows: 2
        rowSpacing: 0
        columnSpacing: 40

        Label {
            text: "Nombre"
        }

        Label {
            text: "Apellido"
        }

        TextField {
            placeholderText: "Ingrese su nombre..."
        }

        TextField {
            placeholderText: "Ingrese su apellido..."
        }
    }

    ColumnLayout {
        anchors.top: completeNameGL.bottom
        anchors.topMargin: this.height / 4
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width
        spacing: 20

        Label {

            text: "Correo electronico"
        }

        TextField {
            placeholderText: "Ingrese su correo electronico..."
        }

        Label {
            text: "Contrasena"
        }

        TextField {
            placeholderText: "Ingrese su contrasena..."
            echoMode: TextField.Password
            passwordMaskDelay: 600
        }
    }

    Button {
        width: parent.width/2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: this.height * 2
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Registrarse"
        onClicked:
            mainStackView.replace("qrc:/qml/Home.qml")
    }
}
