import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/utils"
import "qrc:/users"

Drawer {
    id: root
    width: .7 * parent.width
    height: parent.height

    ColumnLayout {
        width: parent.width
        height: parent.height/2
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 15

        Label {
            text: "Informacion Personal"
            font.pointSize: 12
            font.bold: true
        }

        Label {
            text: "Usario: " + TheCurrentUser.username
            font.pointSize: 11
            visible: TheCurrentUser.username
        }

        Label {
            text: "Nombre"
            font.pointSize: 11
            font.bold: true
        }

        Label {
            text: TheCurrentUser.forename + " " + TheCurrentUser.surname
            font.pointSize: 11
        }

        Label {
            Layout.fillWidth: true
            text: "**Correo electronico**"
            textFormat: Label.MarkdownText
            font.pointSize: 11
        }

        Label { text: TheCurrentUser.email }

        Button {
            Layout.alignment: Qt.AlignBottom
            text: "Cerrar sesion"
            flat: true
            onClicked: {
                close();
                TheCurrentUser.clear();
                mainStackView.replace("qrc:/login/Login.qml");
            }
        }
    }
}
