import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

ToolBar {
    background: Rectangle {
        anchors.fill: parent
        color: "floralwhite"
    }

    RowLayout {
        anchors.fill: parent
        spacing: 20

        ToolButton {
            id: settingBtn
            background: Image {
                source: "qrc:/icons/menu-burger.svg"
                antialiasing: false
                sourceSize: iconSize
            }
            onClicked: sideMenu.open()
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 5
            Layout.leftMargin: 5
        }

        Label {
            id: homeLbl
            text: "Inicio"
            color: "black"
            font.pointSize: 24
            font.bold: true
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
        }

        ToolButton {
            background: Image {
                antialiasing: false
                source: "qrc:/icons/confetti.svg"
                sourceSize: iconSize
            }
            onClicked: message.open()
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 5
            Layout.rightMargin: 5
        }
    }

    MessageDialog {
        id: message
        text: "Ups! Aun estamos trabajando en esta opcion!"
        informativeText: "Mantente al tanto en nuestras redes sociales!"
        buttons: MessageDialog.Ok


        onButtonClicked: code => {
                             if (code === MessageDialog.Ok)
                                close();
                         }
    }
}
