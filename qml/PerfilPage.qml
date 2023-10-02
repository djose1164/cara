import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import core

Page {
    id: root

    readonly property var stackView: StackView.view
    readonly property UserEntity user: CurrentUser.user

    title: "Perfil"
    header: TitleHeader {
        title: root.title
        onBackClicked: stackView.pop()
        btnLoader.sourceComponent: ToolButton {
            text: "Guardar"
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20

        Label {
            text: "Nombre"
        }

        TextField {
            id: nameField
            text: user.forename
            Layout.fillWidth: true
        }

        Label {
            text: "Apellido"
        }

        TextField {
            id: lastNameField
            text: user.surname
            Layout.fillWidth: true
        }

        Label {
            text: "Correo electronico"
        }

        TextField {
            id: emailField
            text: user.email
            Layout.fillWidth: true
        }

        Column {
            Layout.fillWidth: true
        Label {
            text: "Contraseña"
        }

        TextField {
            id: passwordField
            width: parent.width
            text: "Contraseña"
            echoMode: TextField.Password

        }
        }
    }
}
