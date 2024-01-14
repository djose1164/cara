import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    id: root
    required property StackView stackView

    background: Rectangle {
        anchors.fill: parent
        color: "floralwhite"
    }

    signal acceptedText(text: string)

    RowLayout {
        anchors.fill: parent
        spacing: 10

        ToolButton {
            id: settingBtn
            background: Image {
                source: "qrc:/icons/menu-burger.svg"
                antialiasing: false
                sourceSize: iconSize
            }
            onClicked: sideMenu.open()
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 5
        }

        Label {
            id: homeLbl
            text: "Inicio"
            color: "black"
            font.pointSize: 20
            font.bold: true
            Layout.leftMargin: 10
            Layout.fillWidth: true
        }

        /*RoundButton {
            id: btn
            readonly property string newNotifications: "qrc:/icons/new_notifications.svg"
            readonly property string nonNotifications: "qrc:/icons/notifications_black_24dp.svg"
            onClicked: {
                iconImage.source = nonNotifications;
                menu.open();
            }
            Layout.alignment: Qt.AlignRight
            flat: true

            IconImage {
                id: iconImage
                anchors.centerIn: parent
                source: btn.newNotifications
            }
        }*/
    }

    Drawer {
        id: menu
        edge: Qt.RightEdge
        width: root.width
        height: root.height
        ListView {
            anchors.fill: parent
            anchors.topMargin: 20
            model: ["En Cara trabajamos para ser confiables de tu sonrisa."]
            delegate: Text {
                width: ListView.view.width
                text: modelData
                font.bold: true
                font.pointSize: 12
                color: "black"
                wrapMode: Text.Wrap
            }
        }
    }
}
