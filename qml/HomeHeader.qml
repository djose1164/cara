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
            font.pointSize: 18
            font.bold: true
            Layout.leftMargin: 10
            Layout.fillWidth: true
        }

        RoundButton {
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
        }

        RoundButton {
            id: userBtn
            flat: true
            onClicked: {
                stackView.push("qrc:/search/SearchPage.qml")
                const item = stackView.currentItem;
                // BUG: SearchBar is not working. I don't know even if the model
                // is being binded to the SearchPage.
                item.acceptedText.connect(function(t) {
                    root.acceptedText(t);
                });
            }
            Layout.alignment: Qt.AlignTrailing
            Layout.rightMargin: 10

            IconImage {
                anchors.centerIn: parent
                source: "qrc:/icons/search.svg"
                sourceSize: "24x24"
            }
        }
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
