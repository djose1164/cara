import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "qrc:/main"

// TODO: Display user info and later make it a page. Or start it right now in
// another qml.

Page {
    id: home
    property string currentUser

    header: HomeHeader {}

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 0

        Label {
            Layout.alignment: Qt.AlignCenter
            Layout.bottomMargin: -75
            text: "Cara"
            font.pointSize: 64
            font.bold: true
        }

        Image {
            Layout.alignment: Qt.AlignCenter
            Layout.topMargin: -75
            source: "qrc:/icons/smile.png"
            sourceSize: "256x256"
            antialiasing: false
        }
    }

    ListModel {
        id: userModel
    }

    footer: ToolBar {
        background: Rectangle {
            anchors.fill: parent
            color: "floralwhite"
        }

        RowLayout {
            anchors.fill: parent
            spacing: 30

            ToolButton {
                id: homeBtn
                Layout.alignment: Qt.AlignCenter
                Layout.bottomMargin: 5
                background: Image {
                    source: "qrc:/icons/home.svg"
                    antialiasing: false
                    sourceSize: iconSize
                }
            }

            ToolButton {
                id: userBtn
                Layout.alignment: Qt.AlignCenter
                Layout.bottomMargin: 5
                background: Image {
                    source: "qrc:/icons/search.svg"
                    antialiasing: false
                    sourceSize: iconSize
                }
                onClicked: mainStackView.push("qrc:/search/SearchPage.qml")
            }

            ToolButton {
                id: shopBtn
                Layout.alignment: Qt.AlignCenter
                Layout.bottomMargin: 5
                background: Image {
                    source: "qrc:/icons/shopping-cart.svg"
                    antialiasing: false
                    sourceSize: iconSize
                }
            }

            ToolButton {
                id: billBtn
                Layout.alignment: Qt.AlignCenter
                Layout.bottomMargin: 5
                background: Image {
                    source: "qrc:/icons/bill.svg"
                    antialiasing: false
                    sourceSize: "32x32"
                }
                onClicked: mainStackView.push("qrc:/bills/BillPage.qml")
            }
        }
    }

    SidePane {
        id: sideMenu
    }
}
