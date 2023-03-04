import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    required property StackView stackView

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
            font.pointSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: 5
        }

        TabButton {
            id: userBtn
            background: IconImage {
                source: "qrc:/icons/search.svg"
                antialiasing: false
                sourceSize: iconSize
            }
            onClicked: stackView.push("qrc:/search/SearchPage.qml")
        }
    }
}
