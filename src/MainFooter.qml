import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    background: Rectangle {
        anchors.fill: parent
        color: "floralwhite"
    }

    RowLayout {
        anchors.fill: parent
        anchors.bottomMargin: 5
        spacing: 30

        ToolButton {
            id: homeBtn
            Layout.alignment: Qt.AlignCenter
            background: Image {
                source: "qrc:/icons/home.svg"
                antialiasing: false
                sourceSize: iconSize
            }
        }

        ToolButton {
            id: userBtn
            Layout.alignment: Qt.AlignCenter
            background: Image {
                source: "qrc:/icons/search.svg"
                antialiasing: false
                sourceSize: iconSize
            }
            onClicked: {

                stackLayout.currentIndex = 1
            }
        }

        ToolButton {
            id: shopBtn
            Layout.alignment: Qt.AlignCenter
            background: Image {
                source: "qrc:/icons/shopping-cart.svg"
                antialiasing: false
                sourceSize: iconSize
            }
            onClicked: stackLayout.currentIndex = 2
        }

        ToolButton {
            id: billBtn
            Layout.alignment: Qt.AlignCenter
            background: Image {
                source: "qrc:/icons/bill.svg"
                antialiasing: false
                sourceSize: "32x32"
            }
            onClicked: stackLayout.currentIndex = 3
        }
    }
}
